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
            QString stringValue(lua_Integer pos, int base, DataType::Type type, QSysInfo::Endian endian);
            QString stringValue(lua_Integer pos, int base, DataType::Type type);
            QSysInfo::Endian endian();
            bool willOverflow(lua_Integer pos, DataType::Type type, QSysInfo::Endian endian) const;
            bool willOverflow(lua_Integer pos, DataType::Type type) const;
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
            lua_Integer readType(lua_Integer pos, lua_Integer type);
            void writeType(lua_Integer pos, lua_Integer dt, lua_Integer val);

        private:
            template<typename DataType::Type TYPE> typename Data::ToType<TYPE>::Type readT(lua_Integer pos, QSysInfo::Endian endian) const
            {
                this->adjustOffset(pos);
                QByteArray ba = this->_hexeditdata->read(pos, DataType::sizeOf(TYPE));
                QDataStream ds(ba);

                if(endian == QSysInfo::LittleEndian)
                    ds.setByteOrder(QDataStream::LittleEndian);
                else /* if(endian == QSysInfo::BigEndian) */
                    ds.setByteOrder(QDataStream::BigEndian);

                typename Data::ToType<TYPE>::Type t;
                ds >> t;
                return t;
            }

            template<typename DataType::Type TYPE> bool willOverflowT(typename Data::ToType<TYPE>::Type val) const
            {
                if(std::numeric_limits< typename Data::ToType<TYPE>::Type >::is_signed)
                {
                    return static_cast<std::uintmax_t>(val) > static_cast<std::uintmax_t>(INTMAX_MAX) ||
                           static_cast<std::intmax_t>(val) < static_cast<std::intmax_t>(std::numeric_limits< typename Data::ToType<TYPE>::Type >::min()) ||
                           static_cast<std::intmax_t>(val) > static_cast<std::intmax_t>(std::numeric_limits< typename Data::ToType<TYPE>::Type >::max());
                }

                return (val < 0) || (val > static_cast<std::uintmax_t>(std::numeric_limits< typename Data::ToType<TYPE>::Type >::max()));
            }

        private:
            void writeInteger(lua_Integer pos, DataType::Type type, lua_Integer val);

        private:
            QHexEditData* _hexeditdata;
            QSysInfo::Endian _endian;
    };
}

#endif // PREFSDK_LIBBYTEBUFFER_H
