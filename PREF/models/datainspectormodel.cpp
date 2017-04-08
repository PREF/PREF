#include "datainspectormodel.h"
#include "../platform/loadeddata.h"
#include "../platform/btvmex.h"
#include <support/datainspector.h>

DataInspectorModel::DataInspectorModel(QHexEdit *hexedit, QObject *parent) : BasicListModel(parent), _hexedit(hexedit)
{
    connect(this->_hexedit->document()->cursor(), &QHexCursor::positionChanged, this, &DataInspectorModel::inspect);
    this->inspect();
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

void DataInspectorModel::inspect()
{
    QHexDocument* document = this->_hexedit->document();
    LoadedData loadeddata(document);
    loadeddata.seek(document->cursor()->offset());

    this->beginResetModel();

    BTVMEX btvm(this->_hexedit->document(), &loadeddata);
    btvm.evaluate(DATA_INSPECTOR_CODE);
    BTEntryList btentries = btvm.createTemplate();

    if(!btentries.empty())
        this->_data = btentries.front()->value;
    else
        this->_data = NULL;

    this->endResetModel();
}
