#ifndef STRINGSWORKER_H
#define STRINGSWORKER_H

#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"

class StringsWorker : public QThread
{
    Q_OBJECT

    public:
        typedef QList<qint64> OffsetList;
        typedef QPair<qint64, qint64> StringRange;
        typedef QHash<qint64, StringRange> StringMap;

    public:
        explicit StringsWorker(QObject *parent = 0);
        void setData(QHexEditData* hexeditdata);
        const StringsWorker::OffsetList& offsetList() const;
        const StringsWorker::StringMap& strings() const;

    protected:
        virtual void run();

    signals:
        void searchState(bool b);

    private:
        static const qint64 MIN_LENGTH;
        static const qint64 MAX_LENGTH;
        QHexEditData* _hexeditdata;
        OffsetList _offsetlist;
        StringMap _strings;
};

#endif // STRINGSWORKER_H
