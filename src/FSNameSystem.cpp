#include "FSNameSystem.h"

FSNameSystem::FSNameSystem()
{

}

FSNameSystem::~FSNameSystem()
{
    //dtor
}


void FSNameSystem::saveNameSpace(){
    FSImage fsImage(Config::get("pfs,imgFile"));

/**/
fsImage.setNumFiles(4);
INodeFile* file = new INodeFile("/abc.txt", 3, 1024, 1);
Block* blk = new Block(111,222,1234567);
file->addBlock(blk);
fsImage.getRoot()->addChild(file,1);

INodeDirectory * dir1 = new INodeDirectory("/ddd");
fsImage.getRoot()->addChild(dir1,1);

INodeDirectory * dir2 = new INodeDirectory("/ddd/ee");
fsImage.getRoot()->addChild(dir2,1);
/**/

    fsImage.saveImage();
}

void FSNameSystem::loadNameSpace(){
    FSImage fsImage(Config::get("pfs,imgFile"));

    fsImage.loadImage();


/**/
    map<string,INode*>::iterator iter = fsImage.getRoot()->getChildren()->begin();

    for(;iter != fsImage.getRoot()->getChildren()->end();iter++) {
        cout<<iter->first<<endl;
    }

/**/
}
