#ifndef PREFSDK_PROCESSORLOADER_H
#define PREFSDK_PROCESSORLOADER_H

#include "qhexedit/qhexeditdata.h"
#include "prefsdk/libs/qt/qtlua.h"
#include "prefsdk/debug/debugobject.h"
#include "prefsdk/format/formatdefinition.h"
#include "prefsdk/disassembler/symbol/symboltable.h"
#include "prefsdk/disassembler/references/referencetable.h"
#include "prefsdk/disassembler/disassemblerlisting.h"
#include "processordefinition.h"
#include "processoremulator.h"

namespace PrefSDK
{
    class ProcessorLoader: public DebugObject
    {
        Q_OBJECT

        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(QString author READ author WRITE setAuthor)
        Q_PROPERTY(QString version READ version WRITE setVersion)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction map READ map WRITE setMap)
        Q_PROPERTY(PrefSDK::QtLua::LuaFunction baseAddress READ baseAddress WRITE setBaseAddress)
        Q_PROPERTY(PrefSDK::FormatDefinition* format READ format)
        Q_PROPERTY(PrefSDK::ProcessorDefinition* processor READ processor)

        public:
            explicit ProcessorLoader(const QString& name, const QString& author, const QString& version, FormatDefinition* formatdefinition, ProcessorDefinition* processordefinition, QObject *parent = 0);
            void disassemble(QHexEditData *hexeditdata);

        public:
            QString id() const;
            const QString& name() const;
            const QString& author() const;
            const QString& version() const;
            const PrefSDK::QtLua::LuaFunction& map() const;
            const QtLua::LuaFunction& baseAddress() const;
            PrefSDK::FormatDefinition* format() const;
            PrefSDK::ProcessorDefinition* processor() const;
            void setName(const QString& n);
            void setAuthor(const QString& a);
            void setVersion(const QString& v);
            void setMap(const PrefSDK::QtLua::LuaFunction& mf);
            void setBaseAddress(const PrefSDK::QtLua::LuaFunction& ba);
            bool validate(QHexEditData* hexeditdata);
            void callMap(DisassemblerListing *listing, QHexEditData* hexeditdata, LogWidget *logwidget);

        public slots:
            void createSegment(const QString& name, lua_Integer segmenttype, lua_Integer startaddress, lua_Integer size, lua_Integer baseoffset);
            void createEntryPoint(const QString& name, lua_Integer address);

        private:
            DataValue callBaseAddress();
            void disassembleInstruction(QHexEditData *hexeditdata);

        private:
            PrefSDK::QtLua::LuaFunction _mapfunc;
            PrefSDK::QtLua::LuaFunction _baseaddressfunc;
            QStack<Function*> _functionstack;
            ProcessorDefinition* _processordefinition;
            ProcessorEmulator* _processoremulator;
            FormatDefinition* _formatdefinition;
            DisassemblerListing* _listing;
            FormatTree* _formattree;
            DataValue _baseaddress;
            QString _name;
            QString _author;
            QString _version;
    };
}

#endif // PREFSDK_PROCESSORLOADER_H
