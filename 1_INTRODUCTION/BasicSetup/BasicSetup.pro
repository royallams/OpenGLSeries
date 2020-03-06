TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp


win32 {
unix:!macx: LIBS += -L$$PWD/../../LIBRARIES/WINDOWS/glew-2.1.0/lib/ -lGLEW

INCLUDEPATH += $$PWD/../../LIBRARIES/WINDOWS/glew-2.1.0/include
DEPENDPATH += $$PWD/../../LIBRARIES/WINDOWS/glew-2.1.0/include


#Graphics library Frame work :- This helps to create windows or context where we can draw or render stuffs.
unix:!macx: LIBS += -L$$PWD/../../LIBRARIES/WINDOWS/glfw-3.3.2/BUILD/src/ -lglfw -lGL

INCLUDEPATH += $$PWD/../../LIBRARIES/WINDOWS/glfw-3.3.2/include
DEPENDPATH += $$PWD/../../LIBRARIES/WINDOWS/glfw-3.3.2/include

#Open GL Math Library :- This will largely help us to do graphics related mathematics quick and easy . In this case for transformation.
INCLUDEPATH += $$PWD/../../LIBRARIES/WINDOWS/glm-0.9.9.0
DEPENDPATH += $$PWD/../../LIBRARIES/WINDOWS/glm-0.9.9.0
}

unix{
#Open GL Wrangler :- This helps to support opengl as cross platform. OS dependent opengl calls handling..

unix:!macx: LIBS += -L$$PWD/../../LIBRARIES/LINUX/glew-2.1.0/lib/ -lGLEW

INCLUDEPATH += $$PWD/../../LIBRARIES/LINUX/glew-2.1.0/include
DEPENDPATH += $$PWD/../../LIBRARIES/LINUX/glew-2.1.0/include


#Graphics library Frame work :- This helps to create windows or context where we can draw or render stuffs.
unix:!macx: LIBS += -L$$PWD/../../LIBRARIES/LINUX/glfw-3.3.2/BUILD/src/ -lglfw -lGL

INCLUDEPATH += $$PWD/../../LIBRARIES/LINUX/glfw-3.3.2/include
DEPENDPATH += $$PWD/../../LIBRARIES/LINUX/glfw-3.3.2/include

#Open GL Math Library :- This will largely help us to do graphics related mathematics quick and easy . In this case for transformation.
INCLUDEPATH += $$PWD/../../LIBRARIES/LINUX/glm-0.9.9.0
DEPENDPATH += $$PWD/../../LIBRARIES/LINUX/glm-0.9.9.0

}



