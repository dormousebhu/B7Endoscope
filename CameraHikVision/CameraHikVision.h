/****************************************************************************
** file: HikVisionCamera.h
** brief: 海康相机的支持代码
** Copyright (C) LiYuan
** Author: LiYuan
** E-Mail: 18069211#qq(.)com
** Version 2.0.1
** Last modified: 2019.03.20
** Modified By: LiYuan
****************************************************************************/

#pragma execution_character_set("utf-8")

#ifndef HIKVISION_CAMERA_H
#define HIKVISION_CAMERA_H

///
/// 海康相机的支持代码。
///
#include "MvCameraControl.h"
#include "IndustryCamera.h"
#include "DialogCameraSetup.h"
#include <QRect>
#include <QString>
#include <QSettings>

namespace Qly {

class HikVisionCameraInfo: public IndustryCameraInfo
{
public:
    QString description()  Q_DECL_OVERRIDE;//
    QString deviceName() Q_DECL_OVERRIDE;//
    bool isNull() Q_DECL_OVERRIDE;
    QList<IndustryCameraInfo*> availableCameras() Q_DECL_OVERRIDE;
    IndustryCameraInfo* defaultCamera() Q_DECL_OVERRIDE;//
private:
    MV_CC_DEVICE_INFO m_info;
    QString m_manufacturerName;
};

class CameraHikVision;

class HikVisionCameraImageProcessing : public CameraImageProcessing
{
    friend CameraHikVision;
    public:
    bool setWhiteBalanceMode(WhiteBalanceMode mode) Q_DECL_OVERRIDE;
    bool setWhiteBalance(int red, int green, int blue) Q_DECL_OVERRIDE;
    bool whiteBalance(int &red, int &green, int &blue) Q_DECL_OVERRIDE;
    bool autoWhiteBalance(bool &on);
    bool setWhiteBalanceRed(int red) Q_DECL_OVERRIDE;
    bool setWhiteBalanceGreen(int green) Q_DECL_OVERRIDE;
    bool setWhiteBalanceBlue(int blue) Q_DECL_OVERRIDE;
    bool whiteBalanceRedRange(int &nMin, int &nMax, int &nValue) Q_DECL_OVERRIDE;
    bool whiteBalanceGreenRange(int &nMin, int &nMax, int &nValue) Q_DECL_OVERRIDE;
    bool whiteBalanceBlueRange(int &nMin, int &nMax, int &nValue) Q_DECL_OVERRIDE;

    bool setBrightness(double value) Q_DECL_OVERRIDE;
    bool brightness(double &nValue) Q_DECL_OVERRIDE;

    bool setGamma(double v) Q_DECL_OVERRIDE;
    bool gamma(double &v) Q_DECL_OVERRIDE;
    bool gammaEnable(bool &on) Q_DECL_OVERRIDE;
    bool setGammaEnable(bool on) Q_DECL_OVERRIDE;

    bool setGainMode(GainMode mode)  Q_DECL_OVERRIDE;
    bool setGain(double fValue) Q_DECL_OVERRIDE;
    bool gain(double &fValue) Q_DECL_OVERRIDE;
    bool autoGain(bool &on);

    bool setSharpness(double nValue) Q_DECL_OVERRIDE;
    bool sharpness(double &nValue) Q_DECL_OVERRIDE;
    bool sharpnessEnable(bool &on) Q_DECL_OVERRIDE;
    bool setSharpnessEnable(bool on) Q_DECL_OVERRIDE;

    bool setBlackLevel(double nValue) Q_DECL_OVERRIDE;
    bool blackLevel(double &nValue) Q_DECL_OVERRIDE;

    bool setContrast(double nValue) Q_DECL_OVERRIDE;
    bool contrast(double &nValue)  Q_DECL_OVERRIDE;

    bool setSaturation(double nValue) Q_DECL_OVERRIDE;
    bool saturation(double &value) Q_DECL_OVERRIDE;
    bool setSaturationEnable(bool on) Q_DECL_OVERRIDE;
    bool saturationEnable(bool &on) Q_DECL_OVERRIDE;

    bool setDenoise(double value) Q_DECL_OVERRIDE;
    bool denoise(double &nValue) Q_DECL_OVERRIDE;
    bool denoiseEnable(bool &on) Q_DECL_OVERRIDE;
    bool setDenoiseEnable(bool on) Q_DECL_OVERRIDE;

