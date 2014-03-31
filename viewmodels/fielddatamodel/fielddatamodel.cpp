#include "fielddatamodel.h"

FieldDataModel::FieldDataModel(QObject *parent): QAbstractItemModel(parent)
{
}

bool FieldDataModel::validateValue(QVariant value, lua_Integer datatype, int base, QSysInfo::Endian endian, QByteArray &ba)
{
    QString stringvalue = value.toString();

    if(datatype == DataType::character() || datatype == DataType::isString(datatype)) /* (Single Char) OR (Char Array (aka String)) */
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

        if(datatype == DataType::uint8())
            ds << static_cast<quint8>(stringvalue.toUInt(&isok, base));
        else if(datatype == DataType::uint16())
            ds << (quint16)stringvalue.toUShort(&isok, base);
        else if(datatype == DataType::uint32())
            ds << (quint32)stringvalue.toUInt(&isok, base);
        else if(datatype == DataType::uint64())
            ds << (quint64)stringvalue.toULongLong(&isok, base);
        else if(datatype == DataType::int8())
            ds << (qint8)stringvalue.toInt(&isok, base);
        else if(datatype == DataType::int16())
            ds << (qint16)stringvalue.toShort(&isok, base);
        else if(datatype == DataType::int32())
            ds << (qint32)stringvalue.toInt(&isok, base);
        else if(datatype == DataType::int64())
            ds << (qint64)stringvalue.toLongLong(&isok, base);

        return isok;
    }

    return false;
}
