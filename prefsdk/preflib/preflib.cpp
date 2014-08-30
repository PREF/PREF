#include "preflib.h"

namespace PrefSDK
{
    PrefLib* PrefLib::_instance = nullptr;

    PrefLib::PrefLib(QObject *parent): QObject(parent)
    {

    }

    void PrefLib::open(lua_State *l)
    {
        if(PrefLib::_instance)
            return;

        PrefLib::_instance = new PrefLib();
        PrefLib::_instance->_state = l;

        PrefLib::_instance->_methods.append( {"setSdkVersion", &PrefLib::setSdkVersion} );
        PrefLib::_instance->_methods.append( {nullptr, nullptr} );

        luaL_register(l, "pref", PrefLib::_instance->_methods.cbegin());
        lua_pop(l, 1); /* Pop Table */
    }

    PrefLib *PrefLib::instance()
    {
        return PrefLib::_instance;
    }

    QString PrefLib::sdkVersion()
    {
        if(this->_sdkversion.IsLoaded)
        {
            QString s = QString("%1.%2").arg(QString::number(this->_sdkversion.Major), QString::number(this->_sdkversion.Minor));

            if(this->_sdkversion.Revision > 0)
                s.append(".").append(QString::number(this->_sdkversion.Revision));

            if(!this->_sdkversion.Custom.isEmpty())
                s.append(" ").append(this->_sdkversion.Custom);

            return s;
        }

        return "Missing SDK or Invalid Version";
    }

    HexView *PrefLib::addHexView(QHexEditData *hexeditdata, const QString &name, QLabel *lblinfo, QWidget *parent)
    {
        HexView* hv = new HexView(hexeditdata, name, lblinfo, parent);
        this->_loadedviews[hexeditdata] = hv;

        connect(hv, SIGNAL(destroyed(QObject*)), this, SLOT(onViewDestroyed(QObject*)));
        return hv;
    }

    DisassemblerView *PrefLib::addDisassemblerView(LoaderList::LoaderId selectedloader, QHexEditData *hexeditdata, const QString &name, QLabel *lblinfo, QWidget *parent)
    {
        DisassemblerView* dv = new DisassemblerView(selectedloader, hexeditdata, name, lblinfo, parent);
        this->_loadedviews[hexeditdata] = dv;

        connect(dv, SIGNAL(destroyed(QObject*)), this, SLOT(onViewDestroyed(QObject*)));
        return dv;
    }

    CompareView *PrefLib::addCompareView(const QString &leftcompare, const QString &rightcompare, const QString &name, QLabel *lblinfo, QWidget *parent)
    {
        CompareView* cv = new CompareView(leftcompare, rightcompare, name, lblinfo, parent);
        this->_loadedviews[cv->data()] = cv;

        connect(cv, SIGNAL(destroyed(QObject*)), this, SLOT(onViewDestroyed(QObject*)));
        return cv;
    }

    void PrefLib::onViewDestroyed(QObject* obj)
    {
        AbstractView* abstractview = qobject_cast<AbstractView*>(obj);
        this->_loadedviews.remove(abstractview->data());
    }

    int PrefLib::setSdkVersion(lua_State *l)
    {
        int argc = lua_gettop(l);

        if((argc < 3) || (argc > 4) || (lua_type(l, 1) != LUA_TNUMBER) || (lua_type(l, 2) != LUA_TNUMBER) || (lua_type(l, 3) != LUA_TNUMBER))
            return 0;

        PrefLib::_instance->_sdkversion.IsLoaded = true;
        PrefLib::_instance->_sdkversion.Major = lua_tointeger(l, 1);
        PrefLib::_instance->_sdkversion.Minor = lua_tointeger(l, 2);
        PrefLib::_instance->_sdkversion.Revision = lua_tointeger(l, 3);

        if(argc == 4)
            PrefLib::_instance->_sdkversion.Custom = QString::fromUtf8(lua_tostring(l, 4));

        return 0;
    }
}
