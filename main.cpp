#include "Log.h"
#include "tinyxml2.h"
#include "INodeDirectory.h"
#include "INodeFile.h"
#include <stddef.h>
#include "FSImage.h"
#include "FSNameSystem.h"
#include <thread>
#include <iostream>

using namespace std;
using namespace tinyxml2;

void f(int id)
{
    for(int n=0; n<10000; ++n)
        std::cout << "Output from thread " << id << '\n';
}

int main(int argc, char** argv) {

//XMLDocument doc;
//if (doc.LoadFile("/home/eltonsky/cpp/pfs/conf/pfs_base.cfg") == XML_SUCCESS)
//   {
//       cout<<"haha"<<endl;
//    }
//else
//    cout<<"woowoo"<<endl;
////
////    doc.LoadFile("/home/eltonsky/cpp/pfs/conf/abc");
////
////    // Text is just another Node to TinyXML-2. The more
////    // general way to get to the XMLText:
//    const char* title = doc.FirstChildElement( "pfs" )->FirstChildElement( "log_conf" ) ->GetText();
//    printf( "Name of play (1): %s\n", title );

//std::thread t1(f,1);
//t1.join();

    //init:
    //load config
    Config::load(string("/home/eltonsky/cpp/pfs/conf/pfs_base.cfg"));
    const char* logConf = Config::get("pfs,log_conf");
    // log
    Log::init(logConf);


    cout<<"configure file for log is :"<<logConf<<", "<<Config::get("pfs,imgFile,path")<<","<<Config::get("pfs,imgFile,version")<<endl;

    //start head node
    FSNameSystem::getFSNamesystem().initialize();

//    FSNameSystem::getFSNamesystem().saveNameSpace();

    FSNameSystem::getFSNamesystem().loadNameSpace();

///**/
//INodeFileUnderConstruction fileUnder("/d1/f99", 3, 1223343,2048,1,3);
//Block blk(999,222999,991234567);
//fileUnder.addBlock(blk);
//FSNameSystem::getFSNamesystem().getEditLog()->logOpenFile(fileUnder.getPath(),fileUnder);
//
//INodeFile file("/f101", 3, 1024, 2);
//Block blk01(10011,2222,12345671);
//file.addBlock(blk01);
//Block blk02(10013,2223,12345673);
//file.addBlock(blk02);
//FSNameSystem::getFSNamesystem().getEditLog()->logCloseFile(file.getPath(),file);
//
//INodeFileUnderConstruction fileUnder1("/d1/f100", 4,555555,4048,1,5);
//Block blk1(1099,102999,881234567);
//fileUnder1.addBlock(blk1);
//FSNameSystem::getFSNamesystem().getEditLog()->logOpenFile(fileUnder1.getPath(),fileUnder1);
//
//
//FSNameSystem::getFSNamesystem().getEditLog()->logSync();
//
//FSNameSystem::getFSNamesystem().getEditLog()->close();
///**/
//
//    FSNameSystem::getFSNamesystem().getEditLog()->loadEdits();
//


    FSNameSystem::getFSNamesystem().getFSImage()->getRoot()->print(true);


    //FSImage fsImage("/home/eltonsky/cpp/pfs/image/fsImage.0");

    //fsImage.loadImage();
    //start 2nd head node

    //start data nodes

//TODO: change INode.children to use map for search.


	return 0;
}


