#include "exportermodel.h"

ExporterModel::ExporterModel(QObject *parent): QAbstractItemModel(parent)
{
    QImage img;
    img.load(":/action_icons/res/export.png");

    this->_icoexport = img.scaled(16, 16, Qt::KeepAspectRatio);
}

int ExporterModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant ExporterModel::headerData(int section, Qt::Orientation orientation, int role) const
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

            case 3:
                return "Description";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant ExporterModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        const ExporterDefinition* e = ExporterList::instance()->exporter(index.row());

        switch(index.column())
        {
            case 0:
                return e->name();

            case 1:
                return e->author();

            case 2:
                return e->version();

            case 3:
                return e->description();

            default:
                break;
        }
    }
    else if((role == Qt::DecorationRole) && (index.column() == 0))
        return this->_icoexport;

    return QVariant();
}

QModelIndex ExporterModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex ExporterModel::parent(const QModelIndex &) const
{
    return QModelIndex(); /* It's a List: No Parents! */
}

int ExporterModel::rowCount(const QModelIndex &) const
{
    return ExporterList::instance()->length();
}

Qt::ItemFlags ExporterModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
