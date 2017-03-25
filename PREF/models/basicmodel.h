#ifndef BASICMODEL_H
#define BASICMODEL_H

#include <QAbstractListModel>
#include <QColor>
#include <QFont>
#include <preflib.h>

#define s_qs(s) QString::fromStdString(s)

class BasicModel: public QAbstractListModel
{
    Q_OBJECT

    public:
        BasicModel(QObject* parent = 0);
        virtual QVariant data(const QModelIndex &index, int role) const;

    protected:
        QColor highlight(const VMValuePtr& vmvalue) const;

    private:
        static QFont _monospacefont;
};

#endif // BASICMODEL_H
