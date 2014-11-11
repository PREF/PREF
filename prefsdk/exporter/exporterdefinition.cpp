#include "exporterdefinition.h"

namespace PrefSDK
{
    ExporterDefinition::ExporterDefinition(const QtLua::LuaTable &exportertable, QObject *parent): QObject(parent), _exportertable(exportertable)
    {

    }

    QString ExporterDefinition::id()
    {
        return QString(QCryptographicHash::hash(this->name().toUpper().toUtf8(), QCryptographicHash::Md5));
    }

    QString ExporterDefinition::name() const
    {
        return this->_exportertable.getString("name");
    }

    QString ExporterDefinition::description() const
    {
        return this->_exportertable.getString("description");
    }

    QString ExporterDefinition::author() const
    {
        return this->_exportertable.getString("author");
    }

    QString ExporterDefinition::version() const
    {
        return this->_exportertable.getString("version");
    }

    void ExporterDefinition::callDump(QHexEditData *hexeditdatain, const QString& filename, qint64 startoffset, qint64 endoffset)
    {
        if(!this->_exportertable.fieldExists("dump"))
            return;

        lua_State* l = this->_exportertable.instance();
        QtLua::LuaFunction dumpfunc = this->_exportertable.getFunction("dump");

        QHexEditData* hexeditdataout = QHexEditData::fromFile(filename);
        DataBuffer databufferin(hexeditdatain);
        DataBuffer databufferout(hexeditdataout);

        QtLua::pushObject(l, &databufferin);
        QtLua::pushObject(l, &databufferout);
        lua_pushinteger(l, startoffset);
        lua_pushinteger(l, endoffset);
        bool err = dumpfunc(4);

        if(err)
        {
            throw PrefException(QString("ExporterDefinition:dump(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
            lua_pop(l, 1);
        }

        hexeditdataout->deleteLater();
    }}
