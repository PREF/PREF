#ifndef PREFSDK_FORMATDEFINITION_H
#define PREFSDK_FORMATDEFINITION_H

#include <utility>
#include <QtCore>
#include "prefsdk/io/bytebuffer.h"
#include "prefsdk/prefdebug.h"
#include "prefsdk/disassembler/disassemblerloader.h"
#include "viewmodels/formatmodel/formatmodel.h"

namespace PrefSDK
{
    using namespace Lua;

    typedef bool (*ValidateProc)(QHexEditData* hexeditdata, int64_t baseoffset);
    typedef FormatTree* (*ParseProc)(FormatModel* formatmodel, QHexEditData* hexeditdata, int64_t baseoffset);
    typedef const char* (*OptionNameProc)(int i);
    typedef int64_t (*OptionCountProc)();
    typedef void (*ExecuteOptionProc)(int optindex, QHexEditData* hexeditdata);

    typedef struct FormatDefinition
    {
      const char* Name;
      const char* Category;
      const char* Author;
      const char* Version;
      int Endian;

      ValidateProc ValidateProcedure;
      ParseProc ParseProcedure;
      OptionNameProc OptionName;
      OptionCountProc OptionCount;
      ExecuteOptionProc ExecuteOption;

    } FormatDefinition;
}

#endif // PREFSDK_FORMATDEFINITION_H
