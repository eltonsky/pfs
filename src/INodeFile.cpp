#include "INodeFile.h"

INodeFile::INodeFile() : INode()
{
    _blocks.reserve(10);
}


INodeFile::INodeFile(string path, short replication, long blkSize, int numBlocks)
    :INode(path, replication, blkSize, numBlocks){

    _blocks.reserve(10);
}


INodeFile::INodeFile(INodeFile* file) : INode(file){
    _blocks.reserve(10);

    vector<Block>::iterator iter;

    for(iter = file->getBlocks().begin(); iter!=file->getBlocks().end(); ++iter) {
        Block blk(iter->getBlkID(), iter->getBlkLength(), iter->getBlkGenStamp());

        _blocks.push_back(blk);
    }
}


void INodeFile::addBlock(Block* blk) {
    Block block(blk->getBlkID(), blk->getBlkLength(), blk->getBlkGenStamp());
    _blocks.push_back(block);
}

void INodeFile::setBlock(int index, Block* blk) {
    vector<Block>::iterator iter = _blocks.begin();
    Block block(blk->getBlkID(), blk->getBlkLength(), blk->getBlkGenStamp());

    _blocks.insert(iter+index, block);
}


vector<Block> INodeFile::getBlocks(){
    return _blocks;
}


bool INodeFile::isDirectory(){
    return false;
}


bool INodeFile::isFile(){
    return true;
}


void INodeFile::print(bool recursive) {
    cout<<"\n"+this->getPath()+":"<<endl;

    vector<Block>::iterator iter = _blocks.begin();

    for(;iter != _blocks.end();iter++){
        iter->print();
    }

}


INodeFile::~INodeFile()
{
    //dtor
}
