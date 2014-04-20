#include "formatapi.h"

namespace PrefSDK
{
    namespace API
    {
        void Format_register(const char* name, const char* category, const char* author, const char* version, FormatList::FormatId formatid)
        {
            FormatList::registerFormat(QString::fromUtf8(name), QString::fromUtf8(category), QString::fromUtf8(author), QString::fromUtf8(version), formatid);
        }

        void Format_registerOption(FormatList::FormatId formatid, int optionidx, const char *name, const char *description)
        {
            FormatList::registerOption(formatid, optionidx, QString::fromUtf8(name), QString::fromUtf8(description));
        }

        void Format_enableDisassembler(FormatList::FormatId formatid)
        {
            FormatList::Format& format = FormatList::formatFromId(formatid);
            format.enableDisassembler();
        }

        bool Format_checkUInt8(QHexEditData* hexeditdata, uint64_t offset, uint8_t value)
        {
            uint8_t dataval = static_cast<uint8_t>(hexeditdata->at(offset));
            return value == dataval;
        }

        bool Format_checkUInt16(QHexEditData* hexeditdata, uint64_t offset, uint16_t value, int byteorder)
        {
            uint16_t dataval = hexeditdata->readUInt16(offset, static_cast<QSysInfo::Endian>(byteorder));
            return value == dataval;
        }

        bool Format_checkUInt32(QHexEditData* hexeditdata, uint64_t offset, uint32_t value, int byteorder)
        {
            uint32_t dataval = hexeditdata->readUInt32(offset, static_cast<QSysInfo::Endian>(byteorder));
            return value == dataval;
        }

        bool Format_checkUInt64(QHexEditData* hexeditdata, uint64_t offset, uint64_t value, int byteorder)
        {
            uint64_t dataval = hexeditdata->readUInt64(offset, static_cast<QSysInfo::Endian>(byteorder));
            return value == dataval;
        }

        bool Format_checkInt8(QHexEditData* hexeditdata, uint64_t offset, int8_t value)
        {
            int8_t dataval = static_cast<int8_t>(hexeditdata->at(offset));
            return value == dataval;
        }

        bool Format_checkInt16(QHexEditData* hexeditdata, uint64_t offset, int16_t value, int byteorder)
        {
            int16_t dataval = hexeditdata->readInt16(offset, static_cast<QSysInfo::Endian>(byteorder));
            return value == dataval;
        }

        bool Format_checkInt32(QHexEditData* hexeditdata, uint64_t offset, int32_t value, int byteorder)
        {
            int32_t dataval = hexeditdata->readInt32(offset, static_cast<QSysInfo::Endian>(byteorder));
            return value == dataval;
        }

        bool Format_checkInt64(QHexEditData* hexeditdata, uint64_t offset, int64_t value, int byteorder)
        {
            int64_t dataval = hexeditdata->readInt64(offset, static_cast<QSysInfo::Endian>(byteorder));
            return value == dataval;
        }

        bool Format_checkAsciiString(QHexEditData *hexeditdata, uint64_t offset, const char *value)
        {
            /* Test First Byte */
            uint8_t b = hexeditdata->at(offset);

            if(b == static_cast<uint8_t>(value[0])) /* The first byte is equal, check entire string */
            {
                QString s = QString::fromUtf8(value);
                QByteArray dataval = hexeditdata->read(offset, s.length());
                return QString::compare(s, QString(dataval), Qt::CaseSensitive) == 0;
            }

            return false;
        }

        Structure *FormatTree_addStructure(FormatTree *__this, const char *name)
        {
            return __this->addStructure(QString::fromUtf8(name));
        }

        Structure *FormatTree_addStructureAtOffset(FormatTree *__this, const char *name, uint64_t offset)
        {
            return __this->addStructure(QString::fromUtf8(name), offset);
        }

        Structure *FormatTree_getStructure(FormatTree *__this, uint64_t i)
        {
            return __this->structure(i);
        }

        uint64_t FormatTree_structureCount(FormatTree *__this)
        {
            return __this->structureCount();
        }

        uint64_t FormatElement_getOffset(FormatElement *__this)
        {
            return __this->offset();
        }

        uint64_t FormatElement_getEndOffset(FormatElement *__this)
        {
            return __this->endOffset();
        }

        uint64_t FormatElement_getSize(FormatElement *__this)
        {
            return __this->size();
        }

        int FormatElement_getBase(FormatElement *__this)
        {
            return __this->base();
        }

        void FormatElement_setBase(FormatElement *__this, int b)
        {
            __this->setBase(b);
        }

        int FieldElement_getDataType(FieldElement *__this)
        {
            return __this->dataType();
        }

        bool FieldElement_isSigned(FieldElement *__this)
        {
            return __this->isSigned();
        }

        bool FieldElement_isInteger(FieldElement *__this)
        {
            return __this->isInteger();
        }

        bool FieldElement_isOverflowed(FieldElement *__this)
        {
            return __this->isOverflowed();
        }

        Structure *Structure_addStructure(Structure *__this, const char *name)
        {
            return __this->addStructure(QString::fromUtf8(name));
        }

        FieldElement *Structure_addField(Structure* __this, int datatype, const char *name, uint64_t count)
        {
            return __this->addField(static_cast<DataType::Type>(datatype), QString::fromUtf8(name), count);
        }

        uint64_t Structure_getFieldCount(Structure* __this)
        {
            return __this->fieldCount();
        }

        int FieldArray_getItemType(FieldArray *__this)
        {
            return __this->itemType();
        }

        int FieldArray_itemCount(FieldArray *__this)
        {
            return __this->itemCount();
        }

        BitField *Field_setBitField(Field *__this, const char *name, int bitstart, int bitend)
        {
            return __this->setBitField(QString::fromUtf8(name), bitstart, bitend);
        }

        int Field_getBitFieldCount(Field *__this)
        {
            return __this->bitFieldCount();
        }

        int BitField_getBitStart(BitField *__this)
        {
            return __this->bitStart();
        }

        int BitField_getBitEnd(BitField *__this)
        {
            return __this->bitEnd();
        }

        uint64_t BitField_getMask(BitField *__this)
        {
            return __this->mask();
        }

        const char *FormatElement_getId(FormatElement *__this)
        {
            return __this->id().toString().toUtf8().constData();
        }

        void FormatElement_setDynamic(FormatElement *__this, bool b)
        {
            __this->setDynamic(b);
        }
    }
}
