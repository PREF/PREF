#ifndef DATATYPESMODEL_H
#define DATATYPESMODEL_H

#include <QtCore>
#include <QtGui>
#include <types/datatype.h>
#include <types/datavalue.h>
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include "qhexedit/qhexeditdatawriter.h"

using namespace PrefLib;

class DataTypesModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit DataTypesModel(QObject *parent = 0);
        Endianness::Type endian();
        int base();

    public slots:
        void setOffset(qint64 pos);
        void setData(QHexEditData *hexeditdata);
        void setEndian(int endian);
        void setBase(int base);

    private:
        DataValue readValue(int row) const;
        void updateData();

    private:
        static QHash<Endianness::Type, QHash<DataType::Type, DataType::Type> > _botypes;
        static QList<DataType::Type> _types;
        static QList<QString> _typenames;

    private:
        static const int STRING_LENGTH;
        QFont _monospacefont;
        QHexEditData* _hexeditdata;
        QHexEditDataReader* _reader;
        Endianness::Type _endian;
        qint64 _offset;
        int _base;

    public: /* Overriden Methods */
        virtual int columnCount(const QModelIndex& = QModelIndex()) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex&) const;
        virtual int rowCount(const QModelIndex& = QModelIndex()) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // DATATYPESMODEL_H
