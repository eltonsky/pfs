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
        virtual ~INodeFile();

        void addBlock(Block*);
        void setBlock(int, Block*);
        vector<Block*> getBlocks();

        void spaceConsumed();
        void getLastBlock();

        bool isDirectory();
        bool isFile();

    protected:
        INode* parent; // must be a INodeDirectory

        vector<Block*> _blocks;
    private:
};

#endif // INODEFILE_H
