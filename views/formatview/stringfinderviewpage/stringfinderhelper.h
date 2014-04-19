#ifndef STRINGFINDERHELPER_H
#define STRINGFINDERHELPER_H

#include <QtCore>
#include "prefsdk/io/bytebuffer.h"

using namespace PrefSDK;

class StringFinderHelper : public QObject
{
    Q_OBJECT

    public:
        explicit StringFinderHelper(QObject *parent = 0);

    public slots:
        void stop();
        void run(QHexEditData *hexeditdata, int minlength, int maxlength);

    private:
        void updateProgressBar(QHexEditData *hexeditdata, qreal newval);

    signals:
        void finished();
        void progressChanged(int);
        void stringFound(quint64 offset, QString string);

    private:
        bool _cancontinue;
        int _lastperc;
};

#endif // STRINGFINDERHELPER_H
