QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    param_sfo.cpp \
    fps_dialog.cpp \
    flowlayout.cpp

HEADERS += \
    mainwindow.h \
    param_sfo.h \
    fps_dialog.h \
    flowlayout.h \
    elf.h
	    
FORMS += \
    mainwindow.ui \
    fps_dialog.ui
    
RESOURCES += \
    resources.qrc \
    dark/darkstyle.qrc \
    dark/darkstyle.qss


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

