#include "basicmodel.h"
#include <QGuiApplication>

QFont BasicModel::_monospacefont;

BasicModel::BasicModel()
{
    if(BasicModel::_monospacefont.family().isEmpty())
        BasicModel::_monospacefont = QFont("Monospace", qApp->font().pointSize());
}

QVariant BasicModel::defaultData(int role) const
{
    if(role == Qt::FontRole)
        return BasicListModel::_monospacefont;

    return QVariant();
}

QColor BasicModel::highlight(const VMValuePtr &vmvalue) const
{
    if(vmvalue->is_integer())
        return QColor(Qt::darkBlue);
    else if(vmvalue->is_floating_point())
        return QColor(Qt::darkRed);
    else if(vmvalue->is_string())
        return QColor(Qt::darkGreen);

    return QColor(Qt::black);
}

BasicListModel::BasicListModel(QObject *parent): QAbstractListModel(parent), BasicModel()
{
}

QVariant BasicListModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index);
    return this->defaultData(role);
}

BasicItemModel::BasicItemModel(QObject *parent): QAbstractItemModel(parent), BasicModel()
{
}

QVariant BasicItemModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index);
    return this->defaultData(role);
}
