#include "disassemblerdefinition.h"

namespace PrefSDK
{
    DisassemblerDefinition::DisassemblerDefinition(const QtLua::LuaTable &disassemblertable, QObject *parent): QObject(parent), _disassemblertable(disassemblertable), _listing(nullptr), _formattree(nullptr), _memorybuffer(nullptr)
    {
        this->_formatdefinition = new FormatDefinition(this->_disassemblertable.getTable("formatdefinition"), this);
        this->_baseaddress = DataValue(this->addressType());
        this->_disassemblertable.bind(this);
    }

    void DisassemblerDefinition::callDisassemble(QLabel* infolabel, Logger* logger)
    {   
        if(!this->_disassemblertable.fieldExists("disassemble"))
            return;

        lua_State* l = this->_disassemblertable.instance();
        QtLua::LuaFunction disassemblefunc = this->_disassemblertable.getFunction("disassemble");

        while(!this->_addrstack.isEmpty())
        {
            DataValue address = this->_addrstack.pop();

            do
            {
                if(this->_listing->isDecoded(address))
                    break;

                if(!this->_listing->isAddress(address))
                {
                    logger->warning(QString("Trying to disassemble %1, this address does not belong to any Segment").arg(address.toString(16)));
                    break;
                }

                QMetaObject::invokeMethod(infolabel, "setText", Qt::QueuedConnection, Q_ARG(QString, QString("Disassembling: %1").arg(address.toString(16))));

                lua_pushinteger(l, address.compatibleValue<lua_Integer>());
                bool err = disassemblefunc(1, true);

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

                address = DataValue::create(lua_tointeger(l, -1), this->addressType());
                lua_pop(l, 1);
            }
            while(!address.isZero());
        }
    }

    void DisassemblerDefinition::callOutput(ListingPrinter *printer, Instruction *instruction)
    {
        if(!this->_disassemblertable.fieldExists("output"))
            return;

        lua_State* l = this->_disassemblertable.instance();
        QtLua::LuaFunction outputfunc = this->_disassemblertable.getFunction("output");

        QtLua::pushObject(l, printer);
        instruction->push();
        bool err = outputfunc(2);

        if(err)
        {
            throw PrefException(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);
            return;
        }
    }

    void DisassemblerDefinition::callInitialize()
    {
        if(!this->_disassemblertable.fieldExists("initialize"))
            return;

        lua_State* l = this->_disassemblertable.instance();
        QtLua::LuaFunction initializefunc = this->_disassemblertable.getFunction("initialize");

        bool err = initializefunc();

        if(err)
        {
            throw PrefException(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);
            return;
        }
    }

    void DisassemblerDefinition::callFinalize()
    {
        if(!this->_disassemblertable.fieldExists("finalize"))
            return;

        lua_State* l = this->_disassemblertable.instance();
        QtLua::LuaFunction finalizefunc = this->_disassemblertable.getFunction("finalize");

        bool err = finalizefunc();

        if(err)
        {
            throw PrefException(QString::fromUtf8(lua_tostring(l, -1)));
            lua_pop(l, 1);
            return;
        }
    }

    bool DisassemblerDefinition::validate(QHexEditData *hexeditdata)
    {
        return this->_formatdefinition->callValidate(hexeditdata, nullptr, 0);
    }

    QString DisassemblerDefinition::emitInstruction(Instruction *instruction)
    {
        ListingPrinter printer(this->addressType());

        this->callOutput(&printer, instruction);
        return printer.printString();
    }

    bool DisassemblerDefinition::callMap(QHexEditData* hexeditdata, Logger* logger)
    {
        bool b = this->validate(hexeditdata);

        if(!b || !this->_disassemblertable.fieldExists("map"))
            return false;

        this->_formattree = this->_formatdefinition->callParse(hexeditdata, logger, 0);

        if(this->_formattree->isEmpty())
            return false;

        lua_State* l = this->_disassemblertable.instance();
        QtLua::LuaFunction mapfunc = this->_disassemblertable.getFunction("map");

        this->_baseaddress = this->callBaseAddress();
        this->_listing = new DisassemblerListing(hexeditdata, logger, this->addressType(), this);
        this->_memorybuffer = new MemoryBuffer(hexeditdata, this->_listing, logger, this->_baseaddress, this->addressType(), this);
        bool err = mapfunc();

        if(err)
        {
            throw PrefException(QString("ProcessorLoader::callMap(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
            lua_pop(l, 1);
            return false;
        }

        /* Load Entry Points */
        const QList<Function*>& ep = this->_listing->entryPoints();

        for(int i = 0; i < ep.count(); i++)
            this->_addrstack.push(ep[i]->startAddress());

        return true;
    }

    void DisassemblerDefinition::enqueue(lua_Integer address)
    {
        this->_addrstack.push(DataValue::create(address, this->addressType()));
    }

    lua_Integer DisassemblerDefinition::next(const QtLua::LuaTable &instructiontable)
    {
        return instructiontable.getInteger("address") + instructiontable.getInteger("size");
    }

    QString DisassemblerDefinition::hexdump(const QtLua::LuaTable &instructiontable)
    {
        QString dump;
        Instruction instruction(instructiontable, this->addressType());
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
        if(!this->_disassemblertable.fieldExists("baseAddress"))
            return DataValue();

        lua_State* l = this->_disassemblertable.instance();
        QtLua::LuaFunction baseaddressfunc = this->_disassemblertable.getFunction("baseAddress");
        bool err = baseaddressfunc(0, true);

        if(err)
        {
            throw PrefException(QString("ProcessorLoader::disassemble(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
            lua_pop(l, 1);
            return DataValue();
        }

        DataValue baseaddress = DataValue::create(lua_tointeger(l, -1), this->addressType());
        lua_pop(l, 1);
        return baseaddress;
    }

    FormatTree *DisassemblerDefinition::formatTree() const
    {
        return this->_formattree;
    }

    DisassemblerListing *DisassemblerDefinition::listing() const
    {
        return this->_listing;
    }

    MemoryBuffer *DisassemblerDefinition::memoryBuffer() const
    {
        return this->_memorybuffer;
    }

    QString DisassemblerDefinition::id() const
    {
        return QString(QCryptographicHash::hash(QString("%1").arg(this->name().toUpper()).toUtf8(), QCryptographicHash::Md5));
    }

    QString DisassemblerDefinition::name() const
    {
        return this->_disassemblertable.getString("name");
    }

    QString DisassemblerDefinition::author() const
    {
        return this->_disassemblertable.getString("author");
    }

    QString DisassemblerDefinition::version() const
    {
        return this->_disassemblertable.getString("version");
    }

    DataType::Type DisassemblerDefinition::addressType() const
    {
        return static_cast<DataType::Type>(this->_disassemblertable.getInteger("addresstype"));
    }
}
