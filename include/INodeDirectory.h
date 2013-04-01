#ifndef INODEDIRECTORY_H
#define INODEDIRECTORY_H
#include "INode.h"
#include "INodeFile.h"
#include "Log.h"
#include <set>
#include <time.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

// it's a dir if numBlocks is 0
class INodeDirectory : public INode
{
    public:
        INodeDirectory();
        INodeDirectory(string);
        INodeDirectory(INode*);
        INodeDirectory(INodeDirectory&);
        INodeDirectory& operator = (const INodeDirectory& );

        virtual ~INodeDirectory();

        vector<shared_ptr<INode>>& getChildren() const {return (vector<shared_ptr<INode>>&)_children;}
        vector<shared_ptr<INode>>& getChildren() {return (vector<shared_ptr<INode>>&)_children;}

        INode* addChild(shared_ptr<INode>, bool);
        void setParent(INode* parent);

        bool isDirectory();
        bool isFile();

        static INodeDirectory* getParent(string path, INodeDirectory* root);
        static INodeDirectory* findByPath(string path, vector<short>pos, INodeDirectory* parent);

        INode* find_child(INode*);
        void print(bool);
    protected:
        vector<std::shared_ptr<INode>> _children;

    private:
};

#endif // INODEDIRECTORY_H



