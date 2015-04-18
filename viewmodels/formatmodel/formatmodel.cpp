#include "formatmodel.h"

FormatModel::FormatModel(QHexEditData *hexeditdata, FormatTree* formattree, QObject *parent): FieldDataModel(parent), _hexeditdata(hexeditdata), _formattree(formattree)
{
    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);

    /* Thread Safety: Don't use QPixmap */
    this->_icostruct.load(":/format_icons/res/struct.png");
    this->_icofield.load(":/format_icons/res/field.png");
    this->_icobitfield.load(":/format_icons/res/bitfield.png");

    connect(this->_hexeditdata, SIGNAL(dataChanged(qint64,qint64,QHexEditData::ActionType)), this, SLOT(updateModelData(qint64,qint64,QHexEditData::ActionType)));
}

QHexEditData *FormatModel::data()
{
    return this->_hexeditdata;
}

void FormatModel::updateModelData(qint64 offset, qint64 length, QHexEditData::ActionType)
{
    if(!this->_formattree)
        return;

    for(size_t i = 0; i < this->_formattree->length(); i++)
    {
        FormatElement* formatelement = this->_formattree->element(i);

        if(formatelement->contains(offset))
        {
            QModelIndex midx = this->index(i, 0);
            emit dataChanged(midx, this->index(midx.row(), this->columnCount() - 1, midx.parent()));

            if(formatelement->contains(offset + length))
                break;
        }
    }
}

int FormatModel::columnCount(const QModelIndex&) const
{
    return 5;
}

QVariant FormatModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
            case 0:
                return "Offset";

            case 1:
                return "Type";

            case 2:
                return "Name";

            case 3:
                return "Value";

            case 4:
                return "Info";

            default:
                break;
        }
    }

    return QVariant();
}

QVariant FormatModel::data(const QModelIndex &index, int role) const
{
    if(!this->_formattree || !index.isValid())
        return QVariant();

    FormatElement* formatelement = reinterpret_cast<FormatElement*>(index.internalPointer());

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(role == Qt::DisplayRole)
        {
            if(index.column() == 0)
                return QString("%1").arg(formatelement->offset(), 8, 16, QLatin1Char('0')).toUpper();

            if(index.column() == 1)
            {
                if(formatelement->isStructure())
                    return "Struct";

                if(formatelement->isFieldArray())
                    return QString("%1[]").arg(DataType::stringValue(dynamic_cast<FieldArray*>(formatelement)->dataType()));

                if(formatelement->isField())
                    return DataType::stringValue(dynamic_cast<Field*>(formatelement)->dataType());

                if(formatelement->isBitField())
                {
                    BitField* bitfield = dynamic_cast<BitField*>(formatelement);

                    if(bitfield->bitstart() != bitfield->bitend())
                        return "bit[]";

                    return "bit";
                }

                return QString();
            }

            if(index.column() == 2)
            {
                if(formatelement->isFieldArray())
                    return QString("%1[%2]").arg(formatelement->name()).arg(formatelement->length(), 0, formatelement->base());

                if(formatelement->isBitField())
                {
                    BitField* bitfield = dynamic_cast<BitField*>(formatelement);

                    if(bitfield->bitstart() == bitfield->bitend())
                        return QString("%1[%2]").arg(bitfield->name()).arg(bitfield->bitstart(), 0, bitfield->base());

                    return QString("%1[%2..%3]").arg(bitfield->name()).arg(bitfield->bitstart(), 0, bitfield->base()).arg(bitfield->bitend(), 0, bitfield->base());
                }

                return formatelement->name();
            }

            if(index.column() == 3)
            {
                if(formatelement->isField())
                {
                    Field* field = dynamic_cast<Field*>(formatelement);
                    DataValue dv = field->value();

                    if(dv.isCharacter())
                        return QString("'%1'").arg(QString(dv.toString()).simplified());

                    return dv.toString(field->base(), DataType::sizeOf(field->dataType()));
                }

                if(formatelement->isFieldArray())
                {
                    FieldArray* fieldarray = dynamic_cast<FieldArray*>(formatelement);

                    if(fieldarray->elementType() == DataType::Character)
                    {
                        QHexEditDataReader reader(this->_hexeditdata);
                        QString("'%1'").arg(QString(reader.read(fieldarray->offset(), fieldarray->length())).simplified());
                    }
                }

                if(formatelement->isBitField())
                {
                    BitField* bitfield = dynamic_cast<BitField*>(formatelement);
                    return bitfield->value().toString(bitfield->base(), (bitfield->bitend() - bitfield->bitstart()) + 1);
                }

                return QString();
            }

            if(index.column() == 4)
                return formatelement->info();
        }
    }
    else if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
        {
            if(formatelement->isStructure())
                return this->_icostruct;
            if(formatelement->isField() || formatelement->isFieldArray())
                return this->_icofield;
            else if(formatelement->isBitField())
                return this->_icobitfield;
        }
    }
    else if(role == Qt::FontRole)
    {
        if((index.column() == 0) || (index.column() == 3))
            return this->_monospacefont;
    }
    else if(role == Qt::ForegroundRole)
    {
        if(index.column() == 0)
            return QColor(Qt::darkBlue);
        else if(((index.column() == 3) && (formatelement->isField() || formatelement->isFieldArray() || formatelement->isBitField())) || (index.column() == 4))
        {
            if(index.column() == 4)
                return QColor(Qt::darkGreen);

            if(index.column() == 3)
            {
                if(formatelement->isField())
                {
                    Field* field = dynamic_cast<Field*>(formatelement);

                    if(field->dataType() == DataType::Blob)
                        return QColor(Qt::darkGray);

                    DataValue dv = field->value();

                    if(dv.isOverflowed())
                        return QColor(Qt::red);

                    if(dv.isArithmetic())
                        return QColor(Qt::darkBlue);

                    if(dv.isString())
                        return QColor(Qt::darkGreen);
                }
                else if(formatelement->isFieldArray())
                {
                    FieldArray* fa = dynamic_cast<FieldArray*>(formatelement);

                    if(fa->elementType() == DataType::Character)
                        return QColor(Qt::darkGreen);
                    else if(fa->elementType() == DataType::Blob)
                        return QColor(Qt::darkGray);
                }
                else if(formatelement->isBitField())
                    return QColor(Qt::darkRed);
            }
        }
    }

    return QVariant();
}

