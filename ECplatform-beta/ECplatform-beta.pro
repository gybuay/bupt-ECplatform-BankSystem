#-------------------------------------------------
#
# Project created by QtCreator 2017-12-10T13:44:34
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ECplatform
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
    product.cpp \
    user.cpp \
    book.cpp \
    plat_register.cpp \
    plat_login.cpp \
    addlist.cpp \
    shoplist.cpp \
    user_list.cpp \
    food.cpp \
    product_minus.cpp \
    product_discount.cpp \
    clothes.cpp \
    bind_card.cpp \
    mycard.cpp \
    afford.cpp

HEADERS  += mainwindow.h \
    product.h \
    user.h \
    book.h \
    plat_register.h \
    plat_login.h \
    state.h \
    addlist.h \
    shoplist.h \
    user_list.h \
    food.h \
    product_minus.h \
    product_discount.h \
    clothes.h \
    bind_card.h \
    mycard.h \
    afford.h

FORMS    += mainwindow.ui \
    plat_register.ui \
    plat_login.ui \
    addlist.ui \
    shoplist.ui \
    product_minus.ui \
    product_discount.ui \
    bind_card.ui \
    mycard.ui \
    afford.ui
