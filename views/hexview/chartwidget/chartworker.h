#ifndef CHARTWORKER_H
#define CHARTWORKER_H

#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"

class ChartWorker : public QThread
{
    Q_OBJECT

    public:
        explicit ChartWorker(QObject *parent = 0);
        const QList<qint64>& occurrences() const;
        void setData(QHexEditData *hexeditdata);

    protected:
        virtual void run();

    private:
        QList<qint64> _occurrences;
        QHexEditData* _hexeditdata;
};

#endif // CHARTWORKER_H
