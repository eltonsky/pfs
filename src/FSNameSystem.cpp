#include "FSNameSystem.h"

FSNameSystem::FSNameSystem()
{
    _fsImage = new FSImage(Config::get("pfs,imgFile,path"));
    _fsEditLog = new FSEditLog(_fsImage);
}

FSNameSystem::~FSNameSystem()
{
    if(_fsImage != NULL)
        delete _fsImage;

    if(_fsEditLog != NULL)
        delete _fsEditLog;
}


void FSNameSystem::initialize(){
}


void FSNameSystem::saveNameSpace(){

/**/
INodeFile* file = new INodeFile("/f1", 3, 1024, 3);
Block blk(111,222,1234567);
file->addBlock(blk);
Block blk01(1111,2222,12345671);
file->addBlock(blk01);
Block blk02(1113,2223,12345673);
file->addBlock(blk02);
shared_ptr<INode> pNode(file);
FSNameSystem::getFSNamesystem().getFSImage()->addFile(pNode,false,true);

INodeDirectory* dir1 = new INodeDirectory("/d1");
shared_ptr<INode> pNode1(dir1);
FSNameSystem::getFSNamesystem().getFSImage()->addFile(pNode1,false,true);

INodeDirectory * dir2 = new INodeDirectory("/d1/d11");
shared_ptr<INode> pNode2(dir2);
FSNameSystem::getFSNamesystem().getFSImage()->addFile(pNode2,false,true);

INodeFile* file1 = new INodeFile("/d1/f11", 3, 1024, 1);
Block blk1(1111,2222,1234567);
file1->addBlock(blk1);
shared_ptr<INode> pNode3(file1);
FSNameSystem::getFSNamesystem().getFSImage()->addFile(pNode3,false,true);

INodeFile* file2 = new INodeFile("/d1/d11/f111", 3, 10240, 1);
Block blk2(222,333,654321);
file2->addBlock(blk2);
shared_ptr<INode> pNode4(file2);
FSNameSystem::getFSNamesystem().getFSImage()->addFile(pNode4,false,true);
/**/

    _fsImage->saveImage();

//    cout<<pNode1.get()->getPath()<<" : "<<pNode1.use_count()<<endl;
//    cout<<"GET HERE 1"<<endl;
}



void FSNameSystem::loadNameSpace(){

    _fsImage->loadImage();

    FSImage::print(_fsImage->getRoot());
}









