#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <lua.hpp>
#include "qhexedit/qhexedit.h"
#include "prefsdk/format/formatlist.h"

using namespace PrefSDK;

class OptionMenu : public QMenu
{
    Q_OBJECT

    public:
        explicit OptionMenu(lua_State *l, QHexEdit *hexedit, FormatList::Format &format, QWidget *parent = 0);
        void executeOption(int optionidx);

    private slots:
        void onActionTriggered(QAction* action);

    private:
        lua_State* _state;
        QHexEdit* _hexedit;
};

#endif // OPTIONMENU_H
