#ifndef DATANODE_H
#define DATANODE_H
#include <string>
using namespace std;

class DataNode
{
    public:
        DataNode();
        DataNode(string,string,short,long,long,long,int);
        virtual ~DataNode();
    protected:
        string _name;
        string _storageID;
        short _infoPort;

        long _capacity;
        long _remaining;
        long _lastUpdate;
        int _xceiverCount;

    private:
};

#endif // DATANODE_H
