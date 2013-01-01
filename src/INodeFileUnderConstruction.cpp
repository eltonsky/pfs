#include "INodeFileUnderConstruction.h"

INodeFileUnderConstruction::INodeFileUnderConstruction() : INodeFile()
{
    //ctor
}

INodeFileUnderConstruction::INodeFileUnderConstruction(string path,short rep,
        long modTime,long preferBlkSize,int numBlks,int numLocs): INodeFile(){
    _path = path;
    _replication = rep;
    _modTime = modTime;
    _preferredBlockSize = preferBlkSize;
    _numBlocks = numBlks;
    _numLocations = numLocs;
}


void INodeFileUnderConstruction::write(ostream* os) {
    Writable::writeString(os, _path);

    os->write((char*)&_replication, sizeof(_replication));

    os->write((char*)&_modTime, sizeof(_modTime));

    os->write((char*)&_accessTime, sizeof(_accessTime));

    os->write((char*)&_preferredBlockSize, sizeof(_preferredBlockSize));

    os->write((char*)&_numBlocks, sizeof(_numBlocks));
}

void INodeFileUnderConstruction::readFields(istream* is) {

    _path = Writable::readString(is);

    is->read((char*)&_replication, sizeof(_replication));

    is->read((char*)&_modTime, sizeof(_modTime));

    is->read((char*)&_accessTime, sizeof(_accessTime));

    is->read((char*)&_blockSize, sizeof(_blockSize));

    is->read((char*)&_numBlocks, sizeof(_numBlocks));
}


INodeFileUnderConstruction::~INodeFileUnderConstruction()
{
    //dtor
}
