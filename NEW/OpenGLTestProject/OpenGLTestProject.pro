TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        glad.c \
        main.cpp




INCLUDEPATH += $$PWD/../Libraries/GLAD/include
DEPENDPATH += $$PWD/../Libraries/GLAD/include

LIBS += -ldl

unix:!macx: LIBS += -L$$PWD/../Libraries/GLFW/build/src/ -lglfw

INCLUDEPATH += $$PWD/../Libraries/GLFW/include
DEPENDPATH += $$PWD/../Libraries/GLFW/include
