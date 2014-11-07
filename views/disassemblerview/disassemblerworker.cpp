#include "disassemblerworker.h"

DisassemblerWorker::DisassemblerWorker(QHexEditData *hexeditdata, DisassemblerDefinition *disassemblerdefinition, LogWidget *logwidget, QLabel *infolabel, QObject *parent): QThread(parent), _listing(nullptr), _hexeditdata(hexeditdata), _disassemblerdefinition(disassemblerdefinition), _logwidget(logwidget), _infolabel(infolabel)
{
    disassemblerdefinition->moveToThread(this);
}

DisassemblerListing *DisassemblerWorker::listing() const
{
    return this->_listing;
}

void DisassemblerWorker::run()
{
    Logger* logger = new Logger(this->_logwidget);

    this->_listing = new DisassemblerListing(this->_hexeditdata);
    this->_disassemblerdefinition->callMap(this->_listing, this->_listing->data(), logger);
    this->_disassemblerdefinition->callDisassemble(this->_infolabel);

    this->_listing->moveToThread(qApp->instance()->thread());
    this->_disassemblerdefinition->moveToThread(qApp->instance()->thread());

    QMetaObject::invokeMethod(this->_infolabel, "setText", Qt::QueuedConnection, Q_ARG(QString, "Done"));
}
