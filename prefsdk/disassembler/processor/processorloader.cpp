#include "processorloader.h"

namespace PrefSDK
{
    ProcessorLoader::ProcessorLoader(const QString &name, const QString &author, const QString &version, FormatDefinition *formatdefinition, ProcessorDefinition *processordefinition, QObject *parent): DebugObject(parent), _processordefinition(processordefinition), _processoremulator(nullptr), _formatdefinition(formatdefinition), _listing(nullptr), _formattree(nullptr), _name(name), _author(author), _version(version)
    {
        this->_baseaddress = DataValue(processordefinition->addressType());
    }

    void ProcessorLoader::disassemble(QHexEditData *hexeditdata)
    {
        this->bind(hexeditdata);

        while(this->_processoremulator->hasMoreInstructions())
            this->disassembleInstruction(hexeditdata);

        this->_listing->calcFunctionBounds();
        this->_processordefinition->callElaborate(this->_listing, hexeditdata);
        this->_listing->analyzeOperands();
        this->unbind();
    }

    bool ProcessorLoader::validate(QHexEditData *hexeditdata)
    {
        this->bind(hexeditdata);
        bool v = this->_formatdefinition->callValidate(hexeditdata, 0, true);
        this->unbind();
        return v;
    }

    void ProcessorLoader::callMap(DisassemblerListing* listing, QHexEditData* hexeditdata, LogWidget* logwidget)
    {
        bool b = this->validate(hexeditdata);

        if(!b || !this->_mapfunc.isValid())
            return;

        this->bind(hexeditdata);
        this->_baseaddress = this->callBaseAddress();

        lua_State* l = this->_mapfunc.state();
        this->_formattree = this->_formatdefinition->callParse(hexeditdata, logwidget, 0);

        if(this->_formattree->isEmpty())
        {
            this->unbind();
            return;
        }

        this->_listing = listing;
        this->_listing->setAddressType(this->_processordefinition->addressType());
        this->_processoremulator = new ProcessorEmulator(listing, this->_processordefinition->addressType(), this);

        QtLua::pushObject(l, this->_formattree);
        bool err = this->_mapfunc(1);

        if(err)
        {
            throw PrefException(QString("ProcessorLoader::callMap(): %1").arg(QString::fromUtf8(lua_tostring(l, -1))));
            lua_pop(l, 1);
        }

        this->unbind();
    }

    void ProcessorLoader::createSegment(const QString &name, lua_Integer segmenttype, lua_Integer startaddress, lua_Integer size, lua_Integer baseoffset)
    {
        DataType::Type addresstype = static_cast<DataType::Type>(this->_processordefinition->addressType());

        this->_listing->createSegment(name, static_cast<Segment::Type>(segmenttype), DataValue::create(startaddress, addresstype),
                                                                                     DataValue::create(size, addresstype),
                                                                                     DataValue::create(baseoffset, addresstype));
    }

    void ProcessorLoader::createEntryPoint(const QString &name, lua_Integer address)
    {
        DataType::Type addresstype = static_cast<DataType::Type>(this->_processordefinition->addressType());
        DataValue addressvalue = DataValue::create(address, addresstype);

        this->_listing->createFunction(name, FunctionTypes::EntryPoint, addressvalue);
        this->_processoremulator->pushValue(addressvalue, Reference::EntryPoint);
    }

    DataValue ProcessorLoader::callBaseAddress()
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

        DataValue baseaddress = DataValue::create(lua_tointeger(l, -1), this->_processordefinition->addressType());
        lua_pop(l, 1);
        return baseaddress;
    }

    void ProcessorLoader::disassembleInstruction(QHexEditData* hexeditdata)
    {
        ProcessorEmulator::Address procaddress = this->_processoremulator->popValue();

        Instruction* instruction = this->_listing->createInstruction(procaddress.first, this->_processordefinition->instructionSet()->opcodeType());
        lua_Integer size = this->_processordefinition->callAnalyze(instruction, this->_baseaddress, hexeditdata);

        if(size <= 0)
        {
            this->warning(QString("Unknown Instruction at %1h").arg(procaddress.first.toString(16)));
            this->_processoremulator->pushValue(procaddress.first + instruction->sizeValue(), Reference::Flow);
        }
        else
        {
            this->_processordefinition->decode(instruction);
            const RegisterSet* registerset = this->_processordefinition->registerSet();

            for(int i = 0; i < instruction->operandsCount(); i++)
            {
                Operand* operand = instruction->operand(i);

                if(operand->type() == Operand::Register)
                    operand->setRegisterName(registerset->name(operand->operandValue()));
            }

            if(Reference::isCall(procaddress.second))
                this->_listing->createFunction(FunctionTypes::Function, procaddress.first);
            else if(Reference::isJump(procaddress.second))
            {
                SymbolTable* symboltable = this->_listing->symbolTable();
                symboltable->set(procaddress.first, QString("j_%1").arg(procaddress.first.toString(16)));
            }

            this->_processordefinition->callEmulate(this->_processoremulator, instruction, hexeditdata);
        }
    }

    FormatDefinition *ProcessorLoader::format() const
    {
        return this->_formatdefinition;
    }

    ProcessorDefinition *ProcessorLoader::processor() const
    {
        return this->_processordefinition;
    }

    const QtLua::LuaFunction &ProcessorLoader::map() const
    {
        return this->_mapfunc;
    }

    void ProcessorLoader::setName(const QString &n)
    {
        this->_name = n;
    }

    void ProcessorLoader::setAuthor(const QString &a)
    {
        this->_author = a;
    }

    void ProcessorLoader::setVersion(const QString &v)
    {
        this->_version = v;
    }

    void ProcessorLoader::setBaseAddress(const PrefSDK::QtLua::LuaFunction &ba)
    {
        this->_baseaddressfunc = ba;
    }

    QString ProcessorLoader::id() const
    {
        return QString(QCryptographicHash::hash(QString("%1").arg(this->_name.toUpper()).toUtf8(), QCryptographicHash::Md5));
    }

    const QString &ProcessorLoader::author() const
    {
        return this->_author;
    }

    const QString &ProcessorLoader::version() const
    {
        return this->_version;
    }

    const PrefSDK::QtLua::LuaFunction &ProcessorLoader::baseAddress() const
    {
        return this->_baseaddressfunc;
    }

    const QString &ProcessorLoader::name() const
    {
        return this->_name;
    }

    void ProcessorLoader::setMap(const PrefSDK::QtLua::LuaFunction &mf)
    {
        this->_mapfunc = mf;
    }
}
