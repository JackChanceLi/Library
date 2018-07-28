#-------------------------------------------------
#
# Project created by QtCreator 2017-03-08T11:39:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Library
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    newbookdialog.cpp \
    borrowdialog.cpp \
    register.cpp \
    header.cpp \
    logviewdialog.cpp \
    bookmanagedialog.cpp \
    utility.cpp \
    datethread.cpp \
    dialogfine.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    newbookdialog.h \
    borrowdialog.h \
    register.h \
    header.h \
    logviewdialog.h \
    bookmanagedialog.h \
    utility.h \
    datethread.h \
    dialogfine.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    newbookdialog.ui \
    borrowdialog.ui \
    register.ui \
    logviewdialog.ui \
    bookmanagedialog.ui \
    dialogfine.ui

RESOURCES += \
    resourse.qrc
