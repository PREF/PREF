INCLUDEPATH += $$PWD/PrefLib

unix|win32: LIBS += -L$$OUT_PWD/../PrefLib/ -lPrefLib

#win32: RC_FILE = resources.rc
