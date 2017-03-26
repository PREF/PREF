#include "templatemodel.h"
#include <preflib.h>

using namespace PrefLib;

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
    this->_template = PrefContext::instance()->executeFormat(this->_loadeddata, qs_s(btfile));
    this->endResetModel();
}

QModelIndex TemplateModel::index(int row, int column, const QModelIndex &parent) const
{
    return this->createIndex(row, column);
}

QModelIndex TemplateModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
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
    return BasicItemModel::data(index, role);
}

int TemplateModel::columnCount(const QModelIndex &) const
{
    return 5;
}

int TemplateModel::rowCount(const QModelIndex &parent) const
{
    return this->_template.size();
}
