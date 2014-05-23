#ifndef PREFSDK_API_FORMATAPI_H
#define PREFSDK_API_FORMATAPI_H

#include "qhexedit/qhexeditdata.h"
#include "prefsdk/format/formatlist.h"
#include "prefsdk/format/formattree.h"

namespace PrefSDK
{
    namespace API
    {
        extern "C"
        {
            void Format_register(const char* name, const char* category, const char* author, const char* version, FormatList::FormatId formatid);
            void Format_registerOption(QHexEditData *hexeditdata, int optionidx, const char* name);

            bool Format_checkUInt8(QHexEditDataReader* reader, uint64_t offset,  uint8_t value);
            bool Format_checkUInt16(QHexEditDataReader* reader, uint64_t offset, uint16_t value, int byteorder);
            bool Format_checkUInt32(QHexEditDataReader* reader, uint64_t offset, uint32_t value, int byteorder);
            bool Format_checkUInt64(QHexEditDataReader* reader, uint64_t offset, uint64_t value, int byteorder);
            bool Format_checkInt8(QHexEditDataReader* reader, uint64_t offset, int8_t value);
            bool Format_checkInt16(QHexEditDataReader* reader, uint64_t offset, int16_t value, int byteorder);
            bool Format_checkInt32(QHexEditDataReader* reader, uint64_t offset, int32_t value, int byteorder);
            bool Format_checkInt64(QHexEditDataReader* hexeditdata, uint64_t offset, int64_t value, int byteorder);
            bool Format_checkAsciiString(QHexEditDataReader *reader, uint64_t offset, const char* value);

            Structure* FormatTree_addStructure(FormatTree* __this, const char* name);
            Structure* FormatTree_insertStructure(FormatTree* __this, const char* name, int64_t offset);
            Structure* FormatTree_getStructure(FormatTree* __this, uint64_t i);
            uint64_t FormatTree_getStructureCount(FormatTree* __this);

            const char* FormatElement_getId(FormatElement* __this);
            uint64_t FormatElement_getOffset(FormatElement* __this);
            uint64_t FormatElement_getEndOffset(FormatElement* __this);
            uint64_t FormatElement_getSize(FormatElement* __this);
            int FormatElement_getBase(FormatElement* __this);
            void FormatElement_setBase(FormatElement* __this, int b);
            void FormatElement_setDynamic(FormatElement* __this, bool b);

            int FieldElement_getDataType(FieldElement* __this);
            bool FieldElement_isSigned(FieldElement* __this);
            bool FieldElement_isInteger(FieldElement* __this);
            bool FieldElement_isOverflowed(FieldElement* __this);

            int FieldArray_getItemType(FieldArray* __this);
            int FieldArray_getItemCount(FieldArray* __this);

            BitField* Field_setBitField(Field* __this, const char* name, int bitstart, int bitend);
            int Field_getBitFieldCount(Field* __this);

            uint64_t BitField_getMask(BitField* __this);
            int BitField_getBitStart(BitField* __this);
            int BitField_getBitEnd(BitField* __this);

            Structure *Structure_addStructure(Structure* __this, const char* name);
            FieldElement* Structure_addField(Structure* __this, int datatype, const char* name, uint64_t count);
            uint64_t Structure_getFieldCount(Structure* __this);
        }
    }
}

#endif // PREFSDK_API_FORMATAPI_H
