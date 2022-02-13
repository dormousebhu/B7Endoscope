#include "CameraFactory.h"
#include <QCameraInfo>
#include "CameraPC.h"
#include "../CameraHikVision/CameraHikVision.h"
//#include "../CameraToupCam/CameraToupCam.h"
//#include "../CameraImagingSource/CameraTis.h"
//#include "../CameraBasler/BaslerCamera.h"

namespace Qly{

CameraFactory::CameraFactory()
{

}

IndustryCamera * CameraFactory::findCameraByName(QString name)
{
    IndustryCamera * camera = nullptr;

    camera = findCameraHikVisionCamera(name);
    if(camera)
    {
        return camera;
    }
//    camera = findCameraTisByName(name);
//    if(camera)
//    {
//        return camera;
//    }
//    camera = findCameraToupCamByName(name);
//    if(camera)
//    {
//        return camera;
//    }
    camera = findCameraPCByName(name);
    if(camera)
    {
        return camera;
    }
    return camera;
}

Qly::CameraHikVision * CameraFactory::findCameraHikVisionCamera(QString name)
{
    Qly::CameraHikVision * camera = nullptr;
    if(!camera->openCameraByID(name))
    {
        delete camera;
        camera = nullptr;
    }
    return camera;
}

//CameraToupCam * CameraFactory::findCameraToupCamByName(QString name)
//{
//    CameraToupCam * camera = nullptr;
//    camera = new CameraToupCam;
//    if(!camera->openCameraByID(name))
//    {
//        delete camera;
//        camera = nullptr;
//    }
//    return camera;
//}

//BaslerGigECamera * CameraFactory::findBaslerGigECameraByName(QString name)
//{
//    BaslerGigECamera * camera = nullptr;
//    QList<BaslerCameraInfo> cameras =  BaslerCameraInfo::availableCameras();
//    for (const BaslerCameraInfo &cameraInfo : cameras)
//    {
//        if (cameraInfo.deviceName() == name)
//        {
//            camera = new BaslerGigECamera;
//            camera->open(cameraInfo);
//        }
//    }
//    return camera;
//}

//CameraTis * CameraFactory::findCameraTisByName(QString name)
//{
//    CameraTis * camera = nullptr;
//    TisCameraInfo info(name);
//    if(!info.isNull())
//    {
//        camera = new CameraTis;
//        camera->openDevice(info);
//    }
//    return camera;
//}

PCCamera * CameraFactory::findCameraPCByName(QString name)
{
    PCCamera * camera = nullptr;
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : cameras)
    {
        if (cameraInfo.deviceName() == name)
        {
            camera = new PCCamera;
            camera->openCameraByInfo(cameraInfo);
        }
    }
      return camera;
}

IndustryCamera * CameraFactory::defaultCamera(CameraType type)
{
    IndustryCamera * camera = nullptr;
//    if(type & TIS_GIGE)
//    {
//        qDebug() << "start defaultTisCamera()";
//        camera = defaultTisCamera();
//        qDebug() << "defaultTisCamera() = " << camera;
//        if(camera)
//        {
//            return camera;
//        }
//    }

    if(type & HIK_GIGE)
    {
        camera = defaultHikCamera(HIK_GIGE);
        qDebug() << "defaultHikCamera(HIK_GIGE) = " << camera;
        if(camera)
        {
            return camera;
        }
    }

//    if(type & BASLER_GIGE)
//    {
//        camera = defaultBaslerGigECamera();
//        qDebug() << "defaultBaslerGigECamera() = " << camera;
//        if(camera)
//        {
//            return camera;
//        }
//    }

    if(type & HIK_USB)
    {
        camera = defaultHikCamera(HIK_USB);
        qDebug() << "defaultHikCamera(HIK_USB) = " << camera;
        if(camera)
        {
            return camera;
        }
    }
//    if(type & TOUP_USB)
//    {
//        camera = defaultToupCam();
//        qDebug() << "defaultToupCam() = " << camera;
//        if(camera)
//        {
//            return camera;
//        }
//    }

    if( type & SYS_CAM)
    {
        camera = defaultPCCamera();
        qDebug() << "defaultPCCamera() = " << camera;
        if(camera)
        {
            return camera;
        }
    }

    return camera;
}

//BaslerGigECamera * CameraFactory::defaultBaslerGigECamera()
//{
//    BaslerGigECamera * camera = new BaslerGigECamera(nullptr);
//    if( camera->openDefaultCamera())
//    {
//        return camera;
//    }
//    delete camera;
//    return nullptr;
//}

//CameraTis * CameraFactory::defaultTisCamera()
//{
//    TisCameraInfo info = TisCameraInfo::defaultCamera();
//    if(info.isNull())
//    {
//        return nullptr;
//    }
//    CameraTis *camera = new CameraTis;
//    if(camera->openDefaultCamera())
//    {
//        return camera;
//    }
//    else
//    {
//        delete camera;
//    }

//    return nullptr;

//}

PCCamera * CameraFactory::defaultPCCamera()
{
    QCameraInfo info = QCameraInfo::defaultCamera();
    if(info.isNull()) return nullptr;
    PCCamera * c = new PCCamera;
    c->openDefaultCamera();

    return c;
}

//CameraToupCam * CameraFactory::defaultToupCam()
//{
//    CameraToupCam * camera = new CameraToupCam;
//    if( camera->openByIdx(0) )
//    {
//        return camera;
//    }
//    delete camera;
//    return nullptr;
//}

Qly::CameraHikVision * CameraFactory::defaultHikCamera(CameraType type)
{
    unsigned int nTLayerType;
    if( (type & HIK_GIGE) && (type & HIK_USB) )
    {
        nTLayerType = MV_GIGE_DEVICE | MV_USB_DEVICE;
    }
    else if(type & HIK_GIGE)
    {
        nTLayerType = MV_GIGE_DEVICE;
    }
    else if(type & HIK_USB)
    {
        nTLayerType = MV_USB_DEVICE;
    }
    else
    {
        return nullptr;
    }
    MV_CC_DEVICE_INFO_LIST stDevList = {0};
    int nRet = MV_CC_EnumDevices(nTLayerType, &stDevList);
    if (MV_OK != nRet)
        {
            qWarning("error: MV_CC_EnumDevices fail [%x]", nRet);
            return nullptr;
        }
        if (stDevList.nDeviceNum == 0)
        {
            qWarning("no hikvision camera found!");
            return nullptr;
        }
    //选择查找到的第一台在线设备，创建设备句柄
    int nDeviceIndex = 0;
    MV_CC_DEVICE_INFO stDevInfo = {0};
    memcpy(&stDevInfo, stDevList.pDeviceInfo[nDeviceIndex], sizeof(MV_CC_DEVICE_INFO));

    Qly::CameraHikVision * camera = new Qly::CameraHikVision();
    if(camera->open(stDevInfo))
    {
        return camera;
    }
    return nullptr;
}
} //namespace Qly
