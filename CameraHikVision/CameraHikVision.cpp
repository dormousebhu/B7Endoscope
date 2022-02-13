/****************************************************************************
** file: HikVisionCamera.cpp
** brief: 海康相机的支持代码
** Copyright (C) LiYuan
** Author: LiYuan
** E-Mail: 18069211#qq(.)com
** Version 2.0.1
** Last modified: 2019.02.02
** Modified By: LiYuan
****************************************************************************/


#include <QDebug>
#include <QDir>
#include <QSettings>
#include "CameraHikVision.h"

namespace Qly {

QString HikVisionCameraInfo::description()
{
//    if(m_info.nTLayerType == MV_GIGE_DEVICE)
//    {
//        //QString modelName(m_info.SpecialInfo.stGigEInfo.chModelName);
//        //QString serialNumber(m_info.SpecialInfo.stGigEInfo.chSerialNumber);
//        //return modelName + " :" + serialNumber;
//    }
    return "";
}

//QList<IndustryCameraInfo*> CameraHikVision::availableCameras()
//{
//    unsigned int nTLayerType = MV_GIGE_DEVICE | MV_USB_DEVICE;
//    MV_CC_DEVICE_INFO_LIST m_stDevList = {0};
//    int nRet = MV_CC_EnumDevices(nTLayerType, &m_stDevList);
//    if (MV_OK != nRet)
//    {
//        qDebug() << "error: EnumDevices fail";
//        return QList<IndustryCameraInfo*>();
//    }

//    QList<IndustryCameraInfo*> info;


//}

bool CameraHikVision::openCameraByID(QString name)
{
    unsigned int nTLayerType = MV_GIGE_DEVICE | MV_USB_DEVICE;
    MV_CC_DEVICE_INFO_LIST m_stDevList = {0};
    int nRet = MV_CC_EnumDevices(nTLayerType, &m_stDevList);
    if (MV_OK != nRet)
    {
        qWarning("error: MV_CC_EnumDevices fail [%x]", nRet);
        return false;
    }
    for (int i = 0; i < m_stDevList.nDeviceNum; i++)
    {
        if(m_stDevList.pDeviceInfo[i]->nTLayerType == MV_GIGE_DEVICE)
        {
            QString sn = QString::fromLocal8Bit(reinterpret_cast<char *>(m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chSerialNumber));
            if(sn == name)
            {
                return open(*m_stDevList.pDeviceInfo[i]);
            }
        }
    }
}

CameraHikVision::CameraHikVision(QObject *parent)
    :IndustryCamera(parent),
      m_hDevHandle(nullptr)
{
    //connect(&dia, SIGNAL(exposureTimeAbsChanged(double)), this, SLOT(setExpoureTime(double)));
    //connect(&dia, SIGNAL(gammaChanged(double)), this, SLOT(setGamma(double)));
    //connect(&dia, SIGNAL(autoWhiteBalanceChanged(int)), this, SLOT(autoWhiteBalance(int)));

    m_dialog = new DialogCameraSetup(this);
    m_cameraImageProcessing = new HikVisionCameraImageProcessing;
//    m_dia->show();
}

///TODO: 这个函数需要重写
bool CameraHikVision::isLive()
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    //获取设备采集模式
    MVCC_ENUMVALUE struEnumValue = {0};
    int nRet = MV_CC_GetAcquisitionMode(m_hDevHandle, &struEnumValue);
    if (MV_OK != nRet)
    {
        return false;
    }

    return struEnumValue.nCurValue == MV_ACQ_MODE_CONTINUOUS;
}
CameraHikVision::~CameraHikVision()
{
    close();
    delete m_dialog;
}


QString CameraHikVision::cameraName()
{
    if( m_hDevHandle == nullptr ) return QString();
    //获取相机用户ID信息
    MVCC_STRINGVALUE struStrValue = {0};
    memset(&struStrValue, 0, sizeof(MVCC_STRINGVALUE));
    QString DeviceVendorName, DeviceModelName;
    int nRet = MV_CC_GetStringValue(m_hDevHandle, "DeviceVendorName", &struStrValue);
    if (MV_OK == nRet)
    {
        DeviceVendorName = QString(struStrValue.chCurValue);
    }

    nRet = MV_CC_GetStringValue(m_hDevHandle, "DeviceModelName", &struStrValue);
    if (MV_OK == nRet)
    {
        DeviceModelName = QString(struStrValue.chCurValue);
    }

    return DeviceVendorName + " " +  DeviceModelName;
}



