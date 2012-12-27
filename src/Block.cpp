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

void Block::readFileds(ifstream* ifs) {
    ifs->read((char*)&_blockID, sizeof(_blockID));

    ifs->read((char*)&_blockLength, sizeof(_blockLength));

    ifs->read((char*)&_genStamp, sizeof(_genStamp));
}


void Block::write(ofstream* ofs) {
    ofs->write((char*)&_blockID, sizeof(_blockID));

    ofs->write((char*)&_blockLength, sizeof(_blockLength));

    ofs->write((char*)&_genStamp, sizeof(_genStamp));
}



