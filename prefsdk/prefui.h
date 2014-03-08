#ifndef PREFSDK_PREFUI_H
#define PREFSDK_PREFUI_H

#include <QtWidgets>
#include "qlua.h"

namespace PrefSDK
{
    class PrefUI : public LuaCTable
    {
        public:
            typedef std::shared_ptr<PrefUI> Ptr;

        public:
            PrefUI(lua_State *l);
            static PrefUI::Ptr create(lua_State* l);
            lua_String getOpenFileName(lua_String title);
            lua_String getOpenFileName(lua_String title, lua_String filter);
            lua_String getSaveFileName(lua_String title);
            lua_String getSaveFileName(lua_String title, lua_String filter);
            lua_Integer messageBox(lua_String title, lua_String text);
            lua_Integer messageBox(lua_String title, lua_String text, lua_Integer buttons);
            virtual void pushGlobal();

        private:
            LuaTable::Ptr _standardbuttons;
    };
}

#endif // PREFSDK_PREFUI_H
