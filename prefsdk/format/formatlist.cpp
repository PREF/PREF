#include "formatlist.h"

namespace PrefSDK
{
    QList<FormatList::Format> FormatList::_formats;
    QHash<FormatList::FormatId, int> FormatList::_formatmap;
    QHash<QHexEditData*, FormatList::LoadedFormat> FormatList::_loadedformats;

    const QString FormatList::FORMATS_DIR = "formats";
    const QString FormatList::FORMAT_MAIN_FILE = "main.lua";

    FormatList::FormatList()
    {

    }

    void FormatList::loadFormats(lua_State *l, const QString& dir)
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
                    DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(l, -1)));
                    lua_pop(l, 1);
                }
            }
        }
    }

    void FormatList::load(lua_State *l)
    {
        QDir d(qApp->applicationDirPath());
        QString formatspath = d.absoluteFilePath(FormatList::FORMATS_DIR);

        if(!QDir(formatspath).exists())
            return;

        FormatList::loadFormats(l, formatspath);
    }

    void FormatList::registerFormat(const QString& name, const QString& category, const QString& author, const QString& version, FormatId formatid)
    {
        int idx = FormatList::_formats.length();

        CategoryManager::add(category, idx);
        FormatList::_formats.append(Format(name, category, author, version, formatid));
        FormatList::_formatmap[formatid] = idx;
    }

    void FormatList::registerOption(FormatList::FormatId formatid, int optionidx, const QString &name, const QString& description)
    {
        int idx = FormatList::_formatmap[formatid];
        FormatList::Format& format = FormatList::_formats[idx];
        format.addOption(optionidx, name, description);
    }

    void FormatList::addLoadedFormat(FormatList::FormatId formatid, FormatTree *formattree, QHexEditData* hexeditdata)
    {
        FormatList::_loadedformats[hexeditdata] = LoadedFormat(formatid, formattree);
    }

    void FormatList::removeLoadedFormat(QHexEditData *hexeditdata)
    {
        if(FormatList::_loadedformats.contains(hexeditdata))
            FormatList::_loadedformats.remove(hexeditdata);
    }

    int FormatList::length()
    {
        return FormatList::_formats.length();
    }

    FormatList::Format &FormatList::format(int i)
    {
        return FormatList::_formats[i];
    }

    FormatList::Format &FormatList::formatFromId(FormatList::FormatId id)
    {
        return FormatList::_formats[FormatList::_formatmap[id]];
    }

    FormatList::LoadedFormat &FormatList::loadedFormat(QHexEditData *hexeditdata)
    {
        return FormatList::_loadedformats[hexeditdata];
    }
}
