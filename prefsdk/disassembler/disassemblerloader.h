#ifndef PREFSDK_DISASSEMBLERLOADER_H
#define PREFSDK_DISASSEMBLERLOADER_H

#include "../qlua.h"
#include "disassemblersegment.h"
#include "processor/processordefinition.h"

namespace PrefSDK
{
    class DisassemblerLoader : public LuaCTable
    {
        public:
            typedef std::shared_ptr<DisassemblerLoader> Ptr;

        private:
            typedef QPair<lua_Integer, QString> EntryPair;

        public:
            DisassemblerLoader(lua_State *l);
            void addSegment(lua_Integer baseaddress, lua_Integer start, lua_Integer end, QString name, DisassemblerSegment::SegmentType type);
            DisassemblerSegment& segment(lua_Integer address);
            bool inSegment(lua_Integer address);
            void addEntry(lua_Integer address, QString name);
            lua_Integer entryAddress(int i);
            QString entryName(int i);
            int entryCount();
            ProcessorDefinition::Ptr processor();

        public:
            static DisassemblerLoader::Ptr create(lua_State* l);

        lua_private:
            void addSegment(lua_Integer baseaddress, lua_Integer start, lua_Integer end, lua_String name, lua_Integer type);
            void addEntry(lua_Integer address, lua_String name);

        private:
            QVector<DisassemblerSegment> _segments;
            QVector<EntryPair> _entries;
            ProcessorDefinition::Ptr _processordef;
    };
}

#endif // PREFSDK_DISASSEMBLERLOADER_H
