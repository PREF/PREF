#ifndef PREFSDK_TABLEMODEL_H
#define PREFSDK_TABLEMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "viewmodels/fielddatamodel/fielddatamodel.h"
#include "prefsdk/format/elements/formatelement.h"
#include "prefsdk/format/elements/field.h"
#include "prefsdk/format/elements/fieldarray.h"
#include "prefsdk/format/elements/field.h"

namespace PrefSDK
{
    class TableModel: public FieldDataModel
    {
        Q_OBJECT

        private:
            enum ItemType { Empty, Element, String };
            struct TableItem { TableModel::ItemType Type; FormatElement* Element; QString String; };
            typedef QList<TableItem> ElementList;

        public:
            explicit TableModel(int rows, int columns, QObject *parent = 0);
            void setHeaderItem(int column, const QString& s);
            void setItem(int row, int column, FormatElement* element);
            void setItem(int row, int column, const QString& s);

        public: /* Overriden Methods */
            virtual int columnCount(const QModelIndex& = QModelIndex()) const;
            virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
            virtual QVariant data(const QModelIndex &index, int role) const;
            virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
            virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
            virtual QModelIndex parent(const QModelIndex &) const;
            virtual int rowCount(const QModelIndex& = QModelIndex()) const;
            virtual Qt::ItemFlags flags(const QModelIndex &index) const;

        private:
            QFont _monospacefont;
            QList<QString> _headeritems;
            QList<ElementList> _items;
    };
}

#endif // PREFSDK_TABLEMODEL_H
