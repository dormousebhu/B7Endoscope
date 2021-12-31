QT += gui
QT += multimedia

SOURCES += $$PWD/QlyVideoReader.cpp \
        $$PWD/QlyAVCodec.cpp \
        $$PWD/QlyAVCodecContext.cpp \
        $$PWD/QlyAVFrame.cpp \
        $$PWD/QlyAVFormatContext.cpp \
        $$PWD/QlyAVPacket.cpp \
        $$PWD/QlyAVStream.cpp \
        $$PWD/QlyMPEG4VideoWriter.cpp \
        $$PWD/QlySwsContext.cpp  \
        $$PWD/QlyVideoWriter.cpp

HEADERS += \
    $$PWD/QlyAVCodec.h \
    $$PWD/QlyAVCodecContext.h \
    $$PWD/QlyAVFrame.h \
    $$PWD/QlyMPEG4VideoWriter.h \
    $$PWD/QlyVideoReader.h \
    $$PWD/QlyAVFormatContext.h \
    $$PWD/QlyAVPacket.h \
    $$PWD/QlyAVStream.h \
    $$PWD/QlySwsContext.h		 \
    $$PWD/QlyVideoWriter.h

VCPKG_DIR = C:/vcpkg-new/

win32{
        contains(QT_ARCH, i386) {
        message("ffmpeg support code 32-bit")
                INCLUDEPATH += $$VCPKG_DIR/installed/x86-windows/include
                DEPENDPATH += $$VCPKG_DIR/installed/x86-windows/bin

        CONFIG(release, debug|release): LIBS += -L$$VCPKG_DIR/installed/x86-windows/lib/ \
                                                        -lavcodec \
                                                        -lavdevice \
                                                        -lavfilter \
                                                        -lavformat \
                                                        -lavutil \
                                                        -lswresample \
                                                        -lswscale

                else:CONFIG(debug, debug|release): LIBS += -L$$VCPKG_DIR/installed/x86-windows/debug/lib/ \
                                                        -lavcodec \
                                                        -lavdevice \
                                                        -lavfilter \
                                                        -lavformat \
                                                        -lavutil \
                                                        -lswresample \
                                                        -lswscale
    } else {
        message("ffmpeg support code 64-bit")
                INCLUDEPATH += $$VCPKG_DIR/installed/x64-windows/include
                DEPENDPATH +=  $$VCPKG_DIR/installed/x64-windows/bin
        CONFIG(release, debug|release): LIBS += -L$$VCPKG_DIR/installed/x64-windows/lib/ \
                                                        -lavcodec \
                                                        -lavdevice \
                                                        -lavfilter \
                                                        -lavformat \
                                                        -lavutil \
                                                        -lswresample \
                                                        -lswscale

                else:CONFIG(debug, debug|release): LIBS += -L$$VCPKG_DIR/installed/x64-windows/debug/lib/ \
                                                        -lavcodec \
                                                        -lavdevice \
                                                        -lavfilter \
                                                        -lavformat \
                                                        -lavutil \
                                                        -lswresample \
                                                        -lswscale
        # message($$LIBS)
    }
}
