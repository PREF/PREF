#include "disassemblerdefinition.h"

namespace PrefSDK
{
    DisassemblerDefinition::DisassemblerDefinition(const QString &name, const QString &author, const QString &version, DataType::Type addresstype, FormatDefinition *formatdefinition, QObject *parent): LogObject(parent), _addresstype(addresstype), _memorybuffer(nullptr), _formatdefinition(formatdefinition), _listing(nullptr), _formattree(nullptr), _name(name), _author(author), _version(version)
    {
        this->_formatdefinition->setParent(this);
        this->_baseaddress = DataValue(addresstype);
    }

    void DisassemblerDefinition::callDisassemble(QLabel* infolabel, bool elaborateinstructions, bool analyzelisting)
    {   
        if(!this->_disassemblefunc.isValid())
            return;

        lua_State* l = this->_disassemblefunc.state();

        while(!this->_addrstack.isEmpty())
        {
            DataValue address = this->_addrstack.pop();

            do
            {
                if(this->_listing->isDecoded(address))
                    break;

                if(!this->_listing->isAddress(address))
                {
                    this->warning(QString("Trying to disassemble %1, this address does not belong to any Segment").arg(address.toString(16)));
                    break;
                }

                QMetaObject::invokeMethod(infolabel, "setText", Qt::QueuedConnection, Q_ARG(QString, QString("Disassembling: %1").arg(address.toString(16))));

                lua_pushinteger(l, address.compatibleValue<lua_Integer>());
                bool err = this->_disassemblefunc(1, 1);

                if(err)
                {
                    throw PrefException(QString::fromUtf8(lua_tostring(l, -1)));
                    lua_pop(l, 1);
                    return;
                }

                if(lua_isnoneornil(l, -1))
                {
                    lua_pop(l, 1);
                    break;
                }

                address = DataValue::create(lua_tointeger(l, -1), this->_addresstype);
                lua_pop(l, 1);
            }
            while(!address.isZero());
        }
    }

    void DisassemblerDefinition::callOutput(ListingPrinter *printer, Instruction *instruction)
    {
        if(!this->_outputfunc.isValid())
            return;

        lua_State* l = this->_outputfunc.state();

        QtLua::pushObject(l, printer);
        instruction->push();
        bool err = this->_outputfunc(2);

        if(err)
        {
            throw PrefException(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);
            return;
        }
    }

    bool DisassemblerDefinition::validate(QHexEditData *hexeditdata, Logger* logger)
    {
        return this->_formatdefinition->callValidate(hexeditdata, logger, 0, true);
    }

    QString DisassemblerDefinition::emitInstruction(Instruction *instruction)
    {
        ListingPrinter printer(this->_addresstype);

        this->callOutput(&printer, instruction);
        return printer.printString();
    }

