#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <fstream>
using namespace std;

class Block
{
    public:
        Block();
        Block(long id, long len, long genStamp);
        virtual ~Block();

        void readFields(istream*);
        void write(ostream*);

        long getBlkID() {return _blockID;}
        long getBlkLength() {return _blockLength;}
        long getBlkGenStamp() {return _genStamp;}

        void print();
    protected:
        long _blockID;
        long _blockLength;
        long _genStamp;
    private:
};

#endif // BLOCK_H
