#ifndef PREFSDK_FORMATVALIDATOR_H
#define PREFSDK_FORMATVALIDATOR_H

#include <QtCore>
#include "prefsdk/type/datatype.h"
#include "qhexedit/qhexeditdatareader.h"
#include "prefsdk/libs/preflib/databuffer.h"
#include "prefsdk/luastate.h"

namespace PrefSDK
{
    class FormatValidator : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(PrefSDK::DataBuffer* buffer READ buffer)
        Q_PROPERTY(bool validated READ validated WRITE setValidated)

        public:
            explicit FormatValidator(QHexEditData* hexeditdata, qint64 baseoffset, QObject *parent = 0);
            bool validated() const;
            void setValidated(bool b);

        private:
            PrefSDK::DataBuffer* buffer();
            bool checkInt8(lua_Integer offset, qint8 expected, bool throwerr);
            bool checkInt16(lua_Integer offset, qint16 expected, int byteorder, bool throwerr);
            bool checkInt32(lua_Integer offset, qint32 expected, int byteorder, bool throwerr);
            bool checkInt64(lua_Integer offset, qint64 expected, int byteorder, bool throwerr);
            bool checkUInt8(lua_Integer offset, quint8 expected, bool throwerr);
            bool checkUInt16(lua_Integer offset, quint16 expected, int byteorder, bool throwerr);
            bool checkUInt32(lua_Integer offset, quint32 expected, int byteorder, bool throwerr);
            bool checkUInt64(lua_Integer offset, quint64 expected, int byteorder, bool throwerr);

        public slots:
            bool checkAscii(lua_Integer offset, const QString& expected, bool throwerr = true);
            bool checkType(lua_Integer offset, lua_Integer expected, lua_Integer datatype, bool throwerr = true);
            void expect(const QString& expect, const QString& found, const QString &at);
            void error(const QString& msg);

        private:
            QHexEditData* _hexeditdata;
            QHexEditDataReader* _reader;
            DataBuffer* _databuffer;
            qint64 _baseoffset;
            bool _validated;
    };
}

#endif // PREFSDK_FORMATVALIDATOR_H
