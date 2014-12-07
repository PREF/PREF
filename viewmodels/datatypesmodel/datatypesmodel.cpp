#include "datatypesmodel.h"

const int DataTypesModel::STRING_LENGTH = 15;
QHash<QSysInfo::Endian, QHash<DataType::Type, DataType::Type> > DataTypesModel::_botypes;
QList<DataType::Type> DataTypesModel::_types;
QList<QString> DataTypesModel::_typenames;

DataTypesModel::DataTypesModel(QObject *parent): QAbstractItemModel(parent)
{
    this->_offset = 0;
    this->_base = 16;
    this->_endian = QSysInfo::BigEndian;
    this->_hexeditdata = nullptr;

    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);

    if(DataTypesModel::_types.isEmpty())
    {
        DataTypesModel::_botypes[QSysInfo::LittleEndian] = QHash<DataType::Type, DataType::Type>();
        DataTypesModel::_botypes[QSysInfo::BigEndian] = QHash<DataType::Type, DataType::Type>();

        DataTypesModel::_botypes[QSysInfo::LittleEndian][DataType::Int8] = DataType::Int8;
        DataTypesModel::_botypes[QSysInfo::LittleEndian][DataType::UInt8] = DataType::UInt8;
        DataTypesModel::_botypes[QSysInfo::LittleEndian][DataType::Int16] = DataType::Int16_LE;
        DataTypesModel::_botypes[QSysInfo::LittleEndian][DataType::UInt16] = DataType::UInt16_LE;
        DataTypesModel::_botypes[QSysInfo::LittleEndian][DataType::Int32] = DataType::Int32_LE;
        DataTypesModel::_botypes[QSysInfo::LittleEndian][DataType::UInt32] = DataType::UInt32_LE;
        DataTypesModel::_botypes[QSysInfo::LittleEndian][DataType::Int64] = DataType::Int64_LE;
        DataTypesModel::_botypes[QSysInfo::LittleEndian][DataType::UInt64] = DataType::UInt64_LE;

        DataTypesModel::_botypes[QSysInfo::BigEndian][DataType::Int8] = DataType::Int8;
        DataTypesModel::_botypes[QSysInfo::BigEndian][DataType::UInt8] = DataType::UInt8;
        DataTypesModel::_botypes[QSysInfo::BigEndian][DataType::Int16] = DataType::Int16_BE;
        DataTypesModel::_botypes[QSysInfo::BigEndian][DataType::UInt16] = DataType::UInt16_BE;
        DataTypesModel::_botypes[QSysInfo::BigEndian][DataType::Int32] = DataType::Int32_BE;
        DataTypesModel::_botypes[QSysInfo::BigEndian][DataType::UInt32] = DataType::UInt32_BE;
        DataTypesModel::_botypes[QSysInfo::BigEndian][DataType::Int64] = DataType::Int64_BE;
        DataTypesModel::_botypes[QSysInfo::BigEndian][DataType::UInt64] = DataType::UInt64_BE;

        DataTypesModel::_types.append(DataType::Character);
        DataTypesModel::_types.append(DataType::Int8);
        DataTypesModel::_types.append(DataType::UInt8);
        DataTypesModel::_types.append(DataType::Int16);
        DataTypesModel::_types.append(DataType::UInt16);
        DataTypesModel::_types.append(DataType::Int32);
        DataTypesModel::_types.append(DataType::UInt32);
        DataTypesModel::_types.append(DataType::Int64);
        DataTypesModel::_types.append(DataType::UInt64);
        DataTypesModel::_types.append(DataType::Array);

        DataTypesModel::_typenames.append("Char");
        DataTypesModel::_typenames.append("Int8");
        DataTypesModel::_typenames.append("UInt8");
        DataTypesModel::_typenames.append("Int16");
        DataTypesModel::_typenames.append("UInt16");
        DataTypesModel::_typenames.append("Int32");
        DataTypesModel::_typenames.append("UInt32");
        DataTypesModel::_typenames.append("Int64");
        DataTypesModel::_typenames.append("UInt64");
        DataTypesModel::_typenames.append("Ascii String");
    }
}

