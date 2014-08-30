#ifndef PREFSDK_PREFLIB_H
#define PREFSDK_PREFLIB_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "qhexedit/qhexeditdata.h"
#include "views/hexview/hexview.h"
#include "views/disassemblerview/disassemblerview.h"
#include "views/compareview/compareview.h"
#include "prefsdk/disassembler/loader/loaderlist.h"
#include "lua.hpp"

namespace PrefSDK
{
    class PrefLib: public QObject
    {
        Q_OBJECT

        private:
            struct SdkVersion { bool IsLoaded; lua_Integer Major; lua_Integer Minor; lua_Integer Revision; QString Custom; };
            typedef QHash<QHexEditData*, AbstractView*> LoadedViews;

        private:
            explicit PrefLib(QObject* parent = 0);

        public:
            static void open(lua_State* l);
            static PrefLib* instance();
            QString sdkVersion();
            HexView* addHexView(QHexEditData* hexeditdata, const QString& name, QLabel* lblinfo, QWidget *parent);
            DisassemblerView* addDisassemblerView(LoaderList::LoaderId selectedloader, QHexEditData* hexeditdata, const QString& name, QLabel* lblinfo, QWidget *parent);
            CompareView* addCompareView(const QString& leftcompare, const QString& rightcompare, const QString& name, QLabel* lblinfo, QWidget* parent);

        private slots:
            void onViewDestroyed(QObject *obj);

        private:
            static int setSdkVersion(lua_State* l);

        private:
            lua_State* _state;
            QVector<luaL_Reg> _methods;

        private:
            SdkVersion _sdkversion;
            LoadedViews _loadedviews;

        private:
            static PrefLib* _instance;
    };
}

#endif // PREFSDK_PREF_H