bool CameraHikVision::open(MV_CC_DEVICE_INFO info)
{
    if(m_hDevHandle)
    {
        close();
    }
    int nRet = MV_CC_CreateHandle(&m_hDevHandle, &info);
    if (MV_OK != nRet)
    {
        m_hDevHandle = nullptr;
        qWarning() << "MV_CC_CreateHandle fail!";
        return false;
    }

    //连接设备
    unsigned int nAccessMode = MV_ACCESS_Exclusive;
    unsigned short nSwitchoverKey = 0;
    nRet = MV_CC_OpenDevice(m_hDevHandle, nAccessMode, nSwitchoverKey);
    if (MV_OK != nRet)
    {
        qDebug() << "error: MV_CC_OpenDevice fail";
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = 0;
        return false;
    }

    registerExceptionCallBack();
    return registerImageCallBack();
}

bool CameraHikVision::openDefaultCamera()
{
    unsigned int nTLayerType = MV_GIGE_DEVICE | MV_USB_DEVICE;
    MV_CC_DEVICE_INFO_LIST m_stDevList = {0};
    int nRet = MV_CC_EnumDevices(nTLayerType, &m_stDevList);
    if (MV_OK != nRet)
    {
        qWarning("error: EnumDevices fail [%x]", nRet);
        return false;
    }
    if (m_stDevList.nDeviceNum == 0)
    {
        qWarning("no camera found!");
        return false;
    }


    //选择查找到的第一台在线设备，创建设备句柄
    int nDeviceIndex = 0;
    MV_CC_DEVICE_INFO m_stDevInfo = {0};
    memcpy(&m_stDevInfo, m_stDevList.pDeviceInfo[nDeviceIndex], sizeof(MV_CC_DEVICE_INFO));
    nRet = MV_CC_CreateHandle(&m_hDevHandle, &m_stDevInfo);
    if (MV_OK != nRet)
    {
        qWarning() << "MV_CC_CreateHandle fail!";
        return false;
    }

    //连接设备
    unsigned int nAccessMode = MV_ACCESS_Exclusive;
    unsigned short nSwitchoverKey = 0;
    nRet = MV_CC_OpenDevice(m_hDevHandle, nAccessMode, nSwitchoverKey);
    if (MV_OK != nRet)
    {
        qDebug() << "error: OpenDevice fail";
        return false;
    }

    registerExceptionCallBack();
    return registerImageCallBack();
}


AVPixelFormat CameraHikVision::getBayer8Patern()
{
    if (nullptr == m_hDevHandle)
    {
        return AV_PIX_FMT_GRAY8;
    }

    //Get Enum parameters
    MVCC_ENUMVALUE struEnumValue = {0};

    bool nRet = MV_CC_GetEnumValue(m_hDevHandle, "PixelFormat", &struEnumValue);
    if (MV_OK != nRet)
    {
        qWarning() << "error: GetEnumValue fail";
        return AV_PIX_FMT_GRAY8;
    }
    for(unsigned int i = 0; i < struEnumValue.nSupportedNum; i++ )
    {
        switch (struEnumValue.nSupportValue[i])
        {
        case PixelType_Gvsp_BayerGR8:
            return AV_PIX_FMT_BAYER_GRBG8;
            break;
        case PixelType_Gvsp_BayerRG8:
            return AV_PIX_FMT_BAYER_RGGB8;
            break;
        case PixelType_Gvsp_BayerGB8:
            return AV_PIX_FMT_BAYER_GBRG8;
            break;
        case PixelType_Gvsp_BayerBG8:
            return AV_PIX_FMT_BAYER_BGGR8;
            break;
        default:
            break;
        }
    }
    return AV_PIX_FMT_GRAY8;
}

bool CameraHikVision::setPixelType(AVPixelFormat type)
{
    if (nullptr == m_hDevHandle)
    {
        return false;
    }

    unsigned int enValue = PixelType_Gvsp_Mono8;
    switch (type)
    {
    case AV_PIX_FMT_GRAY8:
        enValue = PixelType_Gvsp_Mono8;
        qDebug() << "set PixelType = PixelType_Gvsp_Mono8";
        break;
    case AV_PIX_FMT_RGB24:
        enValue = PixelType_Gvsp_RGB8_Packed;
        qDebug() << "set PixelType = PixelType_Gvsp_RGB8_Packed";
        break;
    case AV_PIX_FMT_BGRA:
        enValue = PixelType_Gvsp_RGBA8_Packed;
        break;
    case AV_PIX_FMT_BAYER_GRBG8:
        enValue = PixelType_Gvsp_BayerGR8;
        qDebug() << "set PixelType = PixelType_Gvsp_BayerGR8";
        break;
    case AV_PIX_FMT_BAYER_RGGB8:
        enValue = PixelType_Gvsp_BayerRG8;
        qDebug() << "set PixelType = PixelType_Gvsp_BayerRG8";
        break;
    case AV_PIX_FMT_BAYER_BGGR8:
        enValue = PixelType_Gvsp_BayerBG8;
        qDebug() << "set PixelType = PixelType_Gvsp_BayerBG8";
        break;
    case AV_PIX_FMT_BAYER_GBRG8:
        enValue = PixelType_Gvsp_BayerGB8;
        qDebug() << "set PixelType = PixelType_Gvsp_BayerGB8";
        break;
    case AV_PIX_FMT_YUYV422:
        enValue = PixelType_Gvsp_YUV422_YUYV_Packed;
        break;
    default:
        enValue = PixelType_Gvsp_Mono8;
        break;
    }
    int nRet = MV_CC_SetPixelFormat(m_hDevHandle, enValue);
    if (MV_OK != nRet)
    {
        qWarning() << "error: SetPixelFormat fail";

        return false;
    }
    qInfo() << "SetPixelFormat() success";

    return true;
}

