#ifndef CHARTHELPER_H
#define CHARTHELPER_H

#include <QtCore>
#include "qhexedit/qhexeditdata.h"

class ChartHelper: public QObject
{
    Q_OBJECT

    public:
        explicit ChartHelper(QObject *parent = 0);
        QList<qint64> occurrences();
        quint64 maxOccurrence();

    private:
        void initMap();
        void updateProgressBar(qint64 length, qreal newval);

    public slots:
        void run(QHexEditData *hexeditdata, qint64 start, qint64 end);
        void stop();

    private:
        int _lastperc;
        qint64 _maxocc;
        qint64 _start;
        qint64 _end;
        QHexEditData* _hexeditdata;
        QList<qint64> _occlist;
        bool _cancontinue;

    signals:
        void finished();
        void progressChanged(int);
};

#endif // CHARTHELPER_H
