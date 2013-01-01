#include "FSNameSystem.h"

FSNameSystem::FSNameSystem()
{
    _fsImage = new FSImage(Config::get("pfs,imgFile,path"));
    _fsEditLog = new FSEditLog(_fsImage);
}

FSNameSystem::~FSNameSystem()
{
    //dtor
}


void FSNameSystem::initialize(){
}


void FSNameSystem::saveNameSpace(){

///**/
//_fsImage->setNumFiles(4);
//INodeFile* file = new INodeFile("/abc.txt", 3, 1024, 1);
//Block* blk = new Block(111,222,1234567);
//file->addBlock(blk);
//_fsImage->getRoot()->addChild(file,1);
//
//INodeDirectory * dir1 = new INodeDirectory("/ddd");
//_fsImage->getRoot()->addChild(dir1,1);
//
//INodeDirectory * dir2 = new INodeDirectory("/ddd/ee");
//dir1->addChild(dir2,1);
///**/

    _fsImage->saveImage();
}



void FSNameSystem::loadNameSpace(){
    _fsImage->loadImage();

///**/
//    map<string,INode*>::iterator iter = _fsImage->getRoot()->getChildren()->begin();
//
//    for(;iter != _fsImage->getRoot()->getChildren()->end();iter++) {
//        cout<<iter->first<<endl;
//    }

    FSImage::print(_fsImage->getRoot());
}