bool FormatModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(this->_formattree && (role == Qt::EditRole) && index.isValid())
    {
        FormatElement* formatelement = reinterpret_cast<FormatElement*>(index.internalPointer());

        if((formatelement->isField()) && (index.column() == 3))
        {
            Field* fieldelement = dynamic_cast<Field*>(formatelement);
            DataValue currvalue = fieldelement->value();
            QString newvalue = value.toString();

            if(!QString::compare(currvalue.toString(formatelement->base()), newvalue, Qt::CaseInsensitive))  // Does 'currvalue == newvalue' ?
                return false;

            QByteArray newdata;
            bool valid = FormatModel::validateValue(value, fieldelement->dataType(), fieldelement->base(), newdata);

            if(valid)
            {
                QHexEditDataWriter writer(this->_hexeditdata);
                writer.replace(fieldelement->offset(), newdata.length(), newdata);
                return true;
            }

            return false;
        }
    }

    return FieldDataModel::setData(index, value, role);
}

void FormatModel::fetchMore(const QModelIndex &parent)
{
    if(parent.isValid())
    {
        FormatElement* formatelement = reinterpret_cast<FormatElement*>(parent.internalPointer());
        formatelement->populate();

        /* Notify the View that 'count' items has been inserted */
        this->beginInsertRows(parent, 0, formatelement->length());
        this->endInsertRows();
        return;
    }

    QAbstractItemModel::fetchMore(parent);
}

QModelIndex FormatModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->_formattree || !this->hasIndex(row, column, parent))
        return QModelIndex();

    FormatElement* element = nullptr;

    if(parent.isValid())
    {
        FormatElement* parentelement = reinterpret_cast<FormatElement*>(parent.internalPointer());

        if(parentelement->isStructure())
            element = dynamic_cast<Structure*>(parentelement)->field(row);
        else if(parentelement->isFieldArray())
            element = dynamic_cast<FieldArray*>(parentelement)->item(row);
        else if(parentelement->isField())
            element = dynamic_cast<Field*>(parentelement)->bitField(row);
    }
    else
        element = this->_formattree->element(row);

    if(!element)
        return QModelIndex();

    return this->createIndex(row, column, element);
}

QModelIndex FormatModel::parent(const QModelIndex &child) const
{
    if(!this->_formattree || !child.isValid())
        return QModelIndex();

    FormatElement* childelement = reinterpret_cast<FormatElement*>(child.internalPointer());
    FormatElement* parentelement = childelement->parent();

    if(!parentelement)
        return QModelIndex();

    return this->createIndex(parentelement->indexOf(childelement), 0, parentelement);
}

bool FormatModel::canFetchMore(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        FormatElement* parentelement = reinterpret_cast<FormatElement*>(parent.internalPointer());
        return parentelement->isDynamic();
    }

    return QAbstractItemModel::canFetchMore(parent);
}

bool FormatModel::hasChildren(const QModelIndex &parent) const
{
    if(!this->_formattree)
        return false;

    if(!parent.isValid())
        return this->_formattree->length() > 0;

    FormatElement* parentelement = reinterpret_cast<FormatElement*>(parent.internalPointer());

    if(parentelement->isFieldArray() && (dynamic_cast<FieldArray*>(parentelement)->elementType() == DataType::Blob))
        return false;

    return parentelement->isDynamic() || (parentelement->length() > 0);
}

int FormatModel::rowCount(const QModelIndex &parent) const
{
    if(!this->_formattree)
        return 0;

    if(!parent.isValid())
        return this->_formattree->length();

    if(parent.column() == 0)
    {
        FormatElement* parentelement = reinterpret_cast<FormatElement*>(parent.internalPointer());

        if(!parentelement->isDynamic())
            return parentelement->length();
    }

    return 0;
}

Qt::ItemFlags FormatModel::flags(const QModelIndex &index) const
{    
    if(!this->_formattree || !index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    FormatElement* formatelement = reinterpret_cast<FormatElement*>(index.internalPointer());

    if((index.column() == 3) && formatelement->isField())
        flags |= Qt::ItemIsEditable;

    return flags;
}