void CameraHikVision::close()
{
    if (NULL == m_hDevHandle)
    {
        qDebug() << "error: Close Device fail";
        return;
    }
    stopGrabbing();
    MV_CC_DestroyHandle(m_hDevHandle);
    m_hDevHandle = 0;
}

bool CameraHikVision::saveDeviceControlSettings(QSettings &settings)
{
    if( m_hDevHandle == nullptr ) return false;
    //获取相机用户ID信息
    MVCC_STRINGVALUE struStrValue = {0};
    memset(&struStrValue, 0, sizeof(MVCC_STRINGVALUE));

    int nRet = MV_CC_GetStringValue(m_hDevHandle, "DeviceVendorName", &struStrValue);
    if (MV_OK == nRet)
    {
        QString DeviceVendorName(struStrValue.chCurValue);
        settings.setValue("DeviceControl/DeviceVendorName", DeviceVendorName);
    }

    nRet = MV_CC_GetStringValue(m_hDevHandle, "DeviceModelName", &struStrValue);
    if (MV_OK == nRet)
    {
        QString DeviceModelName(struStrValue.chCurValue);
        settings.setValue("DeviceControl/DeviceModelName", DeviceModelName);
    }

    nRet = MV_CC_GetStringValue(m_hDevHandle, "DeviceManufacturerInfo", &struStrValue);
    if (MV_OK == nRet)
    {
        QString DeviceManufacturerInfo(struStrValue.chCurValue);
        settings.setValue("DeviceControl/DeviceManufacturerInfo", DeviceManufacturerInfo);
    }

    nRet = MV_CC_GetStringValue(m_hDevHandle, "DeviceVersion", &struStrValue);
    if (MV_OK == nRet)
    {
        QString DeviceVersion(struStrValue.chCurValue);
        settings.setValue("DeviceControl/DeviceVersion", DeviceVersion);
    }

    nRet = MV_CC_GetStringValue(m_hDevHandle, "DeviceFirmwareVersion", &struStrValue);
    if (MV_OK == nRet)
    {
        QString DeviceFirmwareVersion(struStrValue.chCurValue);
        settings.setValue("DeviceControl/DeviceFirmwareVersion", DeviceFirmwareVersion);
    }

    nRet = MV_CC_GetStringValue(m_hDevHandle, "DeviceSerialNumber", &struStrValue);
    if (MV_OK == nRet)
    {
        QString DeviceSerialNumber(struStrValue.chCurValue);
        settings.setValue("DeviceControl/DeviceSerialNumber", DeviceSerialNumber);
    }

    nRet = MV_CC_GetStringValue(m_hDevHandle, "DeviceID", &struStrValue);
    if (MV_OK == nRet)
    {
        QString DeviceID(struStrValue.chCurValue);
        settings.setValue("DeviceControl/DeviceID", DeviceID);
    }

    nRet = MV_CC_GetStringValue(m_hDevHandle, "DeviceUserID", &struStrValue);
    if (MV_OK == nRet)
    {
        QString DeviceUserID(struStrValue.chCurValue);
        settings.setValue("DeviceControl/DeviceUserID", DeviceUserID);
    }

    nRet = MV_CC_GetDeviceUserID(m_hDevHandle, &struStrValue);
    if (MV_OK == nRet)
    {
        QString UserID(struStrValue.chCurValue);
        settings.setValue("DeviceControl/UserID", UserID);
    }
    return true;
}


bool CameraHikVision::saveAnalogControlSettings(QSettings &settings)
{
    if( m_hDevHandle == nullptr ) return false;
    return m_cameraImageProcessing->writeSettings(settings);
}


