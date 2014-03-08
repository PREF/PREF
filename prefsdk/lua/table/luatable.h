#ifndef LUA_LUATABLE_H
#define LUA_LUATABLE_H

#include <string>
#include <map>
#include <vector>
#include <cstring>
#include "../luareference.h"
#include "../utils/safecall.h"
#include "../utils/overloadtable.h"
#include "../utils/callhelper.h"
#include "../function/luacmethod.h"
#include "../function/luacfunction.h"
#include "../function/luamethod.h"
#include "../function/luafunction.h"

namespace Lua
{
    class LuaTable : public LuaReference
    {
        private:
            template<typename K, typename V> struct ValueSetter
            {
                inline void operator()(LuaTable* t, K key, V value)
                {
                    t->push();
                    luaT_pushvalue(t->state(), key);
                    luaT_pushvalue(t->state(), value);
                    lua_settable(t->state(), -3);
                    lua_pop(t->state(), 1);
                }
            };

            template<typename K> struct LuaFunctionSetter
            {
                inline void operator()(LuaTable* t, K key, lua_CFunction value, void* upvalue)
                {
                    t->push();
                    luaT_pushvalue(t->state(), key);

                    lua_pushlightuserdata(t->state(), upvalue);
                    lua_pushcclosure(t->state(), value, 1);

                    lua_settable(t->state(), -3);
                    lua_pop(t->state(), 1);
                }
            };

            template<typename K> struct ReferenceSetter
            {
                inline void operator()(LuaTable* t, K key, LuaReference::Ptr value)
                {
                    t->push();
                    luaT_pushvalue(t->state(), key);
                    value->push();
                    lua_settable(t->state(), -3);
                    lua_pop(t->state(), 1);
                }
            };

            template<typename K> struct MethodSetter
            {
                inline void operator()(LuaTable* t, K key, LuaCMethod::Ptr value)
                {
                    t->push();
                    luaT_pushvalue(t->state(), key);
                    value->push();
                    lua_settable(t->state(), -3);
                    lua_pop(t->state(), 1);
                }
            };

            template<typename K> struct FunctionSetter
            {
                inline void operator()(LuaTable* t, K key, LuaCFunction::Ptr value)
                {
                    t->push();
                    luaT_pushvalue(t->state(), key);
                    value->push();
                    lua_settable(t->state(), -3);
                    lua_pop(t->state(), 1);
                }
            };

        public:
            typedef std::shared_ptr<LuaTable> Ptr;

        private:
            static const lua_String COBJECT_FIELD;

        public:
            class Iterator
            {
                private:
                    Iterator(LuaTable* owner, bool finished): _owner(owner), _finished(finished), _keyref(LUA_NOREF), _valueref(LUA_NOREF) { }
                    Iterator(LuaTable* owner): _owner(owner), _finished(false), _keyref(LUA_NOREF), _valueref(LUA_NOREF)
                    {
                        owner->push();
                        lua_pushnil(owner->state());
                        this->nextPair();
                    }

                public:
                    Iterator(): _owner(nullptr), _finished(false), _keyref(LUA_NOREF), _valueref(LUA_NOREF) { }

                    Iterator(const Iterator& it): _owner(it._owner), _finished(it._finished)
                    {
                         *this = it;
                    }

                    ~Iterator()
                    {
                        this->unrefKey();
                        this->unrefValue();
                    }

                    Iterator& operator++(int)
                    {
                        this->unrefValue();

                        this->_owner->push();
                        lua_rawgeti(this->_owner->state(), LUA_REGISTRYINDEX, this->_keyref);
                        this->nextPair();

                        return *this;
                    }

                    LuaTable::Iterator& operator=(const LuaTable::Iterator& rhs)
                    {
                        if(*this != rhs)
                        {
                            this->_finished = rhs._finished;
                            this->_owner = rhs._owner;
                            this->_keyref = rhs.cloneRef(rhs._keyref);
                            this->_valueref = rhs.cloneRef(rhs._valueref);
                        }

                        return *this;
                    }

                    bool operator ==(const LuaTable::Iterator& it) const
                    {
                        return (this->_keyref == it._keyref) && (this->_valueref == it._valueref);
                    }

