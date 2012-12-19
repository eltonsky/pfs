#include "INodeDirectory.h"
using namespace std;

INodeDirectory::INodeDirectory(string path) : INode(path)
{
    _parent = 0L;
}

INodeDirectory::~INodeDirectory()
{
    //dtor
}


/*
change string path to vector<string> paths.
*/
INode* INodeDirectory::addChild(INode* child, bool inheritPerm) {
    if(inheritPerm){
        //TODO:
    }

    // if already exits.
    map<string,INode*>::iterator iter;
    int last_index_of_slash = child->getPath().find_last_of("/");

    string name;
    if(last_index_of_slash == -1)
        name = child->getPath();
    else
        name = child->getPath().substr(last_index_of_slash);

    iter = _children.find(name);

    // if travel to the end and the last elmt is not, did not find.
    if (iter == _children.end() && name != _children.end()->first)
        _children.insert(_children.begin(), pair<string,INode*>(child->getPath(),child));
    else
        return 0L;

    child->setModTime(time(NULL));

    return child;
}


void INodeDirectory::setParent(INode* parent) {
    _parent = parent;
}


map<string, INode*> INodeDirectory::getChildren() {
    return _children;
}

