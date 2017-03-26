#include "templatemodel.h"
#include "../platform/btvmex.h"

TemplateModel::TemplateModel(QHexEdit *hexedit, QObject *parent) : BasicItemModel(parent), _hexedit(hexedit)
{
    this->_loadeddata = new LoadedData(hexedit->data());
}

TemplateModel::~TemplateModel()
{
    delete this->_loadeddata;
    this->_loadeddata = NULL;
}

void TemplateModel::execute(const QString &btfile)
{
    this->beginResetModel();

    BTVMEX btvm(this->_hexedit, this->_loadeddata);
    btvm.execute(qs_s(btfile));
    this->_template = btvm.createTemplate();

    this->endResetModel();
}

QModelIndex TemplateModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!parent.isValid())
        return this->createIndex(row, column, this->_template[row].get());

    BTEntry* parententry = reinterpret_cast<BTEntry*>(parent.internalPointer());
    return this->createIndex(row, column, parententry->children[row].get());
}

QModelIndex TemplateModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return QModelIndex();

    BTEntry* childentry = reinterpret_cast<BTEntry*>(child.internalPointer());

    if(!childentry->parent)
        return QModelIndex();

    int idx = this->childIndex(childentry, childentry->parent->children);

    if(idx == -1)
        return QModelIndex();

    return this->createIndex(idx, 0, childentry->parent.get());
}

QVariant TemplateModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if((orientation != Qt::Horizontal) || (role != Qt::DisplayRole))
        return QVariant();

    if(section == 0)
        return tr("Name");
    else if(section == 1)
        return tr("Value");
    else if(section == 2)
        return tr("Start");
    else if(section == 3)
        return tr("Size");
    else if(section == 4)
        return tr("Comment");

    return QVariant();
}

QVariant TemplateModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return BasicItemModel::data(index, role);

    BTEntry* btentry = reinterpret_cast<BTEntry*>(index.internalPointer());

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
            return s_qs(btentry->name);
        else if(index.column() == 1)
            return s_qs(btentry->value->printable(16));
        else if(index.column() == 2)
            return QString::number(btentry->location.offset, 16).toUpper() + "h";
        else if(index.column() == 3)
            return QString::number(btentry->location.size, 16).toUpper() + "h";
        else if(index.column() == 4)
            return s_qs(btentry->value->value_comment);
    }
    else if((role == Qt::DecorationRole) && (index.column() == 0))
    {
        if(btentry->value->is_struct() || btentry->value->is_union())
            return QIcon(":/res/struct.png");

        return QIcon(":/res/field.png");
    }
    else if(role == Qt::ForegroundRole)
    {
        if(index.column() == 1)
            return this->highlight(btentry->value);
        else if((index.column() == 2) || (index.column() == 3))
            return QColor(Qt::darkBlue);
        else if(index.column() == 4)
            return QColor(Qt::darkGreen);
    }
    else if((role == Qt::FontRole) && (index.column() == 0))
        return QVariant(); // Use default font for column 0

    return BasicItemModel::data(index, role);
}

int TemplateModel::columnCount(const QModelIndex &) const
{
    return 5;
}

int TemplateModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return this->_template.size();

    BTEntry* parententry = reinterpret_cast<BTEntry*>(parent.internalPointer());
    return parententry->children.size();
}

int TemplateModel::childIndex(BTEntry *btentry, const BTEntryList &entries) const
{
    for(auto it = entries.begin(); it != entries.end(); it++)
    {
        if(btentry == (*it).get())
            return std::distance(entries.begin(), it);
    }

    return -1;
}