    bool setHue(double nValue) Q_DECL_OVERRIDE;
    bool hue(double &value) Q_DECL_OVERRIDE;
    bool hueEnable(bool &on) Q_DECL_OVERRIDE;
    bool setHueEnable(bool on) Q_DECL_OVERRIDE;

public:
    bool gammaRange(double &fMin, double &fMax, double &fValue) Q_DECL_OVERRIDE;
    bool autoGainLowerLimitRange(double &fMin, double &fMax, double &fValue);
    bool autoGainUpperLimitRange(double &fMin, double &fMax, double &fValue);

    bool autoGainLowerLimit(double &fValue);
    bool autoGainUpperLimit(double &fValue);

    bool sharpnessRange(int &nMin, int &nMax, int &nValue);
    bool brightnessRange(int &nMin, int &nMax, int &nValue);
    bool contrastRange(int &nMin, int &nMax, int &nValue);
    bool blackLevelRange(int &nMin, int &nMax, int &nValue);
    bool gainRange(double &fMin, double &fMax, double &fValue) Q_DECL_OVERRIDE;
    bool hueRange(int &nMin, int &nMax, int &nValue);
    bool saturationRange(int &nMin, int &nMax, int &nValue);
    bool denoiseRange(int &nMin, int &nMax, int &nValue);

private:
    void * m_hDevHandle;
};


class CameraHikVision : public IndustryCamera
{
public:
    explicit CameraHikVision(QObject *parent = nullptr);
    ~CameraHikVision() Q_DECL_OVERRIDE;
    QString cameraName();
    bool imageSize(QSize &size, QSize &max, QSize &min);
    bool exposureRange(double &fMin, double &fMax, double &fValue);

    bool autoExposureTimeLowerLimitRange(int &nMin, int &nMax, int &nValue);
    bool autoExposureTimeUpperLimitRange(int &nMin, int &nMax, int &nValue);

    AVPixelFormat getBayer8Patern();
    bool isLive() Q_DECL_OVERRIDE;
public slots:
    bool open(MV_CC_DEVICE_INFO info);
    bool openCameraByID(QString deviceName);
    bool openDefaultCamera() Q_DECL_OVERRIDE;
    bool startGrabbing() Q_DECL_OVERRIDE;
    bool stopGrabbing() Q_DECL_OVERRIDE;
    bool setPixelType(AVPixelFormat type) Q_DECL_OVERRIDE;
    bool saveSettingsToFile(QString fileName) Q_DECL_OVERRIDE;
    bool loadSettingsFromFile(QString fileName) Q_DECL_OVERRIDE;
    bool saveSettings(QSettings &settings);
    bool loadSettings(QSettings &settings);
    void close() Q_DECL_OVERRIDE;

    bool autoWhiteBalance(bool &on);
    bool autoGain(bool &on);
    bool autoBrightness(bool &on);
    bool autoExposure(bool &on);

    //bool setAutoExposureTime(bool on) Q_DECL_OVERRIDE;
    bool setExposureTimeMode(AutoExposureMode mode) Q_DECL_OVERRIDE;
//    bool setExpoureTimeOnePush();

    bool setExposureTime(double us) Q_DECL_OVERRIDE;
    bool exposureTime(double &us) Q_DECL_OVERRIDE;

    bool showVCDPropertyPage() Q_DECL_OVERRIDE;

    bool frameRate(double &fps) Q_DECL_OVERRIDE;
    bool setFrameRate(double fps) Q_DECL_OVERRIDE;

    bool setAOIRegion(QRect rect);
    bool setHeartBeatTimeout(int ms);

private:
    void * m_hDevHandle;
    DialogCameraSetup * m_dialog;
    bool saveDeviceControlSettings(QSettings &settings);
    bool saveAcquisitionControlSettings(QSettings &settings);
    bool loadAcquisitionControlSettings(QSettings &settings);
    bool saveTLayerControlSettings(QSettings &settings);
    bool saveCameraSizeSettings(QSettings &settings);
    bool loadCameraSizeSettings(QSettings &settings);
    bool savePixelFormatSettings(QSettings &settings);
    bool loadPixelFormatSettings(QSettings &settings);
    bool saveAnalogControlSettings(QSettings &settings);
    bool loadAnalogControlSettings(QSettings &settings);
    bool registerImageCallBack();
    bool registerExceptionCallBack();
    /*相机抓图回调*/
    static void __stdcall ImageCallBack(unsigned char* pData, MV_FRAME_OUT_INFO* pFrameInfo, void* pUser);
    static void __stdcall ExceptionCallBack(unsigned int nMsgType, void *pUser);

};

} //namespace Qly
#endif // HIKVISION_CAMERA_H
