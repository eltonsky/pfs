#ifndef PERMISSION_H
#define PERMISSION_H
#include <iostream>
#include "Writable.h"
using namespace std;

class Permission
{
    public:
        Permission();
        Permission(Permission*);
        Permission(Permission&);
        virtual ~Permission();

        void readFields(istream*);

        void write(ostream*);

        string getUserName() {return _userName;}
        string getGroupName() {return _groupName;}
        short getPerm() {return _perm;}

        friend std::ostream& operator<<(std::ostream&, const Permission&);

        void print();
    protected:
        string _userName;
        string _groupName;
        short _perm;
    private:
};

#endif // PERMISSION_H
