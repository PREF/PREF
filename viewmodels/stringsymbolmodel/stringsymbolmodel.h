#ifndef STRINGSYMBOLMODEL_H
#define STRINGSYMBOLMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include "prefsdk/disassembler/disassemblerlisting.h"

using namespace PrefSDK;

class StringSymbolModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit StringSymbolModel(DisassemblerListing* listing, QHexEditData* hexeditdata, QObject *parent = 0);
        const DataValue& string(qint64 idx);

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
        QHexEditData* _hexeditdata;
        QHexEditDataReader* _reader;
        QList<DataValue> _strings;

    private:
        static const qint64 STRING_MAX_LENGTH;
};

#endif // STRINGSYMBOLMODEL_H
