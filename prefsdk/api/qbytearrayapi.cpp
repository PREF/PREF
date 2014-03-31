#include "qbytearrayapi.h"

namespace PrefSDK
{
    namespace API
    {
        QByteArray *QByteArray_new()
        {
            return new QByteArray();
        }

        void QByteArray_delete(QByteArray *__thethis)
        {
            delete __thethis;
        }

        int QByteArray_length(QByteArray *__thethis)
        {
            return __thethis->length();
        }

        int8_t QByteArray_at(QByteArray *__thethis, int64_t pos)
        {
            return __thethis->at(pos);
        }

        bool QByteArray_eq(QByteArray *__thethis, QByteArray *ba)
        {
            return (*__thethis == *ba);
        }

        const char *QByteArray_toString(QByteArray *__thethis)
        {
            return __thethis->constData();
        }
    }
}
