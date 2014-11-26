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

        switch(DataType::bitWidth(datatype))
        {
            case 8:
            {
                if(DataType::isSigned(datatype))
                    ds << static_cast<qint8>(stringvalue.toInt(&isok, base));
                else
                    ds << static_cast<quint8>(stringvalue.toUInt(&isok, base));

                break;
            }

            case 16:
            {
                if(DataType::isSigned(datatype))
                    ds << static_cast<qint16>(stringvalue.toShort(&isok, base));
                else
                    ds << static_cast<quint16>(stringvalue.toUShort(&isok, base));

                break;
            }

            case 32:
            {
                if(DataType::isSigned(datatype))
                    ds << static_cast<qint32>(stringvalue.toInt(&isok, base));
                else
                    ds << static_cast<quint32>(stringvalue.toUInt(&isok, base));

                break;
            }

            case 64:
            {
                if(DataType::isSigned(datatype))
                    ds << static_cast<qint64>(stringvalue.toLongLong(&isok, base));
                else
                    ds << static_cast<quint64>(stringvalue.toULongLong(&isok, base));

                break;
            }

            default:
                break;
        }

        return isok;
    }

    return false;
}
