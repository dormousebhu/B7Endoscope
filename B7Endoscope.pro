include(./ffmpeg/ffmpeg.pri)
include(./Logger/messageLogger.pri)
QT       += core gui
QT      += multimediawidgets
QT      += multimedia
QT += network

RC_FILE += app.rc
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AboutDialog.cpp \
    CamerasDisplayWidget.cpp \
    ImageSaveToolBar.cpp \
    PageToolBar.cpp \
    RemoteControl.cpp \
    SettingDialog.cpp \
    SettingsToolBar.cpp \
    VideoRecordToolBar.cpp \
    camera/CameraImage.cpp \
    camera/CameraImage_RGB32.cpp \
    camera/CameraPC.cpp \
    camera/ImageView.cpp \
    camera/IndustryCamera.cpp \
    camera/PixelFormatConvert.cpp \
    camera/imagemonitor.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AboutDialog.h \
    CamerasDisplayWidget.h \
    ImageSaveToolBar.h \
    PageToolBar.h \
    RemoteControl.h \
    SettingDialog.h \
    SettingsToolBar.h \
    VideoRecordToolBar.h \
    camera/CameraImage.h \
    camera/CameraImage_RGB32.h \
    camera/CameraPC.h \
    camera/ImageMonitor.h \
    camera/ImageView.h \
    camera/IndustryCamera.h \
    camera/PixelFormatConvert.h \
    mainwindow.h

FORMS += \
    AboutDialog.ui \
    SettingDialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    B7Endoscope_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    icon.qrc

win32{
        contains(QT_ARCH, i386) {
        message("build dir 32-bit")
                RCC_DIR += ../temp_x86/b7endoscope/rcc
                MOC_DIR += ../temp_x86/b7endoscope/moc
                OBJECTS_DIR = ../temp_x86/b7endoscope/obj
                UI_DIR = ../temp_x86/b7endoscope/ui

    } else {
        message("build dir 64-bit")
                RCC_DIR += ../temp_x64/b7endoscope/rcc
                MOC_DIR += ../temp_x64/b7endoscope/moc
                OBJECTS_DIR = ../temp_x64/b7endoscope_/obj
                UI_DIR = ../temp_x64/b7endoscope/ui
    }
}
