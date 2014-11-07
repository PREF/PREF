#include "crossreferencemodel.h"

CrossReferenceModel::CrossReferenceModel(Block *block, QObject *parent): QAbstractItemModel(parent), _sources(block->sources()), _address(block->startAddress())
{
    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);
}

int CrossReferenceModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant CrossReferenceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Direction";

            case 1:
                return "Virtual Address";

            case 2:
                return "Instruction";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant CrossReferenceModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        DataValue sourceaddress = this->_sources[index.row()];

        if(index.column() == 0)
        {
            if(sourceaddress > this->_address)
                return "Down";
            else if(sourceaddress < this->_address)
                return "Up";
            else
                return "---";
        }
        else if(index.column() == 1)
            return sourceaddress.toString(16).append("h");
    }
    else if(role == Qt::FontRole && index.column() > 0)
        return this->_monospacefont;
    else if(role == Qt::ForegroundRole && index.column() == 1)
        return QColor(Qt::darkBlue);

    return QVariant();
}

QModelIndex CrossReferenceModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex CrossReferenceModel::parent(const QModelIndex &) const
{
    return QModelIndex(); /* It's a List: No Parents! */
}

int CrossReferenceModel::rowCount(const QModelIndex&) const
{
    return this->_sources.length();
}

Qt::ItemFlags CrossReferenceModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
