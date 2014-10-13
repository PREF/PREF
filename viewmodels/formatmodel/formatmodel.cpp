#include "formatmodel.h"

FormatModel::FormatModel(QHexEditData *hexeditdata, QObject *parent): FieldDataModel(parent), _hexeditdata(hexeditdata), _formattree(nullptr)
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

void FormatModel::setFormatTree(FormatTree *formattree)
{
    this->beginInsertRows(QModelIndex(), 0, formattree->structureCount());
    this->_formattree = formattree;
    this->endInsertRows();
}

void FormatModel::updateModelData(qint64 offset, qint64 length, QHexEditData::ActionType)
{
    if(!this->_formattree)
        return;

    qint64 startoffset = offset;
    qint64 endoffset = offset + length;

    for(quint64 i = 0; i < this->_formattree->structureCount(); i++)
    {
        Structure* s = this->_formattree->structure(i);

        if(s->containsOffset(startoffset))
        {
            QModelIndex midx = this->index(i, 0);
            emit dataChanged(midx, this->index(midx.row(), this->columnCount() - 1, midx.parent()));

            if(s->containsOffset(endoffset))
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
            else if(index.column() == 1)
                return formatelement->displayType();
            else if(index.column() == 2)
                return formatelement->displayName();
            else if(index.column() == 4)
                return formatelement->info();
        }

        if(index.column() == 3 && ((formatelement->elementType() == FormatElement::FieldType || formatelement->elementType() == FormatElement::FieldArrayType) || (formatelement->elementType() == FormatElement::BitFieldType)))
            return formatelement->displayValue();
    }
    else if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
        {
            if(formatelement->elementType() == FormatElement::StructureType)
                return this->_icostruct;
            if((formatelement->elementType() == FormatElement::FieldType) || (formatelement->elementType() == FormatElement::FieldArrayType))
                return this->_icofield;
            else if(formatelement->elementType() == FormatElement::BitFieldType)
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
        else if(((index.column() == 3) && ((formatelement->elementType() == FormatElement::FieldType) || (formatelement->elementType() == FormatElement::FieldArrayType) || (formatelement->elementType() == FormatElement::BitFieldType))) || (index.column() == 4))
        {
            if(index.column() == 4)
                return QColor(Qt::darkGreen);

            if(index.column() == 3)
            {
                if(formatelement->elementType() == FormatElement::FieldType)
                {
                    FieldElement* fieldelement = qobject_cast<FieldElement*>(formatelement);

                    if(fieldelement->isOverflowed())
                        return QColor(Qt::red);

                    if(fieldelement->isInteger())
                        return QColor(Qt::darkBlue);

                    if(fieldelement->dataType() == DataType::Character)
                        return QColor(Qt::darkGreen);

                    if(fieldelement->dataType() == DataType::Blob)
                        return QColor(Qt::darkGray);
                }
                else if(formatelement->elementType() == FormatElement::FieldArrayType)
                {
                    FieldArray* fa = qobject_cast<FieldArray*>(formatelement);

                    if(fa->itemType() == DataType::Character)
                        return QColor(Qt::darkGreen);
                    else if(fa->itemType() == DataType::Blob)
                        return QColor(Qt::darkGray);
                }
                else if(formatelement->elementType() == FormatElement::BitFieldType)
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

        if((formatelement->elementType() == FormatElement::FieldType) && (index.column() == 3))
        {
            FieldElement* fieldelement = qobject_cast<FieldElement*>(formatelement);
            QString currvalue = fieldelement->displayValue();
            QString newvalue = value.toString();

            if(fieldelement->dataType() == DataType::Character)
                currvalue = currvalue.mid(1, currvalue.length() - 2); /* Strip ' ' */

            if(!QString::compare(currvalue, newvalue, Qt::CaseInsensitive))  /* Does 'currvalue == newvalue' ? */
                return false;

            QByteArray newdata;
            bool valid = FormatModel::validateValue(value, fieldelement->dataType(), fieldelement->base(), DataType::byteOrder(fieldelement->dataType()), newdata);

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
        quint64 count = 0;
        FormatElement* formatelement = reinterpret_cast<FormatElement*>(parent.internalPointer());
        formatelement->parseChildren();

        switch(formatelement->elementType())
        {
            case FormatElement::StructureType:
                count = qobject_cast<Structure*>(formatelement)->fieldCount();
                break;

            case FormatElement::FieldArrayType:
                count = qobject_cast<FieldArray*>(formatelement)->itemCount();
                break;

            case FormatElement::FieldType:
                count = qobject_cast<Field*>(formatelement)->bitFieldCount();
                break;

            default:
                break;
        }

        /* Notify the View that 'count' items has been inserted */
        this->beginInsertRows(parent, 0, count);
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
        element = this->_formattree->elementFromPool(row, parentelement);
    }
    else
        element = this->_formattree->elementFromPool(row);

    if(!element)
        return QModelIndex();

    return this->createIndex(row, column, element);
}

QModelIndex FormatModel::parent(const QModelIndex &child) const
{
    if(!this->_formattree || !child.isValid())
        return QModelIndex();

    FormatElement* childelement = reinterpret_cast<FormatElement*>(child.internalPointer());

    if(!childelement->hasParent())
        return QModelIndex();

    FormatElement* parentelement = childelement->parentElement();
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
        return this->_formattree->structureCount() > 0;

    FormatElement* parentelement = reinterpret_cast<FormatElement*>(parent.internalPointer());
    return parentelement->hasChildren();
}

int FormatModel::rowCount(const QModelIndex &parent) const
{
    if(!this->_formattree)
        return 0;

    if(!parent.isValid())
        return this->_formattree->structureCount();

    if(parent.column() == 0)
    {
        FormatElement* parentelement = reinterpret_cast<FormatElement*>(parent.internalPointer());

        if(!parentelement->isDynamic())
        {
            if(parentelement->elementType() == FormatElement::StructureType)
                return qobject_cast<Structure*>(parentelement)->fieldCount();
            else if(parentelement->elementType() == FormatElement::FieldType)
                return qobject_cast<Field*>(parentelement)->bitFieldCount();
            else if(parentelement->elementType() == FormatElement::FieldArrayType)
            {
                FieldArray* fieldarray = qobject_cast<FieldArray*>(parentelement);

                if(fieldarray->itemType() != DataType::Blob)
                    return qobject_cast<FieldArray*>(parentelement)->itemCount();
            }
        }
    }

    return 0;
}

Qt::ItemFlags FormatModel::flags(const QModelIndex &index) const
{    
    if(!this->_formattree || !index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    FormatElement* formatelement = reinterpret_cast<FormatElement*>(index.internalPointer());

    if((index.column() == 3) && (formatelement->elementType() == FormatElement::FieldType))
        flags |= Qt::ItemIsEditable;

    return flags;
}
