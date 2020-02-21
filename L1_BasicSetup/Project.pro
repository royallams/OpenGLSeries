TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp


unix:!macx: LIBS += -L$$PWD/../glew-2.1.0/lib/ -lGLEW

INCLUDEPATH += $$PWD/../glew-2.1.0/include
DEPENDPATH += $$PWD/../glew-2.1.0/include


unix:!macx: LIBS += -L$$PWD/../glfw-3.3.2/BUILD/src/ -lglfw -lGL

INCLUDEPATH += $$PWD/../glfw-3.3.2/include
DEPENDPATH += $$PWD/../glfw-3.3.2/include
