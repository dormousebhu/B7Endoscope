SOURCES += $$PWD/convert.cpp

HEADERS += $$PWD/convert.h
win32{
        contains(QT_ARCH, i386) {
                DEPENDPATH += C:/opencv_2.4.13/build/x86/vc12/bin
        message("OpenCV 2.4.13 32-bit")
                CONFIG(release, debug|release): LIBS += -LC:/opencv_2.4.13/build/x86/vc12/lib/     \
		-lopencv_calib3d2413   \
		-lopencv_contrib2413   \
		-lopencv_core2413  \
		-lopencv_features2d2413  \
		-lopencv_flann2413  \
		-lopencv_gpu2413 \
		-lopencv_highgui2413  \
		-lopencv_imgproc2413  \
		-lopencv_legacy2413  \
		-lopencv_ml2413 \
		-lopencv_nonfree2413  \
		-lopencv_objdetect2413  \
		-lopencv_ocl2413  \
		-lopencv_photo2413  \
		-lopencv_stitching2413  \
		-lopencv_superres2413  \
		-lopencv_ts2413  \
		-lopencv_video2413  \
		-lopencv_videostab2413
		
                CONFIG(debug, debug|release): LIBS += -LC:/opencv_2.4.13/build/x86/vc12/lib/    \
		-lopencv_calib3d2413d   \
		-lopencv_contrib2413d   \
		-lopencv_core2413d  \
		-lopencv_features2d2413d  \
		-lopencv_flann2413d  \
		-lopencv_gpu2413d \
		-lopencv_highgui2413d  \
		-lopencv_imgproc2413d  \
		-lopencv_legacy2413d  \
		-lopencv_ml2413d \
		-lopencv_nonfree2413d  \
		-lopencv_objdetect2413d  \
		-lopencv_ocl2413d  \
		-lopencv_photo2413d  \
		-lopencv_stitching2413d  \
		-lopencv_superres2413d  \
		-lopencv_ts2413d  \
		-lopencv_video2413d  \
		-lopencv_videostab2413d 
		
    } else {
        message("OpenCV 2.4.13 64-bit")
                DEPENDPATH += C:/opencv_2.4.13/build/x64/vc12/bin
		
                CONFIG(release, debug|release): LIBS += -LC:/opencv_2.4.13/build/x64/vc12/lib/     \
		-lopencv_calib3d2413   \
		-lopencv_contrib2413   \
		-lopencv_core2413  \
		-lopencv_features2d2413  \
		-lopencv_flann2413  \
		-lopencv_gpu2413 \
		-lopencv_highgui2413  \
		-lopencv_imgproc2413  \
		-lopencv_legacy2413  \
		-lopencv_ml2413 \
		-lopencv_nonfree2413  \
		-lopencv_objdetect2413  \
		-lopencv_ocl2413  \
		-lopencv_photo2413  \
		-lopencv_stitching2413  \
		-lopencv_superres2413  \
		-lopencv_ts2413  \
		-lopencv_video2413  \
		-lopencv_videostab2413
		
                CONFIG(debug, debug|release): LIBS += -LC:/opencv_2.4.13/build/x64/vc12/lib/    \
		-lopencv_calib3d2413d   \
		-lopencv_contrib2413d   \
		-lopencv_core2413d  \
		-lopencv_features2d2413d  \
		-lopencv_flann2413d  \
		-lopencv_gpu2413d \
		-lopencv_highgui2413d  \
		-lopencv_imgproc2413d  \
		-lopencv_legacy2413d  \
		-lopencv_ml2413d \
		-lopencv_nonfree2413d  \
		-lopencv_objdetect2413d  \
		-lopencv_ocl2413d  \
		-lopencv_photo2413d  \
		-lopencv_stitching2413d  \
		-lopencv_superres2413d  \
		-lopencv_ts2413d  \
		-lopencv_video2413d  \
		-lopencv_videostab2413d 
    }
}

INCLUDEPATH += C:/opencv_2.4.13/build/include

