#ifndef DISASSEMBLERWORKER_H
#define DISASSEMBLERWORKER_H

#include <QtCore>
#include <QtGui>
#include <lua.hpp>
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/sdkmanager.h"
#include "prefsdk/format/formatlist.h"

using namespace PrefSDK;

class DisassemblerWorker : public QThread
{
    Q_OBJECT

    public:
        explicit DisassemblerWorker(QObject *parent = 0);
        void setData(QHexEditData* hexeditdata);
        qint64 instructionCount() const;

    protected:
        virtual void run();

    signals:
        void error(QString errmsg);

    private:
        QHexEditData* _hexeditdata;
        qint64 _instructioncount;
};

#endif // DISASSEMBLERWORKER_H
