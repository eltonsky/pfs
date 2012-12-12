#ifndef INODEDIRECTORY_H
#define INODEDIRECTORY_H

class INode;

#include "INode.h"

class INodeDirectory : public INode
{
    public:
        INodeDirectory();
        virtual ~INodeDirectory();
    protected:
        INode parent; // to beat
    private:
};

#endif // INODEDIRECTORY_H
