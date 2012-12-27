#include "INodeFileUnderConstruction.h"

INodeFileUnderConstruction::INodeFileUnderConstruction()
{
    //ctor
}

INodeFileUnderConstruction::INodeFileUnderConstruction(string name,short rep,
        long modTime,long preferBlkSize,int numBlks,int numLocs){
    _name = name;
    _replication = rep;
    _modTime = modTime;
    _preferredBlockSize = preferBlkSize;
    _numBlocks = numBlks;
    _numLocations = numLocs;

}

INodeFileUnderConstruction::~INodeFileUnderConstruction()
{
    //dtor
}
