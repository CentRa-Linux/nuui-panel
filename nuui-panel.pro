QT       += core gui x11extras KWindowSystem KGlobalAccel

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lxcb

SOURCES += \
    launcher.cpp \
    main.cpp \
    mainpanel.cpp \
    taskmanager.cpp \
    timewidget.cpp

HEADERS += \
    launcher.h \
    mainpanel.h \
    taskmanager.h \
    timewidget.h

FORMS += \
    mainpanel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
