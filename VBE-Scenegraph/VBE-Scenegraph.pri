INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

LIBS += -lVBE-Scenegraph

# This is needed so the game is recompiled every time
# we change something in VBE-Scenegraph
PRE_TARGETDEPS += ../VBE-Scenegraph/libVBE-Scenegraph.a

win32 {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../VBE-Scenegraph/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../VBE-Scenegraph/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../VBE-Scenegraph/release/VBE-Scenegraph.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../VBE-Scenegraph/debug/VBE-Scenegraph.lib
}

unix {
        LIBS += -L$$OUT_PWD/../VBE-Scenegraph/
        PRE_TARGETDEPS += $$OUT_PWD/../VBE-Scenegraph/libVBE-Scenegraph.a
}
