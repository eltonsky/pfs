#include "FSImage.h"

using namespace std;
using namespace boost;



FSImage::FSImage()
{
    _imgVersion = atoi(Config::get("pfs,imgFile,version"));
    _namespaceId = -1;
    _numFiles = 1; // _root always exists
    _genStamp = -1;

    // init lock
    m_lock1 = boost::mutex::scoped_lock(m_log1);

    //init sem
    sem_init(&_sem_image, 0, 1);

    //init root
    _root = new INodeDirectory("/");
}

FSImage::~FSImage()
{
    sem_destroy(&_sem_image);
}

FSImage::FSImage(string imgFile) : FSImage() {

    _imageFile = imgFile;
}


void FSImage::replaceRoot(INode* newRoot) {
    delete _root;

    _root = new INodeDirectory(newRoot);
}


/* load image file to in mem struct
*/
void FSImage::loadImage() {
    ifstream imgStream;
    INodeDirectory* parent;
    INode* child;

    try{
        imgStream.open(_imageFile.c_str(),ios::in | ios::binary);

        if(imgStream.is_open()){

            // global param
            imgStream.read((char*)&_imgVersion, sizeof(_imgVersion));

            imgStream.read((char*)&_namespaceId, sizeof(_namespaceId));

            imgStream.read((char*)&_numFiles, sizeof(_numFiles));

            imgStream.read((char*)&_genStamp, sizeof(_genStamp));

            if (imgStream.good()) {

                // load INodes
                for(int i=0; i < _numFiles; i++) {
                    INode* current = new INode();
                    current->readFields(&imgStream);
                    parent = INodeDirectory::getParent(current->getPath(), _root);

                    // if dir
                    if (current->getBlockNum() == 0){
                        INodeDirectory dir(current->getPath());

                        if (parent==NULL) {
                            replaceRoot(current);
                            child = _root;
                        }
                        else
                            child = parent->addChild(&dir, 1);

                    } else {
                    // if file
                        INodeFile file(current->getPath(),current->getReplication(),
                                       current->getBlockSize(), current->getBlockNum());

                        child = parent->addChild(&file, 1);

                        // read blocks
                        for(int fileIndex = 0; fileIndex < current->getBlockNum(); fileIndex++) {
                            Block blk;
                            blk.readFields(&imgStream);
                            file.addBlock(&blk);
                        }
                    }

                    if (child == 0L) {
                        throw("failed to insert INode " + current->getPath());
                    }

                    //get permission for this INode
                    Permission perm;
                    perm.readFields(&imgStream);

                    if(parent==NULL)
                        _root->setPermission(&perm);
                    else
                        child->setPermission(&perm);
                }


                // load data nodes


                // load file under construct

            }
            //Log::write(INFO, "imgVersion is : "  + boost::lexical_cast<string>(_imgVersion));

        }
    }catch(char* err){
        Log::write(INFO, err);

        if(imgStream.is_open())
            imgStream.close();

        return;
    }

    if(imgStream.is_open())
            imgStream.close();

    //mark image as ready
    sem_wait(&_sem_image);
    _ready = true;
    sem_post(&_sem_image);
    m_cond1.notify_all();

    return;
}



void FSImage::saveImage() {
    ofstream imgOStream;

    try{
        imgOStream.open(_imageFile.c_str(),ios::out | ios::binary);

        if(imgOStream.is_open()){
            imgOStream.write((char*)&_imgVersion, sizeof(_imgVersion));

            imgOStream.write((char*)&_namespaceId, sizeof(_namespaceId));

            imgOStream.write((char*)&_numFiles, sizeof(_numFiles));

            imgOStream.write((char*)&_genStamp, sizeof(_genStamp));

            //recursively write each inode.
            saveINode(_root, &imgOStream);
            saveINodeWrap(_root, &imgOStream);
        }


    }catch(char* err){
        Log::write(INFO, err);

        if(imgOStream.is_open())
            imgOStream.close();

        return;
    }

    if(imgOStream.is_open())
            imgOStream.close();

    return;
}


