QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent

CONFIG += gnu++2b
QMAKE_CC  = clang-17
QMAKE_CXX = clang++-17
QMAKE_CXXFLAGS += -std=gnu++2b
QMAKE_CFLAGS += -std=gnu++2b
LIBS += /usr/local/lib/libcryptopp.a -lz-ng

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    flowlayout.cpp \
    emulator/param_sfo.cpp \
    emulator/fps_dialog.cpp \
    emulator/crypto.cpp \
    emulator/io_file.cpp \
    emulator/psf.cpp \
    emulator/pkg.cpp \
    emulator/pkg_type.cpp \
    emulator/loader.cpp

    

HEADERS += \
    mainwindow.h \
    flowlayout.h \
    emulator/param_sfo.h \
    emulator/fps_dialog.h \
    emulator/elf.h \
    emulator/crypto.h \
    emulator/endian.h \
    emulator/io_file.h \
    emulator/keys.h \
    emulator/pfs.h \
    emulator/psf.h \
    emulator/pkg.h \
    emulator/pkg_type.h \
    emulator/loader.h \
    emulator/types.h
    
	    
FORMS += \
    mainwindow.ui \
    emulator/fps_dialog.ui
    
RESOURCES += \
	resources.qrc \
	dark/darkstyle.qrc \
	dark/darkstyle.qss

INCLUDEPATH += \
	/usr/local/include \
	/usr/local \
	/usr/local/include/cryptopp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

