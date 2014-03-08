#include "prefui.h"

namespace PrefSDK
{
    PrefUI::PrefUI(lua_State* l): LuaCTable(l, "PrefUI")
    {
        this->exportMethod<lua_String, PrefUI, lua_String>("getOpenFileName", &PrefUI::getOpenFileName);
        this->exportMethod<lua_String, PrefUI, lua_String, lua_String>("getOpenFileName", &PrefUI::getOpenFileName);
        this->exportMethod<lua_String, PrefUI, lua_String>("getSaveFileName", &PrefUI::getSaveFileName);
        this->exportMethod<lua_String, PrefUI, lua_String, lua_String>("getSaveFileName", &PrefUI::getSaveFileName);
        this->exportMethod<lua_Integer, PrefUI, lua_String, lua_String>("messageBox", &PrefUI::messageBox);
        this->exportMethod<lua_Integer, PrefUI, lua_String, lua_String, lua_Integer>("messageBox", &PrefUI::messageBox);

        this->_standardbuttons = LuaTable::create(l);
        this->_standardbuttons->set("Ok", QMessageBox::Ok);
        this->_standardbuttons->set("Yes", QMessageBox::Yes);
        this->_standardbuttons->set("No", QMessageBox::No);
        this->_standardbuttons->set("Cancel", QMessageBox::Cancel);
    }

    PrefUI::Ptr PrefUI::create(lua_State *l)
    {
        return PrefUI::Ptr(new PrefUI(l));
    }

    lua_String PrefUI::getOpenFileName(lua_String title)
    {
        return this->getOpenFileName(title, QString().toLatin1());
    }

    lua_String PrefUI::getOpenFileName(lua_String title, lua_String filter)
    {
        QString s = QFileDialog::getOpenFileName(qApp->activeWindow(), title, QString(), filter);
        return s.toLatin1().constData();
    }

    lua_String PrefUI::getSaveFileName(lua_String title)
    {
        return this->getSaveFileName(title, QString().toLatin1());
    }

    lua_String PrefUI::getSaveFileName(lua_String title, lua_String filter)
    {
        QString s = QFileDialog::getSaveFileName(qApp->activeWindow(), title, QString(), filter);
        return s.toLatin1().constData();
    }

    lua_Integer PrefUI::messageBox(lua_String title, lua_String text)
    {
        return this->messageBox(title, text, QMessageBox::Ok);
    }

    lua_Integer PrefUI::messageBox(lua_String title, lua_String text, lua_Integer buttons)
    {
        QMessageBox m;
        m.setWindowTitle(QString::fromLatin1(title));
        m.setText(QString::fromLatin1(text));
        m.setStandardButtons(static_cast<QMessageBox::StandardButton>(buttons));
        return m.exec();
    }

    void PrefUI::pushGlobal()
    {
       LuaCTable::pushGlobal();

       this->_standardbuttons->push();
       lua_setglobal(this->state(), "StandardButtons");
    }
}
