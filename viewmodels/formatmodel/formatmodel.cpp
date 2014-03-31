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

void FormatModel::setFormatTree(FormatTree *formattree)
{
    this->_formattree = formattree;
}

void FormatModel::updateModelData(qint64 offset, qint64 length, QHexEditData::ActionType)
{
    qint64 startoffset = offset;
    qint64 endoffset = offset + length;

    for(uint64_t i = 0; i < this->_formattree->StructureCount(); i++)
    {
        const Structure* s = this->_formattree->GetStructure(i + 1);

        if(s->ElementHdr.ContainsOffset(startoffset))
        {
            QModelIndex midx = this->index(i, 0);
            emit dataChanged(midx, this->index(midx.row(), this->columnCount() - 1, midx.parent()));

            if(s->ElementHdr.ContainsOffset(endoffset))
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

    const ElementHeader* elemhdr = reinterpret_cast<const ElementHeader*>(index.internalPointer());

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(role == Qt::DisplayRole)
        {
            if(index.column() == 0)
                return QString("%1").arg(elemhdr->Offset(), 8, 16, QLatin1Char('0')).toUpper();
            else if(index.column() == 1)
                return elemhdr->DisplayType();
            else if(index.column() == 2)
                return elemhdr->DisplayName();
            else if(index.column() == 4)
                return elemhdr->Info();
        }

        if(index.column() == 3 && ((elemhdr->ElementType == ElementType::Field || elemhdr->ElementType == ElementType::FieldArray) || (elemhdr->ElementType == ElementType::BitField)))
            return elemhdr->DisplayValue();
    }
    else if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
        {
            if(elemhdr->ElementType == ElementType::Structure)
                return this->_icostruct;
            if((elemhdr->ElementType == ElementType::Field) || (elemhdr->ElementType == ElementType::FieldArray))
                return this->_icofield;
            else if(elemhdr->ElementType == ElementType::BitField)
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
        else if(((index.column() == 3) && ((elemhdr->ElementType == ElementType::Field) || (elemhdr->ElementType == ElementType::FieldArray) || (elemhdr->ElementType == ElementType::BitField))) || (index.column() == 4))
        {
            if(index.column() == 4)
                return QColor(Qt::darkGreen);

            if(index.column() == 3)
            {
                if(elemhdr->ElementType == ElementType::Field)
                {
                    const FieldHeader* fieldhdr = reinterpret_cast<const FieldHeader*>(elemhdr);

                    if(fieldhdr->IsOverflowed())
                        return QColor(Qt::red);

                    if(fieldhdr->IsInteger())
                        return QColor(Qt::darkBlue);

                    if(fieldhdr->Type == DataType::Character)
                        return QColor(Qt::darkGreen);

                    if(fieldhdr->Type == DataType::Blob)
                        return QColor(Qt::darkGray);
                }
                else if(elemhdr->ElementType == ElementType::FieldArray)
                {
                    const FieldArray* fa = reinterpret_cast<const FieldArray*>(elemhdr);

                    if(fa->ItemType() == DataType::Character)
                        return QColor(Qt::darkGreen);
                    else if(fa->ItemType() == DataType::Blob)
                        return QColor(Qt::darkGray);
                }
                else if(elemhdr->ElementType == ElementType::BitField)
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
        ElementHeader* elementhdr = reinterpret_cast<ElementHeader*>(index.internalPointer());

        if((elementhdr->ElementType == ElementType::Field) && (index.column() == 3))
        {
            FieldHeader* fieldhdr = reinterpret_cast<FieldHeader*>(elementhdr);
            QString currvalue = QString::fromUtf8(fieldhdr->ElementHdr.DisplayValue());
            QString newvalue = value.toString();

            if(fieldhdr->Type == DataType::Character)
                currvalue = currvalue.mid(1, currvalue.length() - 2); /* Strip ' ' */

            if(!QString::compare(currvalue, newvalue, Qt::CaseInsensitive))  /* Does 'currvalue == newvalue' ? */
                return false;

            /* NOTE: !!!
            QByteArray newdata;

            bool valid = FormatModel::validateValue(value, field->dataType(), field->base(), this->_hexeditdata->endian(), newdata);

            if(valid)
            {
                this->_hexeditdata->replace(field->offset(), newdata.length(), newdata);
                return true;
            }
            */

            return false;
        }
    }

    return FieldDataModel::setData(index, value, role);
}

void FormatModel::fetchMore(const QModelIndex &parent)
{
    if(parent.isValid())
    {
        ElementHeader* elementhdr = reinterpret_cast<ElementHeader*>(parent.internalPointer());
        elementhdr->ParseChildren();
        return;
    }

    QAbstractItemModel::fetchMore(parent);
}

QModelIndex FormatModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!this->_formattree || !this->hasIndex(row, column, parent))
        return QModelIndex();

    const ElementHeader* element = nullptr;

    if(parent.isValid())
    {
        const ElementHeader* parentelement = reinterpret_cast<const ElementHeader*>(parent.internalPointer());
        element = this->_formattree->ElementFromPool(row, parentelement);
    }
    else
        element = this->_formattree->ElementFromPool(row, nullptr);

    if(!element)
        return QModelIndex();

    return this->createIndex(row, column, const_cast<ElementHeader*>(element));
}

QModelIndex FormatModel::parent(const QModelIndex &child) const
{
    if(!this->_formattree || !child.isValid())
        return QModelIndex();

    const ElementHeader* childelemhdr = reinterpret_cast<const ElementHeader*>(child.internalPointer());

    if(!childelemhdr->HasParent())
        return QModelIndex();

    QString parentid = QString::fromUtf8(childelemhdr->ParentId);
    const ElementHeader* parentelement = this->_formattree->ElementFromPoolById(parentid.toUtf8().constData());
    return this->createIndex(parentelement->IndexOf(childelemhdr), 0, const_cast<ElementHeader*>(parentelement));
}

bool FormatModel::canFetchMore(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        const ElementHeader* parentelemhdr = reinterpret_cast<const ElementHeader*>(parent.internalPointer());
        return parentelemhdr->IsDynamic();
    }

    return QAbstractItemModel::canFetchMore(parent);
}

