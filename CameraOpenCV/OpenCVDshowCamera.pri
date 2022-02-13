VCPKG_DIR = C:/vcpkg-new/

SOURCES += $$PWD/OpenCVDshowCamera.cpp
HEADERS += $$PWD/OpenCVDshowCamera.h

win32: LIBS += -lStrmiids -lQuartz -lOle32 -lOleAut32

win32{
        contains(QT_ARCH, i386) {
        message("OpenCV 4 support code 32-bit")
                INCLUDEPATH += $$VCPKG_DIR/installed/x86-windows/include
                DEPENDPATH += $$VCPKG_DIR/installed/x86-windows/bin
        CONFIG(release, debug|release): LIBS += -L$$VCPKG_DIR/installed/x86-windows/lib  \
                                                            -lopencv_core  \
                                                            -lopencv_highgui  \
                                                            -lopencv_imgproc  \
                                                            -lopencv_imgcodecs \
                                                            -lopencv_videoio \
                                                            -lopencv_features2d\
                                                            -lopencv_ml \
                                                            -lopencv_objdetect  \
                                                            -lopencv_photo  \
                                                            -lopencv_stitching  \
                                                            -lopencv_superres \
                                                            -lopencv_video  \
                                                            -lopencv_videostab  \
                                                            -lopencv_calib3d  \
                                                            -lopencv_flann

                else:CONFIG(debug, debug|release): LIBS += -L$$VCPKG_DIR/installed/x86-windows/debug/lib \
                                                            -lopencv_cored  \
                                                            -lopencv_highguid \
                                                            -lopencv_imgprocd \
                                                            -lopencv_imgcodecsd   \
                                                            -lopencv_videoiod \
                                                            -lopencv_features2dd\
                                                            -lopencv_mld \
                                                            -lopencv_objdetectd  \
                                                            -lopencv_photod  \
                                                            -lopencv_stitchingd  \
                                                            -lopencv_superresd  \
                                                            -lopencv_videod  \
                                                            -lopencv_videostabd \
                                                            -lopencv_calib3dd \
                                                            -lopencv_flannd
    } else {
        message("OpenCV 4 support code 64-bit")
                INCLUDEPATH += $$VCPKG_DIR/installed/x64-windows/include
                DEPENDPATH += $$VCPKG_DIR/installed/x64-windows/bin
        CONFIG(release, debug|release): LIBS += -L$$VCPKG_DIR/installed/x64-windows/lib  \
                                                            -lopencv_core  \
                                                            -lopencv_highgui  \
                                                            -lopencv_imgproc  \
                                                            -lopencv_imgcodecs \
                                                            -lopencv_videoio \
                                                            -lopencv_features2d\
                                                            -lopencv_ml \
                                                            -lopencv_objdetect  \
                                                            -lopencv_photo  \
                                                            -lopencv_stitching  \
                                                            -lopencv_superres \
                                                            -lopencv_video  \
                                                            -lopencv_videostab  \
                                                            -lopencv_calib3d  \
                                                            -lopencv_flann

                else:CONFIG(debug, debug|release): LIBS += -L$$VCPKG_DIR/installed/x64-windows/debug/lib \
                                                            -lopencv_cored  \
                                                            -lopencv_highguid \
                                                            -lopencv_imgprocd \
                                                            -lopencv_imgcodecsd   \
                                                            -lopencv_videoiod \
                                                            -lopencv_features2dd\
                                                            -lopencv_mld \
                                                            -lopencv_objdetectd  \
                                                            -lopencv_photod  \
                                                            -lopencv_stitchingd \
                                                            -lopencv_superresd  \
                                                            -lopencv_videod  \
                                                            -lopencv_videostabd \
                                                            -lopencv_calib3dd \
                                                            -lopencv_flannd
        # message($$LIBS)
    }
}
