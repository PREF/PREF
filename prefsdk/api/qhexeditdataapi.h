#ifndef PREFSDK_API_QHEXEDITDATAAPI_H
#define PREFSDK_API_QHEXEDITDATAAPI_H

#include "qhexedit/qhexeditdata.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            QHexEditData* QHexEditData_fromFile(const char* filename);
            int64_t QHexEditData_length(QHexEditData* __this);
            int64_t QHexEditData_indexOf(QHexEditData* __this, QByteArray* ba, int64_t start);
            u_int8_t QHexEditData_at(QHexEditData* __this, int64_t pos);
            QByteArray* QHexEditData_read(QHexEditData* __this, int64_t pos, int64_t len);
            void QHexEditData_append(QHexEditData* __this, QByteArray *ba);
            void QHexEditData_insert(QHexEditData* __this, int64_t pos, QByteArray* ba);
            void QHexEditData_remove(QHexEditData* __this, int64_t pos, int64_t len);
            void QHexEditData_replace(QHexEditData* __this, int64_t pos, int64_t len, QByteArray* ba);
            void QHexEditData_save(QHexEditData* __this);
        }
    }
}

#endif // PREFSDK_API_QHEXEDITDATAAPI_H
