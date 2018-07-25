QT          += core
QT          -= gui
QT          += network

TARGET       = Server

CONFIG      += console

TEMPLATE     = app

SOURCES     += \
    src/main.cpp \
    src/server.cpp \
    src/worker.cpp

HEADERS     += \
    src/server.h \
    src/worker.h

OTHER_FILES += etc/config.ini

include(../QtWebApp/QtWebApp/httpserver/httpserver.pri)
