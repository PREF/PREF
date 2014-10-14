#include "formatdefinition.h"

namespace PrefSDK
{
    FormatDefinition::FormatDefinition(QObject *parent): LogObject(parent)
    {

    }

    FormatDefinition::FormatDefinition(const QString& name, const QString& category, const QString& author, const QString& version, QObject *parent): LogObject(parent), _name(name), _category(category), _author(author), _version(version)
    {

    }

    QString FormatDefinition::id() const
    {
        return QString(QCryptographicHash::hash(QString("%1%2").arg(this->_name.toUpper(), this->_category.toUpper()).toUtf8(), QCryptographicHash::Md5));
    }

    bool FormatDefinition::hasView() const
    {
        return this->_viewfunc.isValid();
    }

    const QString &FormatDefinition::name() const
    {
        return this->_name;
    }

    const QString &FormatDefinition::category() const
    {
        return this->_category;
    }

    const QString &FormatDefinition::author() const
    {
        return this->_author;
    }

    const QString &FormatDefinition::version() const
    {
        return this->_version;
    }

    const PrefSDK::QtLua::LuaFunction &FormatDefinition::validate() const
    {
        return this->_validatefunc;
    }

    const PrefSDK::QtLua::LuaFunction &FormatDefinition::parse() const
    {
        return this->_parsefunc;
    }

    const PrefSDK::QtLua::LuaFunction &FormatDefinition::view() const
    {
        return this->_viewfunc;
    }

    void FormatDefinition::setValidate(const PrefSDK::QtLua::LuaFunction &vf)
    {
        this->_validatefunc = vf;
    }

    void FormatDefinition::setParse(const PrefSDK::QtLua::LuaFunction &pf)
    {
        this->_parsefunc = pf;
    }

    void FormatDefinition::setView(const PrefSDK::QtLua::LuaFunction &vf)
    {
        this->_viewfunc = vf;
    }

    bool FormatDefinition::callValidate(QHexEditData *hexeditdata, Logger* logger, qint64 baseoffset, bool ignoreerror)
    {
        this->setLogger(logger);

        if(!this->_validatefunc.isValid())
            return true; /* If 'validate-procedure' is not set, the format doesn't require validation */

        lua_State* l = LuaState::instance();
        FormatValidator fv(hexeditdata, baseoffset);

        QtLua::pushObject(l, &fv);
        bool err = this->_validatefunc(1, 0, true);

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
        lua_State* l = LuaState::instance();
        FormatTree* formattree = new FormatTree(hexeditdata, logger, baseoffset);

        QtLua::pushObject(LuaState::instance(), formattree);
        bool err = this->_parsefunc(1, 0, true);

        if(err)
        {
            this->error(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);

            formattree->deleteLater();
            return nullptr;
        }

        return formattree;
    }

    QWidget *FormatDefinition::callView(QHexEditData* hexeditdata, FormatTree *formattree)
    {
        if(!this->_viewfunc.isValid())
            return nullptr;

        QWidget* w = nullptr;
        lua_State* l = LuaState::instance();

        QtLua::pushObject(l, formattree);
        bool err = this->_viewfunc(1, 1);
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

    void FormatDefinition::setName(const QString &s)
    {
        this->_name = s;
    }

    void FormatDefinition::setCategory(const QString &s)
    {
        this->_category = s;
    }

    void FormatDefinition::setAuthor(const QString &s)
    {
        this->_author = s;
    }

    void FormatDefinition::setVersion(const QString &s)
    {
        this->_version = s;
    }
}
