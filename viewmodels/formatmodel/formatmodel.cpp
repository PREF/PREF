#include "formatmodel.h"

FormatModel::FormatModel(const FormatTree::Ptr &formattree, ByteBuffer *bytebuffer, QObject *parent): FieldDataModel(parent)
{
    this->_formattree = formattree;
    this->_bytebuffer = bytebuffer;

    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);

    /* Thread Safety: Don't use QPixmap */
    this->_icostruct.load(":/format_icons/res/struct.png");
    this->_icofield.load(":/format_icons/res/field.png");
    this->_icobitfield.load(":/format_icons/res/bitfield.png");

    connect(this->_bytebuffer->hexEditData(), SIGNAL(dataChanged(qint64,qint64,QHexEditData::ActionType)), this, SLOT(updateModelData(qint64,qint64,QHexEditData::ActionType)));
}

FormatTree::Ptr FormatModel::tree()
{
    return this->_formattree;
}

void FormatModel::updateModelData(qint64 offset, qint64 length, QHexEditData::ActionType)
{
    qint64 startoffset = offset;
    qint64 endoffset = offset + length;

    for(int i = 0; i < this->_formattree->structureCount(); i++)
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

    FormatElement* fo = reinterpret_cast<FormatElement*>(index.internalPointer());

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(role == Qt::DisplayRole)
        {
            if(index.column() == 0)
                return QString("%1").arg(fo->offset(), 8, 16, QLatin1Char('0')).toUpper();
            else if(index.column() == 1)
                return fo->displayType();
            else if(index.column() == 2)
                return fo->displayName();
            else if(index.column() == 4)
                return fo->info();
        }

        if(index.column() == 3 && ((fo->elementType() == ElementType::field() || fo->elementType() == ElementType::fieldArray()) || (fo->elementType() == ElementType::bitField())))
            return fo->displayValue();
    }
    else if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
        {
            if(fo->elementType() == ElementType::structure())
                return this->_icostruct;
            if((fo->elementType() == ElementType::field()) || (fo->elementType() == ElementType::fieldArray()))
                return this->_icofield;
            else if(fo->elementType() == ElementType::bitField())
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
        else if(((index.column() == 3) && ((fo->elementType() == ElementType::field()) || (fo->elementType() == ElementType::fieldArray()) || (fo->elementType() == ElementType::bitField()))) || (index.column() == 4))
        {
            if(index.column() == 4)
                return QColor(Qt::darkGreen);

            if(index.column() == 3)
            {
                if(fo->elementType() == ElementType::field())
                {
                    Field* f = formatelement_cast<Field>(fo);

                    if(f->isOverflowed())
                        return QColor(Qt::red);

                    if(f->isInteger())
                        return QColor(Qt::darkBlue);

                    if(f->dataType() == DataType::Char)
                        return QColor(Qt::darkGreen);

                    else if(f->dataType() == DataType::Blob)
                        return QColor(Qt::darkGray);
                }
                else if(fo->elementType() == ElementType::fieldArray())
                {
                    FieldArray* fa = formatelement_cast<FieldArray>(fo);

                    if(fa->elementType() == DataType::Char)
                        return QColor(Qt::darkGreen);
                    else if(fa->elementType() == DataType::Blob)
                        return QColor(Qt::darkGray);
                }
                else if(fo->elementType() == ElementType::bitField())
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
        FormatElement* formatobj = reinterpret_cast<FormatElement*>(index.internalPointer());

        if((formatobj->elementType() == ElementType::field()) && (index.column() == 3))
        {
            Field* field = formatelement_cast<Field>(formatobj);
            QString currvalue = field->displayValue();
            QString newvalue = value.toString();

            if(field->dataType() == DataType::Char)
                currvalue = currvalue.mid(1, currvalue.length() - 2); /* Strip ' ' */

            if(!QString::compare(currvalue, newvalue, Qt::CaseInsensitive))  /* Does 'currvalue == newvalue' ? */
                return false;

            QByteArray newdata;
            bool valid = FormatModel::validateValue(value, field->dataType(), field->base(), this->_bytebuffer->endian(), newdata);

            if(valid)
            {
                this->_bytebuffer->write(field->offset(), newdata.length(), newdata);
                return true;
            }

            return false;
        }
    }

    return FieldDataModel::setData(index, value, role);
}

QModelIndex FormatModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->_formattree || !this->hasIndex(row, column, parent))
        return QModelIndex();

    if(!parent.isValid())
        return this->createIndex(row, column, this->_formattree->structure(row));

    FormatElement* parentelem = reinterpret_cast<FormatElement*>(parent.internalPointer());

    if(parentelem->elementType() == ElementType::structure())
    {
        Structure* parentstruct = formatelement_cast<Structure>(parentelem);
        return this->createIndex(row, column, parentstruct->field(row));
    }
    else if(parentelem->elementType() == ElementType::fieldArray())
    {
        FieldArray* parentfieldarray = formatelement_cast<FieldArray>(parentelem);
        return this->createIndex(row, column, parentfieldarray->element(row));
    }
    else if(parentelem->elementType() == ElementType::field())
    {
        Field* parentfield = formatelement_cast<Field>(parentelem);
        return this->createIndex(row, column, parentfield->bitField(row));
    }

    return QModelIndex();
}

