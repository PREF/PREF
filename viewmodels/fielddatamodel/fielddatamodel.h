#ifndef FIELDDATAMODEL_H
#define FIELDDATAMODEL_H

#include <QtCore>
#include <prefcontext.h>

using namespace PrefLib;

class FieldDataModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        explicit FieldDataModel(QObject *parent = 0);

    protected:
        static bool validateValue(QVariant value, DataType::Type datatype, int base, Endianness::Type endian, QByteArray& ba);
};

#endif // FIELDDATAMODEL_H
