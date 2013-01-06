#include "Block.h"
using namespace std;

Block::Block()
{
    _blockID = 0;
    _blockLength = 0;
    _genStamp = -1;
}

Block::Block(long id, long len, long genStamp) {
    _blockID = id;
    _blockLength = len;
    _genStamp = genStamp;
}

Block::~Block()
{
    //dtor
}

void Block::readFields(istream* is) {
    is->read((char*)&_blockID, sizeof(_blockID));

    is->read((char*)&_blockLength, sizeof(_blockLength));

    is->read((char*)&_genStamp, sizeof(_genStamp));
}


void Block::write(ostream* os) {
    os->write((char*)&_blockID, sizeof(_blockID));

    os->write((char*)&_blockLength, sizeof(_blockLength));

    os->write((char*)&_genStamp, sizeof(_genStamp));
}


void Block::print(){
    cout<<_blockID<<","<<_blockLength<<","<<_genStamp<<endl;
}
