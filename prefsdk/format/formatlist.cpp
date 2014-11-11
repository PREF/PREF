#include "formatlist.h"

namespace PrefSDK
{
    FormatList* FormatList::_instance = nullptr;
    const QString FormatList::FORMATS_DIR = "formats";
    const QString FormatList::FORMAT_MAIN_FILE = "definition.lua";

    FormatList::FormatList(QObject *parent): QObject(parent)
    {

    }

    FormatList *FormatList::instance()
    {
        return FormatList::_instance;
    }

    void FormatList::load()
    {
        if(FormatList::_instance)
            return;

        FormatList::_instance = new FormatList();
        QDir d(qApp->applicationDirPath());
        QString formatspath = d.absoluteFilePath(FormatList::FORMATS_DIR);

        if(!QDir(formatspath).exists())
            return;

        lua_State* l = LuaState::instance();
        FormatList::_instance->load(l, formatspath);
    }

    void FormatList::load(lua_State *l, const QString& dir)
    {
        QDir d(dir);
        QFileInfoList dirs = d.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

        foreach(QFileInfo fi, dirs)
        {
            QDir fd(fi.absoluteFilePath());

            if(fd.exists(FormatList::FORMAT_MAIN_FILE))
            {
                int res = luaL_dofile(l, fd.absoluteFilePath(FormatList::FORMAT_MAIN_FILE).toUtf8().constData());

                if(res != 0)
                {
                    throw PrefException(QString("FormatList::load(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
                    lua_pop(l, 1);
                }

                if(lua_type(l, -1) != LUA_TTABLE)
                {
                    throw PrefException(QString("FormatList::load(): Got '%1'' instead of format table").arg(QString::fromUtf8(lua_typename(l, lua_type(l, -1)))));
                    lua_pop(l, 1);
                    continue;
                }

                FormatDefinition* formatdefinition = new FormatDefinition(QtLua::LuaTable(l, -1));
                lua_pop(l, 1);

                if(this->_formatmap.contains(formatdefinition->id()))
                {
                    throw PrefException(QString("FormatList::load(): Format '%1' already loaded").arg(formatdefinition->name()));
                    continue;
                }

                int idx = this->_formats.length();
                CategoryManager::add(formatdefinition->category(), idx);
                this->_formats.append(formatdefinition);
                this->_formatmap[formatdefinition->id()] = idx;
            }
        }
    }

    int FormatList::length()
    {
        return this->_formats.length();
    }

    FormatDefinition *FormatList::format(int i)
    {
        return this->_formats[i];
    }
}
