#include "Permission.h"

Permission::Permission()
{
    _userName.clear();
    _groupName.clear();
    _perm = 0;
}

Permission::~Permission()
{
    //dtor
}

void Permission::readFileds(istream* is) {
    _userName = Writable::readString(is);

    _groupName = Writable::readString(is);

    is->read((char*)&_perm, sizeof(_perm));
}


void Permission::write(ostream* os) {

    Writable::writeString(os, _userName);

    Writable::writeString(os, _groupName);

    os->write((char*)&_perm, sizeof(_perm));
}
