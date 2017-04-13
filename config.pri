INCLUDEPATH += $$PWD/PrefLib

unix: LIBS += -L$$OUT_PWD/../PrefLib/ -lPrefLib
win32: LIBS += -L$$OUT_PWD/../PrefLib/release -lPrefLib

#win32: RC_FILE = resources.rc
