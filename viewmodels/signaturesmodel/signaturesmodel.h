#ifndef SIGNATURESMODEL_H
#define SIGNATURESMODEL_H

#include <QtCore>
#include <QtGui>

class SignaturesModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        typedef QPair<qint64, QString> SignatureData;
        typedef QList<qint64> OffsetList;
        typedef QHash<qint64, SignatureData> SignatureMap;

    public:
        explicit SignaturesModel(QObject *parent = 0);
        void setData(const SignaturesModel::OffsetList& offsetlist, const SignaturesModel::SignatureMap& signatures);
        qint64 offset(int i) const;
        QString signatureName(int i) const;

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        OffsetList _offsetlist;
        SignatureMap _signatures;
};

#endif // SIGNATURESMODEL_H
