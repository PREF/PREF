#include "symboltable.h"

namespace PrefSDK
{
    SymbolTable::SymbolTable(DataType::Type addresstype, QObject *parent): LogObject(parent), _addresstype(addresstype)
    {
    }

    lua_Integer SymbolTable::count() const
    {
        return this->_symboltable.count();
    }

    DataType::Type SymbolTable::addressType() const
    {
        return this->_addresstype;
    }

    SymbolTable::Iterator SymbolTable::begin()
    {
        return this->_symboltable.begin();
    }

    SymbolTable::Iterator SymbolTable::end()
    {
        return this->_symboltable.end();
    }

    bool SymbolTable::contains(const DataValue &address) const
    {
        return this->_symboltable.contains(address);
    }

    bool SymbolTable::isType(const DataValue &address, Symbol::Type symboltype) const
    {
        if(this->_symboltable.contains(address))
        {
            Symbol* symbol = this->_symboltable[address];
            return symbol->type() == symboltype;
        }

        return false;
    }

    void SymbolTable::set(Symbol::Type symboltype, const DataValue &address)
    {
        if(symboltype == Symbol::Unknown)
            return;

        QString symbolname;

        if(symboltype == Symbol::Function)
            symbolname = "sub_";
        else if(symboltype == Symbol::Label)
            symbolname = "label_";
        else if(symboltype == Symbol::String)
            symbolname = "string_";
        else
            symbolname = "data_";

        this->set(symboltype, address, DataValue(), symbolname + address.toString(16));
    }

    void SymbolTable::set(Symbol::Type symboltype, const DataValue &address, const QString &name)
    {
        this->set(symboltype, address, DataValue(), address.dataType(), name);
    }

    void SymbolTable::set(Symbol::Type symboltype, const DataValue &address, const DataValue &calleraddress, const QString &name)
    {
        DataValue sizevalue = DataValue::create(DataType::sizeOf(address.dataType()), this->_addresstype);
        this->set(symboltype, address, sizevalue, calleraddress, name);
    }

    void SymbolTable::set(Symbol::Type symboltype, const DataValue &address, const DataValue &size, const DataValue &calleraddress)
    {
        this->set(symboltype, address, size, calleraddress, QString());
    }

    void SymbolTable::set(Symbol::Type symboltype, const DataValue &address, const DataValue& size, const DataValue &calleraddress, const QString &name)
    {
        Symbol* symbol = nullptr;

        if(this->_symboltable.contains(address))
        {
            symbol = this->_symboltable[address];

            if(!calleraddress.isNull())
                symbol->addSource(calleraddress);

            if(symbol->type() > symboltype) /* Check SymbolType's weight */
                return;

            this->popSymbol(symbol);

            symbol->setType(symboltype);
            symbol->setSize(size);

            if(!name.isEmpty())
                symbol->setName(name);
        }
        else
        {
            QString symbolname = name;

            if(symbolname.isEmpty())
            {
                if(symboltype == Symbol::Function)
                    symbolname = "sub_";
                else if(symboltype == Symbol::Label)
                    symbolname = "label_";
                else if(symboltype == Symbol::String)
                    symbolname = "string_";
                else
                    symbolname = "data_";

                symbolname.append(address.toString(16));
            }

            symbol = new Symbol(symboltype, address, size, symbolname, this);

            if(!calleraddress.isNull())
                symbol->addSource(calleraddress);

            this->_symboltable[address] = symbol;
        }

        this->pushSymbol(symbol);
    }

    QString SymbolTable::name(const DataValue &address) const
    {
        if(!this->_symboltable.contains(address))
            return QString();

        return this->_symboltable[address]->name();
    }

    Symbol *SymbolTable::get(const DataValue &address) const
    {
        return this->_symboltable[address];
    }

    QList<Symbol *> SymbolTable::functions() const
    {
        return this->_functions.values();
    }

    QList<Symbol *> SymbolTable::labels() const
    {
        return this->_labels.values();
    }

    QList<Symbol*> SymbolTable::variables() const
    {
        return this->_variables.values();
    }

    QList<Symbol *> SymbolTable::strings() const
    {
        return this->_strings.values();
    }

    bool SymbolTable::contains(lua_Integer address)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        return this->contains(addressvalue);
    }

    void SymbolTable::set(lua_Integer address, lua_Integer size, lua_Integer calleraddress, lua_Integer symboltype, const QString &name)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        DataValue sizevalue = DataValue::create(size, this->_addresstype);
        DataValue calleraddressvalue = DataValue::create(calleraddress, this->_addresstype);
        this->set(static_cast<Symbol::Type>(symboltype), addressvalue, sizevalue, calleraddressvalue, name);
    }

    void SymbolTable::set(lua_Integer address, lua_Integer calleraddress, lua_Integer symboltype, const QString &name)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        DataValue calleraddressvalue = DataValue::create(calleraddress, this->_addresstype);
        this->set(static_cast<Symbol::Type>(symboltype), addressvalue, calleraddressvalue, name);
    }

    void SymbolTable::set(lua_Integer address, lua_Integer size, lua_Integer calleraddress, lua_Integer symboltype)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        DataValue sizevalue = DataValue::create(size, this->_addresstype);
        DataValue calleraddressvalue = DataValue::create(calleraddress, this->_addresstype);
        this->set(static_cast<Symbol::Type>(symboltype), addressvalue, sizevalue, calleraddressvalue);
    }

    void SymbolTable::set(lua_Integer address, lua_Integer calleraddress, lua_Integer symboltype)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        DataValue calleraddressvalue = DataValue::create(calleraddress, this->_addresstype);
        this->set(static_cast<Symbol::Type>(symboltype), addressvalue, calleraddressvalue, QString());
    }

    void SymbolTable::set(lua_Integer address, lua_Integer symboltype)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        this->set(static_cast<Symbol::Type>(symboltype), addressvalue);
    }

    QString SymbolTable::name(lua_Integer address)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);
        return this->name(addressvalue);
    }

    lua_Integer SymbolTable::type(lua_Integer address)
    {
        DataValue addressvalue = DataValue::create(address, this->_addresstype);

        if(!this->_symboltable.contains(addressvalue))
            return Symbol::Unknown;

        return static_cast<lua_Integer>(this->_symboltable[addressvalue]->type());
    }

    void SymbolTable::popSymbol(Symbol *symbol)
    {
        Symbol::Type symboltype = symbol->type();

        if((symboltype == Symbol::Address) || (symboltype == Symbol::String))
        {
            this->_variables.remove(symbol->startAddress());

            if(symboltype == Symbol::String)
                this->_strings.remove(symbol->startAddress());
        }
        else if(symboltype == Symbol::Function)
            this->_functions.remove(symbol->startAddress());
        else if(symboltype == Symbol::Label)
            this->_labels.remove(symbol->startAddress());
    }

    void SymbolTable::pushSymbol(Symbol *symbol)
    {
        Symbol::Type symboltype = symbol->type();

        if((symboltype == Symbol::Address) || (symboltype == Symbol::String))
        {
            this->_variables[symbol->startAddress()] = symbol;

            if(symboltype == Symbol::String)
                this->_strings[symbol->startAddress()] = symbol;
        }
        else if(symboltype == Symbol::Function)
            this->_functions[symbol->startAddress()] = symbol;
        else if(symboltype == Symbol::Label)
            this->_labels[symbol->startAddress()] = symbol;
    }
}
