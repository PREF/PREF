#include "debugobject.h"

namespace PrefSDK
{
    DebugObject::DebugObject(QObject *parent): QObject(parent), _hexeditdata(nullptr)
    {

    }

    QHexEditData *DebugObject::buffer()
    {
        return this->_hexeditdata;
    }

    void DebugObject::bind(QHexEditData *hexeditdata)
    {
        this->_hexeditdata = hexeditdata;
    }

    void DebugObject::unbind()
    {
        this->_hexeditdata = nullptr;
    }

    void DebugObject::log(const QString &msg)
    {
        AbstractView* view = LoadedViews::instance()->view(this->_hexeditdata);
        view->log(msg);
    }

    void DebugObject::logline(const QString &msg)
    {
        AbstractView* view = LoadedViews::instance()->view(this->_hexeditdata);
        view->logLine(msg);
    }

    void DebugObject::notice(const QString &msg)
    {
        AbstractView* view = LoadedViews::instance()->view(this->_hexeditdata);
        view->logLine(msg, LogWidget::Notice);
    }

    void DebugObject::warning(const QString &msg)
    {
        AbstractView* view = LoadedViews::instance()->view(this->_hexeditdata);
        view->logLine(msg, LogWidget::Warning);
    }

    void DebugObject::error(const QString &msg)
    {
        AbstractView* view = LoadedViews::instance()->view(this->_hexeditdata);
        view->logLine(msg, LogWidget::Error);
    }
}
