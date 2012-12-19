#ifndef INODE_H
#define INODE_H
#include "Block.h"
#include "Permission.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class INode
{
    public:
        INode();
        virtual ~INode();
        INode(string, short, long, int);
        INode(string);

        string getPath();
        int getBlockNum();
        short getReplication();
        long getBlockSize();
        void setModTime(long modTime);

        void readFileds(ifstream*);

        long nsCount; //namespace consumed
        long dsCount; //diskspace consumed

    protected:
        long _modTime;
        long _accessTime;
        string _path;
        short _replication;
        long _blockSize;
        int _numBlocks;

        Permission perm;

    private:


};


struct INodeCompare {
  bool operator() (INode* lhs, INode* rhs) const
  {return lhs->getPath().compare(rhs->getPath())<0;}
};



#endif // INODE_H
