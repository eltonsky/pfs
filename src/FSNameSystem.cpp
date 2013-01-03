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

    /**/
INodeFile* file = new INodeFile("/f1", 3, 1024, 1);
Block* blk = new Block(111,222,1234567);
file->addBlock(blk);
FSNameSystem::getFSNamesystem().getFSImage()->addFile(file,false,true);

INodeDirectory * dir1 = new INodeDirectory("/d1");
FSNameSystem::getFSNamesystem().getFSImage()->addFile(dir1,false,true);

INodeDirectory * dir2 = new INodeDirectory("/d1/d11");
FSNameSystem::getFSNamesystem().getFSImage()->addFile(dir2,false,true);

INodeFile* file1 = new INodeFile("/d1/f11", 3, 1024, 1);
Block* blk1 = new Block(1111,2222,1234567);
file1->addBlock(blk1);
FSNameSystem::getFSNamesystem().getFSImage()->addFile(file1,false,true);

INodeFile* file2 = new INodeFile("/d1/d11/f111", 3, 10240, 1);
Block* blk2 = new Block(222,333,654321);
file2->addBlock(blk2);
FSNameSystem::getFSNamesystem().getFSImage()->addFile(file2,false,true);


/**/

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









