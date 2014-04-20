#ifndef PREFSDK_PROCESSORDEFINITION_H
#define PREFSDK_PROCESSORDEFINITION_H

#include <utility>
#include <QtCore>
#include "prefsdk/qlua.h"
#include "instruction.h"
#include "prefsdk/disassembler/addressqueue.h"
#include "prefsdk/disassembler/outputbuffer.h"
#include "prefsdk/disassembler/referencetable.h"

namespace PrefSDK
{
    class ProcessorDefinition: public LuaTable
    {
        public:
            typedef std::shared_ptr<ProcessorDefinition> Ptr;

        public:
            ProcessorDefinition(lua_State* l);
            void initializePc(lua_Integer basevalue);
            void setPc(lua_Integer value);
            lua_Integer pc();

        lua_public:
            Instruction::InstructionFeatures features(lua_Integer itype);
            lua_Integer analyze(const Instruction *instr);
            void emulate(const AddressQueue::Ptr& queue, ReferenceTable *reftable, const Instruction *instr);
            QString output(ReferenceTable *reftable, const Instruction *instr);

        public slots:
            void updatePc(lua_Integer amount);
    };
}

#endif // PREFSDK_PROCESSORDEFINITION_H
