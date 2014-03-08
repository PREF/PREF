#include "disassemblerloader.h"

namespace PrefSDK
{
    DisassemblerLoader::DisassemblerLoader(lua_State* l): LuaCTable(l, "DisassemblerLoader")
    {
        this->exportField("processor", reinterpret_cast<LuaTable::Ptr*>(&this->_processordef));
        this->exportMethod<void, DisassemblerLoader, lua_Integer, lua_String>("addEntry", &DisassemblerLoader::addEntry);
        this->exportMethod<void, DisassemblerLoader, lua_Integer, lua_Integer, lua_Integer, lua_String, lua_Integer>("addSegment", &DisassemblerLoader::addSegment);
    }

    void DisassemblerLoader::addSegment(lua_Integer baseaddress, lua_Integer start, lua_Integer end, QString name, DisassemblerSegment::SegmentType type)
    {
        this->_segments.append(DisassemblerSegment(baseaddress, start, end, name, type));
    }

    void DisassemblerLoader::addEntry(lua_Integer address, QString name)
    {
        this->_entries.append(EntryPair(address, name));
    }

    lua_Integer DisassemblerLoader::entryAddress(int i)
    {
        return this->_entries[i].first;
    }

    QString DisassemblerLoader::entryName(int i)
    {
        return this->_entries[i].second;
    }

    int DisassemblerLoader::entryCount()
    {
        return this->_entries.count();
    }

    DisassemblerSegment &DisassemblerLoader::segment(lua_Integer address)
    {
        for(int i = 0; i < this->_segments.count(); i++)
        {
            if((address >= this->_segments[i].start()) && (address < this->_segments[i].end()))
                return this->_segments[i];
        }

        throw std::runtime_error("Segment Not Found");
    }

    bool DisassemblerLoader::inSegment(lua_Integer address)
    {
        for(int i = 0; i < this->_segments.count(); i++)
        {
            if((address >= this->_segments[i].start()) && (address < this->_segments[i].end()))
                return true;
        }

        return false;
    }

    ProcessorDefinition::Ptr DisassemblerLoader::processor()
    {
        return this->_processordef;
    }

    DisassemblerLoader::Ptr DisassemblerLoader::create(lua_State *l)
    {
        return DisassemblerLoader::Ptr(new DisassemblerLoader(l));
    }

    void DisassemblerLoader::addSegment(lua_Integer baseaddress, lua_Integer start, lua_Integer end, lua_String name, lua_Integer type)
    {
        this->addSegment(baseaddress, start, end, QString::fromLatin1(name), static_cast<DisassemblerSegment::SegmentType>(type));
    }

    void DisassemblerLoader::addEntry(lua_Integer address, lua_String name)
    {
        this->addEntry(address, QString::fromLatin1(name));
    }
}
