#include "INode.h"
using namespace std;

INode::INode()
{
    _path.clear();
    _modTime = 0;
    _accessTime = 0;
    _replication = 0;
    _blockSize = 0;
    _numBlocks = 0;
    nsQuota = 0;
    dsQuota = 0;

    _perm = new Permission();
}

//file
INode::INode(string path, short replication, long blockSize, int numBlocks): INode(){
    _path = path;
    _replication = replication;
    _blockSize = blockSize;
    _numBlocks = numBlocks;
}

//dir
INode::INode(string path): INode(){
    _path = path;
}


void INode::readFileds(ifstream* ifs) {

    _path = Writable::readString(ifs);

    ifs->read((char*)&_replication, sizeof(_replication));

    ifs->read((char*)&_modTime, sizeof(_modTime));

    ifs->read((char*)&_accessTime, sizeof(_accessTime));

    ifs->read((char*)&_blockSize, sizeof(_blockSize));

    ifs->read((char*)&_numBlocks, sizeof(_numBlocks));

    if (_numBlocks == 0) {
        ifs->read((char*)&nsQuota, sizeof(nsQuota));

        ifs->read((char*)&dsQuota, sizeof(dsQuota));
    }
}

void INode::setPermission(Permission* perm) {
    _perm = perm;
}

bool INode::isDirectory(){
    return false;
}

bool INode::isFile() {
    return false;
}

Permission* INode::getPerm(){
    return _perm;
}

long INode::getModTime(){
    return _modTime;
}

long INode::getAccessTime(){
    return _accessTime;
}

string INode::getPath(){
    return _path;
}

int INode::getBlockNum() {
    return _numBlocks;
}

short INode::getReplication(){
    return _replication;
}

long INode::getBlockSize(){
    return _blockSize;
}

void INode::setModTime(long modTime) {
    _modTime = modTime;
}

INode::~INode()
{
    //dtor
}







