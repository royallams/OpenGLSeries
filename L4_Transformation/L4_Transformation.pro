TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp


#Open GL Wrangler :- This helps to support opengl as cross platform. OS dependent opengl calls handling..
unix:!macx: LIBS += -L$$PWD/../glew-2.1.0/lib/ -lGLEW

INCLUDEPATH += $$PWD/../glew-2.1.0/include
DEPENDPATH += $$PWD/../glew-2.1.0/include


#Graphics library Frame work :- This helps to create windows or context where we can draw or render stuffs.
unix:!macx: LIBS += -L$$PWD/../glfw-3.3.2/BUILD/src/ -lglfw -lGL

INCLUDEPATH += $$PWD/../glfw-3.3.2/include
DEPENDPATH += $$PWD/../glfw-3.3.2/include

#Open GL Math Library :- This will largely help us to do graphics related mathematics quick and easy . In this case for transformation.
INCLUDEPATH += $$PWD/../glm-0.9.9.0
DEPENDPATH += $$PWD/../glm-0.9.9.0


