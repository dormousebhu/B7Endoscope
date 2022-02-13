QT += multimedia
QT += multimediawidgets
include(./ffmpeg/ffmpeg.pri)
HEADERS  += \
    $$PWD/CameraFactory.h \
    $$PWD/CameraPC.h \
    $$PWD/PixelFormatConvert.h \
    $$PWD/ToolBarCamera.h \
    $$PWD/CameraImage_RGB24.h \
    $$PWD/CameraImage_RGB32.h \
    $$PWD/CameraImage.h \
    $$PWD/ScreenCamera.h \
    $$PWD/DialogCameraSetup.h \
    $$PWD/IndustryCamera.h

SOURCES += \
    $$PWD/CameraFactory.cpp \
    $$PWD/CameraImageProcessing.cpp \
    $$PWD/CameraPC.cpp \
    $$PWD/PixelFormatConvert.cpp \
    $$PWD/ToolBarCamera.cpp \
    $$PWD/CameraImage_RGB24.cpp \
    $$PWD/CameraImage_RGB32.cpp \
    $$PWD/CameraImage.cpp \
    $$PWD/ScreenCamera.cpp \
    $$PWD/DialogCameraSetup.cpp \
    $$PWD/IndustryCamera.cpp

INCLUDEPATH += $$PWD
RESOURCES += $$PWD/CameraCommon.qrc

FORMS += \
    $$PWD/DialogCameraSetup.ui
