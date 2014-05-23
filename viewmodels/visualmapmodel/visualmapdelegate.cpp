#include "visualmapdelegate.h"

VisualMapDelegate::VisualMapDelegate(QHexEditData *hexeditdata, QObject *parent): QStyledItemDelegate(parent), _hexeditdata(hexeditdata)
{

}

QWidget* VisualMapDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex &index) const
{
    if(!index.row())
    {
        QComboBox* cbx = new QComboBox(parent);
        cbx->addItem("Dot Plot", BinaryMap::DotPlot);
        cbx->addItem("Bytes as Pixel", BinaryMap::BytesAsPixel);
        return cbx;
    }

    QNumberSpinBox* nsb = new QNumberSpinBox(parent);
    nsb->setMinimum(0);

    if(index.row() == (index.model()->rowCount() - 1))
        nsb->setMaximum(4096);
    else
        nsb->setMaximum(this->_hexeditdata->length());

    nsb->setBase(16);
    return nsb;
}

void VisualMapDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(!index.row())
    {
        int idx = index.model()->data(index, Qt::EditRole).toInt();
        QComboBox* cbx = qobject_cast<QComboBox*>(editor);
        cbx->setCurrentIndex(idx);
        return;
    }

    qint64 val = index.model()->data(index, Qt::EditRole).toLongLong();
    QNumberSpinBox* nsb = qobject_cast<QNumberSpinBox*>(editor);
    nsb->setValue(val);
}

void VisualMapDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(!index.row())
    {
        QComboBox* cbx = qobject_cast<QComboBox*>(editor);
        model->setData(index, cbx->currentData());
        return;
    }

    QNumberSpinBox* nsb = qobject_cast<QNumberSpinBox*>(editor);
    model->setData(index, nsb->value());
}

void VisualMapDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}
