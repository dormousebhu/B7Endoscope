QT += gui
QT += multimedia


win32{
        contains(QT_ARCH, i386) {
        message("ffmpeg support code 32-bit")
                INCLUDEPATH += $$PWD/installed/x86-windows/include
                DEPENDPATH += $$PWD/installed/x86-windows/bin

        CONFIG(release, debug|release): LIBS += -L$$PWD/installed/x86-windows/lib/ \
                                                        -lavcodec \
                                                        -lavdevice \
                                                        -lavfilter \
                                                        -lavformat \
                                                        -lavutil \
                                                        -lswresample \
                                                        -lswscale

                else:CONFIG(debug, debug|release): LIBS += -L$$PWD/installed/x86-windows/debug/lib/ \
                                                        -lavcodec \
                                                        -lavdevice \
                                                        -lavfilter \
                                                        -lavformat \
                                                        -lavutil \
                                                        -lswresample \
                                                        -lswscale
    } else {
        message("ffmpeg support code 64-bit")
                INCLUDEPATH += $$PWD/installed/x64-windows/include
                DEPENDPATH +=  $$PWD/installed/x64-windows/bin
        CONFIG(release, debug|release): LIBS += -L$$PWD/installed/x64-windows/lib/ \
                                                        -lavcodec \
                                                        -lavdevice \
                                                        -lavfilter \
                                                        -lavformat \
                                                        -lavutil \
                                                        -lswresample \
                                                        -lswscale

                else:CONFIG(debug, debug|release): LIBS += -L$$PWD/installed/x64-windows/debug/lib/ \
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
