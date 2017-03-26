#ifndef BASICMODEL_H
#define BASICMODEL_H

#include <QAbstractItemModel>
#include <QColor>
#include <QFont>
#include <preflib.h>

#define s_qs(s) QString::fromStdString(s)
#define qs_s(s) s.toStdString()

class BasicModel
{
    public:
        BasicModel();

    protected:
        QColor highlight(const VMValuePtr& vmvalue) const;
        QVariant defaultData(int role) const;

    protected:
        static QFont _monospacefont;
};

class BasicListModel: public QAbstractListModel, public BasicModel
{
    Q_OBJECT

    public:
        BasicListModel(QObject* parent = 0);
        virtual QVariant data(const QModelIndex &index, int role) const;
};

class BasicItemModel: public QAbstractItemModel, public BasicModel
{
    Q_OBJECT

    public:
        BasicItemModel(QObject* parent = 0);
        virtual QVariant data(const QModelIndex &index, int role) const;
};

#endif // BASICMODEL_H
