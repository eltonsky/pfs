#include "DataNode.h"

DataNode::DataNode()
{
    //ctor
}


DataNode::DataNode(string name,string storageID,short infoPort,long cap,
                   long remaining,long lastUpd,int xceiverCount){
    _name = name;
    _storageID = storageID;
    _infoPort = infoPort;
    _capacity = cap;
    _remaining = remaining;
    _lastUpdate = lastUpd;
    _xceiverCount = xceiverCount;

}


DataNode::~DataNode()
{
    //dtor
}
