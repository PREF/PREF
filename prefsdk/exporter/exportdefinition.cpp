#include "exportdefinition.h"

namespace PrefSDK
{
    ExportDefinition::ExportDefinition(const LuaTable::Ptr &t): _exportertable(t)
    {
    }

    QString ExportDefinition::name()
    {
        return QString::fromLatin1(this->_exportertable->get<lua_String, lua_String>("name"));
    }

    QString ExportDefinition::author()
    {
        return QString::fromLatin1(this->_exportertable->get<lua_String, lua_String>("author"));
    }

    QString ExportDefinition::version()
    {
        return QString::fromLatin1(this->_exportertable->get<lua_String, lua_String>("version"));
    }

    QString ExportDefinition::description()
    {
        return QString::fromLatin1(this->_exportertable->get<lua_String, lua_String>("description"));
    }

    void ExportDefinition::exportData(ByteBuffer *inbuffer, QString filename, lua_Integer startoffset, lua_Integer endoffset)
    {
        QFile f(filename);
        f.open(QFile::ReadWrite | QFile::Truncate);
        FileBuffer* fb = new FileBuffer(inbuffer->state(), &f);

        try
        {
            this->_exportertable->call<void, LuaTable::Ptr, LuaTable::Ptr, lua_Integer, lua_Integer>("exportData", *inbuffer, *fb, startoffset, endoffset);
        }
        catch(LuaException& e)
        {
            f.close();
            PrefDebug::dbgprint(e.what());
        }

        fb->save();
    }
}
