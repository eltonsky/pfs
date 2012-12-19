#ifndef INODEFILE_H
#define INODEFILE_H
#include "INode.h"

class INodeFile : public INode
{
    public:
        INodeFile();
        INodeFile(string, short, long, int);
        virtual ~INodeFile();

        void addBlock();
        void setBlock();
        void spaceConsumed();
        void getLastBlock();

    protected:
        INode* parent; // must be a INodeDirectory

        Block _blockAry[];
    private:
};

#endif // INODEFILE_H
