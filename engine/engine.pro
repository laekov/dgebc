TEMPLATE      = app
CONFIG += c++11
QT  += widgets opengl
HEADERS       = \
    world.h \
    track.h \
    mainwindow.h \
    contactlistener.h \
    car.h \
    engine.h \
    geneticalgorithm.h \  
    render.h
    
SOURCES       = \
                main.cpp \
    world.cpp \
    track.cpp \
    mainwindow.cpp \
    contactlistener.cpp \
    car.cpp \
    engine.cpp \
    geneticalgorithm.cpp \
    render.cpp

LIBS += -L$$PWD/lib/ -lBox2D

INCLUDEPATH += $$PWD/Box2D
INCLUDEPATH += $$PWD/Box2D/Box2D
DEPENDPATH += $$PWD/Box2D

PRE_TARGETDEPS += $$PWD/lib/libBox2D.a
