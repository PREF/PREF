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

        Q_PROPERTY(PrefSDK::QtLua::LuaFunction baseAddress READ baseAddress WRITE setBaseAddress)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction map READ map WRITE setMap)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction disassemble READ disassemble WRITE setDisassemble)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction output READ output WRITE setOutput)

        public:
            explicit DisassemblerDefinition(const QString& name, const QString& author, const QString& version, DataType::Type addresstype, FormatDefinition* formatdefinition, QObject *parent = 0);

        public:
            QString id() const;
            DataType::Type addressType() const;
            const QString& name() const;
            const QString& author() const;
            const QString& version() const;
            const PrefSDK::QtLua::LuaFunction& baseAddress() const;
            const PrefSDK::QtLua::LuaFunction& map() const;
            const PrefSDK::QtLua::LuaFunction& disassemble() const;
            const PrefSDK::QtLua::LuaFunction& output() const;
            PrefSDK::MemoryBuffer* memoryBuffer() const;
            void setName(const QString& n);
            void setAuthor(const QString& a);
            void setVersion(const QString& v);
            void setBaseAddress(const PrefSDK::QtLua::LuaFunction& ba);
            void setMap(const PrefSDK::QtLua::LuaFunction& mf);
            void setDisassemble(const PrefSDK::QtLua::LuaFunction& df);
            void setOutput(const PrefSDK::QtLua::LuaFunction& of);
            MemoryBuffer* callMap(DisassemblerListing *listing, QHexEditData* hexeditdata);
            void callDisassemble(QLabel *infolabel, DisassemblerListing *listing, MemoryBuffer *memorybuffer);
            void callOutput(ListingPrinter* printer, Instruction* instruction, DisassemblerListing *listing, MemoryBuffer *memorybuffer);

        public:
            bool validate(QHexEditData* hexeditdata);
            QString emitInstruction(Instruction* instruction, DisassemblerListing *listing, MemoryBuffer *memorybuffer);

        public slots:
            lua_Integer next(const PrefSDK::QtLua::LuaTable& instructiontable);
            QString hexdump(const PrefSDK::QtLua::LuaTable& instructiontable);
            void enqueue(lua_Integer address);

        private:
            DataValue callBaseAddress();

        private:
            QStack<DataValue> _addrstack;
            PrefSDK::QtLua::LuaFunction _baseaddressfunc;
            PrefSDK::QtLua::LuaFunction _mapfunc;
            PrefSDK::QtLua::LuaFunction _disassemblefunc;
            PrefSDK::QtLua::LuaFunction _outputfunc;
            DataType::Type _addresstype;
            MemoryBuffer* _memorybuffer;
            FormatDefinition* _formatdefinition;
            DataValue _baseaddress;
            QString _name;
            QString _author;
            QString _version;
    };
}

#endif // PREFSDK_DISASSEMBLERDEFINITION_H
