#ifndef BASICWORKER_H
#define BASICWORKER_H

#include <QThread>
#include <qhexedit/qhexeditdata.h>

class BasicWorker : public QThread
{
    Q_OBJECT

    public:
        explicit BasicWorker(QHexEditData *hexeditdata, QObject *parent = 0);

    public slots:
        void abort();

    protected:
        QHexEditData* _hexeditdata;
        bool _cancontinue;
};

#endif // BASICWORKER_H
