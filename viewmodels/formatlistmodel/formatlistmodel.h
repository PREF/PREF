#ifndef FORMATLISTMODEL_H
#define FORMATLISTMODEL_H

#include <QtCore>
#include <QtGui>
#include "prefsdk/categorymanager.h"
#include "prefsdk/format/formatlist.h"

using namespace PrefSDK;

class FormatListModel: public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit FormatListModel(QObject *parent = 0);
        void changeCategory(const CategoryManager::CategoryPtr &c);
        CategoryManager::CategoryPtr selectedCategory() const;

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int, Qt::Orientation, int) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        CategoryManager::CategoryPtr _category;
        QImage _icoformat;
};

#endif // FORMATLISTMODEL_H
