#ifndef CAMERAFACTORY_H
#define CAMERAFACTORY_H

#include "IndustryCamera.h"

namespace Qly {
class CameraHikVision;
class CameraToupCam;
class CameraTis;
class PCCamera;
class BaslerGigECamera;

class CameraFactory
{
public:
    CameraFactory();
    IndustryCamera * defaultCamera(CameraType type);
    IndustryCamera * findCameraByName(QString name);
private:
    CameraHikVision * defaultHikCamera(CameraType type);
//    CameraToupCam * defaultToupCam();
//    CameraTis * defaultTisCamera();
    PCCamera * defaultPCCamera();
//    BaslerGigECamera * defaultBaslerGigECamera();

    CameraHikVision * findCameraHikVisionCamera(QString name);
//    CameraToupCam * findCameraToupCamByName(QString name);
//    CameraTis * findCameraTisByName(QString name);
    PCCamera * findCameraPCByName(QString name);
//    BaslerGigECamera *findBaslerGigECameraByName(QString name);

};
} //namespace Qly
#endif // CAMERAFACTORY_H
