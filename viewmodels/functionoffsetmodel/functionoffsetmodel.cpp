#include "functionoffsetmodel.h"

FunctionOffsetModel::FunctionOffsetModel(QObject *parent): QAbstractItemModel(parent)
{
    /* Thread Safety: Don't use QPixmap */
    this->_functionico.load(":/format_icons/res/method.png");
}

int FunctionOffsetModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant FunctionOffsetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Function";

            case 1:
                return "Offset";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant FunctionOffsetModel::data(const QModelIndex &index, int role) const
{
    //if(!this->_disasmlisting || !this->_disasmlisting->itemCount() || !index.isValid())
        //return QVariant();

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            //quint64 va = static_cast<quint64>(this->_disasmlisting->function(index.row())->virtualAddress());
            //return QString("0x").append(QString("%1").arg(va, 8, 16, QLatin1Char('0')).toUpper());
        }
        else if(index.column() == 1)
        {
            //DisassembledInstruction::Ptr di = this->_disasmlisting->function(index.row());
            //QString funcname = this->_disasmlisting->functionName(di->virtualAddress());
            //return QString("%1()").arg(funcname);
        }
    }
    else if(role == Qt::DecorationRole && index.column() == 0)
        return this->_functionico;
    else if(role == Qt::BackgroundRole)
    {
        //DisassembledInstruction::Ptr di = this->_disasmlisting->function(index.row());

        //if(di->virtualAddress() == this->_disasmlisting->entryPointVA())
            //return Qt::yellow;
    }

    return QVariant();
}

QModelIndex FunctionOffsetModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex FunctionOffsetModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int FunctionOffsetModel::rowCount(const QModelIndex &) const
{
    //if(this->_disasmlisting)
        //return this->_disasmlisting->functionsCount();

    return 0;
}

Qt::ItemFlags FunctionOffsetModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
