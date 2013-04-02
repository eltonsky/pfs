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


int Permission::readFields(tcp::socket * sock) {
    if(sock == NULL)
        return -1;

    _userName = Writable::readString(sock);

    _groupName = Writable::readString(sock);

    size_t l = boost::asio::read(*sock,
            boost::asio::buffer(&_perm, sizeof(_perm)));

    if(l != sizeof(_perm)) {
        Log::write(ERROR,
                   "StringWritable::readFields: expected length %d, read length %d\n",
                   sizeof(_perm), l);
        return -1;
    }

    return 0;
}


int Permission::write(tcp::socket * sock){

    if(sock == NULL)
        return -1;

    Writable::writeString(sock, _userName);

    Writable::writeString(sock, _groupName);

    size_t l = boost::asio::write(*sock, boost::asio::buffer((const char*)&_perm, sizeof(_perm)));

    if(l != sizeof(_perm)) {
        Log::write(ERROR,
                   "StringWritable::write: expected length %d, write length %d\n",
                   sizeof(_perm), l);
        return -1;
    }

    return 0;
}


string Permission::toString() {
    stringstream ss;
    ss<< " user name: " << _userName << ", group name: " << _groupName << ", perm : " << _perm;
    return ss.str();
}


std::ostream& operator<<(std::ostream &strm, const Permission &p) {
  return strm << " user name: " << p._userName << ", group name: " << p._groupName << ", perm : " << p._perm ;
}