    void DisassemblerDefinition::callMap(DisassemblerListing* listing, QHexEditData* hexeditdata, Logger* logger)
    {
        bool b = this->validate(hexeditdata, logger);

        if(!b || !this->_mapfunc.isValid())
            return;

        lua_State* l = this->_mapfunc.state();
        this->_formattree = this->_formatdefinition->callParse(hexeditdata, logger, 0);

        if(this->_formattree->isEmpty())
            return;

        this->_baseaddress = this->callBaseAddress();

        this->_listing = listing;
        this->setLogger(logger);
        this->_memorybuffer = new MemoryBuffer(hexeditdata, listing, logger, this->_baseaddress, this->_addresstype, this);
        bool err = this->_mapfunc(0, 0, true);

        if(err)
        {
            throw PrefException(QString("ProcessorLoader::callMap(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
            lua_pop(l, 1);
        }
    }

    void DisassemblerDefinition::callElaborate()
    {
        if(!this->_elaboratefunc.isValid())
            return;

        lua_State* l = this->_elaboratefunc.state();
        QtLua::pushObject(l, this->_listing);
        QtLua::pushObject(l, this->_formattree);
        bool err = this->_elaboratefunc(2);

        if(err)
        {
            throw PrefException(QString("ProcessorLoader::callElaborate(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
            lua_pop(l, 1);
        }
    }

    void DisassemblerDefinition::setSymbol(const DataValue &address, Symbol::Type symboltype, const QString &name)
    {
        SymbolTable* symboltable = this->_listing->symbolTable();
        QString symbolname = name.isEmpty() ? QString("data_%1").arg(address.toString(16)) : name;

        if((symboltype == Symbol::Address) && this->_listing->isAddress(address))
        {
            qint64 len = this->_listing->pointsToString(address);

            if(len)
            {
                symboltable->set(Symbol::String, address, DataValue::create(len, this->_addresstype), DataType::AsciiString, symbolname);
                return;
            }
        }

        symboltable->set(symboltype, address, symbolname);
    }

    void DisassemblerDefinition::enqueue(lua_Integer address)
    {
        this->_addrstack.push(DataValue::create(address, this->_addresstype));
    }

    lua_Integer DisassemblerDefinition::next(const QtLua::LuaTable &instructiontable)
    {
        return instructiontable.getInteger("address") + instructiontable.getInteger("size");
    }

    QString DisassemblerDefinition::hexdump(const QtLua::LuaTable &instructiontable)
    {
        QString dump;
        Instruction instruction(instructiontable, this->_addresstype);
        DataValue address = instruction.startAddress();

        while(address < instruction.endAddress())
        {
            if(!dump.isEmpty())
                dump.append(" ");

            DataValue b = this->_memorybuffer->read(address, DataType::UInt8);
            dump.append(b.toString(16));
            address++;
        }

        return dump;
    }

    QString DisassemblerDefinition::symbolName(lua_Integer address)
    {
        SymbolTable* symboltable = this->_listing->symbolTable();
        DataValue addressvalue = DataValue::create(address, this->_addresstype);

        if(!symboltable->contains(addressvalue))
        {
            QString addressstring = addressvalue.toString(16);
            this->warning(QString("Trying to get an invalid symbol at %1").arg(addressstring));
            return addressstring;
        }

        return symboltable->name(addressvalue);
    }

    bool DisassemblerDefinition::isAddress(lua_Integer address)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        return this->_listing->isAddress(addressvalue);
    }

    bool DisassemblerDefinition::isSymbol(lua_Integer address)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        return this->_listing->symbolTable()->contains(addressvalue);
    }

    void DisassemblerDefinition::addInstruction(const PrefSDK::QtLua::LuaTable &instructiontable)
    {
        this->_listing->addInstruction(new Instruction(instructiontable, this->_addresstype));
    }

    void DisassemblerDefinition::createSegment(const QString &name, lua_Integer segmenttype, lua_Integer startaddress, lua_Integer size, lua_Integer baseoffset)
    {
        DataType::Type addresstype = static_cast<DataType::Type>(this->_addresstype);

        this->_listing->createSegment(name, static_cast<Segment::Type>(segmenttype), DataValue::create(startaddress, addresstype),
                                                                                     DataValue::create(size, addresstype),
                                                                                     DataValue::create(baseoffset, addresstype));
    }

    void DisassemblerDefinition::createEntryPoint(lua_Integer address, const QString &name)
    {
        DataType::Type addresstype = static_cast<DataType::Type>(this->_addresstype);
        DataValue addressvalue = DataValue::create(address, addresstype);

        this->_listing->createFunction(name, FunctionType::EntryPointFunction, addressvalue);
        this->_addrstack.push(addressvalue);
    }

    void DisassemblerDefinition::createFunction(lua_Integer address, const QString &name)
    {
        DataType::Type addresstype = static_cast<DataType::Type>(this->_addresstype);
        DataValue startaddress = DataValue::create(address, addresstype);

        if(!this->_listing->isAddress(startaddress))
        {
            this->warning(QString("Trying to creating an out of segment function at %1").arg(startaddress.toString(16)));
            return;
        }

        QString funcname = name.isEmpty() ? QString("sub_%1").arg(startaddress.toString(16)) : name;
        this->_listing->createFunction(funcname, FunctionType::NormalFunction, startaddress);
    }

    void DisassemblerDefinition::createFunction(lua_Integer address)
    {
        this->createFunction(address, QString());
    }

    void DisassemblerDefinition::setSymbol(lua_Integer address, lua_Integer symboltype, const QString &name)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        this->setSymbol(addressvalue, static_cast<Symbol::Type>(symboltype), name);
    }

    void DisassemblerDefinition::setSymbol(lua_Integer address, lua_Integer symboltype)
    {
        this->setSymbol(address, symboltype, QString());
    }

    void DisassemblerDefinition::setFunction(lua_Integer address, const QString &name)
    {
        this->setFunction(address, name, FunctionType::NormalFunction);
    }

    void DisassemblerDefinition::setFunction(lua_Integer address, const QString &name, lua_Integer functiontype)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        FunctionType::Type ft = static_cast<FunctionType::Type>(functiontype);
        const DisassemblerListing::FunctionMap& functions = this->_listing->functions();

        if(!functions.contains(addressvalue))
            return;

        this->setSymbol(addressvalue, Symbol::Function, name);
        Function* f = functions[addressvalue];

        if(ft != f->type())
            f->setType(ft);
    }