void FSImage::saveINode(INode* currNode, ofstream* ofs) {

    string path = currNode->getPath();
    Writable::writeString(ofs, path);

    short rep = currNode->getReplication();
    ofs->write((char*)&rep, sizeof(rep));

    long modTime = currNode->getModTime();
    ofs->write((char*)&modTime, sizeof(modTime));

    long accessTime = currNode->getAccessTime();
    ofs->write((char*)&accessTime, sizeof(accessTime));

    long blkSize = currNode->getBlockSize();
    ofs->write((char*)&blkSize, sizeof(blkSize));

    int blkNum = currNode->getBlockNum();
    ofs->write((char*)&blkNum, sizeof(blkNum));

    if(currNode->isDirectory()) {
        long nsQuota = currNode->nsQuota;
        ofs->write((char*)&nsQuota, sizeof(nsQuota));

        long dsQuota = currNode->dsQuota;
        ofs->write((char*)&dsQuota, sizeof(dsQuota));
    } else {
        //write blocks
        vector<Block*> blocks = ((INodeFile*)currNode)->getBlocks();

        for(int i=0; i<currNode->getBlockNum();i++){
            blocks[i]->write(ofs);
        }
    }

    currNode->getPermission()->write(ofs);
}

/*first loop store all children to image file;
* 2nd loop store children recursively. This way keeps the order of
* INode data in image file.
*/
void FSImage::saveINodeWrap(INode* currNode, ofstream* ofs){

    map<string,INode*>* children = ((INodeDirectory*)currNode)->getChildren();

    map<string,INode*>::const_iterator iter;

    for(iter= children->begin(); iter != children->end(); iter++) {
        saveINode(iter->second, ofs);
    }

    iter= children->begin();
    for(; iter != children->end(); iter++) {
        if(iter->second->isDirectory()) {
            saveINodeWrap(iter->second, ofs);
        }
    }
}



/*wait until fsimage finished loading.*/
void FSImage::waitForReady() {

    long start = time(NULL);

    while (!_ready) {
      try {
        m_cond1.timed_wait(m_lock1,boost::posix_time::milliseconds(5000));

        Log::write(INFO, "wait for " + boost::lexical_cast<string>(time(NULL)-start) +" for image ready...");
      } catch (std::exception& e) {
          Log::write(INFO,e.what());
      }
    }
}


void FSImage::addFile(INode* node, bool protect, bool inheritPerm) {
    INodeDirectory* parent = NULL;

    if(protect)
        waitForReady();

    try{
        parent = INodeDirectory::getParent(node->getPath(), _root);

        node->setParent(parent);

        parent->addChild(node, inheritPerm);

        _numFiles++;

        Log::write(INFO, "added node " + node->getPath());
    } catch(char* e) {
        Log::write(ERROR, "fail to add node " + node->getPath()+" : " +e);
    }

}


/*
Accessor & Mutator
*/

INodeDirectory* FSImage::getRoot() {
    return _root;
}

void FSImage::setVersion(int v) {
    _imgVersion = v;
}


int FSImage::getVersion(){
    return _imgVersion;
}

void FSImage::setFile(string file){
    _imageFile = file;
}


string FSImage::getFile(){
    return _imageFile;
}

void FSImage::setNamespaceID(int namespaceID){
    _namespaceId = namespaceID;
}


int FSImage::getNamespaceID(){
    return _namespaceId;
}

void FSImage::setNumFiles(int num){
    _numFiles = num;
}

int FSImage::getNumFiles(){
    return _numFiles;
}


void FSImage::setGenStamp(long gs) {
    _genStamp = gs;
}

long FSImage::getGenStamp(){
    return _genStamp;
}


//print to stdout
void FSImage::print(INode* node) {
    node->print(true);
}











