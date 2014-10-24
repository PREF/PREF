#ifndef PREFSDK_PROCESSORDEFINITION_H
#define PREFSDK_PROCESSORDEFINITION_H

#include "prefsdk/disassembler/disassemblerlisting.h"
#include "prefsdk/disassembler/blocks/instruction.h"
#include "prefsdk/disassembler/instruction/instructionset.h"
#include "prefsdk/disassembler/register/registerset.h"
#include "prefsdk/disassembler/processor/processoremulator.h"
#include "logwidget/logobject.h"
#include <QtCore>

namespace PrefSDK
{
    class ProcessorDefinition : public LogObject
    {
        Q_OBJECT

        Q_PROPERTY(PrefSDK::InstructionSet* instructionset READ instructionSet)
        Q_PROPERTY(PrefSDK::RegisterSet* registerset READ registerSet)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction analyze READ analyze WRITE setAnalyze)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction emulate READ emulate WRITE setEmulate)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction elaborate READ elaborate WRITE setElaborate)

        public:
            explicit ProcessorDefinition(InstructionSet* instructionset, RegisterSet* registerset, DataType::Type addresstype, QObject *parent = 0);
            DataType::Type addressType() const;
            const PrefSDK::QtLua::LuaFunction& analyze() const;
            const PrefSDK::QtLua::LuaFunction& emulate() const;
            const PrefSDK::QtLua::LuaFunction& elaborate() const;
            void setAnalyze(const PrefSDK::QtLua::LuaFunction& af);
            void setEmulate(const PrefSDK::QtLua::LuaFunction& ef);
            void setElaborate(const PrefSDK::QtLua::LuaFunction& ef);
            lua_Integer callAnalyze(Instruction* instruction, const DataValue &baseaddress);
            void callEmulate(ProcessorEmulator* emulator, Instruction* instruction);
            void callElaborate(DisassemblerListing* listing);

        public:
            PrefSDK::InstructionSet *instructionSet() const;
            PrefSDK::RegisterSet* registerSet() const;
            bool decode(Instruction* instruction);

        private:
            PrefSDK::QtLua::LuaFunction _analyzefunc;
            PrefSDK::QtLua::LuaFunction _emulatefunc;
            PrefSDK::QtLua::LuaFunction _elaboratefunc;
            InstructionSet* _instructionset;
            RegisterSet* _registerset;
            DataType::Type _addresstype;
    };
}

#endif // PREFSDK_PROCESSORDEFINITION_H
