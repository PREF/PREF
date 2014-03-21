#ifndef PREFSDK_STRUCTURE_H
#define PREFSDK_STRUCTURE_H

#include <QObject>
#include <QStandardItemModel>
#include <QVector>
#include "prefsdk/format/elements/field.h"
#include "prefsdk/format/elements/fieldarray.h"
#include "prefsdk/format/elements/strings/asciistring.h"
#include "prefsdk/format/elements/strings/unicodestring.h"

namespace PrefSDK
{
    class Structure: public FormatObject
    {
        Q_OBJECT

        public:
            typedef QList<lua_Integer> OffsetList;
            typedef QHash<lua_Integer, FormatObject*> FieldMap;
            typedef QHash<QString, FormatObject*> StringMap;

        public:
            explicit Structure(lua_State *l, lua_Integer offset, QString name, ByteBuffer* bytebuffer, LuaCTable* model, FormatObject* parentobject = 0, QObject *parent = 0);
            Structure* addStructure(const QString& name);
            Structure* addStructure(const QString& name, lua_Integer offset);
            FieldObject *addField(lua_Integer datatype, const QString& name);
            FieldArray* addField(lua_Integer datatype, lua_Integer count, const QString& name);
            bool hidden() const;
            lua_Integer fieldCount();
            FormatObject* field(int i) const;
            FormatObject* find(const QString& name);
            int indexOf(FormatObject* f) const;

        protected:
            virtual void metaIndex(lua_State* l);

        lua_private:
            LuaTable::Ptr addStructure(lua_String name);
            LuaTable::Ptr addStructure(lua_String name, lua_Integer offset);
            LuaTable::Ptr addField(lua_Integer datatype, lua_String name);
            LuaTable::Ptr addField(lua_Integer elementtype, lua_Integer count, lua_String name);
            LuaTable::Ptr find(lua_String fieldname);

        public: /* Overriden Methods */
            virtual void setBase(int base);
            virtual QString displayType();
            virtual QString displayValue();
            virtual lua_Integer value();
            virtual FormatObject::FormatObjectType objectType();
            virtual lua_Integer size();

        protected:
            OffsetList _offsetlist;
            FieldMap _fieldmap;
            StringMap _stringmap;
    };
}

#endif // PREFSDK_STRUCTURE_H
