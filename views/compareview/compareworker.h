#ifndef COMPAREWORKER_H
#define COMPAREWORKER_H

#include <QtCore>
#include "qhexedit/qhexedit.h"

class CompareWorker : public QThread
{
    Q_OBJECT

    public:
        typedef QList<qint64> OffsetList;
        typedef QHash<qint64, qint64> DifferenceMap;

    public:
        explicit CompareWorker(QHexEdit* lefthexedit, QHexEdit* righthexedit, QObject *parent = 0);
        const CompareWorker::OffsetList& offsetList() const;
        const CompareWorker::DifferenceMap& differences() const;

    protected:
        virtual void run();

    private:
        QHexEdit* _lefthexedit;
        QHexEdit* _righthexedit;
        OffsetList _offsetlist;
        DifferenceMap _differencemap;
};

#endif // COMPAREWORKER_H
