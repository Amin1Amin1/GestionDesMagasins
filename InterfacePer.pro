QT       += core gui \
            quick
QT += serialport
QT += widgets
QT += sql
QT += charts
QT += core gui network sql widgets
QT += core gui sql network
QT += core gui sql network widgets charts
QT += serialport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connection.cpp \
    magasins.cpp \
    main.cpp \
    interfaceper.cpp

HEADERS += \
    connection.h \
    interfaceper.h \
    magasins.h
FORMS += \
    interfaceper.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
