#ifndef COMPAREMODEL_H
#define COMPAREMODEL_H

#include <QAbstractItemModel>

class CompareModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        typedef QList<qint64> OffsetList;
        typedef QHash<qint64, qint64> DifferenceMap;

    public:
        explicit CompareModel(const CompareModel::OffsetList& offsetlist, const CompareModel::DifferenceMap& differences, QObject *parent = 0);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        const OffsetList& _offsetlist;
        const DifferenceMap& _differencemap;
};

#endif // COMPAREMODEL_H
