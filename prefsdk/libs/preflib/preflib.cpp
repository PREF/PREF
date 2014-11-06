#include "preflib.h"

namespace PrefSDK
{
    PrefLib* PrefLib::_instance = nullptr;
    const char* PrefLib::PREF_TABLE = "pref";
    const char* PrefLib::FORMAT_TABLE = "format";

    PrefLib::PrefLib(QObject *parent): QObject(parent), _sdkversion(nullptr)
    {

    }

    void PrefLib::registerPrefTypes()
    {
        qRegisterMetaType<PrefSDK::DataBuffer*>();

        /* Format Parser */
        qRegisterMetaType<PrefSDK::FormatElement*>();
        qRegisterMetaType<PrefSDK::FieldElement*>();
        qRegisterMetaType<PrefSDK::BitField*>();
        qRegisterMetaType<PrefSDK::Field*>();
        qRegisterMetaType<PrefSDK::FieldArray*>();
        qRegisterMetaType<PrefSDK::Structure*>();

        /* Disassembler Engine */
        qRegisterMetaType<PrefSDK::Block*>();
        qRegisterMetaType<PrefSDK::Instruction*>();
        qRegisterMetaType<PrefSDK::Function*>();
        qRegisterMetaType<PrefSDK::Segment*>();
        qRegisterMetaType<PrefSDK::ReferenceSet*>();
        qRegisterMetaType<PrefSDK::Register*>();
        qRegisterMetaType<PrefSDK::ListingPrinter*>();

        /* QML Types */
        qmlRegisterType<PrefSDK::DataType>("Pref", 1, 0, "DataType");
        qmlRegisterType<PrefSDK::FormatElement>("Pref.Format", 1, 0, "ElementType");
    }

    void PrefLib::open(lua_State *l, SdkVersion *sdkversion)
    {
        if(PrefLib::_instance)
            return;

        PrefLib::registerPrefTypes();

        PrefLib::_instance = new PrefLib();
        PrefLib::_instance->_state = l;
        PrefLib::_instance->_sdkversion = sdkversion;

        PrefLib::_instance->_methods.append( {"setSdkVersion", &PrefLib::setSdkVersion} );
        PrefLib::_instance->_methods.append( {nullptr, nullptr} );

        luaL_register(l, PrefLib::PREF_TABLE, PrefLib::_instance->_methods.cbegin());

        PrefLib::_instance->buildByteOrder(l);
        PrefLib::_instance->buildDataType(l);
        PrefLib::_instance->buildMathTable(l);
        PrefLib::_instance->buildExporterTable(l);
        PrefLib::_instance->buildFormatTable(l);
        PrefLib::_instance->buildDisassemblerTable(l);

        lua_pop(l, 1); /* Pop Table */
    }

    PrefLib *PrefLib::instance()
    {
        return PrefLib::_instance;
    }

    int PrefLib::format_create(lua_State *l)
    {
        int argc = lua_gettop(l);

        if(!argc)
        {
            QtLua::pushObject(l, new FormatDefinition());
            return 1;
        }

        if(argc != 4)
        {
            throw PrefException(QString("pref.format.create(): Expected 4 arguments not %1").arg(argc));
            return 0;
        }

        for(int i = 1; i <= argc; i++)
        {
            if(lua_type(l, i) != LUA_TSTRING)
            {
                throw PrefException(QString("pref.format.create(): Argument %1 must be a string").arg(i));
                return 0;
            }
        }

        FormatDefinition* fd = new FormatDefinition(QString::fromUtf8(lua_tostring(l, 1)),
                                                    QString::fromUtf8(lua_tostring(l, 2)),
                                                    QString::fromUtf8(lua_tostring(l, 3)),
                                                    QString::fromUtf8(lua_tostring(l, 4)));
        QtLua::pushObject(l, fd);
        return 1;
    }

    int PrefLib::format_loadView(lua_State *l)
    {
        int argc = lua_gettop(l);

        if(argc < 2)
        {
            throw PrefException("pref.format.loadview(): Expected at least 2 arguments");
            return 0;
        }

        if(lua_type(l, 1) != LUA_TSTRING)
        {
            throw PrefException("pref.format.loadview(): Argument 1 must be a 'string' type");
            return 0;
        }

        if(lua_type(l, 2) != LUA_TUSERDATA)
        {
            throw PrefException("pref.format.loadview(): Argument 2 must be a 'FormatTree' type");
            return 0;
        }

        FormatTree* formattree = *(reinterpret_cast<FormatTree**>(lua_touserdata(l, 2)));
        QString qmlmain = QString("%1%2%3").arg(qApp->applicationDirPath(), QDir::separator(), QString::fromUtf8(lua_tostring(l, 1)));

        if(!QFileInfo(qmlmain).exists())
        {
            formattree->warning(QString("Cannot find '%1'").arg(qmlmain));
            lua_pushnil(l);
        }
        else
        {
            QQuickView* view = new QQuickView();
            QQmlContext* ctx = view->rootContext();

            ctx->setContextProperty("formattree", formattree);
            view->setResizeMode(QQuickView::SizeRootObjectToView);
            view->setSource(qmlmain);

            if(view->status() == QQuickView::Error)
            {
                QList<QQmlError> errors = view->errors();
                formattree->warning("Cannot load the view.");

                foreach(QQmlError error, errors)
                    formattree->logline("- " + error.toString());

                lua_pushnil(l);
            }
            else
                QtLua::pushObject(l, QWidget::createWindowContainer(view));
        }

        return 1;
    }

