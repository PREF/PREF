#include "disassemblerworker.h"

DisassemblerWorker::DisassemblerWorker(QHexEditData *hexeditdata, DisassemblerDefinition *disassembler, LogWidget *logwidget, QLabel *infolabel, QObject *parent): QThread(parent), _hexeditdata(hexeditdata), _disassembler(disassembler), _logwidget(logwidget), _infolabel(infolabel)
{
    disassembler->moveToThread(this);
}

void DisassemblerWorker::run()
{
    Logger* logger = new Logger(this->_logwidget);
    this->_disassembler->setLogger(logger);

    if(this->_disassembler->callMap(this->_hexeditdata))
        this->_disassembler->callDisassemble(this->_infolabel);

    this->_disassembler->moveToThread(qApp->instance()->thread());
    QMetaObject::invokeMethod(this->_infolabel, "setText", Qt::QueuedConnection, Q_ARG(QString, "Done"));
}
