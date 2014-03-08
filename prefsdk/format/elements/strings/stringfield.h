#ifndef PREFSDK_STRINGFIELD_H
#define PREFSDK_STRINGFIELD_H

#include "prefsdk/format/elements/fieldarray.h"

namespace PrefSDK
{
    class StringField : public FieldArray
    {
        Q_OBJECT

        public:
            explicit StringField(lua_State* l, DataType::Type itemtype, lua_Integer offset, QString name, quint64 itemcount, ByteBuffer* bytebuffer, LuaCTable* model, FormatObject* formatobject, DataType::Type type, QObject *parent = 0);

        public: /* Overriden methods */
            virtual QString displayName();
            virtual QString displayValue();
    };
}

#endif // PREFSDK_STRINGFIELD_H
