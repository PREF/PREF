#ifndef OPTIONMODEL_H
#define OPTIONMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/format/formatlist.h"

using namespace PrefSDK;

class OptionModel: public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit OptionModel(const FormatList::Format& format, QObject *parent = 0);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        FormatList::Format _format;
        QImage _icooptions;
};

#endif // OPTIONMODEL_H