    int PrefLib::disassembler_create(lua_State *l)
    {
        int argc = lua_gettop(l);

        if(argc != 5)
        {
            throw PrefException(QString("pref.disassembler.create(): Expected 5 arguments not %1").arg(argc));
            return 0;
        }

        for(int i = 1; i <= 3; i++)
        {
            if(lua_type(l, i) != LUA_TSTRING)
            {
                throw PrefException(QString("pref.disassembler.create(): Argument %1 must be a string").arg(i));
                return 0;
            }
        }

        if(lua_type(l, 4) != LUA_TNUMBER)
        {
            throw PrefException("pref.disassembler.create(): Argument 4 must be a number");
            return 0;
        }

        if(lua_type(l, 5) != LUA_TUSERDATA)
        {
            throw PrefException("pref.disassembler.create(): Argument 5 must be an userdata");
            return 0;
        }

        DisassemblerDefinition* dd = new DisassemblerDefinition(QString::fromUtf8(lua_tostring(l, 1)),
                                                  QString::fromUtf8(lua_tostring(l, 2)),
                                                  QString::fromUtf8(lua_tostring(l, 3)),
                                                  static_cast<DataType::Type>(lua_tointeger(l, 4)),
                                                  *(reinterpret_cast<FormatDefinition**>(lua_touserdata(l, 5))));
        QtLua::pushObject(l, dd);
        return 1;
    }

    int PrefLib::dataType_sizeOf(lua_State *l)
    {
        int argc = lua_gettop(l);

        if(argc != 1)
        {
            throw PrefException(QString("pref.datatype.sizeof(): Expected 1 argument not %1").arg(argc));
            return 0;
        }

        if(lua_type(l, 1) != LUA_TNUMBER)
        {
            throw PrefException("pref.datatype.sizeof(): Argument 1 must be an integer");
            return 0;
        }

        DataType::Type datatype = static_cast<DataType::Type>(lua_tointeger(l, 1));
        lua_pushinteger(l, DataType::sizeOf(datatype));
        return 1;
    }

    int PrefLib::math_entropy(lua_State *l)
    {
        int argc = lua_gettop(l);

        if(!argc)
        {
            throw PrefException("pref.math.entropy(): 'buffer' Expected as first argument");
            return 0;
        }

        DataBuffer** luahexeditdata = reinterpret_cast<DataBuffer**>(lua_touserdata(l, 1));
        qint64 start = 0, size = (*luahexeditdata)->length();

        if(argc >= 2)
        {
            if(lua_type(l, 2) != LUA_TNUMBER)
            {
                throw PrefException("pref.math.entropy(): Argument 2 must be an integer");
                return 0;
            }

            start = static_cast<qint64>(lua_tointeger(l, 2));

            if(argc == 3)
            {
                if(lua_type(l, 3) != LUA_TNUMBER)
                {
                    throw PrefException("pref.math.entropy(): Argument 3 must be an integer");
                    return 0;
                }

                size = static_cast<qint64>(lua_tointeger(l, 3));
            }
        }

        qreal e = entropy((*luahexeditdata)->hexEditData(), start, size);
        lua_pushnumber(l, static_cast<lua_Number>(e));
        return 1;
    }

    int PrefLib::exporter_create(lua_State *l)
    {
        int argc = lua_gettop(l);

        if(!argc)
        {
            QtLua::pushObject(l, new ExporterDefinition());
            return 1;
        }

        if(argc != 4)
        {
            throw PrefException(QString("pref.exporter.create(): Expected 4 arguments not %1").arg(argc));
            return 0;
        }

        for(int i = 1; i <= argc; i++)
        {
            if(lua_type(l, i) != LUA_TSTRING)
            {
                throw PrefException(QString("pref.exporter.create(): Argument %1 must be a string").arg(i));
                return 0;
            }
        }

        ExporterDefinition* ed = new ExporterDefinition(QString::fromUtf8(lua_tostring(l, 1)),
                                                        QString::fromUtf8(lua_tostring(l, 2)),
                                                        QString::fromUtf8(lua_tostring(l, 3)),
                                                        QString::fromUtf8(lua_tostring(l, 4)));
        QtLua::pushObject(l, ed);
        return 1;
    }

