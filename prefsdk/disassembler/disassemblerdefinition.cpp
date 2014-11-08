#include "disassemblerdefinition.h"

namespace PrefSDK
{
    DisassemblerDefinition::DisassemblerDefinition(const QString &name, const QString &author, const QString &version, DataType::Type addresstype, FormatDefinition *formatdefinition, QObject *parent): LogObject(parent), _addresstype(addresstype), _memorybuffer(nullptr), _formatdefinition(formatdefinition), _name(name), _author(author), _version(version)
    {
        this->_formatdefinition->setParent(this);
        this->_baseaddress = DataValue(addresstype);
    }

    void DisassemblerDefinition::callDisassemble(QLabel* infolabel, DisassemblerListing* listing, MemoryBuffer* memorybuffer)
    {   
        if(!this->_disassemblefunc.isValid())
            return;

        lua_State* l = this->_disassemblefunc.state();

        while(!this->_addrstack.isEmpty())
        {
            DataValue address = this->_addrstack.pop();

            do
            {
                if(listing->isDecoded(address))
                    break;

                if(!listing->isAddress(address))
                {
                    this->warning(QString("Trying to disassemble %1, this address does not belong to any Segment").arg(address.toString(16)));
                    break;
                }

                QMetaObject::invokeMethod(infolabel, "setText", Qt::QueuedConnection, Q_ARG(QString, QString("Disassembling: %1").arg(address.toString(16))));

                lua_pushinteger(l, address.compatibleValue<lua_Integer>());
                QtLua::pushObject(l, listing);
                QtLua::pushObject(l, memorybuffer);
                bool err = this->_disassemblefunc(3, 1);

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

    void DisassemblerDefinition::callOutput(ListingPrinter *printer, Instruction *instruction, DisassemblerListing* listing, MemoryBuffer* memorybuffer)
    {
        if(!this->_outputfunc.isValid())
            return;

        lua_State* l = this->_outputfunc.state();

        QtLua::pushObject(l, printer);
        instruction->push();
        QtLua::pushObject(l, listing);
        QtLua::pushObject(l, memorybuffer);
        bool err = this->_outputfunc(4);

        if(err)
        {
            throw PrefException(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);
            return;
        }
    }

    bool DisassemblerDefinition::validate(QHexEditData *hexeditdata)
    {
        return this->_formatdefinition->callValidate(hexeditdata, this->_logger, 0, true);
    }

    QString DisassemblerDefinition::emitInstruction(Instruction *instruction, DisassemblerListing* listing, MemoryBuffer* memorybuffer)
    {
        ListingPrinter printer(this->_addresstype);

        this->callOutput(&printer, instruction, listing, memorybuffer);
        return printer.printString();
    }

    MemoryBuffer *DisassemblerDefinition::callMap(DisassemblerListing* listing, QHexEditData* hexeditdata)
    {
        bool b = this->validate(hexeditdata);

        if(!b || !this->_mapfunc.isValid())
            return nullptr;

        lua_State* l = this->_mapfunc.state();
        FormatTree* formattree = this->_formatdefinition->callParse(hexeditdata, this->_logger, 0);

        if(formattree->isEmpty())
            return nullptr;

        listing->setFormatTree(formattree);
        this->_baseaddress = this->callBaseAddress();
        MemoryBuffer* memorybuffer = new MemoryBuffer(hexeditdata, listing, this->_logger, this->_baseaddress, this->_addresstype, listing);

        QtLua::pushObject(l, listing);
        QtLua::pushObject(l, memorybuffer);
        bool err = this->_mapfunc(2);

        if(err)
        {
            throw PrefException(QString("ProcessorLoader::callMap(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
            lua_pop(l, 1);
        }

        /* Load Entry Points */
        const QList<Function*>& ep = listing->entryPoints();

        for(int i = 0; i < ep.count(); i++)
            this->_addrstack.push(ep[i]->startAddress());

        return memorybuffer;
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

    DataValue DisassemblerDefinition::callBaseAddress()
    {
        if(!this->_baseaddressfunc.isValid())
            return DataValue();

        lua_State* l = this->_baseaddressfunc.state();
        bool err = this->_baseaddressfunc(0, 1);

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
}
