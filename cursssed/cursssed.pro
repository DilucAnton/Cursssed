QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    add_in_view.cpp \
    add_movie.cpp \
    add_payment.cpp \
    add_rental.cpp \
    add_user.cpp \
    connection_dialog.cpp \
    fastadd.cpp \
    main.cpp \
    mainwindow.cpp \
    queries.cpp \
    querymodel.cpp \
    updates/upd_movie.cpp \
    updates/upd_payments.cpp \
    updates/upd_rental.cpp \
    updates/upd_user.cpp \
    updates/upd_view.cpp

HEADERS += \
    add_in_view.h \
    add_movie.h \
    add_payment.h \
    add_rental.h \
    add_user.h \
    connection_dialog.h \
    fastadd.h \
    mainwindow.h \
    queries.h \
    querymodel.h \
    updates/upd_movie.h \
    updates/upd_payments.h \
    updates/upd_rental.h \
    updates/upd_user.h \
    updates/upd_view.h

FORMS += \
    add_in_view.ui \
    add_movie.ui \
    add_payment.ui \
    add_rental.ui \
    add_user.ui \
    connection_dialog.ui \
    fastadd.ui \
    mainwindow.ui \
    queries.ui \
    updates/upd_movie.ui \
    updates/upd_payments.ui \
    updates/upd_rental.ui \
    updates/upd_user.ui \
    updates/upd_view.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
