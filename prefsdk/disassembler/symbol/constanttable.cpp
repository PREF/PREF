#include "constanttable.h"

namespace PrefSDK
{
    ConstantTable::ConstantTable(QObject *parent): QObject(parent)
    {
    }

    void ConstantTable::set(Instruction *instruction, const DataValue &value, const QString &name)
    {
        if(!this->_constants.contains(instruction))
            this->_constants[instruction] = ConstantList();

        ConstantList& constlist = this->_constants[instruction];
        constlist[value] = new Constant(name, value, this);
    }

    bool ConstantTable::isConstant(Instruction *instruction, const DataValue &value)
    {
        if(!this->_constants.contains(instruction))
            return false;

        ConstantList& constlist = this->_constants[instruction];
        return constlist.contains(value);
    }

    QString ConstantTable::name(Instruction *instruction, const DataValue &value)
    {
        return this->_constants[instruction][value]->name();
    }
}
