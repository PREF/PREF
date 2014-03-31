#ifndef PREFSDK_API_QBYTEARRAYAPI_H
#define PREFSDK_API_QBYTEARRAYAPI_H

#include <QtCore>

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            QByteArray* QByteArray_new();
            void QByteArray_delete(QByteArray* __thethis);
            int QByteArray_length(QByteArray* __thethis);
            int8_t QByteArray_at(QByteArray *__thethis, int64_t pos);
            bool QByteArray_eq(QByteArray *__thethis, QByteArray* ba);
            const char* QByteArray_toString(QByteArray* __thethis);
        }
    }
}

#endif // PREFSDK_API_QBYTEARRAYAPI_H
