#ifndef DISASSEMBLERLISTMODEL_H
#define DISASSEMBLERLISTMODEL_H

#include <QtCore>
#include <qhexedit/qhexeditdata.h>
#include <disassembler/disassemblercontext.h>
#include "prefsdk/sdkmanager.h"

using namespace PrefLib::Disassembler;

class DisassemblerListModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit DisassemblerListModel(QHexEditData* hexeditdata, QObject *parent = 0);
        DisassemblerDefinition *disassembler(int idx) const;

    private:
        void validateDisassemblers(QHexEditData* hexeditdata);

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation, int) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    private:
        QList<DisassemblerDefinition*> _disassemblers;
        QImage _icodisassembler;
};

#endif // DISASSEMBLERLISTMODEL_H
