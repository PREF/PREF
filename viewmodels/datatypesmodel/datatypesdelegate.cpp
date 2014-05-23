#include "datatypesdelegate.h"

DataTypesDelegate::DataTypesDelegate(QHexEditData* hexeditdata, QObject *parent): QStyledItemDelegate(parent), _hexeditdata(hexeditdata)
{
}

QWidget *DataTypesDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    if(!index.row() || (index.row() == index.model()->rowCount() - 1))
    {
        QLineEdit* le = new QLineEdit(parent);

        if(!index.row())
            le->setMaxLength(1);

        return le;
    }

    QNumberSpinBox* nsb = new QNumberSpinBox(parent);
    nsb->setBase(index.model()->data(index, Qt::UserRole + 2).toInt());
    nsb->setMinimum(INT_MIN);
    nsb->setMaximum(INT_MAX);
    return nsb;
}

void DataTypesDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(!index.row() || (index.row() == index.model()->rowCount() - 1))
    {
        QLineEdit* le = qobject_cast<QLineEdit*>(editor);
        le->setText(index.model()->data(index, Qt::EditRole).toString());
        return;
    }

    QNumberSpinBox* nsb = qobject_cast<QNumberSpinBox*>(editor);

    if(index.model()->data(index, Qt::UserRole + 1).toBool())
        nsb->setValue(index.model()->data(index, Qt::EditRole).toLongLong());
    else
        nsb->setValue(index.model()->data(index, Qt::EditRole).toULongLong());
}

void DataTypesDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(!index.row() || (index.row() == index.model()->rowCount() - 1))
    {
        QLineEdit* le = qobject_cast<QLineEdit*>(editor);
        model->setData(index, le->text());
        return;
    }

    QNumberSpinBox* nsb = qobject_cast<QNumberSpinBox*>(editor);
    model->setData(index, nsb->value());
}

void DataTypesDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

