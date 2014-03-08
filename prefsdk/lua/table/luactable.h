#ifndef LUA_LUACTABLE_H
#define LUA_LUACTABLE_H

#include <memory>
#include <map>
#include <lua.hpp>
#include "luatable.h"
#include "../function/luacmethod.h"
#include "../function/luacfunction.h"

namespace Lua
{
    class LuaCTable: public LuaObject
    {
        public:
            typedef std::shared_ptr<LuaCTable> Ptr;

        private:
            struct FieldItem { LuaTypes::LuaType Type; void* FieldAddress; };
            typedef std::map<lua_String, FieldItem, Private::StringComparator> FieldMap;

        public:
            LuaCTable(lua_State* l, lua_String name);
            LuaCTable(const LuaTable::Ptr& t);
            virtual ~LuaCTable();
            lua_String name();
            virtual void pushGlobal();

        public:
            operator LuaTable::Ptr();
            LuaCTable& operator=(const LuaTable::Ptr& rhs);

        private:
            void generateTable(lua_State* l);
            static int staticMetaIndex(lua_State* l);
            static int staticMetaNewIndex(lua_State* l);
            static int staticMetaLength(lua_State* l);
            static int staticMetaToString(lua_State* l);
            static int staticMetaEqual(lua_State* l);
            static int staticMetaLessThan(lua_State* l);
            static int staticMetaLessOrEqual(lua_State* l);
            static int staticMetaConcat(lua_State* l);

        private:  /* Overriden Methods */
            virtual void pushGlobal(lua_String name);

        public: /* Overriden Methods */
            virtual lua_Integer length() const;
            virtual LuaTypes::LuaType type() const;
            virtual void push();

        protected:
            virtual void metaIndex(lua_State* l);
            virtual void metaNewIndex(lua_State* l);
            virtual bool metaEqual(lua_State* l);
            virtual bool metaLessThan(lua_State* l);
            virtual bool metaLessOrEqual(lua_State* l);
            virtual void metaConcat(lua_State*l);
            virtual lua_String metaToString(lua_State*l);

        private:
            lua_Integer metaLength();

        protected:
            template<typename ReturnType, typename Self, typename... Args> void exportMethod(lua_String name, ReturnType (Self::*func)(Args...))
            {
                typedef LuaCMethodT<ReturnType, Self, Args...> CMethod;

                typename CMethod::Ptr p = CMethod::create(this->state(), dynamic_cast<Self*>(this), func);
                this->_table->set(name, std::dynamic_pointer_cast<LuaCMethod>(p));
            }

            template<typename ReturnType, typename... Args> void exportFunction(lua_String name, ReturnType(*func)(Args...))
            {
                typedef LuaCFunctionT<ReturnType, Args...> CFunction;

                typename CFunction::Ptr p = CFunction::create(this->state(), func);
                this->_table->set(name, std::dynamic_pointer_cast<LuaCFunction>(p));
            }

            template<typename... Args> void exportConstructor(LuaTable::Ptr(*ctor)(Args...))
            {
                this->exportFunction<LuaTable::Ptr, Args...>("new", ctor);
            }

            template<typename K, typename V> void exportConstant(K key, V value)
            {
                this->_table->set(key, value);
            }

            template<typename T> void exportField(lua_String name, T* value)
            {
                LuaTypes::LuaType lt = TypeOf<T>::LuaType;

                if(lt == LuaTypes::None)
                    throw LuaException("Unsupported Exported Field Type");

                this->_fieldmap[name] = {lt, value};
            }

        private:
            FieldMap _fieldmap;
            lua_String _name;

        protected:
            LuaTable::Ptr _table;
    };

    template<> struct TypeOf<LuaCTable::Ptr>
    {
        typedef LuaCTable::Ptr Type;
        static constexpr LuaTypes::LuaType LuaType = LuaTypes::CTable;
        static constexpr const char* TypeName = "CTable";
    };

    template<> struct ValueExtractor<LuaCTable::Ptr>
    {
        static LuaCTable::Ptr get(lua_State *l)
        {
            LuaCTable::Ptr val;
            *val = LuaTable::fromStack(l, -1);

            lua_pop(l, 1);
            return val;
        }
    };

    void luaT_getvalue(lua_State *l, int index, LuaCTable::Ptr &v);
    void luaT_pushvalue(lua_State *, const LuaCTable::Ptr& v);
}

#endif // LUA_LUACTABLE_H
