#ifndef EXPORTMODEL_H
#define EXPORTMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/exporter/exportlist.h"

using namespace PrefSDK;

class ExportModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit ExportModel(QObject *parent = 0);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        QImage _icoexport;
};

#endif // EXPORTMODEL_H
