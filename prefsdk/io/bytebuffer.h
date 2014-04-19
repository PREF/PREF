#ifndef PREFSDK_BYTEBUFFER_H
#define PREFSDK_BYTEBUFFER_H

#include <QtCore>
#include <limits>
#include <cstdint>
#include "prefsdk/datatype.h"
#include "prefsdk/io/abstractbuffer.h"
#include "qhexedit/qhexeditdata.h"

namespace PrefSDK
{
    using namespace Lua;

    class ByteBuffer: public AbstractBuffer
    {
        public:
            typedef std::shared_ptr<ByteBuffer> Ptr;

        public:
            ByteBuffer(lua_State* state, QHexEditData* hexeditdata, lua_Integer baseoffset = 0);
            virtual ~ByteBuffer();
            ByteBuffer* clone(lua_Integer baseoffset = -1);
            QHexEditData* hexEditData();
            QSysInfo::Endian endian();
            QString stringValue(lua_Integer pos, int base, lua_Integer datatype, QSysInfo::Endian endian);
            void setEndian(QSysInfo::Endian endian);
            void save(QString filename);
            operator QHexEditData*() const;

        protected:  /* Overriden Methods */
            virtual QIODevice *device();

        public: /* Overriden Methods */
            virtual void save();
            virtual void insert(qint64 pos, const QByteArray& ba);
            virtual void write(qint64 pos, qint64 len, const QByteArray& ba);
            virtual void remove(qint64 pos, qint64 len);
            virtual uchar at(qint64 i);
            virtual QByteArray read(qint64 pos, qint64 len);
            virtual lua_Integer length() const;

        lua_public:
            lua_Integer readType(lua_Integer pos, lua_Integer datatype, lua_Integer endian);
            lua_Integer readType(lua_Integer pos, lua_Integer type);
            void writeType(lua_Integer pos, lua_Integer dt, lua_Integer val);
            QString stringValue(lua_Integer pos, lua_Integer base, lua_Integer datatype);

        private:
            QHexEditData* _hexeditdata;
            QSysInfo::Endian _endian;
    };
}

#endif // PREFSDK_LIBBYTEBUFFER_H
