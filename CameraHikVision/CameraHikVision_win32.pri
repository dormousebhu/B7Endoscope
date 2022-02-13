#
#  海康威视 HikVision 工业相机支持代码。
#  author: LiYuan
#  date：2018.03.27
#  Last modified：2018.03.07

MVS_DEV_DIR = "C:/Program Files (x86)/MVS/Development"

#win32: INCLUDEPATH += $$(MVS_DEV_DIR)/Includes
#win32: LIBS += -L$$(MVS_DEV_DIR)/Libraries/win32 -lMvCameraControl
#win32: DEPENDPATH += -L$$(MVS_DEV_DIR)/Dlls/win32

win32: INCLUDEPATH += "C:/Program Files (x86)/MVS/Development/Includes"
win32: LIBS += -L"C:/Program Files (x86)/MVS/Development/Libraries/win32" -lMvCameraControl
win32: DEPENDPATH += -L"C:/Program Files (x86)/MVS/Development/Dlls/win32"

# win32: LIBS += -L$$(MVS_DEV_DIR)/Libraries/GenICam/win32 -lGCBase_MD_VC120_v3_0

DEFINES += USE_GIGE

SOURCES += $$PWD/HikVisionCamera.cpp  \
    $$PWD/CameraSettingDialog.cpp
	
	
HEADERS  += $$PWD/HikVisionCamera.h \
    $$PWD/CameraSettingDialog.h

FORMS    += $$PWD/CameraSettingDialog.ui
