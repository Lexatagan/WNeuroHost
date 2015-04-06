#-------------------------------------------------
#
# Project created by QtCreator 2014-06-21T11:29:06
#
#-------------------------------------------------

QT       += core network serialport

QT       -= gui

TARGET = DesktopServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += c:\windows\system32\libusb0.dll

SOURCES += main.cpp \
    interface.cpp \
    wlanpacket.cpp \
    wlanPipe.cpp \
    tcpserver.cpp \
    USBTransceiver.cpp \
    lan.cpp

HEADERS += \
    interface.h \
    wlanpacket.h \
    wlanPipe.h \
    tcpserver.h \
    lusb0_usb.h \
    USBTransceiver.h \
    lan.h