bool FormatModel::hasChildren(const QModelIndex &parent) const
{
    if(!this->_formattree)
        return false;

    if(!parent.isValid())
        return this->_formattree->StructureCount() > 0;

    const ElementHeader* parentelemhdr = reinterpret_cast<const ElementHeader*>(parent.internalPointer());
    return parentelemhdr->HasChildren();
}

int FormatModel::rowCount(const QModelIndex &parent) const
{
    if(!this->_formattree)
        return 0;

    if(!parent.isValid())
        return this->_formattree->StructureCount();

    if(parent.column() == 0)
    {
        const ElementHeader* parentelemhdr = reinterpret_cast<const ElementHeader*>(parent.internalPointer());

        if(!parentelemhdr->IsDynamic())
        {
            if(parentelemhdr->ElementType == ElementType::Structure)
                return reinterpret_cast<const Structure*>(parentelemhdr)->FieldCount();
            else if(parentelemhdr->ElementType == ElementType::Field)
                return reinterpret_cast<const Field*>(parentelemhdr)->BitFieldCount();
            else if(parentelemhdr->ElementType == ElementType::FieldArray)
            {
                const FieldArray* fieldarray = reinterpret_cast<const FieldArray*>(parentelemhdr);

                if(fieldarray->ItemType() != DataType::Blob)
                    return reinterpret_cast<const FieldArray*>(parentelemhdr)->ItemCount();
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
    const ElementHeader* elemhdr = reinterpret_cast<const ElementHeader*>(index.internalPointer());

    if((index.column() == 3) && (elemhdr->ElementType == ElementType::Field))
        flags |= Qt::ItemIsEditable;

    return flags;
}
