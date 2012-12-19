#include<iostream>
#include "tinyxml2.h"
#include <boost/algorithm/string.hpp>
#include <vector>
using namespace std;
using namespace tinyxml2;

#ifndef CONFIG_H
#define CONFIG_H

class Config
{
    public:
        Config();
        virtual ~Config();
        Config(const char* filename);

        const char* get(string names);
        const char* get(string names, char* attr);
    protected:

    private:
        XMLElement* getElement(string names);
        XMLDocument doc;
};

#endif // CONFIG_H
