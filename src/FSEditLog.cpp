#include "FSEditLog.h"

using namespace std;


FSEditLog::FSEditLog(FSImage* fsimage)
{
    sem_init(&_sem_log, 0, 1);
    _isSyncRunning = false;
    _fsImage = fsimage;
    _version = _fsImage->getVersion();

    m_lock1 = boost::mutex::scoped_lock(m_log1);
}


FSEditLog::~FSEditLog()
{
    sem_destroy(&_sem_log);
}


void FSEditLog::open(OpenMode mode) {
    string file = Config::get("pfs,editLog,path");

    try{

        if(mode == IN) {
            _editIStream = new ifstream(file);
        } else {
            _editOStream = new ofstream(file);
            _editOStream->write((char*)&_version, sizeof(_version));
        }
    } catch (exception& e){
        Log::write(ERROR, e.what());
        exit(EXIT_FAILURE);
    } catch (char* e) {
        Log::write(ERROR, e);
        throw(e);
    }
}


/*
flush.
*/
void FSEditLog::logSync() {
    sem_wait(&_sem_log);

    while(_isSyncRunning) {
        m_cond1.timed_wait(m_lock1,boost::posix_time::milliseconds(1000));
    }

    _isSyncRunning = true;

    _editOStream->flush();

    _isSyncRunning = false;

    sem_post(&_sem_log);

    m_cond1.notify_all();

}

void FSEditLog::logEdit(stringstream* ss) {
    sem_wait(&_sem_log);

    if(_editOStream->good()) {
        _editOStream->write(ss->str().c_str(), ss->str().size()+1);
    }

    sem_post(&_sem_log);
}


//close edit log output stream
void FSEditLog::close(){
    if(_editOStream->is_open())
        _editOStream->close();
}


/*
load edit log from disk.
*/
void FSEditLog::loadEdits() {

    int numEdits = 0;
    int logVersion = 0;
    Permission* perm = 0L;

    string clientName;
    string clientMachine;

    int numOpAdd = 0, numOpClose = 0;
//        numOpDelete = 0,
//        numOpRename = 0, numOpSetRepl = 0, numOpMkDir = 0,
//        numOpSetPerm = 0, numOpSetOwner = 0, numOpSetGenStamp = 0,
//        numOpTimes = 0, numOpGetDelegationToken = 0,
//        numOpRenewDelegationToken = 0, numOpCancelDelegationToken = 0,
//        numOpUpdateMasterKey = 0, numOpOther = 0;

    long startTime = time(NULL);

    open(IN);

    bool avail = true;
    try{
        _editIStream->read((char*)&logVersion, sizeof(logVersion));
    }catch(...){
        avail = false;
        Log::write(ERROR,"fail to get logVersion.");
    }

    assert(avail);

    while(true){
        short opcode = -1;

        try{
            _editIStream->read((char*)&opcode, sizeof(opcode));

            if(opcode==OP_INVALID) {
                Log::write(ERROR,"get OP_INVALID from editLog. break ...");
                break;
            }
        }catch(...){
            Log::write(ERROR,"get error when reading opcode. break ...");
            break;
        }

        numEdits++;

        switch(opcode) {
            case OP_ADD:
            case OP_CLOSE:
                INodeFile* newNode;
                if(opcode==OP_ADD) {
                    newNode = new INodeFileUnderConstruction();
                    numOpAdd++;
                }
                else{
                    newNode = new INodeFile();
                    numOpClose++;
                }

                newNode->readFields(_editIStream);

                for(int i =0; i< newNode->getBlockNum(); i++){
                    Block* blk = new Block();
                    blk->readFields(_editIStream);
                    newNode->addBlock(blk);
                }

                perm = new Permission();
                perm->readFields(_editIStream);
                newNode->setPermission(perm);

                if(opcode==OP_ADD){
                    clientName = Writable::readString(_editIStream);
                    clientMachine = Writable::readString(_editIStream);

                    ((INodeFileUnderConstruction*)newNode)->setClientName(clientName);
                    ((INodeFileUnderConstruction*)newNode)->setClientMachine(clientMachine);
                }

                //add to namespace
                _fsImage->addFile(newNode,false,true);

                break;
            default:
                Log::write(ERROR, "invalid opcode.");
        }


        try{

        }catch(exception& e){
            if(_editIStream->is_open())
                _editIStream->close();

            Log::write(ERROR, e.what());
        }catch(char* e){
            if(_editIStream->is_open())
                _editIStream->close();

            Log::write(ERROR, e);
        }

    }

    if(_editIStream->is_open())
        _editIStream->close();

    long elapse = time(NULL) - startTime;
    Log::write(INFO, "FSEditLog::loadEdits takes " + boost::lexical_cast<string>(elapse)+" secs");
}


/*log OPs*/

void FSEditLog::logOpenFile(string path, INodeFileUnderConstruction* newNode) {
    stringstream* ss = new stringstream();

    //op code
    short op_add = OP_ADD;
    ss->write((char*)&op_add, sizeof(op_add));

    // INode
    newNode->write(ss);

    //blks
    vector<Block> blks = newNode->getBlocks();
    for(vector<Block>::iterator iter = blks.begin(); iter != blks.end(); iter++){
        iter->write(ss);
    }

    //write perm
    newNode->getPermission()->write(ss);

    //write client name /machine
    Writable::writeString(ss,newNode->getClientName());
    Writable::writeString(ss,newNode->getClientMachine());

    //locs
    //version 1.0.4 doesn't write locs to edit log.

    logEdit(ss);

    delete ss;
}


void FSEditLog::logCloseFile(string path, INodeFile* newNode) {
    stringstream* ss = new stringstream();

    //op code
    short op_close = OP_CLOSE;
    ss->write((char*)&op_close, sizeof(op_close));

    // INode
    newNode->write(ss);

    //blks
    vector<Block> blks = newNode->getBlocks();
    for(vector<Block>::iterator iter = blks.begin(); iter != blks.end(); iter++){
        iter->write(ss);
    }

    //write perm
    newNode->getPermission()->write(ss);

    //locs
    //version 1.0.4 doesn't write locs to edit log.

    logEdit(ss);

    delete ss;
}


void FSEditLog::logMkDir(string path, INode* newNode) {
    stringstream* ss = new stringstream();

    //op code
    short op_close = OP_MKDIR;
    ss->write((char*)&op_close, sizeof(op_close));

    //path, modify time, access time
    Writable::writeString(ss,newNode->getPath());

    long modTime= newNode->getModTime();
    ss->write((char*)&modTime,sizeof(modTime));

    long accessTime = newNode->getAccessTime();
    ss->write((char*)&accessTime,sizeof(accessTime));

    //write perm
    newNode->getPermission()->write(ss);


    logEdit(ss);

    delete ss;
}

















