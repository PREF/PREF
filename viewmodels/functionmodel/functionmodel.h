#ifndef FUNCTIONMODEL_H
#define FUNCTIONMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "prefsdk/disassembler/disassemblerlisting.h"

using namespace PrefSDK;

class FunctionModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit FunctionModel(DisassemblerListing* listing, QObject *parent = 0);

    private:
        QString functionType(Function* f) const;

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        QFont _monospacefont;
        DisassemblerListing* _listing;
        QList<Function*> _functions;
};

#endif // FUNCTIONMODEL_H
