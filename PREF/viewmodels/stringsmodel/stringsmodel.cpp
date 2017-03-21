#include "stringsmodel.h"

StringsModel::StringsModel(QHexEditData* hexeditdata, QObject *parent): QAbstractItemModel(parent), _reader(new QHexEditDataReader(hexeditdata, this))
{
    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);
}

void StringsModel::setData(const ByteElaborator::StringList &strings)
{
    this->beginInsertRows(QModelIndex(), strings.size(), strings.size());
    this->_strings = strings;
    this->endInsertRows();
}

QModelIndex StringsModel::indexOf(const QString &searchstring, StringsModel::SearchDirection direction, const QModelIndex &startindex)
{
    switch(direction)
    {
        case StringsModel::Down:
            return this->searchDown(searchstring, startindex);

        case StringsModel::Up:
            return this->searchUp(searchstring, startindex);

        default:
            break;
    }

    return QModelIndex();
}

const ByteElaborator::StringRange &StringsModel::range(size_t i) const
{
    return this->_strings.at(i);
}

uint64_t StringsModel::offset(size_t i) const
{
    return this->_strings.at(i).Start;
}

QString StringsModel::string(size_t i) const
{
    const ByteElaborator::StringRange& range = this->_strings.at(i);
    return this->_reader->read(range.Start, range.End - range.Start).simplified();
}

QModelIndex StringsModel::searchUp(const QString &searchstring, const QModelIndex &startindex)
{
    for(int64_t i = (startindex.isValid() ? startindex.row() - 1 : this->_strings.size() - 1); i >= 0; i--)
    {
        QString s = this->string(i);

        if(s.length() < searchstring.length())
            continue;

        int idx = s.indexOf(searchstring, 0, Qt::CaseInsensitive);

        if(idx == -1)
            continue;

        return this->createIndex(i, 0);
    }

    return QModelIndex();
}

QModelIndex StringsModel::searchDown(const QString &searchstring, const QModelIndex &startindex)
{
    for(uint64_t i = (startindex.isValid() ? startindex.row() + 1 : 0); i < this->_strings.size(); i++)
    {
        QString s = this->string(i);

        if(s.length() < searchstring.length())
            continue;

        int idx = s.indexOf(searchstring, 0, Qt::CaseInsensitive);

        if(idx == -1)
            continue;

        return this->createIndex(i, 0);
    }

    return QModelIndex();
}

int StringsModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant StringsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Offset";

            case 1:
                return "String";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant StringsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        const ByteElaborator::StringRange& range = this->_strings.at(index.row());

        if(index.column() == 0)
            return QString("%1").arg(range.Start, 8, 16, QLatin1Char('0')).toUpper().append("h");
        else if(index.column() == 1)
            return this->string(index.row());
    }
    else if(role == Qt::ForegroundRole)
    {
        if(index.column() == 0)
            return QColor(Qt::darkBlue);
        else if(index.column() == 1)
            return QColor(Qt::darkGreen);
    }
    else if(role == Qt::FontRole)
        return this->_monospacefont;

    return QVariant();
}

QModelIndex StringsModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex StringsModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int StringsModel::rowCount(const QModelIndex &) const
{
    return this->_strings.size();
}

Qt::ItemFlags StringsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
