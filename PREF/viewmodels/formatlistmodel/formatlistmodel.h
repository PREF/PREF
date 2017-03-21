#ifndef FORMATLISTMODEL_H
#define FORMATLISTMODEL_H

#include <QtCore>
#include <QtGui>
#include <prefcontext.h>

using namespace PrefLib;
using namespace PrefLib::Format;

class FormatListModel: public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit FormatListModel(QObject *parent = 0);
        void setCategory(const char *category);
        const char* category() const;

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int, Qt::Orientation, int) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        const char* _category;
        const CategoryContext* _categoryctx;
        QImage _icoformat;
};

#endif // FORMATLISTMODEL_H
