#ifndef LOADERLISTMODEL_H
#define LOADERLISTMODEL_H

#include <QtCore>
#include "prefsdk/sdkmanager.h"
#include "prefsdk/disassembler/loader/loaderlist.h"

using namespace PrefSDK;

class LoaderListModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit LoaderListModel(QHexEditData* hexeditdata, QObject *parent = 0);
        ProcessorLoader *loader(int idx) const;

    private:
        void validateLoaders(QHexEditData* hexeditdata);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation, int) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        QList<ProcessorLoader*> _loaders;
        QImage _icoloader;
};

#endif // LOADERLISTMODEL_H
