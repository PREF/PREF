#ifndef CROSSREFERENCEMODEL_H
#define CROSSREFERENCEMODEL_H

#include <QGuiApplication>
#include <QAbstractItemModel>
#include <QFont>
#include <QColor>
#include <disassembler/blocks/block.h>

using namespace PrefLib::Disassembler;

class CrossReferenceModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit CrossReferenceModel(Block* block, QObject *parent = 0);
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
        //const QList<uint64_t>& _sources;
        QFont _monospacefont;
        uint64_t _address;
};

#endif // CROSSREFERENCEMODEL_H
