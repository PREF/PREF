#ifndef PREFSDK_PREFEXCEPTION_H
#define PREFSDK_PREFEXCEPTION_H

#include <QString>
#include <exception>
#include "debugdialog/debugdialog.h"
#include "prefsdk/luastate.h"

namespace PrefSDK
{
    class PrefException : public std::exception
    {
        public:
            PrefException(const QString& msg) throw();
            const char* what() const throw();

        private:
            QString _msg;
    };
}

#endif // PREFSDK_PREFEXCEPTION_H
