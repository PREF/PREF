#ifndef PREFSDK_EXPORTDEFINITION_H
#define PREFSDK_EXPORTDEFINITION_H

#include "prefsdk/qlua.h"
#include "prefsdk/io/bytebuffer.h"
#include "prefsdk/io/filebuffer.h"
#include "prefsdk/prefdebug.h"

namespace PrefSDK
{
    class ExportDefinition
    {
        public:
            typedef std::shared_ptr<ExportDefinition> Ptr;

        public:
            ExportDefinition(const LuaTable::Ptr& t);
            QString name();
            QString author();
            QString version();
            QString description();
            void exportData(ByteBuffer* inbuffer, QString filename, lua_Integer startoffset, lua_Integer endoffset);

        private:
            LuaTable::Ptr _exportertable;
    };
}

#endif // PREFSDK_EXPORTDEFINITION_H
