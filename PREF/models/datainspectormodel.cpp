#include "datainspectormodel.h"
#include "../platform/loadeddata.h"

using namespace PrefLib;

DataInspectorModel::DataInspectorModel(QHexEdit *hexedit, QObject *parent) : BasicListModel(parent), _hexedit(hexedit)
{
    connect(this->_hexedit, SIGNAL(positionChanged(qint64)), this, SLOT(inspect(qint64)));
    this->inspect(hexedit->cursorPos());
}

int DataInspectorModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant DataInspectorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if((orientation != Qt::Horizontal) || (role != Qt::DisplayRole))
        return QVariant();

    if(section == 0)
        return tr("Type");
    else if(section == 1)
        return tr("Value");

    return QVariant();
}

QVariant DataInspectorModel::data(const QModelIndex &index, int role) const
{
    if(!this->_data)
        return BasicListModel::data(index, role);

    const VMValuePtr& vmvalue = this->_data->m_value[index.row()];

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
            return s_qs(vmvalue->value_typeid);
        else if(index.column() == 1)
            return s_qs(vmvalue->printable(16));
    }
    else if((role == Qt::ForegroundRole) && (index.column() == 1))
        return this->highlight(vmvalue);

    return BasicListModel::data(index, role);
}

int DataInspectorModel::rowCount(const QModelIndex &) const
{
    if(!this->_data)
        return 0;

    return this->_data->m_value.size();
}

void DataInspectorModel::inspect(qint64 offset)
{
    LoadedData loadeddata(this->_hexedit->data());
    loadeddata.seek(offset); // NOTE: Fix Sign

    this->beginResetModel();
    this->_data = PrefContext::instance()->inspect(&loadeddata);
    this->endResetModel();
}
