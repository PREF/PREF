#ifndef DATAINSPECTORMODEL_H
#define DATAINSPECTORMODEL_H

#include <qhexedit/qhexedit.h>
#include "basicmodel.h"

class DataInspectorModel : public BasicListModel
{
    Q_OBJECT

    public:
        explicit DataInspectorModel(QHexEdit* hexedit, QObject *parent = 0);
        virtual int columnCount(const QModelIndex &) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual int rowCount(const QModelIndex &) const;

    private slots:
        void inspect(qint64 offset);

    private:
        VMValuePtr _data;
        QHexEdit* _hexedit;
};

#endif // DATAINSPECTORMODEL_H
