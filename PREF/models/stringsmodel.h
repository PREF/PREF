#ifndef STRINGSMODEL_H
#define STRINGSMODEL_H

#include "basicmodel.h"
#include <qhexedit/qhexeditdatareader.h>
#include <support/byteelaborator.h>

using namespace PrefLib::Support;

class StringsModel : public BasicModel
{
    Q_OBJECT

    public:
        explicit StringsModel(QHexEditData* hexeditdata, QObject *parent = 0);
        void initialize(const ByteElaborator::StringList& stringlist);
        virtual int columnCount(const QModelIndex &) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual int rowCount(const QModelIndex &) const;

    private:
        QString string(const ByteElaborator::StringRange& sr) const;

    private:
        ByteElaborator::StringList _stringlist;
        QHexEditDataReader* _reader;
};

#endif // STRINGSMODEL_H
