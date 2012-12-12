
#include "Log.h"
#include "tinyxml2.h"
//#include "INode.h"
#include "INodeDirectory.h"
#include <stddef.h>
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
    Config* conf = new Config("/home/eltonsky/cpp/pfs/conf/pfs_base.cfg");
    const char* logConf = conf->get("pfs,log_conf");

    cout<<"configure file for log is :"<<logConf<<endl;

    // log
    Log::init(logConf);

    //start head node

    //start 2nd head node

    //start data nodes



	return 0;
}


