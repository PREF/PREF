#include "qhexeditdataapi.h"

namespace PrefSDK
{
    namespace API
    {
        QHexEditData *QHexEditData_fromFile(const char *filename)
        {
            return QHexEditData::fromFile(filename);
        }

        int64_t QHexEditData_length(QHexEditData *__this)
        {
            return __this->length();
        }

        int64_t QHexEditData_indexOf(QHexEditData *__this, QByteArray *ba, int64_t start)
        {
            return __this->indexOf(*ba, start);
        }

        uint8_t QHexEditData_at(QHexEditData *__this, int64_t pos)
        {
            return __this->at(pos);
        }

        QByteArray *QHexEditData_read(QHexEditData *__this, int64_t pos, int64_t len)
        {
            return new QByteArray(__this->read(pos, len));
        }

        void QHexEditData_append(QHexEditData *__this, QByteArray* ba)
        {
            __this->append(*ba);
        }

        void QHexEditData_insert(QHexEditData *__this, int64_t pos, QByteArray *ba)
        {
            __this->insert(pos, *ba);
        }

        void QHexEditData_remove(QHexEditData *__this, int64_t pos, int64_t len)
        {
            __this->remove(pos, len);
        }

        void QHexEditData_replace(QHexEditData *__this, int64_t pos, int64_t len, QByteArray *ba)
        {
            __this->replace(pos, len, *ba);
        }

        void QHexEditData_save(QHexEditData *__this)
        {
            __this->save();
        }

    }
}
