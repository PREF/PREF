#include "disassemblerlist.h"

namespace PrefSDK
{
    DisassemblerList* DisassemblerList::_instance = nullptr;

    const QString DisassemblerList::DISASSEMBLERS_DIR = "disassemblers";
    const QString DisassemblerList::DISASSEMBLER_MAIN_FILE = "definition.lua";

    DisassemblerList::DisassemblerList(QObject *parent): QObject(parent)
    {
    }

    void DisassemblerList::load(lua_State *l, const QString &dir)
    {
        QDir d(dir);
        QFileInfoList dirs = d.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

        foreach(QFileInfo fi, dirs)
        {
            QDir fd(fi.absoluteFilePath());

            if(fd.exists(DisassemblerList::DISASSEMBLER_MAIN_FILE))
            {
                int res = luaL_dofile(l, fd.absoluteFilePath(DisassemblerList::DISASSEMBLER_MAIN_FILE).toUtf8().constData());

                if(res != 0)
                {
                    throw PrefException(QString("DisassemblerList::load(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
                    lua_pop(l, 1);
                }

                if(lua_type(l, -1) != LUA_TTABLE)
                {
                    throw PrefException(QString("DisassemblerList::load(): Got '%1'' instead of disassembler table").arg(QString::fromUtf8(lua_typename(l, lua_type(l, -1)))));
                    lua_pop(l, 1);
                    continue;
                }

                DisassemblerDefinition* disassemblerdefinition = new DisassemblerDefinition(QtLua::LuaTable(l, -1));
                lua_pop(l, 1);

                if(this->_disassemblermap.contains(disassemblerdefinition->id()))
                {
                    throw PrefException(QString("DisassemblerList::load(): Disassembler '%1' already loaded").arg(disassemblerdefinition->name()));
                    continue;
                }

                int idx = this->_disassemblers.length();
                this->_disassemblers.append(disassemblerdefinition);
                this->_disassemblermap[disassemblerdefinition->id()] = idx;
            }
        }
    }

    DisassemblerList *DisassemblerList::instance()
    {
        return DisassemblerList::_instance;
    }

    void DisassemblerList::load()
    {
        if(DisassemblerList::_instance)
            return;

        DisassemblerList::_instance = new DisassemblerList();
        QDir d(qApp->applicationDirPath());
        QString loaderspath = d.absoluteFilePath(DisassemblerList::DISASSEMBLERS_DIR);

        if(!QDir(loaderspath).exists())
            return;

        lua_State* l = LuaState::instance();
        DisassemblerList::_instance->load(l, loaderspath);
    }

    DisassemblerDefinition *DisassemblerList::disassembler(int i)
    {
        return this->_disassemblers[i];
    }

    int DisassemblerList::length()
    {
        return this->_disassemblers.length();
    }
}
