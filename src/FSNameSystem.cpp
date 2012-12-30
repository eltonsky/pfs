#include "FSNameSystem.h"

FSNameSystem::FSNameSystem()
{
    _fsImage = new FSImage(Config::get("pfs,imgFile"));
    _fsEditLog = new FSEditLog(_fsImage);
}

FSNameSystem::~FSNameSystem()
{
    //dtor
}


void FSNameSystem::initialize(){
    m_lock1 = boost::mutex::scoped_lock(m_log1);


}


void FSNameSystem::saveNameSpace(){

/**/
_fsImage->setNumFiles(4);
INodeFile* file = new INodeFile("/abc.txt", 3, 1024, 1);
Block* blk = new Block(111,222,1234567);
file->addBlock(blk);
_fsImage->getRoot()->addChild(file,1);

INodeDirectory * dir1 = new INodeDirectory("/ddd");
_fsImage->getRoot()->addChild(dir1,1);

INodeDirectory * dir2 = new INodeDirectory("/ddd/ee");
_fsImage->getRoot()->addChild(dir2,1);
/**/

    _fsImage->saveImage();
}

void FSNameSystem::loadNameSpace(){
    _fsImage->loadImage();


/**/
    map<string,INode*>::iterator iter = _fsImage->getRoot()->getChildren()->begin();

    for(;iter != _fsImage->getRoot()->getChildren()->end();iter++) {
        cout<<iter->first<<endl;
    }

/**/
}


/*wait until fsimage finished loading.*/
void FSNameSystem::waitForReady() {
    while (!_ready) {
      try {
        m_cond1.timed_wait(m_lock1,boost::posix_time::milliseconds(5000));
      } catch (exception& e) {
          Log::write(INFO,e.what());
      }
    }
  }






