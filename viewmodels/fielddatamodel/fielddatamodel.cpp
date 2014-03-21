#include "fielddatamodel.h"

FieldDataModel::FieldDataModel(QObject *parent): QAbstractItemModel(parent)
{
}

bool FieldDataModel::validateValue(QVariant value, lua_Integer datatype, int base, QSysInfo::Endian endian, QByteArray &ba)
{
    DataType::Type type = static_cast<DataType::Type>(datatype);
    QString stringvalue = value.toString();

    if(type == DataType::Char || type == DataType::Array) /* (Single Char) OR (Char Array (aka String)) */
    {
        ba = stringvalue.toLatin1();
        return true;
    }
    else if(DataType::isInteger(type))
    {
        bool isok = false;
        QDataStream ds(&ba, QIODevice::WriteOnly);

        if(endian == QSysInfo::LittleEndian)
            ds.setByteOrder(QDataStream::LittleEndian);
        else // if(endian == QSysInfo::BigEndian)
            ds.setByteOrder(QDataStream::BigEndian);

        switch(type)
        {
            case DataType::UInt8:
                ds << (quint8)stringvalue.toUInt(&isok, base);
                break;

            case DataType::Int8:
                ds << (qint8)stringvalue.toInt(&isok, base);
                break;

            case DataType::UInt16:
                ds << (quint16)stringvalue.toUShort(&isok, base);
                break;

            case DataType::Int16:
                ds << (qint16)stringvalue.toShort(&isok, base);
                break;

            case DataType::UInt32:
                ds << (quint32)stringvalue.toUInt(&isok, base);
                break;

            case DataType::Int32:
                ds << (qint32)stringvalue.toInt(&isok, base);
                break;

            case DataType::UInt64:
                ds << (quint64)stringvalue.toULongLong(&isok, base);
                break;

            case DataType::Int64:
                ds << (qint64)stringvalue.toLongLong(&isok, base);
                break;

            default:
                break;
        }

        return isok;
    }

    return false;
}
