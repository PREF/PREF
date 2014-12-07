#ifndef PREFSDK_PREFLIB_H
#define PREFSDK_PREFLIB_H

#include "qhexedit/qhexeditdata.h"
#include "views/abstractview.h"
#include "prefsdk/format/formatdefinition.h"
#include "prefsdk/exporter/exporterdefinition.h"
#include "prefsdk/disassembler/disassemblerdefinition.h"
#include "prefsdk/libs/qt/qtlua.h"
#include "prefsdk/prefexception.h"
#include "prefsdk/math.h"
#include "databuffer.h"
#include <QtCore>
#include <QtGui>
#include <QtQuick>
#include <QtQml>
#include <QMainWindow>
#include <QMessageBox>

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
            static LogWidget* getLogWidget();
            static int writeLog(lua_State* l, const QString& methodname);
            static bool validateArgs(lua_State* l, const QString& methodname);
            static QString buildString(lua_State* l);
            void buildByteOrder(lua_State* l);
            void buildDataType(lua_State* l);
            void buildMathTable(lua_State* l);
            void buildMessageBoxTable(lua_State* l);
            void buildExporterTable(lua_State* l);
            void buildFormatTable(lua_State* l);
            void buildDisassemblerTable(lua_State* l);

        private:
            void buildSegmentTypeTable(lua_State* l);
            void buildFunctionTypeTable(lua_State* l);
            void buildBlockTypeTable(lua_State* l);
            void buildSymbolTypeTable(lua_State* l);

        public:
            static void open(lua_State* l, QMainWindow *mainwindow, SdkVersion* sdkversion);
            static PrefLib* instance();

        /* Logger methods */
        private:
            static int logger_log(lua_State* l);
            static int logger_logline(lua_State* l);
            static int logger_notice(lua_State* l);
            static int logger_warning(lua_State* l);
            static int logger_error(lua_State* l);

        /* DataType Methods */
        private:
            static int dataType_sizeOf(lua_State* l);

        /* Math Methods */
        private:
            static int math_entropy(lua_State* l);

        /* MessageBox Methods */
        private:
            static int messageBox_show(lua_State* l);

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
            QMainWindow* _mainwindow;

        private:
            static PrefLib* _instance;
            static const char* PREF_TABLE;
    };
}

#endif // PREFSDK_PREF_H
