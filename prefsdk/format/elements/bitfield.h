#ifndef PREFSDK_BITFIELD_H
#define PREFSDK_BITFIELD_H

#include <QtCore>
#include <QtWidgets>
#include "prefsdk/format/elements/fieldelement.h"

namespace PrefSDK
{
    class BitField : public FieldElement
    {
        public:
            BitField(lua_State *l, int bitstart, int bitend, lua_Integer offset, QString name, ByteBuffer* bytebuffer, LuaCTable* model, FieldElement* parentobject, QObject *parent = 0);
            int bitStart() const;
            int bitEnd() const;

        private:
            quint32 createMask() const;

        public: /* Overriden methods */
            virtual QString displayType();
            virtual lua_Integer value();
            virtual QString displayName();
            virtual QString displayValue();
            virtual FormatElement::FormatObjectType objectType();
            virtual lua_Integer size();

        private:
            int _bitstart;
            int _bitend;
            quint32 _mask;
    };
}
#endif // PREFSDK_BITFIELD_H
