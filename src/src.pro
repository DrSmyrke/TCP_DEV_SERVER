QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcpDEVserver
CONFIG += c++11

CONFIG(debug, debug|release):CONFIGURATION=debug
CONFIG(release, debug|release):CONFIGURATION=release

build_pass:CONFIG(release, debug|release) {
    QMAKE_LFLAGS_RELEASE += -static -static-libgcc
}

build_pass:CONFIG(debug, debug|release) {
    unix: TARGET = $$join(TARGET,,,_debug)
    else: TARGET = $$join(TARGET,,,d)
}

DEFINES += QT_DEPRECATED_WARNINGS

OBJECTS_DIR         = ../build/obj/$${CONFIGURATION}
MOC_DIR             = ../build/$${CONFIGURATION}
RCC_DIR             = ../build/rcc
UI_DIR              = ../build/ui
DESTDIR             = ../bin

win32|win64{
    RC_FILE=  index.rc
    OTHER_FILES+= index.rc
    DISTFILES += index.rc
}

SOURCES += \
    client.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    client.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

RESOURCES += resources.qrc

TRANSLATIONS = lang/ru_RU.ts

exists(./gitversion.pri):include(./gitversion.pri)
exists(./myLibs.pri):include(./myLibs.pri)
