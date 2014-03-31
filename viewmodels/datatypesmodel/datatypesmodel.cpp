#include "datatypesmodel.h"

const int DataTypesModel::STRING_LENGTH = 15;
QVector<lua_Integer> DataTypesModel::_types;
QVector<QString> DataTypesModel::_typenames;

DataTypesModel::DataTypesModel(QObject *parent): FieldDataModel(parent)
{
    this->_offset = 0;
    this->_base = 16;
    this->_endian = QSysInfo::BigEndian;
    this->_bytebuffer = nullptr;

    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);

    if(DataTypesModel::_types.isEmpty())
    {
        DataTypesModel::_types.append(DataType::character());
        DataTypesModel::_types.append(DataType::int8());
        DataTypesModel::_types.append(DataType::uint8());
        DataTypesModel::_types.append(DataType::int16());
        DataTypesModel::_types.append(DataType::uint16());
        DataTypesModel::_types.append(DataType::int32());
        DataTypesModel::_types.append(DataType::uint32());
        DataTypesModel::_types.append(DataType::int64());
        DataTypesModel::_types.append(DataType::uint64());
        DataTypesModel::_types.append(DataType::array());

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
    lua_Integer type = this->_types.at(row);
    int bytelength = DataType::byteWidth(type);

    if(overflow)
        *overflow = false;

    if(bytelength && ((this->_offset + bytelength) < this->_bytebuffer->length()))
    {
        if(type == DataType::character())
            return this->_bytebuffer->readString(this->_offset, 1);

        /* NOTE:
        if(NumericLimits::willOverflow(this->_bytebuffer, this->_offset, type, this->_endian))
        {
            if(overflow)
                *overflow = true;

            return "Overflow";
        }
        */

        return this->_bytebuffer->stringValue(this->_offset, this->_base, type, this->_endian);
    }
    else if(type == DataType::array())
        return this->_bytebuffer->readValidString(this->_offset, DataTypesModel::STRING_LENGTH);

    return QString();
}

void DataTypesModel::updateData()
{
    emit dataChanged(this->index(0, 1), this->index(this->rowCount() - 1, 1));
}

void DataTypesModel::setData(ByteBuffer *bytebuffer)
{
    this->_offset = 0;
    this->_bytebuffer = bytebuffer;
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
    if(!index.isValid() || !this->_bytebuffer)
        return QVariant();

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(index.column() == 0 && role == Qt::DisplayRole)
            return DataTypesModel::_typenames.at(index.row());
        else if(index.column() == 1)
        {
            bool overflow = false;
            //NOTE: QString s = this->readValue(index.row(), &overflow);

            //NOTE: if(!DataType::isInteger(this->_types.at(index.row())) && (role == Qt::DisplayRole) && !overflow)
                //return QString("'%1'").arg(s.simplified());

            //NOTE: return s.simplified();
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

            if((this->_types.at(index.row()) == DataType::character()) || (this->_types.at(index.row()) == DataType::array()))
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
            this->_bytebuffer->write(this->_offset, newdata.length(), newdata);
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

    if(this->_bytebuffer)
    {
        bool overflow = false;
        this->readValue(index.row(), &overflow);

        if(!overflow && (index.column() == 1))
            flags |= Qt::ItemIsEditable;
    }

    return flags;
}
