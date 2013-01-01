#include "INodeDirectory.h"
using namespace std;
using namespace boost;

INodeDirectory::INodeDirectory(string path) : INode(path)
{
    _parent =0L;
}

INodeDirectory::INodeDirectory(INode* node) :
    INode(node)
{
    _parent =0L;
}

INodeDirectory::~INodeDirectory()
{
    //dtor
}


/*
*/
INodeDirectory* INodeDirectory::getParent(string path, INodeDirectory* root) {
    vector<string> parts;

    // if the parent is not root
    if(path.size() > 1) {
        split(parts, path, is_any_of("/"));

        if (parts.size() > 1) {

            //remove first "" and last part of file name.
            //if the path only have 1 level, parts will be empty after trim.
            // then in findByPath it will return root then.
            parts.erase(parts.begin());
            parts.erase(parts.end() - 1);

            //ensure the parent exits.
            INodeDirectory* p = findByPath(&parts, root);

            if (p != NULL)
                return p;

        } else {
            return root;
        }
    } else
        return NULL;

    return NULL;
}


/*assume path.size() > 1
*/
INodeDirectory* INodeDirectory::findByPath(vector<string>* path, INodeDirectory* parent){
    map<string,INode*>::iterator iter;
    INodeDirectory* p = parent;

    while(true){
        for(size_t i=0; i<path->size();i++){
            iter = p->getChildren()->find((*path)[i]);

            if (iter == p->getChildren()->end()) {
                // part of the path doesn't exist
                Log::write(ERROR, "part of path "+(*path)[i]+" doesn't exist!");

                return NULL;
            }
            p = (INodeDirectory*)iter->second;
        }

        break;
    }

    return p;
}


/*
change string path to vector<string> paths.
*/
INode* INodeDirectory::addChild(INode* child, bool inheritPerm) {
    INode* newNode;

    if(inheritPerm){
        Permission* perm = new Permission(this->getPermission());
        child->setPermission(perm);
    }

    child->setModTime(time(NULL));

    // if already exits.
    map<string,INode*>::iterator iter;
    int last_index_of_slash = child->getPath().find_last_of("/");

    string name;
    if(last_index_of_slash == -1)
        name = child->getPath();
    else
        name = child->getPath().substr(last_index_of_slash+1);

    iter = _children.find(name);

    // if travel out of the end, did not find.
    if (iter == _children.end()) {

        if(child->isDirectory())
            newNode = new INodeDirectory(child);
        else
            newNode = new INodeFile((INodeFile*)child);

        _children.insert(_children.begin(), pair<string,INode*>(name,newNode));
    }
    else
        return 0L;

/////////
//cout<<endl;
//for(iter=_children.begin();iter!=_children.end();iter++){
//    cout<<iter->first<<" : " << iter->second->getPath()<<endl;
//}

    return child;
}

bool INodeDirectory::isDirectory(){
    return true;
}


bool INodeDirectory::isFile(){
    return false;
}

void INodeDirectory::setParent(INode* parent) {
    _parent = parent;
}


map<string, INode*>* INodeDirectory::getChildren() {
    return &_children;
}

