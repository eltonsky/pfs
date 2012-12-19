#ifndef INODEDIRECTORY_H
#define INODEDIRECTORY_H
#include "INode.h"
#include <set>
#include <time.h>
#include <string>
#include <map>

// it's a dir if numBlocks is 0
class INodeDirectory : public INode
{
    public:
        INodeDirectory(string);
        virtual ~INodeDirectory();

        map<string, INode*> getChildren();
        INode* addChild(INode*, bool);
        void setParent(INode* parent);
    protected:
        INode* _parent; // must be a INodeDirectory
        map<string,INode*> _children;

    private:
};

#endif // INODEDIRECTORY_H
