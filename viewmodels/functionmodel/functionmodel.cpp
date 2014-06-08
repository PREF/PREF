#include "functionmodel.h"

FunctionModel::FunctionModel(DisassemblerListing* listing, QObject *parent): QAbstractItemModel(parent), _listing(listing)
{
    /* Thread Safety: Don't use QPixmap */
    this->_functionico.load(":/format_icons/res/method.png");

    for(int i = 0; i < this->_listing->segmentsCount(); i++)
    {
        Segment* segment = this->_listing->segment(i);

        for(int j = 0; j < segment->functionsCount(); j++)
            this->_functions.append(segment->function(j));
    }

    std::sort(this->_functions.begin(), this->_functions.end(), &FunctionModel::sortByAddress);
}

bool FunctionModel::sortByAddress(Function *f1, Function *f2)
{
    return f1->startAddress() < f2->startAddress();
}

QString FunctionModel::functionType(Function *f) const
{
    switch(f->type())
    {
        case FunctionTypes::EntryPoint:
            return "EP";

        case FunctionTypes::Import:
            return "I";

        case FunctionTypes::Export:
            return "E";

        default:
            break;
    }

    return "F";
}

int FunctionModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant FunctionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Name";

            case 1:
                return "Address";

            case 2:
                return "Segment";

            case 3:
                return "Type";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant FunctionModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    Function* f = this->_functions[index.row()];

    if(role == Qt::DisplayRole)
    {   
        switch(index.column())
        {
            case 0:
                return f->name();

            case 1:
                return QString("%1").arg(f->startAddress(), 8, 16, QLatin1Char('0')).toUpper();

            case 2:
                return f->segmentName();

            case 3:
                return this->functionType(f);

            default:
                break;
        }
    }
    else if(role == Qt::DecorationRole && index.column() == 0)
        return this->_functionico;
    else if(role == Qt::BackgroundRole)
    {
        switch(f->type())
        {
            case FunctionTypes::EntryPoint:
                return QColor(Qt::yellow);

            case FunctionTypes::Import:
                return QColor(Qt::darkGray);

            case FunctionTypes::Export:
                return QColor(Qt::darkYellow);

            default:
                break;
        }
    }

    return QVariant();
}

QModelIndex FunctionModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex FunctionModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int FunctionModel::rowCount(const QModelIndex &) const
{
    return this->_functions.count();
}

Qt::ItemFlags FunctionModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