bool CameraHikVision::saveAcquisitionControlSettings(QSettings &settings)
{
    if( m_hDevHandle == nullptr ) return false;
    //获取相机采集帧率信息
    MVCC_FLOATVALUE struFloatValue = {0, 0, 0, {0, 0, 0, 0}};
    int nRet = MV_CC_GetFloatValue(m_hDevHandle, "AcquisitionFrameRate", &struFloatValue);
    if (MV_OK == nRet)
    {
        settings.setValue("AcquisitionControl/AcquisitionFrameRate", static_cast<double>(struFloatValue.fCurValue));
    }

    MVCC_ENUMVALUE struEnumValue = {0, 0, 0};
    nRet = MV_CC_GetExposureAutoMode(m_hDevHandle, &struEnumValue);
    if (MV_OK == nRet)
    {
        settings.setValue("AcquisitionControl/AutoExposure", struEnumValue.nCurValue == MV_EXPOSURE_AUTO_MODE_CONTINUOUS);
    }
    MVCC_INTVALUE struIntValue = {0, 0, 0, 0, {0, 0, 0, 0}};
    nRet = MV_CC_GetAutoExposureTimeLower(m_hDevHandle, &struIntValue);
    if (MV_OK == nRet)
    {
        settings.setValue("AcquisitionControl/AutoExposureTimeLower", struIntValue.nCurValue);
    }
    nRet = MV_CC_GetAutoExposureTimeUpper(m_hDevHandle, &struIntValue);
    if (MV_OK == nRet)
    {
        settings.setValue("AcquisitionControl/AutoExposureTimeUpper", struIntValue.nCurValue);
    }

    nRet = MV_CC_GetExposureTime(m_hDevHandle, &struFloatValue);
    if (MV_OK == nRet)
    {
        settings.setValue("AcquisitionControl/ExposureTime",static_cast<double> (struFloatValue.fCurValue));
    }

    //qDebug() << "EXP = " << struFloatValue.fCurValue;

    //Get bool parameters
    bool bValue = 0;
    nRet = MV_CC_GetBoolValue(m_hDevHandle, "HDREnable", &bValue);
    if (MV_OK == nRet)
    {
        settings.setValue("AcquisitionControl/HDREnable", bValue);
    }

    nRet = MV_CC_GetIntValue(m_hDevHandle, "HDRSelector", &struIntValue);
    if (MV_OK == nRet)
    {
        settings.setValue("AcquisitionControl/HDRSelector", struIntValue.nCurValue);
    }

    nRet = MV_CC_GetIntValue(m_hDevHandle, "HDRShuter", &struIntValue);
    if (MV_OK == nRet)
    {
        settings.setValue("AcquisitionControl/HDRShuter", struIntValue.nCurValue);
    }

    nRet = MV_CC_GetFloatValue(m_hDevHandle, "HDRGain", &struFloatValue);
    if (MV_OK == nRet)
    {
        settings.setValue("AcquisitionControl/HDRGain",static_cast<double> (struFloatValue.fCurValue));
    }
    return true;

}

bool CameraHikVision::loadAcquisitionControlSettings(QSettings &settings)
{
    bool ok;
    if( m_hDevHandle == nullptr ) return false;
    double fValue = settings.value("AcquisitionControl/FrameRate", 20.0).toDouble(&ok);
    if(ok)
    {
        setFrameRate(fValue);
    }

    bool on = settings.value("AcquisitionControl/AutoExposure", false).toBool();
    if(on)
    {
        setExposureTimeMode(IndustryCamera::CONTINUOUS);
    }
    else
    {
        setExposureTimeMode(IndustryCamera::OFF);
        fValue = settings.value("AcquisitionControl/ExposureTime", 1000).toDouble();
        setExposureTime(fValue);
    }
    return true;
}

bool CameraHikVision::saveTLayerControlSettings(QSettings &settings)
{
    if( m_hDevHandle == nullptr ) return false;
    // 获取设备信息
    MV_CC_DEVICE_INFO struDeviceInfo;
    memset(&struDeviceInfo, 0, sizeof(MV_CC_DEVICE_INFO));
    int nRet = MV_CC_GetDeviceInfo(m_hDevHandle, &struDeviceInfo);
    if (MV_OK == nRet)
    {
        settings.setValue( "TLayerControl/GevVersionMajor", struDeviceInfo.nMajorVer );
        settings.setValue( "TLayerControl/GevVersionMinor", struDeviceInfo.nMinorVer );
        settings.setValue( "TLayerControl/MacAddrHigh", struDeviceInfo.nMacAddrHigh );
        settings.setValue( "TLayerControl/MacAddrLow", struDeviceInfo.nMacAddrLow );
        settings.setValue( "TLayerControl/TLayerType", struDeviceInfo.nTLayerType );
    }

    return true;
}

