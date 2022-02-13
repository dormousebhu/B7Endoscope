VCPKG_DIR = C:/vcpkg/

SOURCES += $$PWD/OpenCVDshowCamera.cpp
HEADERS += $$PWD/OpenCVDshowCamera.h

win32: LIBS += -lStrmiids -lQuartz -lOle32 -lOleAut32
win32{
        contains(QT_ARCH, i386) {
        message("OpenCV 3.4.3 support code 32-bit")		
                INCLUDEPATH += $$VCPKG_DIR/installed/x86-windows/include
                DEPENDPATH += $$VCPKG_DIR/installed/x86-windows/bin
        CONFIG(release, debug|release): LIBS += -L$$VCPKG_DIR/installed/x86-windows/lib  \
                                                            -lopencv_core343  \
                                                            #-lopencv_highgui343  \
                                                            #-lopencv_imgproc343  \
                                                            #-lopencv_imgcodecs343 \
                                                            -lopencv_videoio343 \
                                                            #-lopencv_video343  \
                                                            -lopencv_videostab343 


                else:CONFIG(debug, debug|release): LIBS += -L$$VCPKG_DIR/installed/x86-windows/debug/lib \
                                                            -lopencv_core343d  \
                                                            #-lopencv_highgui343d \
                                                            #-lopencv_imgproc343d \
                                                            #-lopencv_imgcodecs343d   \
                                                            -lopencv_videoio343d \
                                                            #-lopencv_video343d  \
                                                            -lopencv_videostab343d
    } else {
        message("OpenCV 3.4.3 support code 64-bit")
                INCLUDEPATH += $$VCPKG_DIR/installed/x64-windows/include
                DEPENDPATH += $$VCPKG_DIR/installed/x64-windows/bin
        CONFIG(release, debug|release): LIBS += -L$$VCPKG_DIR/installed/x64-windows/lib  \
                                                            -lopencv_core343  \
                                                            #-lopencv_highgui343  \
                                                            #-lopencv_imgproc343  \
                                                            #-lopencv_imgcodecs343 \
                                                            -lopencv_videoio343 \
                                                            #-lopencv_video343  \
                                                            -lopencv_videostab343  

                else:CONFIG(debug, debug|release): LIBS += -L$$VCPKG_DIR/installed/x64-windows/debug/lib \
                                                            -lopencv_core343d  \
                                                            #-lopencv_highgui343d \
                                                            #-lopencv_imgproc343d \
                                                            #-lopencv_imgcodecs343d   \
                                                            -lopencv_videoio343d \
                                                            #-lopencv_video343d  \
                                                            -lopencv_videostab343d
        # message($$LIBS)
    }
}