                    bool operator !=(const LuaTable::Iterator& it) const
                    {
                        return (this->_keyref != it._keyref) || (this->_valueref != it._valueref);
                    }

                    LuaTypes::LuaType keyType() const
                    {
                        lua_rawgeti(this->_owner->state(), LUA_REGISTRYINDEX, this->_keyref);
                        LuaTypes::LuaType t = luaT_typeof(this->_owner->state(), -1);
                        lua_pop(this->_owner->state(), 1);
                        return t;
                    }

                    LuaTypes::LuaType valueType() const
                    {
                        lua_rawgeti(this->_owner->state(), LUA_REGISTRYINDEX, this->_valueref);
                        LuaTypes::LuaType t = luaT_typeof(this->_owner->state(), -1);
                        lua_pop(this->_owner->state(), 1);
                        return t;
                    }

                    template<typename T> T key() const
                    {
                        lua_rawgeti(this->_owner->state(), LUA_REGISTRYINDEX, this->_keyref);
                        return ValueExtractor<T>::get(this->_owner->state());
                    }

                    template<typename T> T value() const
                    {
                        lua_rawgeti(this->_owner->state(), LUA_REGISTRYINDEX, this->_valueref);
                        return ValueExtractor<T>::get(this->_owner->state());
                    }

                    lua_String keyString() const
                    {
                        lua_rawgeti(this->_owner->state(), LUA_REGISTRYINDEX, this->_keyref);

                        lua_String ks = luaT_typevalue(this->_owner->state(), -1);
                        lua_pop(this->_owner->state(), 1);
                        return ks;
                    }

                    lua_String valueString() const
                    {
                        lua_rawgeti(this->_owner->state(), LUA_REGISTRYINDEX, this->_valueref);

                        lua_String ks = luaT_typevalue(this->_owner->state(), -1);
                        lua_pop(this->_owner->state(), 1);
                        return ks;
                    }

                private:
                    void unrefKey()
                    {
                        if(this->_keyref != LUA_NOREF)
                        {
                            luaL_unref(this->_owner->state(), LUA_REGISTRYINDEX, this->_keyref);
                            this->_keyref = LUA_NOREF;
                        }
                    }

                    void unrefValue()
                    {
                        if(this->_valueref != LUA_NOREF)
                        {
                            luaL_unref(this->_owner->state(), LUA_REGISTRYINDEX, this->_valueref);
                            this->_valueref = LUA_NOREF;
                        }
                    }

                    int cloneRef(int ref) const
                    {
                        if(ref == LUA_NOREF)
                            return ref;

                        lua_rawgeti(this->_owner->state(), LUA_REGISTRYINDEX, ref);
                        return luaL_ref(this->_owner->state(), LUA_REGISTRYINDEX);
                    }

                    void nextPair()
                    {
                        if(lua_next(this->_owner->state(), -2))
                        {
                            this->unrefKey();

                            this->_valueref = luaL_ref(this->_owner->state(), LUA_REGISTRYINDEX);
                            this->_keyref = luaL_ref(this->_owner->state(), LUA_REGISTRYINDEX);
                        }
                        else
                        {
                            this->_keyref = LUA_NOREF;
                            this->_valueref = LUA_NOREF;
                            this->_finished = true;
                        }

                        lua_pop(this->_owner->state(), 1); /* Pop owner */
                    }

                private:
                    LuaTable* _owner;
                    bool _finished;
                    int _keyref;
                    int _valueref;

                friend class LuaTable;
            };

        private:
            void stripSelf(lua_State* l, int &argcount);
            static lua_CFunction methodDispatcher();
            static lua_CFunction functionDispatcher();

        public:
            LuaTable(lua_State* l);
            LuaTable(lua_State* l, lua_String name);
            LuaTable(lua_State* l, int index);
            LuaTable(lua_State *l, const LuaTable::Ptr& meta);
            static LuaTable::Ptr create(lua_State* l);
            static LuaTable::Ptr create(lua_State *l, const LuaTable::Ptr& meta);
            static LuaTable::Ptr fromStack(lua_State* l, int index);
            static LuaTable::Ptr global(lua_State *l, lua_String name);

