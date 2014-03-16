#include "formatmodel.h"

FormatModel::FormatModel(lua_State* l, ByteBuffer *bytebuffer, QObject *parent): FieldDataModel(parent), LuaCTable(l, "FormatModel")
{
    this->_state = l;
    this->_bytebuffer = bytebuffer;

    this->_monospacefont.setFamily("Monospace");
    this->_monospacefont.setPointSize(qApp->font().pointSize());
    this->_monospacefont.setStyleHint(QFont::TypeWriter);

    /* Thread Safety: Don't use QPixmap */
    this->_icostruct.load(":/format_icons/res/struct.png");
    this->_icofield.load(":/format_icons/res/field.png");
    this->_icobitfield.load(":/format_icons/res/bitfield.png");

    connect(this->_bytebuffer->hexEditData(), SIGNAL(dataChanged(qint64,qint64,QHexEditData::ActionType)), this, SLOT(updateModelData(qint64,qint64,QHexEditData::ActionType)));

    this->exportMethod<LuaTable::Ptr, FormatModel, lua_String>("addStructure", &FormatModel::luaAddStructure);
    this->exportMethod<LuaTable::Ptr, FormatModel, lua_String, lua_Integer>("addStructure", &FormatModel::luaAddStructure);
    this->exportMethod<LuaTable::Ptr, FormatModel, lua_String>("find", &FormatModel::luaFind);
    this->exportMethod<LuaTable::Ptr, FormatModel, lua_Integer>("structure", &FormatModel::luaStructure);
    this->exportMethod<lua_Integer, FormatModel>("structureCount", &FormatModel::structureCount);
}

void FormatModel::clearItems()
{
    this->_structuremap.clear();
}

Structure *FormatModel::addStructure(const QString &name)
{
    if(this->_structuremap.empty())
        return this->addStructure(name, this->_bytebuffer->baseOffset());

    Structure* s = this->_structuremap[this->_offsetlist.last()];
    return this->addStructure(name, s->endOffset());
}

Structure *FormatModel::addStructure(const QString &name, lua_Integer offset)
{
    Structure* s = new Structure(this->_state, offset, name, this->_bytebuffer, this);

    this->_offsetlist.append(offset);
    this->_structuremap[offset] = s;
    this->_stringmap[name] = s;

    qSort(this->_offsetlist.begin(), this->_offsetlist.end(), qLess<lua_Integer>());
    return s;
}

ByteBuffer* FormatModel::byteBuffer()
{
    return this->_bytebuffer;
}

LuaTable::Ptr FormatModel::luaAddStructure(lua_String name)
{
    return *(this->addStructure(QString::fromLatin1(name)));
}

LuaTable::Ptr FormatModel::luaAddStructure(lua_String name, lua_Integer offset)
{
     return *(this->addStructure(QString::fromLatin1(name), offset));
}

LuaTable::Ptr FormatModel::luaStructure(lua_Integer i)
{
    return *(this->structure(i - 1));
}

LuaTable::Ptr FormatModel::luaFind(lua_String name)
{
    Structure* s = this->find(QString::fromLatin1(name));

    if(s)
        return *s;

    throw LuaException("Structure not Found!");
}

void FormatModel::metaIndex(lua_State *l)
{
    if(luaT_typeof(l, 2) == LuaTypes::String)
    {
        lua_String luakey;
        luaT_getvalue(l, 2, luakey);
        QString key = QString::fromLatin1(luakey);

        if(this->_stringmap.contains(key))
        {
            FormatObject* fo = this->_stringmap[key];
            fo->push();
            return;
        }
    }

    LuaCTable::metaIndex(l);
}

void FormatModel::updateModelData(qint64 offset, qint64 length, QHexEditData::ActionType)
{
    qint64 startoffset = offset;
    qint64 endoffset = offset + length;

    for(int i = 0; i < this->_offsetlist.length(); i++)
    {
        lua_Integer o = this->_offsetlist[i];
        Structure* s = this->_structuremap[o];

        if(s->containsOffset(startoffset))
        {
            QModelIndex midx = this->index(i, 0);
            emit dataChanged(midx, this->index(midx.row(), this->columnCount() - 1, midx.parent()));

            if(s->containsOffset(endoffset))
                break;
        }
    }
}

Structure* FormatModel::structure(int i)
{
    lua_Integer offset = this->_offsetlist[i];
    return this->_structuremap[offset];
}

