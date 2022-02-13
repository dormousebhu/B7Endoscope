VCPKG_DIR = C:/vcpkg/

SOURCES += $$PWD/convert.cpp


HEADERS += $$PWD/convert.h

win32{
        contains(QT_ARCH, i386) {
        message("OpenCV 3.4.3 support code 32-bit")		
                INCLUDEPATH += $$VCPKG_DIR/installed/x86-windows/include
                DEPENDPATH += $$VCPKG_DIR/installed/x86-windows/bin
        CONFIG(release, debug|release): LIBS += -L$$VCPKG_DIR/installed/x86-windows/lib  \
                                                            -lopencv_core343  \
                                                            -lopencv_highgui343  \
                                                            -lopencv_imgproc343  \
                                                            -lopencv_imgcodecs343 \
                                                            -lopencv_videoio343 \
                                                            -lopencv_features2d343\
                                                            -lopencv_ml343 \
                                                            -lopencv_objdetect343  \
                                                            -lopencv_photo343  \
                                                            -lopencv_stitching343  \
                                                            -lopencv_superres343 \
                                                            -lopencv_video343  \
                                                            -lopencv_videostab343  \
                                                            -lopencv_calib3d343  \
                                                            -lopencv_flann343

                else:CONFIG(debug, debug|release): LIBS += -L$$VCPKG_DIR/installed/x86-windows/debug/lib \
                                                            -lopencv_core343d  \
                                                            -lopencv_highgui343d \
                                                            -lopencv_imgproc343d \
                                                            -lopencv_imgcodecs343d   \
                                                            -lopencv_videoio343d \
                                                            -lopencv_features2d343d\
                                                            -lopencv_ml343d \
                                                            -lopencv_objdetect343d  \
                                                            -lopencv_photo343d  \
                                                            -lopencv_stitching343d  \
                                                            -lopencv_superres343d  \
                                                            -lopencv_video343d  \
                                                            -lopencv_videostab343d \
                                                            -lopencv_calib3d343d \
                                                            -lopencv_flann343d
    } else {
        message("OpenCV 3.4.3 support code 64-bit")
                INCLUDEPATH += $$VCPKG_DIR/installed/x64-windows/include
                DEPENDPATH += $$VCPKG_DIR/installed/x64-windows/bin
        CONFIG(release, debug|release): LIBS += -L$$VCPKG_DIR/installed/x64-windows/lib  \
                                                            -lopencv_core343  \
                                                            -lopencv_highgui343  \
                                                            -lopencv_imgproc343  \
                                                            -lopencv_imgcodecs343 \
                                                            -lopencv_videoio343 \
                                                            -lopencv_features2d343\
                                                            -lopencv_ml343 \
                                                            -lopencv_objdetect343  \
                                                            -lopencv_photo343  \
                                                            -lopencv_stitching343  \
                                                            -lopencv_superres343 \
                                                            -lopencv_video343  \
                                                            -lopencv_videostab343  \
                                                            -lopencv_calib3d343  \
                                                            -lopencv_flann343

                else:CONFIG(debug, debug|release): LIBS += -L$$VCPKG_DIR/installed/x64-windows/debug/lib \
                                                            -lopencv_core343d  \
                                                            -lopencv_highgui343d \
                                                            -lopencv_imgproc343d \
                                                            -lopencv_imgcodecs343d   \
                                                            -lopencv_videoio343d \
                                                            -lopencv_features2d343d\
                                                            -lopencv_ml343d \
                                                            -lopencv_objdetect343d  \
                                                            -lopencv_photo343d  \
                                                            -lopencv_stitching343d \
                                                            -lopencv_superres343d  \
                                                            -lopencv_video343d  \
                                                            -lopencv_videostab343d \
                                                            -lopencv_calib3d343d \
                                                            -lopencv_flann343d
        # message($$LIBS)
    }
}
