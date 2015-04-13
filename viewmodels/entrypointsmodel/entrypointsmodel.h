#ifndef ENTRYPOINTSMODEL_H
#define ENTRYPOINTSMODEL_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include <disassembler/disassemblerlisting.h>

using namespace PrefLib;
using namespace PrefLib::Disassembler;

class EntryPointsModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit EntryPointsModel(DisassemblerListing* listing, QObject *parent = 0);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        //const DisassemblerListing::EntryPointList& _entrypoints;
        DisassemblerListing* _listing;
        QFont _monospacefont;
};

#endif // ENTRYPOINTSMODEL_H
