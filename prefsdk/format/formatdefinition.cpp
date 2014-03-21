#include "formatdefinition.h"

namespace PrefSDK
{
    FormatDefinition::FormatDefinition(const LuaTable::Ptr &t): _formatdeftable(t)
    {

    }

    lua_State *FormatDefinition::state() const
    {
        return this->_formatdeftable->state();
    }

    QString FormatDefinition::name() const
    {
        return QString::fromLatin1(this->_formatdeftable->get<lua_String, lua_String>("name"));
    }

    QString FormatDefinition::author() const
    {
        return QString::fromLatin1(this->_formatdeftable->get<lua_String, lua_String>("author"));
    }

    QString FormatDefinition::version() const
    {
        return QString::fromLatin1(this->_formatdeftable->get<lua_String, lua_String>("version"));
    }

    QSysInfo::Endian FormatDefinition::endian() const
    {
        return static_cast<QSysInfo::Endian>(this->_formatdeftable->get<lua_String, lua_Integer>("endian"));
    }

    int FormatDefinition::optionsCount() const
    {
        LuaTable::Ptr options = this->_formatdeftable->get<lua_String, LuaTable::Ptr>("options");
        return options->length();
    }

    QString FormatDefinition::option(int i) const
    {
        LuaTable::Ptr options = this->_formatdeftable->get<lua_String, LuaTable::Ptr>("options");
        LuaTable::Ptr opt = options->get<lua_Integer, LuaTable::Ptr>(i);
        return QString::fromLatin1(opt->get<lua_String, lua_String>("name"));
    }

    QString FormatDefinition::endianString() const
    {
        QSysInfo::Endian e = this->endian();

        switch(e)
        {
            case QSysInfo::LittleEndian:
                return "Little Endian";

            case QSysInfo::BigEndian:
                return "Big Endian";

            default:
                break;
        }

        return "<Undefined>";
    }

    QString FormatDefinition::category() const
    {
        return QString::fromLatin1(this->_formatdeftable->get<lua_String, lua_String>("category"));
    }

    bool FormatDefinition::hasOptions() const
    {
        return this->optionsCount() > 0;
    }

    bool FormatDefinition::hasCategory() const
    {
        return !this->category().isEmpty();
    }

    bool FormatDefinition::validateFormat(ByteBuffer *bytebuffer, lua_Integer baseoffset)
    {
        ByteBuffer* bb = nullptr;
        bytebuffer->setEndian(this->endian());

        if(baseoffset != bytebuffer->baseOffset())
            bb = bytebuffer->clone(baseoffset);
        else
            bb = bytebuffer;

        try
        {
            return this->_formatdeftable->call<bool, LuaTable::Ptr>("validateFormat", *bb);
        }
        catch(LuaException& e)
        {
            PrefDebug::dbgprint(e.what());
        }

        return false;
    }

    FormatModel *FormatDefinition::parseFormat(ByteBuffer *bytebuffer, lua_Integer baseoffset) const
    {
        ByteBuffer* bb = nullptr;
        bytebuffer->setEndian(this->endian());

        if(baseoffset != bytebuffer->baseOffset())
            bb = bytebuffer->clone(baseoffset);
        else
            bb = bytebuffer;

        try
        {
            FormatTree::Ptr formattree = FormatTree::create(bytebuffer);
            this->_formatdeftable->call<void, LuaTable::Ptr, LuaTable::Ptr>("parseFormat", formattree->luaTable(), *bb);
            return new FormatModel(formattree, bb);
        }
        catch(LuaException& e)
        {
            PrefDebug::dbgprint(e.what());
        }

        return nullptr;
    }

    void FormatDefinition::executeOption(int optindex, FormatTree::Ptr formattree, ByteBuffer* bytebuffer)
    {
        try
        {
            this->_formatdeftable->call<void, lua_Integer, LuaTable::Ptr, LuaTable::Ptr>("executeOption", optindex, formattree->luaTable(), *bytebuffer);
        }
        catch(LuaException& e)
        {
            PrefDebug::dbgprint(e.what());
        }
    }

    DisassemblerLoader::Ptr FormatDefinition::generateLoader(FormatTree::Ptr formattree, ByteBuffer *bytebuffer)
    {
        DisassemblerLoader::Ptr dl = DisassemblerLoader::create(this->_formatdeftable->state());

        try
        {
            this->_formatdeftable->call<void, LuaTable::Ptr, LuaTable::Ptr, LuaTable::Ptr>("generateLoader", *dl, formattree->luaTable(), *bytebuffer);
        }
        catch(LuaException& e)
        {
            PrefDebug::dbgprint(e.what());
        }

        return dl;
    }
}