            template<typename T> bool containsKey(T key)
            {
                this->push();
                luaT_pushvalue(this->state(), key);
                lua_gettable(this->state(), -2);

                bool res = !lua_isnil(this->state(), -1);
                lua_pop(this->state(), 2);
                return res;
            }

            bool containsKey(const LuaReference::Ptr& key);

            template<typename K, typename V> V get(K key)
            {
                this->push();
                luaT_pushvalue(this->state(), key);
                lua_gettable(this->state(), -2);

                V v = ValueExtractor<V>::get(this->state());
                lua_pop(this->state(), 1);
                return v;
            }

            template<typename K> LuaTable::Ptr get(K key)
            {
                this->push();
                luaT_pushvalue(this->state(), key);
                lua_gettable(this->state(), -2);

                LuaTable::Ptr t(new LuaTable(this->state(), -1));
                lua_pop(this->state(), 2);
                return t;
            }

            template<typename T> T* me()
            {
                lua_LightUserData thethis = this->get<lua_String, lua_LightUserData>(LuaTable::COBJECT_FIELD);
                return reinterpret_cast<T*>(thethis);
            }

            template<typename ReturnType, typename... Args> ReturnValue<ReturnType> call(lua_String name, Args... args)
            {
                std::string s = name;
                Utils::Mangler::MangledNameT<Args...>()(&s);

                this->push();
                luaT_pushvalue(this->state(), name);
                lua_gettable(this->state(), -2);

                if(this->_tfoverloads.contains(name, s.c_str())) /* Is a Static Function? */
                {
                    typename LuaFunctionT<ReturnType, Args...>::Ptr f = LuaFunctionT<ReturnType, Args...>::fromStack(this->state(), -1);
                    lua_pop(this->state(), 2); /* Pop the Value and the table itself */
                    Utils::CallHelper<ReturnType, Args...> ch(f.get());
                    return ch(nullptr, args...);
                }

                typename LuaMethodT<ReturnType, Args...>::Ptr m = LuaMethodT<ReturnType, Args...>::fromStack(this->state(), -1);
                lua_pop(this->state(), 2); /* Pop the Value and the table itself */
                Utils::CallHelper<ReturnType, Args...> ch(m.get());
                return ch(this, args...);
            }

            void set(lua_Number key, lua_CFunction value, void* upvalue);
            void set(lua_String key, lua_CFunction value, void* upvalue);
            void set(lua_Number key, lua_LightUserData value);
            void set(lua_String key, lua_LightUserData value);
            void set(lua_Number key, lua_Number value);
            void set(lua_String key, lua_String value);
            void set(lua_String key, lua_Number value);
            void set(lua_Number key, lua_String value);
            void set(lua_Number key, LuaTable::Ptr value);
            void set(lua_String key, LuaTable::Ptr value);
            void set(lua_Number key, LuaCFunction::Ptr value);
            void set(lua_String key, LuaCFunction::Ptr value);
            void set(lua_Number key, LuaCMethod::Ptr value);
            void set(lua_String key, LuaCMethod::Ptr value);

            bool isEmpty();
            LuaTable::Iterator begin();
            LuaTable::Iterator end();
            void setMe(lua_LightUserData thethis);
            int itemCount();

            friend class LuaCTable;

        private:
            Utils::OverloadTable<LuaCMethod::Ptr> _tmoverloads;
            Utils::OverloadTable<LuaCFunction::Ptr> _tfoverloads;
    };

    template<> struct TypeOf<LuaTable::Ptr>
    {
        typedef LuaTable::Ptr Type;
        static constexpr LuaTypes::LuaType LuaType = LuaTypes::Table;
        static constexpr const char* TypeName = "Table";
    };

    template<> struct ValueExtractor<LuaTable::Ptr>
    {
        static LuaTable::Ptr get(lua_State *l)
        {
            LuaTable::Ptr val = LuaTable::fromStack(l, -1);
            lua_pop(l, 1);
            return val;
        }
    };

    void luaT_getvalue(lua_State *l, int index, LuaTable::Ptr &v);
    void luaT_pushvalue(lua_State *, const LuaTable::Ptr& v);

    template<typename T> LuaTable::Ptr luatable_cast(std::shared_ptr<T> t)
    {
        return std::static_pointer_cast<LuaTable>(t);
    }
}

#endif // LUA_LUATABLE_H
