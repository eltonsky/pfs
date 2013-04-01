#ifndef METHOD_H
#define METHOD_H

#include "Writable.h"
#include "IntWritable.h"
#include "MethodWritable.h"
#include <thread>


using namespace std;

class Method
{
    public:
        Method();
        virtual ~Method();

        static shared_ptr<Writable> call(string class_, string method, vector<shared_ptr<Writable>> params);
        static shared_ptr<Writable> getNewInstance(string class_);
    protected:
    private:
};

#endif // METHOD_H