bool CameraHikVision::savePixelFormatSettings(QSettings &settings)
{
    if( m_hDevHandle == nullptr ) return false;
    //获取相机图像的像素格式
    MVCC_ENUMVALUE struEnumValue = {0};
    int nRet = MV_CC_GetPixelFormat(m_hDevHandle, &struEnumValue);
    if (MV_OK == nRet)
    {
        switch (struEnumValue.nCurValue)
        {
        case PixelType_Gvsp_Mono8:
            settings.setValue("Basic/PixelFomat", AV_PIX_FMT_GRAY8);
            break;
        case PixelType_Gvsp_BayerBG8:
            settings.setValue("Basic/PixelFomat", AV_PIX_FMT_BAYER_BGGR8);
            break;
        case PixelType_Gvsp_BayerGB8:
            settings.setValue("Basic/PixelFomat", AV_PIX_FMT_BAYER_GBRG8);
            break;
        case PixelType_Gvsp_BayerRG8:
            settings.setValue("Basic/PixelFomat", AV_PIX_FMT_BAYER_RGGB8);
            break;
        case PixelType_Gvsp_BayerGR8:
            settings.setValue("Basic/PixelFomat", AV_PIX_FMT_BAYER_GRBG8);
            break;
        case PixelType_Gvsp_RGB8_Packed:
            settings.setValue("Basic/PixelFomat", AV_PIX_FMT_RGB24);
            break;
        case PixelType_Gvsp_RGBA8_Packed:
            settings.setValue("Basic/PixelFomat", AV_PIX_FMT_BGRA);
            break;
        case PixelType_Gvsp_YUV422_Packed:
            settings.setValue("Basic/PixelFomat", AV_PIX_FMT_YUYV422);
            break;
        case PixelType_Gvsp_YUV422_YUYV_Packed:
            settings.setValue("Basic/PixelFomat", AV_PIX_FMT_YUYV422);
            break;
        }
    }
    return true;
}

bool CameraHikVision::loadPixelFormatSettings(QSettings &settings)
{
    if( m_hDevHandle == nullptr ) return false;
    AVPixelFormat ptValue = static_cast<AVPixelFormat> (settings.value("Basic/PixelFomat", AV_PIX_FMT_GRAY8).toInt() );
    return setPixelType(ptValue);
}

bool CameraHikVision::saveCameraSizeSettings(QSettings &settings)
{
    if( m_hDevHandle == nullptr ) return false;

    //获取相机图像基本信息
    MV_IMAGE_BASIC_INFO mstruBasicInfo = {0};
    int nRet = MV_CC_GetImageInfo(m_hDevHandle, &mstruBasicInfo);
    if (MV_OK == nRet)
    {
        settings.setValue("Basic/Width", mstruBasicInfo.nWidthValue);
        settings.setValue("Basic/Height", mstruBasicInfo.nHeightValue);
    }

    //获取AOI偏移量X值
    MVCC_INTVALUE struIntValue = {0};
    nRet = MV_CC_GetAOIoffsetX(m_hDevHandle, &struIntValue);
    if (MV_OK == nRet)
    {
        settings.setValue("Basic/OffsetX", struIntValue.nCurValue);
    }
    nRet = MV_CC_GetAOIoffsetY(m_hDevHandle, &struIntValue);
    if (MV_OK == nRet)
    {
        settings.setValue("Basic/OffsetY", struIntValue.nCurValue);
    }
    return true;
}

bool CameraHikVision::loadCameraSizeSettings(QSettings &settings)
{
    if( m_hDevHandle == nullptr ) return false;
    int nValue = settings.value("Basic/OffsetX", 0).toInt();
    int nRet = MV_CC_SetAOIoffsetX(m_hDevHandle, nValue);
    qDebug() << "Basic/OffsetX = " << nValue;

    nValue = settings.value("Basic/OffsetY", 0).toInt();
    nRet = MV_CC_SetAOIoffsetY(m_hDevHandle, nValue);
    qDebug() << "Basic/OffsetY = " << nValue;

    nValue = settings.value("Basic/Width", 0).toInt();
    if(nValue)
    {
        nRet = MV_CC_SetWidth(m_hDevHandle, nValue);
        qDebug() << "Basic/Width = " << nValue;
    }

    nValue = settings.value("Basic/Height", 0).toInt();
    if(nValue)
    {
        nRet = MV_CC_SetHeight(m_hDevHandle, nValue);
        qDebug() << "Basic/Height = " << nValue;
    }
    return true;
}

bool CameraHikVision::saveSettings(QSettings &settings)
{
    saveTLayerControlSettings(settings);
    saveDeviceControlSettings(settings);
    saveAcquisitionControlSettings(settings);
    savePixelFormatSettings(settings);
    saveCameraSizeSettings(settings);
    return saveAnalogControlSettings(settings);
}

bool CameraHikVision::loadAnalogControlSettings(QSettings &settings)
{
    if( m_hDevHandle == nullptr ) return false;
    return m_cameraImageProcessing->loadSettings(settings);
}

bool CameraHikVision::loadSettings(QSettings &settings)
{
    loadCameraSizeSettings(settings);
    loadAcquisitionControlSettings(settings);
    return true;
}


