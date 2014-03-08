#include "formatlist.h"

namespace PrefSDK
{
    LuaTable::Ptr FormatList::_fltable;
    QList<FormatDefinition::Ptr> FormatList::_formats;
    const QString FormatList::FORMATS_DIR = "formats";
    const QString FormatList::FORMAT_MAIN_FILE = "main.lua";

    FormatList::FormatList()
    {

    }

    void FormatList::loadFormats(lua_State *l, QString dir)
    {
        QDir d(dir);
        QFileInfoList dirs = d.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

        foreach(QFileInfo fi, dirs)
        {            
            try
            {
                QDir fd(fi.absoluteFilePath());

                if(fd.exists(FormatList::FORMAT_MAIN_FILE))
                    luaW_dofile(l, fd.absoluteFilePath(FormatList::FORMAT_MAIN_FILE).toLatin1().constData());
            }
            catch(LuaException& e)
            {
                PrefDebug::dbgprint(e.what());
            }
        }
    }

    void FormatList::load(lua_State* l)
    {
        FormatList::_fltable = LuaTable::create(l);
        FormatList::_fltable->pushGlobal("FormatList");

        CategoryManager::clear();
        FormatList::_formats.clear();

        QDir d(qApp->applicationDirPath());
        QString formatspath = d.absoluteFilePath(FormatList::FORMATS_DIR);

        if(!QDir(formatspath).exists())
            return;

        FormatList::loadFormats(l, formatspath);
        CategoryManager::addGlobalCategory(FormatList::_fltable->length());

        for(lua_Integer i = 0; i < FormatList::_fltable->length(); i++)
        {
            LuaTable::Ptr fdt = FormatList::_fltable->get<lua_Integer>(i + 1);
            FormatDefinition::Ptr fd(new FormatDefinition(fdt));
            CategoryManager::add(fd->category(), i);
            FormatList::_formats.append(fd);
        }
    }

    int FormatList::length()
    {
        return FormatList::_fltable->length();
    }

    FormatDefinition::Ptr FormatList::format(int i)
    {
        return FormatList::_formats.at(i);
    }
}
