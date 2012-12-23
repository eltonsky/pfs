#include "Log.h"
#include "tinyxml2.h"
#include "INodeDirectory.h"
#include "INodeFile.h"
#include <stddef.h>
#include "FSImage.h"
#include "FSNameSystem.h"

using namespace tinyxml2;

int main(int argc, char** argv) {
//    Log::init("/home/eltonsky/cpp/pfs/conf/pfs.log.properties");
//
//    Log::write("abc");
//
//    Log::write(ERROR, "ccc");
//
//    Log::write(WARN, "ddd");

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

    //init:
    //load config
    Config::load(string("/home/eltonsky/cpp/pfs/conf/pfs_base.cfg"));
    const char* logConf = Config::get("pfs,log_conf");

    cout<<"configure file for log is :"<<logConf<<", "<<Config::get("pfs,imgFile")<<endl;

    // log
    Log::init(logConf);

    //start head node

    FSNameSystem fsnamesys;

    fsnamesys.saveNameSpace();

    fsnamesys.loadNameSpace();

//    fsnamesys.loadNameSpace();

    //FSImage fsImage("/home/eltonsky/cpp/pfs/image/fsImage.0");

    //fsImage.loadImage();
    //start 2nd head node

    //start data nodes

//TODO: change INode.children to use map for search.


	return 0;
}


