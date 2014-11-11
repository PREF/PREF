#ifndef PREFSDK_DISASSEMBLERDEFINITION_H
#define PREFSDK_DISASSEMBLERDEFINITION_H

#include "logwidget/logobject.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/libs/qt/qtlua.h"
#include "prefsdk/format/formatdefinition.h"
#include "prefsdk/disassembler/symbol/symboltable.h"
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "prefsdk/disassembler/listingprinter.h"
#include "prefsdk/disassembler/memorybuffer.h"
#include <QLabel>

namespace PrefSDK
{
    class DisassemblerDefinition: public LogObject
    {
        Q_OBJECT

        Q_PROPERTY(PrefSDK::FormatTree* formattree READ formatTree)
        Q_PROPERTY(PrefSDK::DisassemblerListing* listing READ listing)
        Q_PROPERTY(PrefSDK::MemoryBuffer* memorybuffer READ memoryBuffer)

        public:
            explicit DisassemblerDefinition(const QtLua::LuaTable& disassemblertable, QObject *parent = 0);

        public:
            QString id() const;
            DataType::Type addressType() const;
            QString name() const;
            QString author() const;
            QString version() const;
            PrefSDK::FormatTree* formatTree() const;
            PrefSDK::DisassemblerListing* listing() const;
            PrefSDK::MemoryBuffer* memoryBuffer() const;
            bool callMap(QHexEditData* hexeditdata);
            void callDisassemble(QLabel *infolabel);
            void callOutput(ListingPrinter* printer, Instruction* instruction);

        public:
            bool validate(QHexEditData* hexeditdata);
            QString emitInstruction(Instruction* instruction);

        public slots:
            lua_Integer next(const PrefSDK::QtLua::LuaTable& instructiontable);
            QString hexdump(const PrefSDK::QtLua::LuaTable& instructiontable);
            void enqueue(lua_Integer address);

        private:
            DataValue callBaseAddress();

        private:
            QStack<DataValue> _addrstack;
            QtLua::LuaTable _disassemblertable;
            DisassemblerListing* _listing;
            FormatTree* _formattree;
            MemoryBuffer* _memorybuffer;
            FormatDefinition* _formatdefinition;
            DataValue _baseaddress;
    };
}

#endif // PREFSDK_DISASSEMBLERDEFINITION_H
