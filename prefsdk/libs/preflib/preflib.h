#ifndef PREFSDK_PREFLIB_H
#define PREFSDK_PREFLIB_H

#include "qhexedit/qhexeditdata.h"
#include "prefsdk/format/formatdefinition.h"
#include "prefsdk/exporter/exporterdefinition.h"
#include "prefsdk/disassembler/disassemblerdefinition.h"
#include "prefsdk/disassembler/instruction/instructionset.h"
#include "prefsdk/disassembler/register/registerset.h"
#include "prefsdk/disassembler/references/reference.h"
#include "prefsdk/libs/qt/qtlua.h"
#include "prefsdk/prefexception.h"
#include "prefsdk/math.h"
#include "databuffer.h"
#include <QtCore>
#include <QtGui>
#include <QtQuick>
#include <QtQml>

namespace PrefSDK
{
    class PrefLib: public QObject
    {
        Q_OBJECT

        public:
            struct SdkVersion { bool IsLoaded; lua_Integer Major; lua_Integer Minor; lua_Integer Revision; QString Custom; };

        private:
            explicit PrefLib(QObject* parent = 0);
            static void registerPrefTypes();
            void buildByteOrder(lua_State* l);
            void buildDataType(lua_State* l);
            void buildMathTable(lua_State* l);
            void buildExporterTable(lua_State* l);
            void buildFormatTable(lua_State* l);
            void buildDisassemblerTable(lua_State* l);

        private:
            void buildSegmentTable(lua_State* l);
            void buildFunctionTypeTable(lua_State* l);
            void buildReferenceTypeTable(lua_State* l);
            void buildBlockTypeTable(lua_State* l);
            void buildSymbolTypeTable(lua_State* l);

        public:
            static void open(lua_State* l, SdkVersion* sdkversion);
            static PrefLib* instance();

        /* DataType Methods */
        private:
            static int dataType_sizeOf(lua_State* l);

        /* Math Methods */
        private:
            static int math_entropy(lua_State* l);

        /* Exporter Methods */
        private:
            static int exporter_create(lua_State* l);

        /* Format Methods */
        private:
            static int format_create(lua_State* l);
            static int format_loadView(lua_State* l);

        /* Disassembler Methods */
        private:
            static int disassembler_create(lua_State* l);

        private:
            static int setSdkVersion(lua_State* l);

        private:
            lua_State* _state;
            QVector<luaL_Reg> _methods;

        private:
            SdkVersion* _sdkversion;

        private:
            static PrefLib* _instance;
            static const char* PREF_TABLE;
            static const char* FORMAT_TABLE;
    };
}

#endif // PREFSDK_PREF_H
