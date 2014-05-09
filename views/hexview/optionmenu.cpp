#include "optionmenu.h"

OptionMenu::OptionMenu(lua_State *l, QHexEdit* hexedit, FormatList::Format& format, QWidget *parent): QMenu(parent), _state(l), _hexedit(hexedit)
{
    for(int i = 0; i < format.optionsCount(); i++)
    {
        FormatList::Format::Option opt = format.option(i);
        QAction* action = this->addAction(opt.name());
        action->setData(i);
    }

    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(onActionTriggered(QAction*)));
}

void OptionMenu::executeOption(int optionidx)
{
    lua_getglobal(this->_state, "Sdk");
    lua_getfield(this->_state, -1, "executeOption");
    lua_pushinteger(this->_state, optionidx);
    lua_pushlightuserdata(this->_state, this->_hexedit->data());
    lua_pushinteger(this->_state, this->_hexedit->selectionStart());
    lua_pushinteger(this->_state, this->_hexedit->selectionEnd());
    int res = lua_pcall(this->_state, 4, 0, 1);

    if(res != 0)
        DebugDialog::instance()->out(QString::fromUtf8(lua_tostring(this->_state, -1)));

    lua_pop(this->_state, (res ? 2 : 1));
}

void OptionMenu::onActionTriggered(QAction *action)
{
    int idx = action->data().toInt();
    this->executeOption(idx + 1);
}
