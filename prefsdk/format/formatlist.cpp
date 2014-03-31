#include "formatlist.h"

namespace PrefSDK
{
    QList<const FormatDefinition*> FormatList::_formats;
    int FormatList::_formatscount = 0;
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
                {
                    FormatList::_formatscount++;
                    luaW_dofile(l, fd.absoluteFilePath(FormatList::FORMAT_MAIN_FILE).toLatin1().constData());
                }
            }
            catch(LuaException& e)
            {
                PrefDebug::dbgprint(e.what());
            }
        }
    }

    void FormatList::load(lua_State* l)
    {
        CategoryManager::clear();
        FormatList::_formats.clear();

        QDir d(qApp->applicationDirPath());
        QString formatspath = d.absoluteFilePath(FormatList::FORMATS_DIR);

        if(!QDir(formatspath).exists())
            return;

        FormatList::loadFormats(l, formatspath);
        CategoryManager::addGlobalCategory(FormatList::_formatscount);
    }

    void FormatList::registerFormat(const FormatDefinition *formatdefinition)
    {
        CategoryManager::add(formatdefinition->Category, FormatList::_formats.length());
        FormatList::_formats.append(formatdefinition);
    }

    int FormatList::length()
    {
        return FormatList::_formats.length();
    }

    const FormatDefinition *FormatList::format(int i)
    {
        return FormatList::_formats.at(i);
    }
}
