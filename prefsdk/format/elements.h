#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <QtCore>

namespace PrefSDK
{
    namespace ElementType
    {
        enum Type
        {
            Invalid    = 0,
            Structure  = 1,
            Field      = 2,
            FieldArray = 3,
            BitField   = 4,
        };
    }

    struct ElementHeader;

    typedef bool (*BoolGetPropertyProc)();
    typedef int (*IntegerGetPropertyProc)();
    typedef uint64_t (*OffsetGetPropertyProc)();
    typedef const char* (*StringGetPropertyProc)();
    typedef const struct ElementHeader* (*GetParentElementProc)();

    typedef void (*BoolSetPropertyProc)(bool);

    typedef int64_t (*IndexOfProc)(const struct ElementHeader* elementheader);
    typedef bool (*ContainsOffsetProc)(uint64_t);
    typedef void (*ParseChildrenProc)();
    typedef const char* (*GetIdProc)(uint64_t);

    typedef struct ElementHeader
    {
        const char* Name;
        const char* Id;
        const char* ParentId;

        int32_t ElementType;

        BoolGetPropertyProc IsDynamic;
        BoolGetPropertyProc HasChildren;
        BoolGetPropertyProc HasParent;

        IntegerGetPropertyProc Base;
        IntegerGetPropertyProc Size;

        OffsetGetPropertyProc Offset;
        OffsetGetPropertyProc EndOffset;

        StringGetPropertyProc DisplayType;
        StringGetPropertyProc DisplayName;
        StringGetPropertyProc DisplayValue;
        StringGetPropertyProc Info;

        GetParentElementProc ParentElement;
        BoolSetPropertyProc SetBase;

        IndexOfProc IndexOf;
        ContainsOffsetProc ContainsOffset;
        ParseChildrenProc ParseChildren;

    } ElementHeader;

    typedef struct FieldHeader
    {
        ElementHeader ElementHdr;

        int32_t Type;

        BoolGetPropertyProc IsSigned;
        BoolGetPropertyProc IsInteger;
        BoolGetPropertyProc IsOverflowed;
    } FieldHeader;

    typedef struct BitField
    {
        FieldHeader FieldHdr;

        int32_t BitStart;
        int32_t BitEnd;
    } BitField;

    typedef struct Field
    {
        FieldHeader FieldHdr;

        IntegerGetPropertyProc BitFieldCount;
        GetIdProc BitFieldId;
    } Field;

    typedef struct FieldArray
    {
        FieldHeader FieldHdr;

        IntegerGetPropertyProc ItemType;
        IntegerGetPropertyProc ItemCount;

        GetIdProc ItemId;
    } FieldArray;

    typedef struct Structure
    {
        ElementHeader ElementHdr;

        IntegerGetPropertyProc FieldCount;
        GetIdProc FieldId;

    } Structure;
}

#endif // ELEMENTS_H
