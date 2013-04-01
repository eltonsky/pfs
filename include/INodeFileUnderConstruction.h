#ifndef INODEFILEUNDERCONSTRUCTION_H
#define INODEFILEUNDERCONSTRUCTION_H
#include "Permission.h"
#include "DataNode.h"
#include "INodeFile.h"
#include <string>
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

class INodeFileUnderConstruction : public INodeFile
{
    public:
        INodeFileUnderConstruction();
        INodeFileUnderConstruction(string,short,long,long,int,int);
        virtual ~INodeFileUnderConstruction();

        void setLocation(shared_ptr<DataNode> loc);
        void setPreferredBlkSize(long prefBlkSize) {_preferredBlockSize = prefBlkSize;}
        long getPreferredBlkSize() {return _preferredBlockSize;}

        virtual void write(ostream*);
        virtual void readFields(istream*);

        string getClientName() {return _clientName;}
        string getClientMachine() {return _clientMachine;}
        void setClientName(string name) {_clientName = name;}
        void setClientMachine(string machine) {_clientMachine = machine;}

    protected:
        long _preferredBlockSize;

        string _clientName;
        string _clientMachine;
        int _numLocations;

        vector<shared_ptr<DataNode>> _locations;

    private:
};

#endif // INODEFILEUNDERCONSTRUCTION_H