QSysInfo::Endian DataTypesModel::endian()
{
    return this->_endian;
}

void DataTypesModel::setEndian(QSysInfo::Endian endian)
{
    this->_endian = endian;
    this->updateData();
}

int DataTypesModel::base()
{
    return this->_base;
}

void DataTypesModel::setOffset(qint64 pos)
{
    this->_offset = pos;
    this->updateData();
}

void DataTypesModel::setBase(int base)
{
    int oldbase = this->_base;
    this->_base = base;

    if(oldbase != base)
        this->updateData();
}

QString DataTypesModel::readValue(int row, bool* overflow) const
{
    DataType::Type type = this->_types.at(row);
    int bytewidth = DataType::byteWidth(type);

    if(overflow)
        *overflow = false;

    if(bytewidth && ((this->_offset + bytewidth) < this->_hexeditdata->length()))
    {
        if(bytewidth >= 1 && NumericLimits::willOverflow(this->_hexeditdata, this->_offset, DataTypesModel::_botypes[this->_endian][type]))
        {
            if(overflow)
                *overflow = true;

            return "Overflow";
        }

        QHexEditDataReader reader(this->_hexeditdata);

        switch(type)
        {
            case DataType::Character:
                return QString("%1").arg(static_cast<char>(reader.at(this->_offset)));

            case DataType::Int8:
                return QString("%1").arg(static_cast<char>(reader.at(this->_offset)), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::UInt8:
                return QString("%1").arg(static_cast<uchar>(reader.at(this->_offset)), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::Int16:
                return QString("%1").arg(reader.readInt16(this->_offset, this->_endian), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::UInt16:
                return QString("%1").arg(reader.readUInt16(this->_offset, this->_endian), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::Int32:
                return QString("%1").arg(reader.readInt32(this->_offset, this->_endian), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::UInt32:
                return QString("%1").arg(reader.readUInt32(this->_offset, this->_endian), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::Int64:
                return QString("%1").arg(static_cast<qint64>(reader.readInt64(this->_offset, this->_endian)), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::UInt64:
                return QString("%1").arg(static_cast<quint64>(reader.readUInt64(this->_offset, this->_endian)), bytewidth, this->_base, QLatin1Char('0'));

            default:
                break;
        }
    }
    else if(type == DataType::Array)
    {
        QHexEditDataReader reader(this->_hexeditdata);
        return reader.readString(this->_offset, DataTypesModel::STRING_LENGTH);
    }

    return QString();
}

QVariant DataTypesModel::readType(int row) const
{
    DataType::Type type = this->_types.at(row);
    QHexEditDataReader reader(this->_hexeditdata);

    switch(type)
    {
        case DataType::Character:
            return QString(static_cast<char>(reader.at(this->_offset)));

        case DataType::Int8:
            return static_cast<char>(reader.at(this->_offset));

        case DataType::UInt8:
            return static_cast<uchar>(reader.at(this->_offset));

        case DataType::Int16:
            return reader.readInt16(this->_offset, this->_endian);

        case DataType::UInt16:
            return reader.readUInt16(this->_offset, this->_endian);

        case DataType::Int32:
            return reader.readInt32(this->_offset, this->_endian);

        case DataType::UInt32:
            return reader.readUInt32(this->_offset, this->_endian);

        case DataType::Int64:
            return static_cast<qint64>(reader.readInt64(this->_offset, this->_endian));

        case DataType::UInt64:
            return static_cast<quint64>(reader.readUInt64(this->_offset, this->_endian));

        case DataType::Array:
        {
            QHexEditDataReader reader(this->_hexeditdata);
            return reader.readString(this->_offset, DataTypesModel::STRING_LENGTH);
        }

        default:
            break;
    }

    return QVariant();
}

void DataTypesModel::updateData()
{
    emit dataChanged(this->index(0, 1), this->index(this->rowCount() - 1, 1));
}

void DataTypesModel::setData(QHexEditData *hexeditdata)
{
    this->_offset = 0;
    this->_hexeditdata = hexeditdata;
    this->updateData();
}

int DataTypesModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant DataTypesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Type";

            case 1:
                return "Value";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant DataTypesModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !this->_hexeditdata)
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0 && role == Qt::DisplayRole)
            return DataTypesModel::_typenames.at(index.row());
        else if(index.column() == 1)
        {
            bool overflow = false;
            DataType::Type currenttype = this->_types.at(index.row());
            QString s = this->readValue(index.row(), &overflow);

            if(DataType::isInteger(currenttype) && !overflow)
                return s.toUpper();
            else if(DataType::isString(currenttype) || currenttype == DataType::Character)
                return QString("'%1'").arg(s.simplified());

            return s;
        }
    }
    else if((role == Qt::EditRole) && (index.column() == 1))
        return this->readType(index.row());
    else if(role == Qt::ForegroundRole)
    {
        if(index.column() == 1)
        {
            bool overflow = false;
            this->readValue(index.row(), &overflow);

            if(overflow)
                return QColor(Qt::red);

            if((this->_types.at(index.row()) == DataType::Character) || (this->_types.at(index.row()) == DataType::Array))
                return QColor(Qt::darkGreen);

            return QColor(Qt::darkBlue);
        }
    }
    else if(role == Qt::FontRole)
    {
        if(index.column() == 1)
            return this->_monospacefont;
    }
    else if(role == (Qt::UserRole + 1))
        return DataType::isSigned(this->_types.at(index.row()));
    else if(role == (Qt::UserRole + 2))
        return this->_base;

    return QVariant();
}

bool DataTypesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole && index.isValid() && (index.column() == 1))
    {
        QHexEditDataWriter writer(this->_hexeditdata);

        if((index.row() == 0) || (index.row() == this->rowCount() - 1))
        {
            writer.replace(this->_offset, value.toString().toUtf8());
            return true;
        }

        DataType::Type type = this->_types.at(index.row());

        switch(type)
        {

            case DataType::Int8:
                writer.writeInt8(this->_offset, value.toInt());
                return true;

            case DataType::UInt8:
                writer.writeUInt8(this->_offset, value.toUInt());
                return true;

            case DataType::Int16:
                writer.writeInt16(this->_offset, value.toInt(), this->_endian);
                return true;

            case DataType::UInt16:
                writer.writeUInt16(this->_offset, value.toUInt(), this->_endian);
                return true;

            case DataType::Int32:
                writer.writeInt32(this->_offset, value.toInt(), this->_endian);
                return true;

            case DataType::UInt32:
                writer.writeUInt32(this->_offset, value.toUInt(), this->_endian);
                return true;

            case DataType::Int64:
                writer.writeInt64(this->_offset, value.toLongLong(), this->_endian);
                return true;

            case DataType::UInt64:
                writer.writeUInt64(this->_offset, value.toULongLong(), this->_endian);
                return true;

            default:
                break;
        }

        return false;
    }

    return QAbstractItemModel::setData(index, value, role);
}

QModelIndex DataTypesModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    return this->createIndex(row, column);
}

QModelIndex DataTypesModel::parent(const QModelIndex &) const
{
    return QModelIndex(); /* It's a List: No Parents! */
}

int DataTypesModel::rowCount(const QModelIndex&) const
{
    return DataTypesModel::_types.count();
}

Qt::ItemFlags DataTypesModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if(this->_hexeditdata)
    {
        bool overflow = false;
        this->readValue(index.row(), &overflow);

        if(!overflow && (index.column() == 1))
            flags |= Qt::ItemIsEditable;
    }

    return flags;
}
