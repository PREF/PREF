#ifndef VARIABLESMODEL_H
#define VARIABLESMODEL_H

#include <QAbstractItemModel>
#include <QGuiApplication>
#include "prefsdk/disassembler/disassemblerlisting.h"

using namespace PrefSDK;

class VariablesModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit VariablesModel(DisassemblerListing* listing, QObject *parent = 0);
        const DataValue& variable(qint64 idx) const;
        DisassemblerListing* listing() const;

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        DisassemblerListing* _listing;
        QFont _monospacefont;
        QList<DataValue> _variables;
};

#endif // VARIABLESMODEL_H
