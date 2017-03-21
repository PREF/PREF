#ifndef SIGNATUREDATABASEMODEL_H
#define SIGNATUREDATABASEMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/signatures/signaturedatabase.h"

using namespace PrefSDK;

class SignatureDatabaseModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit SignatureDatabaseModel(QObject *parent = 0);
        bool isPatternMarked();
        qint64 markedPatternId();
        void updateData();
        void markPattern(qint64 idx);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        qint64 _markedidx;
        QImage _icosignature;
};

#endif // SIGNATUREDATABASEMODEL_H
