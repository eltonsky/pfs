#include "FSImage.h"

using namespace std;
using namespace boost;



FSImage::FSImage()
{
    _imgVersion = -1;
    _namespaceId = -1;
    _numFiles = 1; // _root always exists
    _genStamp = -1;

    //init root
    _root = new INodeDirectory("/");
}

FSImage::~FSImage()
{
    //dtor
}

FSImage::FSImage(string imgFile) : FSImage() {

    _imageFile = imgFile;
}

/*TODO: move this to proper class.
*/
INodeDirectory* FSImage::getParent(string path, INodeDirectory* root) {
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
INodeDirectory* FSImage::findByPath(vector<string>* path, INodeDirectory* parent){
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



void FSImage::replaceRoot(INodeDirectory* newRoot) {
    delete _root;

    _root = newRoot;
}



/* load image file to in mem struct
*/
void FSImage::loadImage() {
    ifstream imgStream;
    INodeDirectory* parent;
    INode* child;

    try{
        imgStream.open(_imageFile.c_str(),ios::in | ios::binary);

        if(imgStream.is_open()){

            // global param
            imgStream.read((char*)&_imgVersion, sizeof(_imgVersion));

            imgStream.read((char*)&_namespaceId, sizeof(_namespaceId));

            imgStream.read((char*)&_numFiles, sizeof(_numFiles));

            imgStream.read((char*)&_genStamp, sizeof(_genStamp));

            if (imgStream.good()) {

                // load INodes
                for(int i=0; i < _numFiles; i++) {
                    INode* current = new INode();
                    current->readFileds(&imgStream);
                    parent = getParent(current->getPath(), _root);

                    // if dir
                    if (current->getBlockNum() == 0){
                        INodeDirectory dir(current->getPath());

                        if (parent==NULL)
                            replaceRoot(new INodeDirectory(current));
                        else
                            child = parent->addChild(&dir, 1);

                    } else {
                    // if file
                        INodeFile file(current->getPath(),current->getReplication(),
                                       current->getBlockSize(), current->getBlockNum());

                        child = parent->addChild(&file, 1);

                        // read blocks
                        for(int fileIndex = 0; fileIndex < current->getBlockNum(); fileIndex++) {
                            Block blk;
                            blk.readFileds(&imgStream);
                            file.addBlock(&blk);
                        }
                    }

                    if (child == 0L) {
                        throw("failed to insert INode " + current->getPath());
                    }

                    //get permission for this INode
                    Permission perm;
                    perm.readFileds(&imgStream);

                    if(parent==NULL)
                        _root->setPermission(&perm);
                    else
                        child->setPermission(&perm);
                }


                // load data nodes


                // load file under construct

            }
            //Log::write(INFO, "imgVersion is : "  + boost::lexical_cast<string>(_imgVersion));

        }
    }catch(char* err){
        Log::write(INFO, err);

        if(imgStream.is_open())
            imgStream.close();

        return;
    }

    if(imgStream.is_open())
            imgStream.close();

    return;
}



void FSImage::saveImage() {
    ofstream imgOStream;

    try{
        imgOStream.open(_imageFile.c_str(),ios::out | ios::binary);

        if(imgOStream.is_open()){
            imgOStream.write((char*)&_imgVersion, sizeof(_imgVersion));

            imgOStream.write((char*)&_namespaceId, sizeof(_namespaceId));

            imgOStream.write((char*)&_numFiles, sizeof(_numFiles));

            imgOStream.write((char*)&_genStamp, sizeof(_genStamp));

            //recursively write each inode.
            saveINode(_root, &imgOStream);
            saveINodeWrap(_root, &imgOStream);
        }


    }catch(char* err){
        Log::write(INFO, err);

        if(imgOStream.is_open())
            imgOStream.close();

        return;
    }

    if(imgOStream.is_open())
            imgOStream.close();

    return;
}


void FSImage::saveINode(INode* currNode, ofstream* ofs) {

    string path = currNode->getPath();
    Writable::writeString(ofs, path);

    short rep = currNode->getReplication();
    ofs->write((char*)&rep, sizeof(rep));

    long modTime = currNode->getModTime();
    ofs->write((char*)&modTime, sizeof(modTime));

    long accessTime = currNode->getAccessTime();
    ofs->write((char*)&accessTime, sizeof(accessTime));

    long blkSize = currNode->getBlockSize();
    ofs->write((char*)&blkSize, sizeof(blkSize));

    int blkNum = currNode->getBlockNum();
    ofs->write((char*)&blkNum, sizeof(blkNum));

    if(currNode->isDirectory()) {
        long nsQuota = currNode->nsQuota;
        ofs->write((char*)&nsQuota, sizeof(nsQuota));

        long dsQuota = currNode->dsQuota;
        ofs->write((char*)&dsQuota, sizeof(dsQuota));
    } else {
        //write blocks
        vector<Block*> blocks = ((INodeFile*)currNode)->getBlocks();

        for(int i=0; i<currNode->getBlockNum();i++){
            blocks[i]->write(ofs);
        }
    }

    currNode->getPerm()->write(ofs);
}

/*first loop store all children to image file;
* 2nd loop store children recursively. This way keeps the order of
* INode data in image file.
*/
void FSImage::saveINodeWrap(INode* currNode, ofstream* ofs){

    map<string,INode*>* children = ((INodeDirectory*)currNode)->getChildren();

    map<string,INode*>::iterator iter;

    for(iter= children->begin(); iter != children->end(); iter++) {
        saveINode(iter->second, ofs);
    }

    for(iter= children->begin(); iter != children->end(); iter++) {
        if(iter->second->isDirectory()) {
            saveINodeWrap(iter->second, ofs);
        }
    }
}



/*
Accessor & Mutator
*/

INodeDirectory* FSImage::getRoot() {
    return _root;
}

void FSImage::setVersion(int v) {
    _imgVersion = v;
}


int FSImage::getVersion(){
    return _imgVersion;
}

void FSImage::setFile(string file){
    _imageFile = file;
}


string FSImage::getFile(){
    return _imageFile;
}

void FSImage::setNamespaceID(int namespaceID){
    _namespaceId = namespaceID;
}


int FSImage::getNamespaceID(){
    return _namespaceId;
}

void FSImage::setNumFiles(int num){
    _numFiles = num;
}

int FSImage::getNumFiles(){
    return _numFiles;
}


void FSImage::setGenStamp(long gs) {
    _genStamp = gs;
}

long FSImage::getGenStamp(){
    return _genStamp;
}






