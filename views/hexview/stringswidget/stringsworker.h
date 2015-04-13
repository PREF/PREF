#ifndef STRINGSWORKER_H
#define STRINGSWORKER_H

#include <QtCore>
#include <support/byteelaborator.h>
#include "views/hexview/worker.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/qdatabuffer.h"

using namespace PrefLib::Support;

class StringsWorker : public Worker
{
    Q_OBJECT

    public:
        explicit StringsWorker(QObject *parent = 0);
        void setData(QHexEditData* databuffer);
        const ByteElaborator::StringList& stringList() const;

    protected:
        virtual void run();

    signals:
        void searchState(bool b);

    private:
        QHexEditData* _hexeditdata;
        ByteElaborator::StringList _stringlist;
};

#endif // STRINGSWORKER_H