Structure *FormatModel::find(const QString &name)
{
    if(this->_stringmap.contains(name))
        return this->_stringmap[name];

    return nullptr;
}

lua_Integer FormatModel::structureCount()
{
    return this->_offsetlist.length();
}

void FormatModel::updateModelData(FormatObject* sender)
{
    Structure* s = nullptr;

    if(sender->objectType() == FormatObject::StructureType)
        s = dynamic_cast<Structure*>(sender);
    else if((sender->objectType() == FormatObject::FieldArrayType) || ((sender->objectType() == FormatObject::FieldType) && (sender->parentObject()->objectType() == FormatObject::StructureType)))
        s = dynamic_cast<Structure*>(sender->parentObject());
    else if((sender->objectType() == FormatObject::FieldType) && (sender->parentObject()->objectType() == FormatObject::FieldArrayType))
        s = dynamic_cast<Structure*>(sender->parentObject()->parentObject());

    for(int i = 0; i < this->_offsetlist.length(); i++)
    {
        lua_Integer o = this->_offsetlist[i];
        Structure* fs = this->_structuremap[o];

        if(fs == s)
        {
            QModelIndex smidx = this->index(i, 0);

            for(int j = 0; j < s->fieldCount(); j++)
            {
                FormatObject* f = s->field(j);

                if(f->objectType() == FormatObject::FieldArrayType)
                {
                    QModelIndex fmidx = this->index(j, 0, smidx);
                    emit dataChanged(fmidx.child(0, 0), fmidx.child(reinterpret_cast<FieldArray*>(f)->elementCount() - 1, this->columnCount() - 1));
                }
            }

            emit dataChanged(smidx.child(0, 0), smidx.child(s->fieldCount() - 1, this->columnCount() - 1));
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
    if(!index.isValid())
        return QVariant();

    FormatObject* fo = (FormatObject*)index.internalPointer();

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(role == Qt::DisplayRole)
        {
            if(index.column() == 0)
                return QString("%1").arg(fo->offset(), 8, 16, QLatin1Char('0')).toUpper();
            else if(index.column() == 1)
                return fo->typeName();
            else if(index.column() == 2)
                return fo->displayName();
            else if(index.column() == 4)
                return fo->info();
        }

        if(index.column() == 3 && ((fo->objectType() == FormatObject::FieldType || fo->objectType() == FormatObject::FieldArrayType) || (fo->objectType() == FormatObject::BitFieldType)))
            return fo->displayValue();
    }
    else if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
        {
            if(fo->objectType() == FormatObject::StructureType)
                return this->_icostruct;
            if((fo->objectType() == FormatObject::FieldType) || (fo->objectType() == FormatObject::FieldArrayType))
                return this->_icofield;
            else if(fo->objectType() == FormatObject::BitFieldType)
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
        else if(((index.column() == 3) && ((fo->objectType() == FormatObject::FieldType) || (fo->objectType() == FormatObject::FieldArrayType) || (fo->objectType() == FormatObject::BitFieldType))) || (index.column() == 4))
        {
            if(index.column() == 4)
                return QColor(Qt::darkGreen);

            if(index.column() == 3)
            {
                if(fo->objectType() == FormatObject::FieldType)
                {
                    Field* f = dynamic_cast<Field*>(fo);

                    if(f->isOverflowed())
                        return QColor(Qt::red);

                    if(f->isInteger())
                        return QColor(Qt::darkBlue);

                    if(f->dataType() == DataType::Char)
                        return QColor(Qt::darkGreen);

                    else if(f->dataType() == DataType::Blob)
                        return QColor(Qt::darkGray);
                }
                else if(fo->objectType() == FormatObject::FieldArrayType)
                {
                    FieldArray* fa = dynamic_cast<FieldArray*>(fo);

                    if(fa->itemDataType() == DataType::Char)
                        return QColor(Qt::darkGreen);
                    else if(fa->itemDataType() == DataType::Blob)
                        return QColor(Qt::darkGray);
                }
                else if(fo->objectType() == FormatObject::BitFieldType)
                    return QColor(Qt::darkRed);
            }
        }
    }

    return QVariant();
}

bool FormatModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole && index.isValid())
    {
        FormatObject* formatobj = (FormatObject*)index.internalPointer();

        if((formatobj->objectType() == FormatObject::FieldType) && (index.column() == 3))
        {
            Field* field = dynamic_cast<Field*>(formatobj);
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
    if(!this->hasIndex(row, column, parent))
        return QModelIndex();

    if(!parent.isValid())
        return this->createIndex(row, column, this->_structuremap[this->_offsetlist[row]]);

    FormatObject* parentobj = reinterpret_cast<FormatObject*>(parent.internalPointer());

    if(parentobj->objectType() == FormatObject::StructureType)
    {
        Structure* parentstruct = dynamic_cast<Structure*>(parentobj);
        return this->createIndex(row, column, parentstruct->field(row));
    }
    else if(parentobj->objectType() == FormatObject::FieldArrayType)
    {
        FieldArray* parentfieldarray = dynamic_cast<FieldArray*>(parentobj);
        return this->createIndex(row, column, parentfieldarray->element(row));
    }
    else if(parentobj->objectType() == FormatObject::FieldType)
    {
        Field* parentfield = dynamic_cast<Field*>(parentobj);
        return this->createIndex(row, column, parentfield->bitField(row));
    }

    return QModelIndex();
}

QModelIndex FormatModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return QModelIndex();

    FormatObject* childobj = reinterpret_cast<FormatObject*>(child.internalPointer());

    if(childobj->objectType() == FormatObject::StructureType)
    {
        if(childobj->parentObject() == nullptr)
            return QModelIndex();

        Structure* parentstruct = dynamic_cast<Structure*>(childobj->parentObject());

        if(parentstruct->parentObject())
        {
            Structure* parentofparent = dynamic_cast<Structure*>(parentstruct->parentObject());
            return this->createIndex(parentofparent->indexOf(parentstruct), 0, parentstruct);
        }
        else
            return this->createIndex(this->_offsetlist.indexOf(parentstruct->offset()), 0, parentstruct);
    }
    else if(childobj->objectType() == FormatObject::FieldType || childobj->objectType() == FormatObject::FieldArrayType)
    {
        FormatObject* parentobj = dynamic_cast<FormatObject*>(childobj->parentObject());

        if(parentobj->objectType() == FormatObject::StructureType)
        {
            if(parentobj->parentObject())
            {
                Structure* parentofparent = dynamic_cast<Structure*>(parentobj->parentObject());
                return this->createIndex(parentofparent->indexOf(parentobj), 0, parentobj);
            }
            else
                return this->createIndex(this->_offsetlist.indexOf(parentobj->offset()), 0, parentobj);
        }

        if(childobj->objectType() == FormatObject::FieldType && parentobj->objectType() == FormatObject::FieldArrayType)
        {
            FieldArray* parentfieldarray = (FieldArray*)parentobj;
            Structure* parentstruct = (Structure*)parentfieldarray->parentObject();
            return this->createIndex(parentstruct->indexOf(parentfieldarray), 0, parentfieldarray);
        }
    }
    else if(childobj->objectType() == FormatObject::BitFieldType)
    {
        Field* parentfield = (Field*)childobj->parentObject();
        FormatObject* parentofparent = parentfield->parentObject();

        if(parentofparent->objectType() == FormatObject::StructureType)
        {
            Structure* s = (Structure*)parentofparent;
            return this->createIndex(s->indexOf(parentfield), 0, parentfield);
        }
        else if(parentofparent->objectType() == FormatObject::FieldArrayType)
        {
            Structure* s = (Structure*)parentofparent->parentObject();
            return this->createIndex(s->indexOf(parentfield), 0, parentfield);
        }
    }

    return QModelIndex();
}

int FormatModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return this->_offsetlist.length();

    if(parent.column() == 0)
    {
        FormatObject* parentfo = (FormatObject*)parent.internalPointer();

        if(parentfo->objectType() == FormatObject::StructureType)
        {
            Structure* parentstruct = (Structure*)parentfo;
            return parentstruct->fieldCount();
        }
        else if(parentfo->objectType() == FormatObject::FieldArrayType)
        {
            FieldArray* parentfieldarray = (FieldArray*)parentfo;
            return parentfieldarray->elementCount();
        }
        else if(parentfo->objectType() == FormatObject::FieldType)
        {
            Field* parentfield = (Field*)parentfo;
            return parentfield-> bitFieldCount();
        }
    }

    return 0;
}

Qt::ItemFlags FormatModel::flags(const QModelIndex &index) const
{    
    if(!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    FormatObject* formatobj = (FormatObject*)index.internalPointer();

    if((index.column() == 3) && (formatobj->objectType() == FormatObject::FieldType))
        flags |= Qt::ItemIsEditable;

    return flags;
}
