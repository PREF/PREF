unix|win32: LIBS += -L$$OUT_PWD/../PrefLib/ -lPrefLib

INCLUDEPATH += $$PWD/PrefLib

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../PrefLib/PrefLib.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../PrefLib/libPrefLib.a

#win32: RC_FILE = resources.rc
