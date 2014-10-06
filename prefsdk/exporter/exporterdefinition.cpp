#include "exporterdefinition.h"

namespace PrefSDK
{
    ExporterDefinition::ExporterDefinition(QObject *parent): QObject(parent)
    {
    }

    ExporterDefinition::ExporterDefinition(const QString &name, const QString &description, const QString &author, const QString &version, QObject *parent): QObject(parent), _name(name), _description(description), _author(author), _version(version)
    {

    }

    QString ExporterDefinition::id()
    {
        return QString(QCryptographicHash::hash(this->_name.toUpper().toUtf8(), QCryptographicHash::Md5));
    }

    const QString &ExporterDefinition::name() const
    {
        return this->_name;
    }

    const QString &ExporterDefinition::description() const
    {
        return this->_description;
    }

    const QString &ExporterDefinition::author() const
    {
        return this->_author;
    }

    const QString &ExporterDefinition::version() const
    {
        return this->_version;
    }

    void ExporterDefinition::callDump(QHexEditData *hexeditdatain, const QString& filename, qint64 startoffset, qint64 endoffset)
    {
        if(!this->_dumpfunction.isValid())
            return;

        lua_State* l = this->_dumpfunction.state();
        QHexEditData* hexeditdataout = QHexEditData::fromFile(filename);
        LuaHexEditData* luahexeditdatain = new LuaHexEditData(hexeditdatain);
        LuaHexEditData* luahexeditdataout = new LuaHexEditData(hexeditdataout);

        QtLua::pushObject(l, luahexeditdatain);
        QtLua::pushObject(l, luahexeditdataout);
        lua_pushinteger(l, startoffset);
        lua_pushinteger(l, endoffset);
        bool err = this->_dumpfunction(4);

        if(err)
        {
            throw PrefException(QString("ExporterDefinition:dump(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
            lua_pop(l, 1);
        }

        luahexeditdataout->deleteLater();
        luahexeditdatain->deleteLater();
        hexeditdataout->deleteLater();
    }

    const PrefSDK::QtLua::LuaFunction& ExporterDefinition::dump() const
    {
        return this->_dumpfunction;
    }

    void ExporterDefinition::setName(const QString &s)
    {
        this->_name = s;
    }

    void ExporterDefinition::setDescription(const QString &s)
    {
        this->_description = s;
    }

    void ExporterDefinition::setAuthor(const QString &s)
    {
        this->_author = s;
    }

    void ExporterDefinition::setVersion(const QString &s)
    {
        this->_version = s;
    }

    void ExporterDefinition::setDump(const PrefSDK::QtLua::LuaFunction &df)
    {
        this->_dumpfunction = df;
    }
}
