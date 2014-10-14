#include "prefexception.h"

namespace PrefSDK
{
    PrefException::PrefException(const QString& msg) throw(): std::exception(), _msg(msg)
    {
        if(!DebugDialog::instance())
            DebugDialog::createInstance(LuaState::instance());

        QMetaObject::invokeMethod(DebugDialog::instance(), "out", Qt::QueuedConnection, Q_ARG(QString, msg));
        QMetaObject::invokeMethod(DebugDialog::instance(), "exec", Qt::BlockingQueuedConnection);
    }

    const char *PrefException::what() const throw()
    {
        return this->_msg.toUtf8().constData();
    }
}