    void DisassemblerDefinition::setConstant(QObject *instruction, lua_Integer datatype, lua_Integer value, const QString &name)
    {
        ConstantTable* constanttable = this->_listing->constantTable();
        constanttable->set(qobject_cast<Instruction*>(instruction), DataValue::create(value, static_cast<DataType::Type>(datatype)), name);
    }

    DataValue DisassemblerDefinition::callBaseAddress()
    {
        if(!this->_baseaddressfunc.isValid())
            return DataValue();

        lua_State* l = this->_baseaddressfunc.state();

        QtLua::pushObject(l, this->_formattree);
        bool err = this->_baseaddressfunc(1, 1);

        if(err)
        {
            throw PrefException(QString("ProcessorLoader::disassemble(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
            lua_pop(l, 1);
            return DataValue();
        }

        DataValue baseaddress = DataValue::create(lua_tointeger(l, -1), this->_addresstype);
        lua_pop(l, 1);
        return baseaddress;
    }

    FormatDefinition *DisassemblerDefinition::format() const
    {
        return this->_formatdefinition;
    }

    FormatTree *DisassemblerDefinition::formatTree() const
    {
        return this->_formattree;
    }

    const QtLua::LuaFunction &DisassemblerDefinition::map() const
    {
        return this->_mapfunc;
    }

    const QtLua::LuaFunction &DisassemblerDefinition::disassemble() const
    {
        return this->_disassemblefunc;
    }

    const QtLua::LuaFunction &DisassemblerDefinition::output() const
    {
        return this->_outputfunc;
    }

    const QtLua::LuaFunction &DisassemblerDefinition::elaborate() const
    {
        return this->_elaboratefunc;
    }

    MemoryBuffer *DisassemblerDefinition::memoryBuffer() const
    {
        return this->_memorybuffer;
    }

    void DisassemblerDefinition::setName(const QString &n)
    {
        this->_name = n;
    }

    void DisassemblerDefinition::setAuthor(const QString &a)
    {
        this->_author = a;
    }

    void DisassemblerDefinition::setVersion(const QString &v)
    {
        this->_version = v;
    }

    void DisassemblerDefinition::setBaseAddress(const PrefSDK::QtLua::LuaFunction &ba)
    {
        this->_baseaddressfunc = ba;
    }

    QString DisassemblerDefinition::id() const
    {
        return QString(QCryptographicHash::hash(QString("%1").arg(this->_name.toUpper()).toUtf8(), QCryptographicHash::Md5));
    }

    const QString &DisassemblerDefinition::author() const
    {
        return this->_author;
    }

    const QString &DisassemblerDefinition::version() const
    {
        return this->_version;
    }

    DataType::Type DisassemblerDefinition::addressType() const
    {
        return this->_addresstype;
    }

    const PrefSDK::QtLua::LuaFunction &DisassemblerDefinition::baseAddress() const
    {
        return this->_baseaddressfunc;
    }

    const QString &DisassemblerDefinition::name() const
    {
        return this->_name;
    }

    void DisassemblerDefinition::setMap(const PrefSDK::QtLua::LuaFunction &mf)
    {
        this->_mapfunc = mf;
    }

    void DisassemblerDefinition::setDisassemble(const QtLua::LuaFunction &df)
    {
        this->_disassemblefunc = df;
    }

    void DisassemblerDefinition::setOutput(const QtLua::LuaFunction &of)
    {
        this->_outputfunc = of;
    }

    void DisassemblerDefinition::setElaborate(const QtLua::LuaFunction &ef)
    {
        this->_elaboratefunc = ef;
    }
}
