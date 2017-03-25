#include "basicmodel.h"
#include <QGuiApplication>

QFont BasicModel::_monospacefont;

BasicModel::BasicModel(QObject *parent)
{
    if(BasicModel::_monospacefont.family().isEmpty())
        BasicModel::_monospacefont = QFont("Monospace", qApp->font().pointSize());
}

QVariant BasicModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index);

    if(role == Qt::FontRole)
        return BasicModel::_monospacefont;

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
