#-------------------------------------------------
#
# Project created by QtCreator 2017-12-06T15:15:54
#
#-------------------------------------------------

QT       += core gui sql network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BankSystem
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
    bank.cpp \
    bank_login.cpp \
    bank_register.cpp \
    bank_deposit.cpp \
    bank_draw.cpp \
    bank_modify.cpp

HEADERS  += mainwindow.h \
    bank.h \
    bank_login.h \
    bank_register.h \
    bank_deposit.h \
    bank_draw.h \
    bank_modify.h

FORMS    += mainwindow.ui \
    bank_login.ui \
    bank_register.ui \
    bank_deposit.ui \
    bank_draw.ui \
    bank_modify.ui
