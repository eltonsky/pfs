#include "INodeFile.h"

INodeFile::INodeFile()
{
    //ctor
}


INodeFile::INodeFile(string path, short replication, long blkSize, int numBlocks)
    :INode(path, replication, blkSize, numBlocks){
}


INodeFile::~INodeFile()
{
    //dtor
}
