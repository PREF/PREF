#include "stringsymbolmodel.h"

const qint64 StringSymbolModel::STRING_MAX_LENGTH = 50;

StringSymbolModel::StringSymbolModel(DisassemblerListing* listing, QHexEditData *hexeditdata, QObject *parent): QAbstractItemModel(parent), _listing(listing), _hexeditdata(hexeditdata)
{
    this->_strings = listing->strings().toList();
    this->_reader = new QHexEditDataReader(hexeditdata, this);

    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);

    std::sort(this->_strings.begin(), this->_strings.end());
}

const DataValue &StringSymbolModel::string(qint64 idx)
{
    return this->_strings[idx];
}

int StringSymbolModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant StringSymbolModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Address";

            case 1:
                return "String";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant StringSymbolModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        DataValue address = this->_strings[index.row()];

        switch(index.column())
        {
            case 0:
                return address.toString(16).append("h");

            case 1:
            {
                Segment* segment = this->_listing->findSegment(address);
                DataValue offset = (address - segment->startAddress()) + segment->baseOffset();
                return QString("'%1'").arg(this->_reader->readString(offset.compatibleValue<qint64>()).replace(QRegExp("[\\n\\r]"), " "));
            }

            default:
                break;
        }
    }
    else if(role == Qt::ForegroundRole)
    {
        if(index.column() == 0)
            return QColor(Qt::darkBlue);
        else
            return QColor(Qt::darkGreen);
    }
    else if(role == Qt::FontRole)
        return this->_monospacefont;

    return QVariant();
}

QModelIndex StringSymbolModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex StringSymbolModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int StringSymbolModel::rowCount(const QModelIndex &) const
{
    return this->_strings.length();
}

Qt::ItemFlags StringSymbolModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
