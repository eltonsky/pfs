#include "StringWritable.h"


StringWritable::StringWritable()
{
    //ctor
}


StringWritable::StringWritable(string val) : _value(val) {
}


StringWritable::~StringWritable()
{
    //dtor
}


int StringWritable::write(tcp::socket * sock) {
    if(sock == NULL)
        return -1;

    size_t length = Writable::writeString(sock, _value);

    return length;
}


int StringWritable::readFields(tcp::socket * sock){

    if(sock == NULL)
        return -1;

    _value = Writable::readFields(sock);

    return _value.length();
}


string StringWritable::printToString() {
    return _value;
}


string StringWritable::toString() {
    return _value;
}



