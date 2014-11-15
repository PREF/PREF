#include "disassemblerworker.h"

DisassemblerWorker::DisassemblerWorker(QHexEditData *hexeditdata, DisassemblerDefinition *disassembler, LogWidget *logwidget, QLabel *infolabel, const QString &loadedfile, bool canloaddatabase, QObject *parent): QThread(parent), _hexeditdata(hexeditdata), _disassembler(disassembler), _logwidget(logwidget), _infolabel(infolabel), _loadedfile(loadedfile), _canloaddatabase(canloaddatabase)
{
    disassembler->moveToThread(this);
}

void DisassemblerWorker::run()
{
    Logger* logger = new Logger(this->_logwidget);
    this->_disassembler->setLogger(logger);

    if(this->_disassembler->callMap(this->_hexeditdata))
        this->_disassembler->callDisassemble(this->_infolabel);

    if(this->_canloaddatabase && DisassemblerDatabase::exists(this->_loadedfile))
    {
        QMetaObject::invokeMethod(this->_infolabel, "setText", Qt::QueuedConnection, Q_ARG(QString, "Loading Database..."));

        DisassemblerDatabase dd(this->_disassembler->listing()->symbolTable(), DisassemblerDatabase::adjustFile(this->_loadedfile));
        dd.load();
    }

    this->_disassembler->moveToThread(qApp->instance()->thread());
    QMetaObject::invokeMethod(this->_infolabel, "setText", Qt::QueuedConnection, Q_ARG(QString, "Ready"));
}
