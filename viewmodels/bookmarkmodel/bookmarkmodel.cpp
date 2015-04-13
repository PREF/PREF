#include "bookmarkmodel.h"

BookmarkModel::BookmarkModel(QObject *parent): QAbstractItemModel(parent) //FIXME:, _bookmarklist(bookmarklist)
{
    /* FIXME:
    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter); */
}

int BookmarkModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant BookmarkModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Address";

            case 1:
                return "Description";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant BookmarkModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    /* FIXME:
    if(role == Qt::DisplayRole)
    {
        const DisassemblerListing::BookmarkEntry& be = this->_bookmarklist[index.row()];

        switch(index.column())
        {
            case 0:
                return be.first->startAddress().toString(16);

            case 1:
                return QString("\"%1\"").arg(be.second).simplified();
        }
    }
    else if(role == Qt::EditRole && (index.column() == 1))
    {
        const DisassemblerListing::BookmarkEntry& be = this->_bookmarklist[index.row()];
        return be.second.simplified();
    }
    else if(role == Qt::ForegroundRole)
    {
        if(index.column() == 0)
            return QColor(Qt::darkBlue);

        if(index.column() == 1)
            return QColor(Qt::darkGreen);
    }
    else if(role == Qt::FontRole)
        return this->_monospacefont;
        */

    return QVariant();
}

bool BookmarkModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    /* FIXME:
    if((role == Qt::EditRole) && index.isValid() && (index.column() == 1))
    {
        DisassemblerListing::BookmarkEntry& be = this->_bookmarklist[index.row()];
        be.second = value.toString();
        return true;
    }
    */

    return QAbstractItemModel::setData(index, value, role);
}

QModelIndex BookmarkModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex BookmarkModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int BookmarkModel::rowCount(const QModelIndex &) const
{
    /* FIXME:
    return this->_bookmarklist.count(); */
}

Qt::ItemFlags BookmarkModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if(index.column() == 1)
        flags |= Qt::ItemIsEditable;

    return flags;
}
