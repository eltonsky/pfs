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
}

//file
INode::INode(string path, short replication, long blockSize, int numBlocks){
    _path = path;
    _replication = replication;
    _blockSize = blockSize;
    _numBlocks = numBlocks;
}

//dir
INode::INode(string path){
    _path = path;
}


void INode::readFileds(ifstream* ifs) {
    *ifs>>_path;

    *ifs>>_replication;

    *ifs>>_modTime;

    *ifs>>_accessTime;

    *ifs>>_blockSize;

    *ifs>>_numBlocks;
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







