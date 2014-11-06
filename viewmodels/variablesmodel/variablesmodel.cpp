#include "variablesmodel.h"

VariablesModel::VariablesModel(DisassemblerListing* listing, QObject *parent): QAbstractItemModel(parent), _listing(listing)
{
    this->_variables = listing->symbolTable()->variables();

    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);
}

DisassemblerListing *VariablesModel::listing() const
{
    return this->_listing;
}

int VariablesModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant VariablesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Offset";

            case 1:
                return "Virtual Address";

            case 2:
                return "Size";

            case 3:
                return "Name";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant VariablesModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        Symbol* symbol = reinterpret_cast<Symbol*>(index.internalPointer());
        const DataValue& address = symbol->address();

        switch(index.column())
        {
            case 0:
            {
                Segment* segment = this->_listing->findSegment(address);

                if(!segment)
                    return "???";

                return ((address - segment->startAddress()) +  segment->baseOffset()).toString(16).append("h");
            }

            case 1:
                return address.toString(16).append("h");

            case 2:
                return symbol->size().toString();

            case 3:
                return symbol->name();

            default:
                break;
        }
    }
    else if(role == Qt::ForegroundRole)
    {
        if(index.column() == 0 || index.column() == 1 || index.column() == 2)
            return QColor(Qt::darkBlue);

        Symbol* symbol = reinterpret_cast<Symbol*>(index.internalPointer());

        if(symbol->type() == Symbol::Address)
            return QColor(Qt::darkCyan);
        else
            return QColor(Qt::darkGreen);
    }
    else if(role == Qt::FontRole)
        return this->_monospacefont;

    return QVariant();
}

QModelIndex VariablesModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column, this->_variables[row]);
}

QModelIndex VariablesModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int VariablesModel::rowCount(const QModelIndex &) const
{
    return this->_variables.length();
}

Qt::ItemFlags VariablesModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
