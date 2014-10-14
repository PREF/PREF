#include "disassemblerworker.h"

DisassemblerWorker::DisassemblerWorker(QHexEditData *hexeditdata, ProcessorLoader *loader, LogWidget *logwidget, QLabel *infolabel, QObject *parent): QThread(parent), _listing(nullptr), _hexeditdata(hexeditdata), _loader(loader), _logwidget(logwidget), _infolabel(infolabel)
{
    loader->moveToThread(this);
}

DisassemblerListing *DisassemblerWorker::listing() const
{
    return this->_listing;
}

void DisassemblerWorker::run()
{
    Logger* logger = new Logger(this->_logwidget);

    this->_listing = new DisassemblerListing(this->_hexeditdata);
    this->_loader->callMap(this->_listing, this->_listing->data(), logger);
    this->_loader->disassemble(this->_infolabel, this->_listing->data());

    this->_listing->moveToThread(qApp->instance()->thread());
    this->_loader->moveToThread(qApp->instance()->thread());

    QMetaObject::invokeMethod(this->_infolabel, "setText", Qt::QueuedConnection, Q_ARG(QString, "Done"));
}
