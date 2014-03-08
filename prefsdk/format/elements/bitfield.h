#ifndef BITFIELD_H
#define BITFIELD_H

#include <QtCore>
#include <QtWidgets>
#include "prefsdk/format/elements/fieldobject.h"

namespace PrefSDK
{
    class BitField : public FieldObject
    {
        public:
            BitField(lua_State *l, int bitstart, int bitend, lua_Integer offset, QString name, ByteBuffer* bytebuffer, LuaCTable* model, FieldObject* parentobject, QObject *parent = 0);
            int bitStart() const;
            int bitEnd() const;

        private:
            quint32 createMask() const;

        public: /* Overriden methods */
            virtual QString typeName();
            virtual lua_Integer value();
            virtual QString displayName();
            virtual QString displayValue();
            virtual FormatObject::FormatObjectType objectType();
            virtual lua_Integer size();

        private:
            int _bitstart;
            int _bitend;
            quint32 _mask;
    };
}
#endif // BITFIELD_H