    int PrefLib::setSdkVersion(lua_State *l)
    {
        int argc = lua_gettop(l);

        if((argc < 3) || (argc > 4))
        {
            throw PrefException("PrefLib::setSdkVersion(): Invalid Argument Count");
            return 0;
        }

        if((lua_type(l, 1) != LUA_TNUMBER) || (lua_type(l, 2) != LUA_TNUMBER) || (lua_type(l, 3) != LUA_TNUMBER))
        {
            throw PrefException("PrefLib::setSdkVersion(): Invalid Argument(s) Type(s)");
            return 0;
        }

        PrefLib::_instance->_sdkversion->IsLoaded = true;
        PrefLib::_instance->_sdkversion->Major = lua_tointeger(l, 1);
        PrefLib::_instance->_sdkversion->Minor = lua_tointeger(l, 2);
        PrefLib::_instance->_sdkversion->Revision = lua_tointeger(l, 3);

        if(argc == 4)
            PrefLib::_instance->_sdkversion->Custom = QString::fromUtf8(lua_tostring(l, 4));

        return 0;
    }

    void PrefLib::buildByteOrder(lua_State *l)
    {
        lua_newtable(l);

        lua_pushinteger(l, QSysInfo::LittleEndian);
        lua_setfield(l, -2, "LittleEndian");

        lua_pushinteger(l, QSysInfo::BigEndian);
        lua_setfield(l, -2, "BigEndian");

        lua_pushinteger(l, QSysInfo::ByteOrder);
        lua_setfield(l, -2, "PlatformEndian");

        lua_setfield(l, -2, "byteorder");
    }

    void PrefLib::buildDataType(lua_State *l)
    {
        const QMetaObject metaobj = DataType::staticMetaObject;
        QMetaEnum metaenum = metaobj.enumerator(metaobj.indexOfEnumerator("Type"));

        QtLua::pushEnum(l, metaenum);
        lua_pushcfunction(l, &PrefLib::dataType_sizeOf);
        lua_setfield(l, -2, "sizeof");

        lua_setfield(l, -2, "datatype");
    }

    void PrefLib::buildMathTable(lua_State *l)
    {
        lua_newtable(l);

        lua_pushcfunction(l, &PrefLib::math_entropy);
        lua_setfield(l, -2, "entropy");

        lua_setfield(l, -2, "math");
    }

    void PrefLib::buildExporterTable(lua_State *l)
    {
        lua_newtable(l);

        lua_pushcfunction(l, &PrefLib::exporter_create);
        lua_setfield(l, -2, "create");

        lua_setfield(l, -2, "exporter");
    }

    void PrefLib::buildFormatTable(lua_State *l)
    {
        lua_newtable(l);

        lua_pushcfunction(l, &PrefLib::format_create);
        lua_setfield(l, -2, "create");

        lua_pushcfunction(l, &PrefLib::format_loadView);
        lua_setfield(l, -2, "loadview");

        lua_setfield(l, -2, "format");
    }

    void PrefLib::buildDisassemblerTable(lua_State *l)
    {
        lua_newtable(l);

        this->buildSegmentTable(l);
        this->buildFunctionTypeTable(l);
        this->buildReferenceTypeTable(l);
        this->buildBlockTypeTable(l);
        this->buildSymbolTypeTable(l);

        lua_pushcfunction(l, &PrefLib::disassembler_create);
        lua_setfield(l, -2, "create");

        lua_setfield(l, -2, "disassembler");
    }

    void PrefLib::buildSegmentTable(lua_State *l)
    {
        const QMetaObject metaobj = Segment::staticMetaObject;
        QMetaEnum metaenum = metaobj.enumerator(metaobj.indexOfEnumerator("Type"));

        QtLua::pushEnum(l, metaenum);
        lua_setfield(l, -2, "segment");
    }

    void PrefLib::buildFunctionTypeTable(lua_State *l)
    {
        const QMetaObject metaobj = FunctionType::staticMetaObject;
        QMetaEnum metaenum = metaobj.enumerator(metaobj.indexOfEnumerator("Type"));

        QtLua::pushEnum(l, metaenum);
        lua_setfield(l, -2, "functiontype");
    }

    void PrefLib::buildReferenceTypeTable(lua_State *l)
    {
        const QMetaObject metaobj = Reference::staticMetaObject;
        QMetaEnum metaenum = metaobj.enumerator(metaobj.indexOfEnumerator("Type"));

        QtLua::pushEnum(l, metaenum);
        lua_setfield(l, -2, "referencetype");
    }

    void PrefLib::buildBlockTypeTable(lua_State *l)
    {
        const QMetaObject metaobj = Block::staticMetaObject;
        QMetaEnum metaenum = metaobj.enumerator(metaobj.indexOfEnumerator("Type"));

        QtLua::pushEnum(l, metaenum);
        lua_setfield(l, -2, "blocktype");
    }

    void PrefLib::buildSymbolTypeTable(lua_State *l)
    {
        const QMetaObject metaobj = Symbol::staticMetaObject;
        QMetaEnum metaenum = metaobj.enumerator(metaobj.indexOfEnumerator("Type"));

        QtLua::pushEnum(l, metaenum);
        lua_setfield(l, -2, "symboltype");
    }
}
