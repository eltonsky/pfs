#include "INodeFile.h"

INodeFile::INodeFile()
{

}


INodeFile::INodeFile(string path, short replication, long blkSize, int numBlocks)
    :INode(path, replication, blkSize, numBlocks){
}


void INodeFile::addBlock(Block* blk) {
    _blocks.push_back(blk);
}

void INodeFile::setBlock(int index, Block* blk) {
    vector<Block*>::iterator iter = _blocks.begin();

    _blocks.insert(iter+index, blk);
}


vector<Block*> INodeFile::getBlocks(){
    return _blocks;
}


bool INodeFile::isDirectory(){
    return false;
}


bool INodeFile::isFile(){
    return true;
}

INodeFile::~INodeFile()
{
    //dtor
}
