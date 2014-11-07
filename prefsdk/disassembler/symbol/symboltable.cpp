#include "symboltable.h"

namespace PrefSDK
{
    SymbolTable::SymbolTable(QObject *parent): QObject(parent)
    {
    }

    lua_Integer SymbolTable::count() const
    {
        return this->_symboltable.count();
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

    void SymbolTable::set(Symbol::Type symboltype, const DataValue &address, const QString &name)
    {
        this->set(symboltype, address, DataValue(), address.dataType(), name);
    }

    void SymbolTable::set(Symbol::Type symboltype, const DataValue &address, const DataValue &calleraddress, const QString &name)
    {
        this->set(symboltype, address, calleraddress, address.dataType(), name);
    }

    void SymbolTable::set(Symbol::Type symboltype, const DataValue &address, const DataValue &calleraddress, DataType::Type datatype, const QString &name)
    {
        DataValue symbolsize = DataValue::create(DataType::sizeOf(datatype), address.dataType());
        this->set(symboltype, address, symbolsize, calleraddress, datatype, name);
    }

    void SymbolTable::set(Symbol::Type symboltype, const DataValue &address, const DataValue& symbolsize, const DataValue &calleraddress, DataType::Type datatype, const QString &name)
    {
        Symbol* symbol = nullptr;

        if(this->_symboltable.contains(address))
        {   
            symbol = this->_symboltable[address];
            symbol->addSource(calleraddress);

            if(symbol->type() > symboltype) /* Check SymbolType's weight */
                return;

            this->popSymbol(symbol);

            symbol->setType(symboltype);
            symbol->setSize(symbolsize);
            symbol->setDataType(datatype);
            symbol->setName(name);
            return;
        }
        else
        {
            symbol = new Symbol(symboltype, address, symbolsize, datatype, name, this);
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
