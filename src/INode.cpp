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


INode::INode(INode* node) : INode() {
    _path = node->getPath();
    _modTime = node->getModTime();
    _accessTime = node->getAccessTime();
    _replication = node->getReplication();
    _blockSize = node->getBlockSize();
    _numBlocks = node->getBlockNum();
}


/*
*/
void INode::write(ostream* os) {
    Writable::writeString(os, _path);

    os->write((char*)&_replication, sizeof(_replication));

    os->write((char*)&_modTime, sizeof(_modTime));

    os->write((char*)&_accessTime, sizeof(_accessTime));

    os->write((char*)&_blockSize, sizeof(_blockSize));

    os->write((char*)&_numBlocks, sizeof(_numBlocks));
}


void INode::readFields(istream* is) {

    _path = Writable::readString(is);

    is->read((char*)&_replication, sizeof(_replication));

    is->read((char*)&_modTime, sizeof(_modTime));

    is->read((char*)&_accessTime, sizeof(_accessTime));

    is->read((char*)&_blockSize, sizeof(_blockSize));

    is->read((char*)&_numBlocks, sizeof(_numBlocks));

    if (_numBlocks == 0) {
        is->read((char*)&nsQuota, sizeof(nsQuota));

        is->read((char*)&dsQuota, sizeof(dsQuota));
    }
}


void INode::setParent(INode* p) {
    _parent = p;
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

Permission* INode::getPermission(){
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







