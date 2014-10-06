#include "prefexception.h"

namespace PrefSDK
{
    PrefException::PrefException(const QString& msg) throw(): std::exception(), _msg(msg)
    {
        if(!DebugDialog::instance())
            DebugDialog::createInstance(LuaState::instance());

        DebugDialog::instance()->out(msg)->exec();
    }

    const char *PrefException::what() const throw()
    {
        return this->_msg.toUtf8().constData();
    }
}
