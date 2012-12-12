#ifndef INODE_H
#define INODE_H

class INode
{
    public:
        INode();
        virtual ~INode();

        long nsCount; //namespace consumed
        long dsCount; //diskspace consumed
    protected:
        char* name;
        long modificationTime;
        long accessTime;
    private:
        long perm;

};

#endif // INODE_H
