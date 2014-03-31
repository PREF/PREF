#include "fielddatamodel.h"

FieldDataModel::FieldDataModel(QObject *parent): QAbstractItemModel(parent)
{
}

bool FieldDataModel::validateValue(QVariant value, DataType::Type datatype, int base, QSysInfo::Endian endian, QByteArray &ba)
{
    QString stringvalue = value.toString();

    if(datatype == DataType::Character || datatype == DataType::isString(datatype)) /* (Single Char) OR (Char Array (aka String)) */
    {
        ba = stringvalue.toLatin1();
        return true;
    }
    else if(DataType::isInteger(datatype))
    {
        bool isok = false;
        QDataStream ds(&ba, QIODevice::WriteOnly);

        if(endian == QSysInfo::LittleEndian)
            ds.setByteOrder(QDataStream::LittleEndian);
        else
            ds.setByteOrder(QDataStream::BigEndian);

        switch(datatype)
        {
            case DataType::UInt8:
                ds << static_cast<qint8>(stringvalue.toUInt(&isok, base));
                break;

            case DataType::UInt16:
                ds << static_cast<qint16>(stringvalue.toUShort(&isok, base));
                break;

            case DataType::UInt32:
                ds << static_cast<qint32>(stringvalue.toUInt(&isok, base));
                break;

            case DataType::UInt64:
                ds << static_cast<quint64>(stringvalue.toULongLong(&isok, base));
                break;

            case DataType::Int8:
                ds << static_cast<qint8>(stringvalue.toInt(&isok, base));
                break;

            case DataType::Int16:
                ds << static_cast<qint16>(stringvalue.toShort(&isok, base));
                break;

            case DataType::Int32:
                ds << static_cast<qint32>(stringvalue.toInt(&isok, base));
                break;

            case DataType::Int64:
                ds << static_cast<qint64>(stringvalue.toLongLong(&isok, base));
                break;

            default:
                break;
        }

        return isok;
    }

    return false;
}
