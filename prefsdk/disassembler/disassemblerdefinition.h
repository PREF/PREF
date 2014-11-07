#ifndef PREFSDK_DISASSEMBLERDEFINITION_H
#define PREFSDK_DISASSEMBLERDEFINITION_H

#include "logwidget/logobject.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/libs/qt/qtlua.h"
#include "prefsdk/format/formatdefinition.h"
#include "prefsdk/disassembler/symbol/symboltable.h"
#include "prefsdk/disassembler/references/referencetable.h"
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "prefsdk/disassembler/listingprinter.h"
#include "prefsdk/disassembler/memorybuffer.h"
#include <QLabel>

namespace PrefSDK
{
    class DisassemblerDefinition: public LogObject
    {
        Q_OBJECT

        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(QString author READ author WRITE setAuthor)
        Q_PROPERTY(QString version READ version WRITE setVersion)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction baseAddress READ baseAddress WRITE setBaseAddress)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction map READ map WRITE setMap)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction disassemble READ disassemble WRITE setDisassemble)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction output READ output WRITE setOutput)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction elaborate READ elaborate WRITE setElaborate)
        Q_PROPERTY(PrefSDK::MemoryBuffer* memorybuffer READ memoryBuffer)
        Q_PROPERTY(PrefSDK::FormatDefinition* format READ format)
        Q_PROPERTY(PrefSDK::FormatTree* formattree READ formatTree)

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
            const PrefSDK::QtLua::LuaFunction& elaborate() const;
            PrefSDK::MemoryBuffer* memoryBuffer() const;
            PrefSDK::FormatDefinition* format() const;
            PrefSDK::FormatTree* formatTree() const;
            void setName(const QString& n);
            void setAuthor(const QString& a);
            void setVersion(const QString& v);
            void setBaseAddress(const PrefSDK::QtLua::LuaFunction& ba);
            void setMap(const PrefSDK::QtLua::LuaFunction& mf);
            void setDisassemble(const PrefSDK::QtLua::LuaFunction& df);
            void setOutput(const PrefSDK::QtLua::LuaFunction& of);
            void setElaborate(const PrefSDK::QtLua::LuaFunction& ef);
            void callMap(DisassemblerListing *listing, QHexEditData* hexeditdata, Logger *logger);
            void callDisassemble(QLabel *infolabel);
            void callOutput(ListingPrinter* printer, Instruction* instruction);
            void callElaborate();

        public:
            void setSymbol(const DataValue& address, Symbol::Type symboltype, const QString& name);
            bool validate(QHexEditData* hexeditdata, Logger *logger);
            QString emitInstruction(Instruction* instruction);

        public slots:
            lua_Integer next(const PrefSDK::QtLua::LuaTable& instructiontable);
            QString hexdump(const PrefSDK::QtLua::LuaTable& instructiontable);
            QString symbolName(lua_Integer address);
            bool isAddress(lua_Integer address);
            bool isSymbol(lua_Integer address);
            bool isStringSymbol(lua_Integer address);
            void enqueue(lua_Integer address);
            void addInstruction(const PrefSDK::QtLua::LuaTable& instructiontable);
            void createSegment(const QString& name, lua_Integer segmenttype, lua_Integer startaddress, lua_Integer size, lua_Integer baseoffset);
            void createEntryPoint(lua_Integer address, const QString& name);
            void createFunction(lua_Integer address, const QString& name);
            void createFunction(lua_Integer address);
            void createLabel(lua_Integer destaddress, const PrefSDK::QtLua::LuaTable& instructiontable, const QString& name);
            void setSymbol(lua_Integer address, lua_Integer symboltype, const QString& name);
            void setSymbol(lua_Integer address, lua_Integer symboltype);
            void setFunction(lua_Integer address, const QString& name);
            void setFunction(lua_Integer address, const QString& name, lua_Integer functiontype);
            void setConstant(QObject* instruction, lua_Integer datatype, lua_Integer value, const QString& name);

        private:
            DataValue callBaseAddress();

        private:
            QStack<DataValue> _addrstack;
            PrefSDK::QtLua::LuaFunction _baseaddressfunc;
            PrefSDK::QtLua::LuaFunction _mapfunc;
            PrefSDK::QtLua::LuaFunction _disassemblefunc;
            PrefSDK::QtLua::LuaFunction _outputfunc;
            PrefSDK::QtLua::LuaFunction _elaboratefunc;
            DataType::Type _addresstype;
            MemoryBuffer* _memorybuffer;
            FormatDefinition* _formatdefinition;
            DisassemblerListing* _listing;
            FormatTree* _formattree;
            DataValue _baseaddress;
            QString _name;
            QString _author;
            QString _version;
    };
}

#endif // PREFSDK_DISASSEMBLERDEFINITION_H
