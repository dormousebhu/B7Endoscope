#
#  海康威视 HikVision 工业相机支持代码。
#  author: LiYuan
#  date：2018.03.27
#  Last modified：2018.03.07

MVS_DEV_DIR = "C:/Program Files (x86)/MVS/Development/"

win32{
        contains(QT_ARCH, i386) {
        message("HikVision Camera 32-bit")
                INCLUDEPATH += $$MVS_DEV_DIR/Includes/
                LIBS += -L$$MVS_DEV_DIR/Libraries/win32/ -lMvCameraControl
                DEPENDPATH += -L$$MVS_DEV_DIR/Dlls/win32/
    } else {
        message("HikVision Camera 64-bit")
                INCLUDEPATH += $$MVS_DEV_DIR/Includes/
                LIBS += -L$$MVS_DEV_DIR/Libraries/win64/ -lMvCameraControl
                DEPENDPATH += -L$$MVS_DEV_DIR/Dlls/win64/
    }
}
#win32: INCLUDEPATH += $$(MVS_DEV_DIR)/Includes
#win32: LIBS += -L$$(MVS_DEV_DIR)/Libraries/win32 -lMvCameraControl
#win32: DEPENDPATH += -L$$(MVS_DEV_DIR)/Dlls/win32
# win32: LIBS += -L$$(MVS_DEV_DIR)/Libraries/GenICam/win32 -lGCBase_MD_VC120_v3_0

DEFINES += USE_GIGE

SOURCES += \
    $$PWD/CameraHikVision.cpp \
    $$PWD/HikVisionCameraImageProcessing.cpp
	
	
HEADERS  += \
    $$PWD/CameraHikVision.h

FORMS    +=
