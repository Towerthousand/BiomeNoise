QT       -= core gui

TARGET = VBE-Scenegraph
TEMPLATE = lib
CONFIG += staticlib

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += include src

include(../VBE/VBE.pri)

LIBS += -lGLEW -lGL -lSDL2
QMAKE_CXXFLAGS += -std=c++0x -fno-exceptions

OTHER_FILES += \
    VBE-Scenegraph.pri

HEADERS += \
    include/VBE-Scenegraph/scenegraph.hpp \
    include/VBE-Scenegraph/VBE-Scenegraph.hpp \
    include/VBE-Scenegraph/scenegraph/Camera.hpp \
    include/VBE-Scenegraph/scenegraph/ContainerObject.hpp \
    include/VBE-Scenegraph/scenegraph/Game.hpp \
    include/VBE-Scenegraph/scenegraph/GameObject.hpp

SOURCES += \
    src/VBE-Scenegraph/scenegraph/Camera.cpp \
    src/VBE-Scenegraph/scenegraph/ContainerObject.cpp \
    src/VBE-Scenegraph/scenegraph/Game.cpp \
    src/VBE-Scenegraph/scenegraph/GameObject.cpp
