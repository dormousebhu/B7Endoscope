SOURCES += $$PWD/convert.cpp

HEADERS += $$PWD/convert.h

CONFIG(release, debug|release): LIBS += -LD:/devel/vcpkg/installed/x64-windows/lib     \
#	-lopencv_calib3d343   \
        -lopencv_core343  \
        -lopencv_features2d343  \
#	-lopencv_flann343  \
        -lopencv_highgui343  \
        -lopencv_imgproc343  \
        -lopencv_imgcodecs343 \
        -lopencv_videoio343 \
#	-lopencv_ml343 \
#	-lopencv_objdetect343  \
#	-lopencv_photo343  \
#	-lopencv_stitching343  \
#	-lopencv_superres343  \
#	-lopencv_video343  \
#	-lopencv_videostab343

else:CONFIG(debug, debug|release): LIBS += -LD:/devel/vcpkg/installed/x64-windows/debug/lib   \
#        -lopencv_calib3d343d   \
        -lopencv_core343d  \
        -lopencv_features2d343d  \
#	-lopencv_flann343d  \
        -lopencv_highgui343d  \
        -lopencv_imgproc343d  \
        -lopencv_imgcodecs343d \
        -lopencv_videoio343d \
#	-lopencv_ml343d \
#	-lopencv_objdetect343d  \
#	-lopencv_photo343d  \
#	-lopencv_stitching343d  \
#	-lopencv_superres343d  \
#	-lopencv_video343d  \
#	-lopencv_videostab343d

INCLUDEPATH += D:\devel\vcpkg\installed\x64-windows\include
DEPENDPATH += D:\devel\vcpkg\installed\x64-windows\bin
