#ifndef CHARTWORKER_H
#define CHARTWORKER_H

#include <QtCore>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/math.h"

using namespace PrefSDK;

class ChartWorker : public QThread
{
    Q_OBJECT

    public:
        explicit ChartWorker(QObject *parent = 0);
        const OccurrenceList& occurrences() const;
        void setData(QHexEditData *hexeditdata);

    protected:
        virtual void run();

    private:
        OccurrenceList _occurrences;
        QHexEditData* _hexeditdata;
};

#endif // CHARTWORKER_H
