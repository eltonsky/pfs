#ifndef INODE_H
#define INODE_H
#include "Block.h"
#include "Permission.h"
#include <iostream>
#include <string>
#include "Writable.h"

using namespace std;

class INode : public Writable
{
    public:
        INode();
        virtual ~INode();
        INode(string, short, long, int);
        INode(string);
        INode(INode*);

        string getPath () const;
        string getPath () {return _path;}
        int getBlockNum();
        short getReplication();
        long getBlockSize();
        long getModTime();
        long getAccessTime();
        Permission& getPermission();
        INode* getParent();

        void setModTime(long modTime);

        virtual void readFields(istream*);
        virtual void write(ostream*);

        void setPermission(Permission&);
        void setParent(INode*);

        long nsQuota; //max namespace allowed
        long dsQuota; //max diskspace allowed

        virtual bool isDirectory();
        virtual bool isFile();
        virtual void print(bool recursive);

        bool operator==(const INode& other) const;
    protected:
        long _modTime;
        long _accessTime;
        string _path;
        short _replication;
        long _blockSize;
        int _numBlocks;

        Permission _perm;

        INode* _parent; // must be a INodeDirectory

    private:


};


struct INodeCompare {
  bool operator() (INode* lhs, INode* rhs) const
  {return lhs->getPath().compare(rhs->getPath())<0;}
};



#endif // INODE_H
