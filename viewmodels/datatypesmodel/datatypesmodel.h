#ifndef DATATYPESMODEL_H
#define DATATYPESMODEL_H

#include <QtCore>
#include <QtGui>
#include "viewmodels/fielddatamodel/fielddatamodel.h"
#include "qhexedit/qhexeditdata.h"
#include "prefsdk/format/elements/structure.h"

class DataTypesModel : public FieldDataModel
{
    Q_OBJECT

    public:
        explicit DataTypesModel(QObject *parent = 0);
        QSysInfo::Endian endian();
        int base();

    public slots:
        void setOffset(qint64 pos);
        void setData(ByteBuffer *bytebuffer);
        void setEndian(QSysInfo::Endian endian);
        void setBase(int base);

    private:
        QString readValue(int row, bool *overflow = nullptr) const;
        void updateData();

    private:
        static QVector<DataType::Type> _types;
        static QVector<QString> _typenames;

    private:
        static const int STRING_LENGTH;
        QFont _monospacefont;
        ByteBuffer* _bytebuffer;
        QSysInfo::Endian _endian;
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
