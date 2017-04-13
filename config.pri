INCLUDEPATH += $$PWD/PrefLib

unix:LIBS  += -L$$OUT_PWD/../PrefLib/ -lPrefLib
win32:LIBS += $$OUT_PWD/../PrefLib/release/PrefLib.lib

#win32: RC_FILE = resources.rc
