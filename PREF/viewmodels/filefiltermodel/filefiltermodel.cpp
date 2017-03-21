#include "filefiltermodel.h"

FileFilterModel::FileFilterModel(QObject *parent): QAbstractItemModel(parent)
{
    QImage img;
    img.load(":/misc_icons/res/file.png");

    this->_icofile = img.scaled(16, 16, Qt::KeepAspectRatio);
}

QFileInfo FileFilterModel::file(int i)
{
    return this->_files[i];
}

QFileInfo FileFilterModel::smallestFile()
{
    return this->_files.last();
}

int FileFilterModel::smallestFileIndex()
{
    return this->_files.length() - 1;
}

void FileFilterModel::setDirectory(QString s)
{
    this->_dir.setPath(s);
    this->updateFilter(this->_filter);
}

void FileFilterModel::updateFilter(QString s)
{
    this->_filter = (!s.isEmpty() ? s : "*.*");

    this->beginResetModel();
    this->_files = this->_dir.entryInfoList(QStringList() << this->_filter, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Size);
    this->endResetModel();
}

int FileFilterModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant FileFilterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Name";

            case 1:
                return "Size";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant FileFilterModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !this->_dir.isReadable())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        QFileInfo fi = this->_files[index.row()];

        switch(index.column())
        {
            case 0:
                return fi.fileName();

            case 1:
                return QString("%1KB").arg(static_cast<qreal>(fi.size()) / 1024.0);

            default:
                break;
        }
    }
    else if(role == Qt::DecorationRole && index.column() == 0)
        return this->_icofile;

    return QVariant();
}

QModelIndex FileFilterModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex FileFilterModel::parent(const QModelIndex &) const
{
    return QModelIndex(); /* It's a List: No Parents! */
}

int FileFilterModel::rowCount(const QModelIndex &) const
{
    return this->_files.length();
}

Qt::ItemFlags FileFilterModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
