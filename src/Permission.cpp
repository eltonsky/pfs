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

void Permission::readFileds(ifstream* ifs) {
    _userName = Writable::readString(ifs);

    _groupName = Writable::readString(ifs);

    ifs->read((char*)&_perm, sizeof(_perm));
}


void Permission::write(ofstream* ofs) {

    Writable::writeString(ofs, _userName);

    Writable::writeString(ofs, _groupName);

    ofs->write((char*)&_perm, sizeof(_perm));
}
