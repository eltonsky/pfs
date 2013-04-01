#ifndef FSNAMESYSTEM_H
#define FSNAMESYSTEM_H
#include "FSImage.h"
#include "FSEditLog.h"
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>

class FSNameSystem
{
    public:
        virtual ~FSNameSystem();

        void saveNameSpace();
        void loadNameSpace();

        void initialize();
        static FSNameSystem& getFSNamesystem() {
            static FSNameSystem fssys;

            return fssys;
        }

        FSImage* getFSImage() {return _fsImage;}
        FSEditLog* getEditLog() {return _fsEditLog;}

    protected:
        FSImage* _fsImage;
        FSEditLog* _fsEditLog;

    private:
        FSNameSystem();

         // Stop the compiler generating methods of copy the object
        FSNameSystem(FSNameSystem const&);
        void operator=(FSNameSystem const&);
};

#endif // FSNAMESYSTEM_H
