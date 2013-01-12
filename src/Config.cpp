#include "Config.h"
using namespace tinyxml2;
using namespace std;

Config::Config(){}


vector<shared_ptr<XMLDocument>> Config::_docs;

void Config::load(string file){

    XMLDocument* doc = new XMLDocument();

    if (doc->LoadFile(file.c_str()) != XML_SUCCESS){
       throw "Fail to load config file" +file;
    }

    shared_ptr<XMLDocument> sDoc(doc);
    _docs.push_back(sDoc);

}


void Config::load(vector<string> files){

    for(vector<string>::iterator iter = files.begin(); iter != files.end(); iter++) {

        XMLDocument* doc = new XMLDocument();

        if (doc->LoadFile((*iter).c_str()) != XML_SUCCESS){
           throw "Fail to load config file" + *iter;
        }

        shared_ptr<XMLDocument> sDoc(doc);
        _docs.push_back(sDoc);
    }
}


const char* Config::get(string names) {
    XMLElement* elmt = _getElement(names);

    if(elmt==NULL){
        Log::write(WARN, "can not find elmnt in " + names+ ". is there white space around it?");
        return string().c_str();
    }

    return elmt->GetText();
}


const char* Config::get(string names, char* attr){
    XMLElement* elmt = _getElement(names);

    if(elmt==NULL){
        Log::write(WARN, "can not find elmnt in " + names+ ". is there white space around it?");
        return string().c_str();
    }

    return elmt->Attribute(attr);
}


/*
assume there's no duplicate conf name across all config files.
*/
XMLElement* Config::_getElement(string names){
    vector <string> fields;
    XMLElement* elmt;
    boost::split( fields, names, boost::is_any_of( "," ) );

    for(vector<shared_ptr<XMLDocument>>::iterator iter = _docs.begin(); iter != _docs.end(); iter++) {
        elmt =iter->get()->FirstChildElement((char*)fields[0].c_str());

        if(elmt == NULL)
            continue;

        if(fields.size() > 1)
            for(size_t i = 1; i<fields.size();i++) {
                elmt = elmt->FirstChildElement((char*)fields[i].c_str());

                if (elmt == NULL) {
                    Log::write(WARN, "can not find elmnt '" + fields[i]+ "' in " + names+ ". is there white space around it?");
                    // let the exception thrown.
                }
            }
    }

    return elmt;
}


Config::~Config()
{
    //dtor
}





