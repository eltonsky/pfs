#ifndef FSEDITLOG_H
#define FSEDITLOG_H
#include <semaphore.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "INodeFileUnderConstruction.h"
#include "INodeFile.h"
#include "Log.h"
#include "Config.h"
#include <exception>
#include <boost/lexical_cast.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>
#include "FSImage.h"
#include "time.h"

using namespace std;

enum OpenMode {IN,OUT};

#define OP_INVALID  -1
#define OP_ADD  0
#define OP_RENAME 1 // rename
#define OP_DELETE 2  // delete
#define OP_MKDIR 3   // create directory
#define OP_SET_REPLICATION 4 // set replication
//the following two are used only for backward compatibility :
//@Deprecated  OP_DATANODE_ADD = 5;
//@Deprecated  OP_DATANODE_REMOVE = 6;
#define OP_SET_PERMISSIONS 7
#define OP_SET_OWNER 8
#define OP_CLOSE  9    // close after write
#define OP_SET_GENSTAMP  10   // store genstamp
/* The following two are not used any more. Should be removed once
* LAST_UPGRADABLE_LAYOUT_VERSION is -17 or newer. */
#define OP_SET_NS_QUOTA  11 // set namespace quota
#define OP_CLEAR_NS_QUOTA  12 // clear namespace quota
#define OP_TIMES  13 // sets mod & access time on a file
#define OP_SET_QUOTA  14 // sets name and disk quotas.
#define OP_GET_DELEGATION_TOKEN  18 //new delegation token
#define OP_RENEW_DELEGATION_TOKEN  19 //renew delegation token
#define OP_CANCEL_DELEGATION_TOKEN  20 //cancel delegation token
#define OP_UPDATE_MASTER_KEY  21


class FSEditLog
{
    public:
        FSEditLog(int);
        FSEditLog(string, int);
        FSEditLog(FSImage*);
        virtual ~FSEditLog();

        void logSync();
        void logEdit(stringstream*);
        void close();
        void open(OpenMode);

        void logOpenFile(string path, INodeFileUnderConstruction& newNode);
        void logCloseFile(string path, INodeFile& newNode);
        void logMkDir(string path, INode& newNode);
        void logRename(string src, string dest, long ts);
        void logReplication(string src, short replication);
        void logSetQuota(string src, long nsQuota, long dsQuota);
        void logDelete(string src, long timestamp);

        void createEditLogFile(string file);

        void loadEdits();
    protected:
        float _version;
        FSImage* _fsImage;

        sem_t _sem_log;
        bool _isSyncRunning;

        ofstream* _editOStream;
        ifstream* _editIStream;

        boost::mutex m_log1;
        boost::condition_variable m_cond1;
        boost::mutex::scoped_lock m_lock1;

    private:
};

#endif // FSEDITLOG_H
