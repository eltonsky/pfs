#include "INodeDirectory.h"
using namespace std;

INodeDirectory::INodeDirectory():INode(){}

INodeDirectory::INodeDirectory(string path) : INode(path)
{
    _parent =0L;
}

INodeDirectory::INodeDirectory(INode* node) :
    INode(node)
{
    _parent =0L;
}

INodeDirectory::INodeDirectory(INodeDirectory& dir) :
    INode(&dir)
{
    _parent =0L;

    _children.reserve(10);
    vector<shared_ptr<INode>>::iterator iter;
    for(iter = dir.getChildren().begin();iter != dir.getChildren().end();iter++){
        _children.push_back(*iter);
    }
}


INodeDirectory& INodeDirectory::operator = (const INodeDirectory& dir) {
    _path = dir.getPath();

    _children.clear();
    vector<shared_ptr<INode>>::iterator iter;
    for(iter = dir.getChildren().begin();iter != dir.getChildren().end();iter++){
        _children.push_back(*iter);
    }

    return *this;
}


INodeDirectory::~INodeDirectory()
{
//    cout<<"dctor dir: "<<this->getPath()<<endl;
}


/*
*/
INodeDirectory* INodeDirectory::getParent(string path, INodeDirectory* root) {
    vector<short> pos;

    // if the parent is not root
    if(path.size() > 1) {
        int i=0;
        while(i != -1) {
            i = path.find('/',i+1);
            if(i != -1)
                pos.push_back(i);
        }

        if (pos.size() > 0) {

            //ensure the parent exits.
            INodeDirectory* p = findByPath(path, pos, root);

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
always start from root so far.
*/
INodeDirectory* INodeDirectory::findByPath(string path, vector<short> pos, INodeDirectory* root){
    vector<shared_ptr<INode>>::iterator iter;
    INodeDirectory* p = root;
    string partialPath;

    for(size_t i=0; i< pos.size();i++){
        partialPath = path.substr(0,pos[i]);

        INode node(partialPath);

        p = (INodeDirectory*) p->find_child(&node);

        if (p == NULL) {
            // part of the path doesn't exist
            Log::write(ERROR, "partial path "+partialPath+" for file "+ path +" can not be located!");

            return NULL;
        }
    }

    return p;
}


///*
//change string path to vector<string> paths.
//*/
//INode* INodeDirectory::addChild(INode* child, bool inheritPerm) {
//
//    if(inheritPerm){
//        Permission* perm = new Permission(this->getPermission());
//        child->setPermission(perm);
//    }
//
//    child->setModTime(time(NULL));
//
//    INode* exist = find_child(child);
//
//    if(exist == NULL){
//        INode* newNode;
//
//        if(child->isDirectory()) {
//            newNode = new INodeDirectory(child);
//        } else
//            newNode = new INodeFile((INodeFile*)child);
//
//        shared_ptr<INode> sChild(newNode);
//        _children.push_back(sChild);
//    }
//    else
//        return NULL;
//
//    return child;
//}


/*
change string path to vector<string> paths.
*/
INode* INodeDirectory::addChild(shared_ptr<INode> sChild, bool inheritPerm) {

    if(inheritPerm){
        Permission perm(this->getPermission());
        sChild.get()->setPermission(perm);
    }

    sChild.get()->setModTime(time(NULL));

    INode* exist = find_child(sChild.get());

    if(exist == NULL){
        _children.push_back(sChild);
    }
    else
        return NULL;

    return sChild.get();
}


bool INodeDirectory::isDirectory(){
    return true;
}


bool INodeDirectory::isFile(){
    return false;
}

INode* INodeDirectory::find_child(INode* node) {
    vector<std::shared_ptr<INode>>::iterator iter;

    for(iter=_children.begin();iter!= _children.end();iter++){
        if(iter->get()->getPath() == node->getPath()){
            return iter->get();
        }
    }

    return NULL;
}

void INodeDirectory::setParent(INode* parent) {
    _parent = parent;
}


void INodeDirectory::print(bool recursive) {
    cout<<"\n"+this->getPath()+" : "<< this->getPermission()<<endl;

    vector<shared_ptr<INode>>::iterator iter;

    for(iter=_children.begin();iter!=_children.end();iter++){
        INode* node = iter->get();

        if(node->isDirectory())
            ((INodeDirectory*)node)->print(recursive);
        else
            ((INodeFile*)node)->print(recursive);
    }
}