QModelIndex FormatModel::parent(const QModelIndex &child) const
{
    if(!this->_formattree || !child.isValid())
        return QModelIndex();

    FormatElement* childobj = reinterpret_cast<FormatElement*>(child.internalPointer());

    if(childobj->elementType() == ElementType::structure())
    {
        if(!childobj->hasParent())
            return QModelIndex();

        Structure* parentstruct = formatelement_cast<Structure>(childobj->parentElement());

        if(parentstruct->hasParent())
        {
            Structure* parentofparent = formatelement_cast<Structure>(parentstruct->parentElement());
            return this->createIndex(parentofparent->indexOf(parentstruct), 0, parentstruct);
        }
        else
            return this->createIndex(this->_formattree->indexOf(parentstruct->offset()), 0, parentstruct);
    }
    else if(childobj->elementType() == ElementType::field() || childobj->elementType() == ElementType::fieldArray())
    {
        FormatElement* parentobj = formatelement_cast<FormatElement>(childobj->parentElement());

        if(parentobj->elementType() == ElementType::structure())
        {
            if(parentobj->hasParent())
            {
                Structure* parentofparent = formatelement_cast<Structure>(parentobj->parentElement());
                return this->createIndex(parentofparent->indexOf(parentobj), 0, parentobj);
            }
            else
                return this->createIndex(this->_formattree->indexOf(parentobj->offset()), 0, parentobj);
        }

        if(childobj->elementType() == ElementType::field() && parentobj->elementType() == ElementType::fieldArray())
        {
            FieldArray* parentfieldarray = formatelement_cast<FieldArray>(parentobj);
            Structure* parentstruct = formatelement_cast<Structure>(parentfieldarray->parentElement());
            return this->createIndex(parentstruct->indexOf(parentfieldarray), 0, parentfieldarray);
        }
    }
    else if(childobj->elementType() == ElementType::bitField())
    {
        Field* parentfield = formatelement_cast<Field>(childobj->parentElement());
        FormatElement* parentofparent = parentfield->parentElement();

        if(parentofparent->elementType() == ElementType::structure())
        {
            Structure* s = formatelement_cast<Structure>(parentofparent);
            return this->createIndex(s->indexOf(parentfield), 0, parentfield);
        }
        else if(parentofparent->elementType() == ElementType::fieldArray())
        {
            Structure* s = formatelement_cast<Structure>(parentofparent->parentElement());
            return this->createIndex(s->indexOf(parentfield), 0, parentfield);
        }
    }

    return QModelIndex();
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

        if(parentelement->elementType() == ElementType::structure())
        {
            Structure* parentstruct = formatelement_cast<Structure>(parentelement);
            return parentstruct->fieldCount();
        }
        else if(parentelement->elementType() == ElementType::fieldArray())
        {
            FieldArray* parentfieldarray = formatelement_cast<FieldArray>(parentelement);
            return parentfieldarray->elementCount();
        }
        else if(parentelement->elementType() == ElementType::field())
        {
            Field* parentfield = formatelement_cast<Field>(parentelement);
            return parentfield->bitFieldCount();
        }
    }

    return 0;
}

Qt::ItemFlags FormatModel::flags(const QModelIndex &index) const
{    
    if(!this->_formattree || !index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    FormatElement* formatobj = reinterpret_cast<FormatElement*>(index.internalPointer());

    if((index.column() == 3) && (formatobj->elementType() == ElementType::field()))
        flags |= Qt::ItemIsEditable;

    return flags;
}
