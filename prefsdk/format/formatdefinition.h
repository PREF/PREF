#ifndef PREFSDK_FORMATDEFINITION_H
#define PREFSDK_FORMATDEFINITION_H

#include <utility>
#include <QtCore>
#include "prefsdk/io/bytebuffer.h"
#include "prefsdk/prefdebug.h"
#include "prefsdk/disassembler/disassemblerloader.h"
#include "viewmodels/formatmodel/formatmodel.h"

namespace PrefSDK
{
    using namespace Lua;

    class FormatDefinition
    {
        public:
            typedef std::shared_ptr<FormatDefinition> Ptr;

        public:
            FormatDefinition(const LuaTable::Ptr& t);
            lua_State *state() const;
            QString name() const;
            QString author() const;
            QString version() const;
            QSysInfo::Endian endian() const;
            int optionsCount() const;
            QString option(int i) const;
            QString endianString() const;
            QString category() const;
            bool hasOptions() const;
            bool hasCategory() const;
            bool validateFormat(ByteBuffer* bytebuffer, lua_Integer baseoffset);
            FormatModel* parseFormat(ByteBuffer* bytebuffer, lua_Integer baseoffset) const;
            void executeOption(int optindex, FormatTree::Ptr formattree, ByteBuffer *bytebuffer);
            DisassemblerLoader::Ptr generateLoader(FormatTree::Ptr formattree, ByteBuffer *bytebuffer);

        private:
            LuaTable::Ptr _formatdeftable;
    };
}

#endif // PREFSDK_FORMATDEFINITION_H
