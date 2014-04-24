#include "datatypesmodel.h"

const int DataTypesModel::STRING_LENGTH = 15;
QHash<QSysInfo::Endian, QHash<DataType::Type, DataType::Type> > DataTypesModel::_botypes;
QList<DataType::Type> DataTypesModel::_types;
QList<QString> DataTypesModel::_typenames;

DataTypesModel::DataTypesModel(QObject *parent): FieldDataModel(parent)
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
        DataTypesModel::_typenames.append("Signed Byte");
        DataTypesModel::_typenames.append("Unsigned Byte");
        DataTypesModel::_typenames.append("Signed Short");
        DataTypesModel::_typenames.append("Unsigned Short");
        DataTypesModel::_typenames.append("Signed Int");
        DataTypesModel::_typenames.append("Unsigned Int");
        DataTypesModel::_typenames.append("Signed Int64");
        DataTypesModel::_typenames.append("Unsigned Int64");
        DataTypesModel::_typenames.append("String");
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

        switch(type)
        {
            case DataType::Character:
                return QString("%1").arg(static_cast<char>(this->_hexeditdata->at(this->_offset)));

            case DataType::Int8:
                return QString("%1").arg(static_cast<char>(this->_hexeditdata->at(this->_offset)), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::UInt8:
                return QString("%1").arg(static_cast<uchar>(this->_hexeditdata->at(this->_offset)), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::Int16:
                return QString("%1").arg(this->_hexeditdata->readInt16(this->_offset, this->_endian), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::UInt16:
                return QString("%1").arg(this->_hexeditdata->readUInt16(this->_offset, this->_endian), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::Int32:
                return QString("%1").arg(this->_hexeditdata->readInt32(this->_offset, this->_endian), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::UInt32:
                return QString("%1").arg(this->_hexeditdata->readUInt32(this->_offset, this->_endian), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::Int64:
                return QString("%1").arg(static_cast<qint64>(this->_hexeditdata->readInt64(this->_offset, this->_endian)), bytewidth, this->_base, QLatin1Char('0'));

            case DataType::UInt64:
                return QString("%1").arg(static_cast<quint64>(this->_hexeditdata->readUInt64(this->_offset, this->_endian)), bytewidth, this->_base, QLatin1Char('0'));

            default:
                break;
        }
    }
    else if(type == DataType::Array)
        return this->_hexeditdata->readString(this->_offset, DataTypesModel::STRING_LENGTH);

    return QString();
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
                return "Name";

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

    if(role == Qt::DisplayRole || role == Qt::EditRole)
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

    return QVariant();
}

bool DataTypesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole && index.isValid() && (index.column() == 1))
    {
        QString currvalue = this->data(index, Qt::DisplayRole).toString();
        QString newvalue = value.toString();

        if(!QString::compare(currvalue, newvalue, Qt::CaseInsensitive)) /* Does 'currvalue == newvalue' ? */
            return false;

        QByteArray newdata;
        bool valid = DataTypesModel::validateValue(value, this->_types.at(index.row()), this->_base, this->_endian, newdata);

        if(valid)
        {
            this->_hexeditdata->replace(this->_offset, newdata.length(), newdata);
            return true;
        }

        return false;
    }

    return FieldDataModel::setData(index, value, role);
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