bool CameraHikVision::saveSettingsToFile(QString fileName)
{
    if (nullptr == m_hDevHandle)
    {
        return false;
    }
    fileName.replace('\\','/');
    int pos = fileName.lastIndexOf('/');
    QString path(fileName.left(pos));
    QDir dir;
    dir.mkpath(path);

    //TODO:应该先判读这个路径是否存在。文件名是否是 ini 结尾的。
    if(fileName.endsWith(".ini", Qt::CaseInsensitive))
    {
        QSettings settings( fileName, QSettings::IniFormat, this );
        return saveSettings(settings);
    }

    return false;
}

bool CameraHikVision::loadSettingsFromFile(QString fileName)
{
    if (NULL == m_hDevHandle)
    {
        return false;
    }
    fileName.replace('\\','/');
    int pos = fileName.lastIndexOf('/');
    QString path(fileName.left(pos));
    QDir dir;
    dir.mkpath(path);

    if( dir.exists(fileName) == false)
    {
        qWarning() << "Camera.ini not exist";
        return false;
    }

    if(fileName.endsWith(".ini", Qt::CaseInsensitive))
    {
        QSettings settings( fileName, QSettings::IniFormat, this );
        return loadSettings(settings);
    }
    return false;
}

bool CameraHikVision::startGrabbing()
{
    m_imageCount = 0;
    if (NULL == m_hDevHandle)
    {
        qDebug() << "error: OpenDevice fail";
        return false;
    }
    int nRet = MV_CC_StartGrabbing(m_hDevHandle);
    if (MV_OK != nRet)
    {
        qDebug() << "error: MV_CC_StartGrabbing fail";
        return false;
    }
    return true;
}

bool CameraHikVision::stopGrabbing()
{
    if (NULL == m_hDevHandle)
    {
        return false;
    }
    int nRet = MV_CC_StopGrabbing(m_hDevHandle);
    if (MV_OK != nRet)
    {
        qDebug() << "error: MV_CC_StopGrabbing fail";
        return false;
    }
    return true;
}

bool CameraHikVision::registerExceptionCallBack()
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    int nRet = MV_CC_RegisterExceptionCallBack(m_hDevHandle, ExceptionCallBack, this);
    if (MV_OK != nRet)
    {
        qWarning() << "error: RegisterExceptionCallBack() fail";
        return false;
    }
    qInfo() << "RegisterExceptionCallBack() Success!";

    return true;
}

bool CameraHikVision::registerImageCallBack()
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    int nRet = MV_CC_RegisterImageCallBack(m_hDevHandle, ImageCallBack, this);
    if (MV_OK != nRet)
    {
        qWarning() << "error: RegisterImageCallBack() fail";
        return false;
    }
    qInfo() << "RegisterImageCallBack() Success!";
    return true;
}

bool CameraHikVision::setExposureTimeMode(AutoExposureMode mode)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    int nRet;
    switch (mode) {
    case IndustryCamera::OFF:
        nRet = MV_CC_SetExposureAutoMode(m_hDevHandle, MV_EXPOSURE_AUTO_MODE_OFF);
        break;
    case IndustryCamera::CONTINUOUS:
        nRet = MV_CC_SetExposureAutoMode(m_hDevHandle, MV_EXPOSURE_AUTO_MODE_CONTINUOUS);
        break;
    case IndustryCamera::ONCE:
        nRet = MV_CC_SetExposureAutoMode(m_hDevHandle, MV_EXPOSURE_AUTO_MODE_ONCE);
        break;
    }
    if (MV_OK != nRet)
    {
        qDebug() << "error: SetExposureAutoMode MV_EXPOSURE_AUTO_MODE_ONCE fail";
        return false;
    }
    return true;
}

//bool CameraHikVision::setExpoureTimeOnePush()
//{
//    if (NULL == m_hDevHandle)
//    {
//         return false;
//    }
//    int nRet = MV_CC_SetExposureAutoMode(m_hDevHandle, MV_EXPOSURE_AUTO_MODE_ONCE);
//    if (MV_OK != nRet)
//    {
//        qDebug() << "error: SetExposureAutoMode MV_EXPOSURE_AUTO_MODE_ONCE fail";
//        return false;
//    }
//    return true;
//}


bool CameraHikVision::showVCDPropertyPage()
{
    m_dialog->initNodeInfo();
    m_dialog->show();
    return false;
}

bool CameraHikVision::setHeartBeatTimeout(int ms)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //获取设置相机心跳
    MVCC_INTVALUE struIntValue = {0};
    int nRet = MV_CC_GetHeartBeatTimeout(m_hDevHandle, &struIntValue);
    if (MV_OK != nRet)
    {
        qDebug() << "error: GetHeartBeatTimeout fail";
        return false;
    }
    unsigned int value = ms;
    value = qBound(struIntValue.nMin, value, struIntValue.nMax);
    nRet = MV_CC_SetHeartBeatTimeout(m_hDevHandle, value);
    if (MV_OK != nRet)
    {
        qDebug() << "error: SetHeartBeatTimeout fail";
        return false;
    }
    return true;
}

