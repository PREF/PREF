#ifndef PREFSDK_FORMATTREE_H
#define PREFSDK_FORMATTREE_H

#include "prefsdk/qlua.h"
#include "prefsdk/prefdebug.h"
#include "elements.h"

namespace PrefSDK
{
    typedef uint64_t (*StructureCountProc)();
    typedef const Structure* (*GetStructureProc)(uint64_t i);
    typedef const ElementHeader* (*ElementFromPoolProc)(int64_t i, const ElementHeader* parent);
    typedef const ElementHeader* (*ElementFromPoolByIdProc)(const char* id);

    typedef struct FormatTree
    {
        StructureCountProc StructureCount;
        GetStructureProc GetStructure;
        ElementFromPoolByIdProc ElementFromPoolById;
        ElementFromPoolProc ElementFromPool;
    } FormatTree;
}

#endif // PREFSDK_FORMATTREE_H
