#include "Writable.h"

Writable::Writable()
{
    //ctor
}

Writable::~Writable()
{
    //dtor
}


void Writable::writeString(ostream* ofs, string str){
    int length = str.size()+1;

    ofs->write((char*)&length, sizeof(length));

    if (length > 1)
        ofs->write(str.c_str(), length);

}

string Writable::readString(istream* ifs) {
    int length = 0;

    ifs->read((char*)&length, sizeof(length));

    if(length > 1) {
        char* buf = new char[length];
        ifs->read(buf, length);
        return string(buf);
    }

    return string("");
}


void Writable::write(ofstream* ofs) {

}

void Writable::readFields(ifstream* ifs) {

}
