#ifndef INODEFILE_H
#define INODEFILE_H
#include "INode.h"
#include "Block.h"
#include <vector>

class INodeFile : public INode
{
    public:
        INodeFile();
        INodeFile(string, short, long, int);
        INodeFile(INodeFile*);
        INodeFile(INodeFile& INodeFile);

        INodeFile& operator = (INodeFile&);

        virtual ~INodeFile();

        void addBlock(Block&);
        void setBlock(int, Block&);
        vector<Block> getBlocks();

        void spaceConsumed();
        void getLastBlock();

        virtual void readFields(istream* is);
        virtual void write(ostream* os) {INode::write(os);}

        bool isDirectory();
        bool isFile();

        void print(bool);

    protected:

        vector<Block> _blocks;
    private:
};

#endif // INODEFILE_H