bool CameraHikVision::setAOIRegion(QRect rect)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    int nRet = MV_CC_SetWidth(m_hDevHandle, rect.width());
    if (MV_OK != nRet)
    {
        qDebug() << "error: MV_CC_SetWidth fail";
        return false;
    }
    nRet = MV_CC_SetHeight(m_hDevHandle, rect.height());
    if (MV_OK != nRet)
    {
        qDebug() << "error: MV_CC_SetHeight fail";
        return false;
    }
    nRet = MV_CC_SetAOIoffsetX(m_hDevHandle, rect.left());
    if (MV_OK != nRet)
    {
        qDebug() << "error: SetAOIoffsetX fail";
        return false;
    }
    nRet = MV_CC_SetAOIoffsetY(m_hDevHandle, rect.top());
    if (MV_OK != nRet)
    {
        qDebug() << "error: SetAOIoffsetY fail";
        return false;
    }
    return true;

}

bool CameraHikVision::frameRate(double &fps)
{
    fps = 0;
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //获取相机采集帧率信息
    MVCC_FLOATVALUE struFloatValue = {0};

    int nRet = MV_CC_GetFrameRate(m_hDevHandle, &struFloatValue);
    if (MV_OK != nRet)
    {
        qDebug() << "error: GetFrameRate fail";
        return false;
    }
    fps = struFloatValue.fCurValue;
    return false;
}

bool CameraHikVision::setFrameRate(double fps)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //设置相机采集帧率

    int nRet = MV_CC_SetFrameRate(m_hDevHandle, fps);
    if (MV_OK != nRet)
    {
        qDebug() << "error: SetFrameRate fail";
        return false;
    }

    return true;
}

bool CameraHikVision::autoExposure(bool &on)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    MVCC_ENUMVALUE struEnumValue = {0};
    on  = false;
    int nRet = MV_CC_GetEnumValue(m_hDevHandle, "ExposureAuto", &struEnumValue);
    if (MV_OK == nRet)
    {
        on = (struEnumValue.nCurValue == MV_EXPOSURE_AUTO_MODE_CONTINUOUS);
        qDebug() << "Get ExposureAuto Success";
        return true;
    }
    qWarning() << "Get ExposureAuto Fail";
    return false;
}

bool CameraHikVision::autoExposureTimeLowerLimitRange(int &nMin, int &nMax, int &nValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //Get int parameter
    MVCC_INTVALUE struIntValue = {0};

    int nRet = MV_CC_GetIntValue(m_hDevHandle, "AutoExposureTimeLowerLimit", &struIntValue);
    if (MV_OK != nRet)
    {
        qWarning() << "error: Get AutoExposureTimeLowerLimit fail";
        return false;
    }
    nMin = struIntValue.nMin;
    nMax = struIntValue.nMax;
    nValue = struIntValue.nCurValue;
    return true;
}

bool CameraHikVision::autoExposureTimeUpperLimitRange(int &nMin, int &nMax, int &nValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //Get int parameter
    MVCC_INTVALUE struIntValue = {0};

    int nRet = MV_CC_GetIntValue(m_hDevHandle, "AutoExposureTimeUpperLimit", &struIntValue);
    if (MV_OK != nRet)
    {
        qWarning() << "error: Get AutoExposureTimeUpperLimit fail";
        return false;
    }
    nMin = struIntValue.nMin;
    nMax = struIntValue.nMax;
    nValue = struIntValue.nCurValue;
    return true;
}

bool CameraHikVision::exposureRange(double &min, double &max, double &fValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //获取相机曝光值
    MVCC_FLOATVALUE struFloatValue = {0};

    int nRet = MV_CC_GetExposureTime(m_hDevHandle, &struFloatValue);
    if (MV_OK != nRet)
    {
        qDebug() << "error: GetExposureTime fail";
        return false;
    }
    min = struFloatValue.fMin;
    max = struFloatValue.fMax;
    fValue = struFloatValue.fCurValue;

    qDebug() << "ExposureTime Min = " << min;
    qDebug() << "ExposureTime Max = " << max;
    qDebug() << "ExposureTime = " << fValue;
    return true;
}

bool CameraHikVision::imageSize(QSize &size, QSize &max, QSize &min)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    MVCC_INTVALUE struIntWidth = {0};
    MVCC_INTVALUE struIntHeight = {0};
    int nRet = MV_CC_GetWidth(m_hDevHandle, &struIntWidth);
    nRet = MV_CC_GetHeight(m_hDevHandle, &struIntHeight);
    if (MV_OK != nRet)
    {
        qDebug() << "error: MV_CC_GetHeight fail";
        return false;
    }
    size.setHeight(struIntHeight.nCurValue);
    size.setWidth(struIntWidth.nCurValue);
    max.setHeight(struIntHeight.nMax);
    max.setWidth(struIntWidth.nMax);
    min.setHeight(struIntHeight.nMin);
    min.setWidth(struIntWidth.nMin);
    return true;
}

