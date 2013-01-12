#include "Permission.h"

Permission::Permission()
{
    _userName.clear();
    _groupName.clear();
    _perm = 0;
}

Permission::Permission(Permission* perm)
{
    _userName = perm->getUserName();
    _groupName = perm->getGroupName();
    _perm = perm->getPerm();
}


Permission::Permission(Permission& perm)
{
    _userName = perm.getUserName();
    _groupName = perm.getGroupName();
    _perm = perm.getPerm();
}


Permission::~Permission()
{
    //dtor
}

void Permission::readFields(istream* is) {
    _userName = Writable::readString(is);

    _groupName = Writable::readString(is);

    is->read((char*)&_perm, sizeof(_perm));
}


void Permission::write(ostream* os) {

    Writable::writeString(os, _userName);

    Writable::writeString(os, _groupName);

    os->write((char*)&_perm, sizeof(_perm));
}


std::ostream& operator<<(std::ostream &strm, const Permission &p) {
  return strm << " user name: " << p._userName << ", group name: " << p._groupName << ", perm : " << p._perm ;
}




