INCLUDEPATH += $$PWD/PrefLib
LIBS += -L$$OUT_PWD/../PrefLib/ -lPrefLib -lsqlite3 -llua

win32: RC_FILE = resources.rc
