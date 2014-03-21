#ifndef FORMATOBJECT_H
#define FORMATOBJECT_H

#include <QtCore>
#include "prefsdk/qlua.h"
#include "prefsdk/io/bytebuffer.h"

namespace PrefSDK
{
    using namespace Lua;

    class FormatObject : public QObject, public LuaCTable
    {
        Q_OBJECT

        private:
            typedef LuaFunctionT<lua_String, LuaTable::Ptr, LuaTable::Ptr> InfoProcedure;

        public:
            enum FormatObjectType {UndefinedType, StructureType, FieldType, FieldArrayType, BitFieldType};
            explicit FormatObject(lua_State* l, lua_String tablename, lua_Integer offset, QString name, ByteBuffer* bytebuffer, LuaCTable* model, FormatObject* parentobject = 0, QObject *parent = 0);
            bool containsOffset(lua_Integer offset);
            void staticInfo(QString s);
            int base();

        public:
            virtual QString displayName();
            virtual QString info();
            virtual void setBase(int b);
            virtual FormatObject* parentObject();

        public: /* Abstract Methods */
            virtual QString displayType() = 0;
            virtual QString displayValue() = 0;
            virtual lua_Integer value() = 0;
            virtual FormatObject::FormatObjectType objectType() = 0;
            virtual lua_Integer size() = 0;

        lua_public:
            LuaTable::Ptr model();
            QString name();
            LuaTable::Ptr getParent();
            LuaTable::Ptr getParent(lua_String name);
            void staticInfo(lua_String s);
            void dynamicInfo(LuaFunction::Ptr infoproc);
            lua_Integer offset();
            lua_Integer endOffset();

        signals:
            void baseChanged(FormatObject* sender);

        private:
            int _base;
            lua_Integer _offset;
            lua_Integer _baseoffset;
            QString _name;
            QString _staticinfo;
            FormatObject* _parentobject;
            LuaFunction::Ptr _infoprocedure;

        protected:
            ByteBuffer* _bytebuffer;
            LuaCTable* _model;
    };
}

#endif // FORMATOBJECT_H