//bool CameraHikVision::setAutoExposureTime(bool on)
//{
//    //设置自动曝光模式
//    unsigned int nValue = MV_EXPOSURE_AUTO_MODE_OFF;
//    if(on) nValue = MV_EXPOSURE_AUTO_MODE_CONTINUOUS;
//    int nRet = MV_CC_SetExposureAutoMode(m_hDevHandle, nValue);
//    if (MV_OK != nRet)
//    {
//        qDebug() << "error: SetExposureAutoMode fail";
//        return false;
//    }
//    return true;
//}

bool CameraHikVision::exposureTime(double &us)
{
    //获取相机曝光值
    MVCC_FLOATVALUE struFloatValue = {0};

    int nRet = MV_CC_GetExposureTime(m_hDevHandle, &struFloatValue);
    if (MV_OK != nRet)
    {
        qDebug() << "error: GetExposureTime fail";
        return false;
    }
    us = struFloatValue.fCurValue;
    return true;
}

bool CameraHikVision::setExposureTime(double us)
{
    //获取相机曝光值
    MVCC_FLOATVALUE struFloatValue = {0};

    int nRet = MV_CC_GetExposureTime(m_hDevHandle, &struFloatValue);
    if (MV_OK != nRet)
    {
        qDebug() << "error: GetExposureTime fail";
        return false;
    }

    //设置设备增益
    float fValue = us;
    fValue = qBound(struFloatValue.fMin, fValue, struFloatValue.fMax);
    nRet = MV_CC_SetExposureTime(m_hDevHandle, fValue);
    if (MV_OK != nRet)
    {
        qDebug() << "error: SetExposureTime fail";
        return false;
    }
    qDebug() << "Exposure Time = " << fValue;
    return true;
}

void __stdcall CameraHikVision::ExceptionCallBack(unsigned int nMsgType, void *pUser)
{
    CameraHikVision *camera = static_cast<CameraHikVision *> (pUser);
    if(0x00008001 == nMsgType)
    {
        emit camera->deviceLost();
    }
}

void __stdcall CameraHikVision::ImageCallBack(unsigned char* pData, MV_FRAME_OUT_INFO* pFrameInfo, void* pUser)
{
    if (pFrameInfo)
    {
        CameraHikVision *camera = static_cast<CameraHikVision *> (pUser);
        MvGvspPixelType pixeltype = pFrameInfo->enPixelType;
        QSize size(pFrameInfo->nWidth, pFrameInfo->nHeight);
        camera->m_imageCount++;
        AVPixelFormat format = AV_PIX_FMT_NONE;
        int stride = pFrameInfo->nWidth * 2;
        switch(pixeltype)
        {
        case PixelType_Gvsp_Mono8:
            format = AV_PIX_FMT_GRAY8;
            stride = pFrameInfo->nWidth;
            //emit camera->Mono8ImageChanged(pData, size);
            break;
        case PixelType_Gvsp_RGB8_Packed:
            format = AV_PIX_FMT_RGB24;
            stride = pFrameInfo->nWidth * 3;
            //emit camera->RGB24ImageChanged(pData, size);
            break;
        case PixelType_Gvsp_YUV422_YUYV_Packed:
            format = AV_PIX_FMT_YUYV422;
            //emit camera->YUYVImageChanged(pData, size);
            break;
        case PixelType_Gvsp_BayerGR8:
            format = AV_PIX_FMT_BAYER_GRBG8;
            //emit camera->Bayer8ImageChanged(pData, size);
            break;
        case PixelType_Gvsp_BayerRG8:
            format = AV_PIX_FMT_BAYER_RGGB8;
            //emit camera->Bayer8ImageChanged(pData, size);
            break;
        case PixelType_Gvsp_BayerGB8:
            format = AV_PIX_FMT_BAYER_GBRG8;
            //emit camera->Bayer8ImageChanged(pData, size);
            break;
        case PixelType_Gvsp_BayerBG8:
            format = AV_PIX_FMT_BAYER_BGGR8;
            //emit camera->Bayer8ImageChanged(pData, size);
            break;
        default:
            break;
        }
        //qDebug() << "ImageDataChanged";
        VideoFrame frame(pData, stride, size, format);
        frame.setExtraInfo(camera->m_imageCount++, QDateTime::currentDateTime());
        emit camera->videoFrameReady(frame);
        //emit camera->ImageDataChanged(pData, stride, size, format);
    }
}

} //namespace Qly
