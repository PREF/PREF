#include "formatdefinition.h"

namespace PrefSDK
{
    FormatDefinition::FormatDefinition(const QtLua::LuaTable &formattable, QObject *parent): LogObject(parent), _formattable(formattable)
    {
        this->_formattable.bind(this);
    }

    QString FormatDefinition::id() const
    {
        return QString(QCryptographicHash::hash(QString("%1%2").arg(this->name().toUpper(), this->category().toUpper()).toUtf8(), QCryptographicHash::Md5));
    }

    bool FormatDefinition::hasView() const
    {
        return this->_formattable.fieldExists("view");
    }

    QString FormatDefinition::name() const
    {
        return this->_formattable.getString("name");
    }

    QString FormatDefinition::category() const
    {
        return this->_formattable.getString("category");
    }

    QString FormatDefinition::author() const
    {
        return this->_formattable.getString("author");
    }

    QString FormatDefinition::version() const
    {
        return this->_formattable.getString("version");
    }

    bool FormatDefinition::callValidate(QHexEditData *hexeditdata, Logger* logger, qint64 baseoffset, bool ignoreerror)
    {
        this->setLogger(logger);

        if(!this->_formattable.fieldExists("validate"))
            return true; /* If 'validate-procedure' is not set, the format doesn't require validation */

        lua_State* l = this->_formattable.instance();
        QtLua::LuaFunction validatefunc = this->_formattable.getFunction("validate");
        FormatValidator fv(hexeditdata, baseoffset);

        QtLua::pushObject(l, &fv);
        bool err = validatefunc(1);

        if(err)
        {
            if(logger && !ignoreerror)
                logger->error(QString::fromUtf8(lua_tostring(l, -1)));

            lua_pop(l, 1);
            return false;
        }

        return fv.validated();
    }

    FormatTree *FormatDefinition::callParse(QHexEditData *hexeditdata, Logger* logger, qint64 baseoffset)
    {
        lua_State* l = this->_formattable.instance();
        QtLua::LuaFunction parsefunc = this->_formattable.getFunction("parse");
        FormatTree* formattree = new FormatTree(hexeditdata, logger, baseoffset);

        QtLua::pushObject(l, formattree);
        bool err = parsefunc(1);

        if(err)
        {
            this->error(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);

            formattree->deleteLater();
            return nullptr;
        }

        return formattree;
    }

    QWidget *FormatDefinition::callView(FormatTree *formattree)
    {
        if(!this->_formattable.fieldExists("view"))
            return nullptr;

        QWidget* w = nullptr;
        lua_State* l = this->_formattable.instance();
        QtLua::LuaFunction viewfunc = this->_formattable.getFunction("view");

        QtLua::pushObject(l, formattree);
        bool err = viewfunc(1, true);
        int t = lua_type(l, -1);

        if(err || (t != LUA_TUSERDATA))
        {
            if(err)
                this->error(QString::fromUtf8(lua_tostring(l, -1)));
            else if(t <= 0) /* lua_isnoneornil() */
                lua_pop(l, 1);

            return nullptr;
        }
        else
            w = *(reinterpret_cast<QWidget**>(lua_touserdata(l, -1)));

        lua_pop(l, 1);
        return w;
    }
}
