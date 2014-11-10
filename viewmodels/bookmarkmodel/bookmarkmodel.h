#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QAbstractItemModel>
#include "prefsdk/disassembler/disassemblerlisting.h"

using namespace PrefSDK;

class BookmarkModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit BookmarkModel(DisassemblerListing::BookmarkList& bookmarklist, QObject *parent = 0);

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
        DisassemblerListing::BookmarkList& _bookmarklist;
        QFont _monospacefont;
};

#endif // BOOKMARKMODEL_H
