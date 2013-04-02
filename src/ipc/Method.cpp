#include "Method.h"
#include "NameNode.h"

Method::Method()
{
    //ctor
}


shared_ptr<Writable> Method::call(string c, string m, vector<shared_ptr<Writable>> params) {

    Log::write(INFO, "Method::call : class %s, method %s\n", c.c_str(), m.c_str());

    if(c == "FSNameSystem") {
        if(m == "create") {
            return make_shared<IntWritable>(32);
        }
    } else if(c == "NameNode") {
        if(m == "create") {
            /// For test ... load port from config
            NameNode nn(1234);

            return nn.create(dynamic_pointer_cast<StringWritable>(params[0]),
                             dynamic_pointer_cast<IntWritable>(params[1]),
                             dynamic_pointer_cast<Permission>(params[2]));
        }
    }

    return NULL;
}


shared_ptr<Writable> Method::getNewInstance(string class_) {
    if(class_ == "IntWritable") {
        return shared_ptr<Writable>(new IntWritable());
    } else if(class_ == "MethodWritable"){
        return shared_ptr<Writable>(new MethodWritable());
    } else if(class_ == "StringWritable"){
        return shared_ptr<Writable>(new StringWritable());
    }

    return shared_ptr<Writable>(new Writable());
}


Method::~Method()
{
    //dtor
}
