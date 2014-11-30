#ifndef CHARTWORKER_H
#define CHARTWORKER_H

#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/math.h"
#include "views/hexview/worker.h"

using namespace PrefSDK;

class ChartWorker : public Worker
{
    Q_OBJECT

    public:
        explicit ChartWorker(QObject *parent = 0);
        const OccurrenceList& occurrences() const;
        const QList<QPointF>& dataEntropy() const;
        void setData(QHexEditData *hexeditdata);

    private:
        quint64 calculateBlockSize();

    protected:
        virtual void run();

    signals:
        void occurrencesListCompleted();
        void dataEntropyCompleted();

    private:
        static const quint64 NUM_POINTS;
        static const quint64 MIN_BLOCK_SIZE;

    private:
        OccurrenceList _occurrences;
        QList<QPointF> _dataentropy;
        QHexEditData* _hexeditdata;
};

#endif // CHARTWORKER_H
