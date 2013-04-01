#include<iostream>
#include "tinyxml2.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <memory>
#include "Log.h"

using namespace std;
using namespace tinyxml2;

#ifndef CONFIG_H
#define CONFIG_H

class Config
{
    public:
        Config();
        virtual ~Config();

        static void load(vector<string> files);
        static void load(string file);

        static const char* get(string names);
        static const char* get(string names, char* attr);

    protected:
        static vector<shared_ptr<XMLDocument>> _docs;

    private:
        static XMLElement* _getElement(string names);
};

#endif // CONFIG_H
