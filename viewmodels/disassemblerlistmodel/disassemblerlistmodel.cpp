#include "disassemblerlistmodel.h"

DisassemblerListModel::DisassemblerListModel(QHexEditData *hexeditdata, QObject *parent): QAbstractItemModel(parent)
{
    this->validateDisassemblers(hexeditdata);
    this->_icodisassembler.load(":/action_icons/res/cpu.png");
    this->_icodisassembler = this->_icodisassembler.scaled(22, 22);
}

DisassemblerDefinition* DisassemblerListModel::disassembler(int idx) const
{
    return this->_disassemblers[idx];
}

void DisassemblerListModel::validateDisassemblers(QHexEditData *hexeditdata)
{
    /* FIXME:
    DisassemblerList* disassemblerlist = DisassemblerList::instance();

    for(int i = 0; i < disassemblerlist->length(); i++)
    {
        DisassemblerDefinition* dd = disassemblerlist->disassembler(i);

        if(dd->validate(hexeditdata))
            this->_disassemblers.append(dd);
    }
    */
}

int DisassemblerListModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant DisassemblerListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Name";

            case 1:
                return "Author";

            case 2:
                return "Version";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant DisassemblerListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        DisassemblerDefinition* dd = this->_disassemblers[index.row()];

        switch(index.column())
        {
            case 0:
                return dd->name();

            case 1:
                return dd->author();

            case 2:
                return dd->version();

            default:
                break;
        }
    }
    else if(role == Qt::DecorationRole && index.column() == 0)
        return this->_icodisassembler;

    return QVariant();
}

QModelIndex DisassemblerListModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex DisassemblerListModel::parent(const QModelIndex &) const
{
    return QModelIndex(); /* It's a List: No Parents! */
}

int DisassemblerListModel::rowCount(const QModelIndex &) const
{
    return this->_disassemblers.length();
}

Qt::ItemFlags DisassemblerListModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
