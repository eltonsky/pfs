#ifndef FSIMAGE_H
#define FSIMAGE_H
#include <iostream>
#include <fstream>
#include <memory>
#include <set>
#include "Log.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>
#include <exception>
#include <semaphore.h>
#include <time.h>
#include "DataNode.h"
#include "INodeFileUnderConstruction.h"
#include "INodeDirectory.h"
#include "INodeFile.h"

class FSImage
{
    public:
        FSImage();
        virtual ~FSImage();

        FSImage(string imgFile);

        void loadImage();
        void saveImage();
        void saveINode(INode*, ofstream*);
        void saveINodeWrap(INode*, ofstream*);

        INodeDirectory* getParent(string path, INodeDirectory* root);

        INodeDirectory* findByPath(vector<string>*, INodeDirectory*);

        void setVersion(float);
        float getVersion();
        void setFile(string);
        string getFile();
        void setNamespaceID(int);
        int getNamespaceID();
        void setNumFiles(int);
        int getNumFiles();
        void setGenStamp(long);
        long getGenStamp();

        INodeDirectory* getRoot();

        void replaceRoot(INode*);

        void addFile(shared_ptr<INode>, bool protect, bool inheritPerm);

        // test
        static void print(INode*);
    protected:

        float _imgVersion;
        string _imageFile;
        int _namespaceId;
        int _numFiles;
        long _genStamp;
        INodeDirectory* _root;

        vector<DataNode*> dataNodes;

        vector<INodeFileUnderConstruction*> fileUnderConst;

    private:
        bool _ready = false;
        boost::mutex m_log1;
        boost::condition_variable m_cond1;
        boost::mutex::scoped_lock m_lock1;
        sem_t _sem_image;

        void waitForReady();
};

#endif // FSIMAGE_H





