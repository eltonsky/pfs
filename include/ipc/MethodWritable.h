#ifndef METHODWRITABLE_H
#define METHODWRITABLE_H

#include "Writable.h"
#include "IntWritable.h"
#include <string>
#include <boost/asio.hpp>
#include "Method.h"

using boost::asio::ip::tcp;

class MethodWritable : public Writable
{
    public:
        MethodWritable();
        MethodWritable(string,string,vector<shared_ptr<Writable>>);
        virtual ~MethodWritable();

        virtual int readFields(tcp::socket * sock);
        virtual int write(tcp::socket * sock);
        virtual string toString();
        virtual string printToString();
        virtual string getClass();

    protected:
    private:
        string _class_name;
        string _method_name;
        vector<shared_ptr<Writable>> _params;
};

#endif // METHODWRITABLE_H
