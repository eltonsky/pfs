#include "Config.h"
using namespace tinyxml2;
using namespace boost;

Config::Config()
{
    //ctor
}

Config::Config(const char* filename){
    if (doc.LoadFile(filename) != XML_SUCCESS){
       throw "Fail to load config file!";
    }
}

const char* Config::get(string names) {
    return getElement(names)->GetText();
}


const char* Config::get(string names, char* attr){
    return getElement(names)-> Attribute(attr);
}


XMLElement* Config::getElement(string names){
    vector <string> fields;
    split( fields, names, is_any_of( "," ) );

    XMLElement* elmt = doc.FirstChildElement((char*)fields[0].c_str());
    if(fields.size() > 1)
        for(size_t i = 1; i<fields.size();i++) {
            elmt = elmt->FirstChildElement((char*)fields[i].c_str());
        }

    return elmt;
}


Config::~Config()
{
    //dtor
}





