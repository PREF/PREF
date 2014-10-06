#ifndef CROSSREFERENCEMODEL_H
#define CROSSREFERENCEMODEL_H

#include "prefsdk/disassembler/disassemblerlisting.h"
#include <QGuiApplication>
#include <QAbstractItemModel>

using namespace PrefSDK;

class CrossReferenceModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit CrossReferenceModel(ReferenceSet *referenceset, const QList<Reference*> references, DisassemblerListing* listing, QObject *parent = 0);
        explicit CrossReferenceModel(Block* block, DisassemblerListing* listing, QObject *parent = 0);
        void addCrossReference(quint64 va);
    
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
        Block* _block;
        DisassemblerListing* _listing;
        QList<Reference*> _references;
};

#endif // CROSSREFERENCEMODEL_H
