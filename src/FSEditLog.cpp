#include "FSEditLog.h"
using namespace std;

FSEditLog::FSEditLog(int logVersion)
{
    sem_init(&_sem_log, 0, 1);
    _isSyncRunning = false;
    _version = logVersion;

    m_lock1 = boost::mutex::scoped_lock(m_log1);
}

FSEditLog::FSEditLog(string file, int logVersion): FSEditLog(logVersion) {
    _editStream = new ofstream(file);
    _editStream->write((char*)&_version, sizeof(_version));
}

FSEditLog::~FSEditLog()
{
    sem_destroy(&_sem_log);
}

void FSEditLog::close(){
    if(_editStream->is_open())
        _editStream->close();
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

    _editStream->flush();

    _isSyncRunning = false;

    sem_post(&_sem_log);

    m_cond1.notify_all();

}

void FSEditLog::logEdit(stringstream* ss) {
    sem_wait(&_sem_log);

    if(_editStream->good()) {
        _editStream->write(ss->str().c_str(), ss->str().size()+1);
    }

    sem_post(&_sem_log);
}

void FSEditLog::logOpenFile(string path, INodeFileUnderConstruction* newNode) {
    stringstream ss;

    ss.write((char*)&OP_ADD, sizeof(OP_ADD));

    Writable::writeString(&ss, newNode->getPath());

    short rep = newNode->getReplication();
    ss.write((char*)&rep, sizeof(rep));

    long m_time = newNode->getModTime();
    ss.write((char*)&m_time, sizeof(m_time));

    long blkSize = newNode->getBlockSize();
    ss.write((char*)&blkSize, sizeof(blkSize));

    int numBlocks = newNode->getBlockNum();
    ss.write((char*)&numBlocks, sizeof(numBlocks));

    //write blks

    //write perm

    //write client name /machine

    //locs

    delete ss;
}








