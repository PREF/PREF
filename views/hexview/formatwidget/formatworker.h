#ifndef FORMATWORKER_H
#define FORMATWORKER_H

#include <QtCore>
#include "prefsdk/sdkmanager.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/format/formattree.h"
#include "qhexedit/qhexeditdata.h"
#include "views/hexview/worker.h"

using namespace PrefSDK;

class FormatWorker : public Worker
{
    Q_OBJECT

    public:
        explicit FormatWorker(QObject *parent = 0);
        void setData(QHexEditData* hexeditdata, FormatList::FormatId formatid, qint64 baseoffset);
        FormatTree* tree();

    public: /* Overriden Methods */
        virtual void abort();

    protected:
        virtual void run();

    private:
        QHexEditData* _hexeditdata;
        FormatTree* _formattree;
        FormatList::FormatId _formatid;
        qint64 _baseoffset;
        lua_State* _state;
        lua_State* _thread;
};

#endif // FORMATWORKER_H
