/****************************************************************************
** file: IndustryCamera.h
** brief: 工业相机的支持代码
** Copyright (C) LiYuan
** Author: LiYuan
** E-Mail: 18069211#qq(.)com
** Version 1.0.1
** Last modified: 2019.02.02
** Modified By: LiYuan
****************************************************************************/

#ifndef INDUSTRYCAMERA_H
#define INDUSTRYCAMERA_H

#include <QObject>
#include <QSize>
#include <QString>
#include <QDebug>

extern "C" {
#include "libavutil/pixfmt.h"
}

namespace Qly {

enum CameraType {HIK_GIGE = 1, HIK_USB = 2, TOUP_USB = 4, TIS_GIGE = 8, TIS_USB = 16, SYS_CAM = 32, BASLER_GIGE = 64,  ALL = 0xFFFFFFFF};
//enum PixelFormat {Undefined, Mono8, RGB24, BGR24, RGB32, Bayer8, BayerGR8, BayerRG8, BayerGB8, BayerBG8,
//                  YUYV, YUY2, UYVY, Y8, Y16, JPEG};
class IndustryCameraInfo
{
public:
    virtual QString description()  = 0;//
    virtual QString deviceName() = 0;//
    virtual bool isNull() = 0;
    virtual QList<IndustryCameraInfo*> availableCameras() = 0;
    virtual IndustryCameraInfo* defaultCamera() = 0;//
    virtual ~IndustryCameraInfo() {}
};

/// 工业相机的接口
class IndustryCamera : public QObject
{
    Q_OBJECT
public:

    enum AutoMode {OFF, ONCE, CONTINUOUS};
    explicit IndustryCamera(QObject *parent = 0);
    virtual ~IndustryCamera() {}
    virtual bool isLive() = 0;
signals:
    void ImageDataChanged(const uint8_t *imgBuf, QSize size, AVPixelFormat format);
    void Mono8ImageChanged(const uint8_t *imgBuf, QSize size);
    void RGB24ImageChanged(const uint8_t *imgBuf, QSize size);
    void RGB32ImageChanged(const uint8_t *imgBuf, QSize size);
    void Bayer8ImageChanged(const uint8_t *imgBuf, QSize size);
    void YUV422ImageChanged(const uint8_t *imgBuf, QSize size);
    void YUY2ImageChanged(const uint8_t *imgBuf, QSize size);
    void VYUYImageChanged(const uint8_t *imgBuf, QSize size);
    void UYVYImageChanged(const uint8_t *imgBuf, QSize size);
    void YUYVImageChanged(const uint8_t *imgBuf, QSize size);
    void deviceLost();

public slots:
    // 以下为相机设置的通用接口。除这些接口之外，相机还可以有其他特殊的接口。
    virtual bool openDefaultCamera() = 0;

    /**
     * @brief startLive 启动实时视频采集
     * @return
     */
    virtual bool startGrabbing() = 0;

    /**
     * @brief stop 停止实时视频采集
     * @return
     */
    virtual bool stopGrabbing() = 0;

    /**
     * @brief setPixelType 设置图像格式
     * @param type
     * @return
     */
    virtual bool setPixelType(AVPixelFormat type);

    /**
     * @brief saveSettingsToFile 从文件中加载相机设置参数。
     * 该文件格式为通用格式，并不特定针对某一个相机。因此更换相机后还可以保证新相机的工作参数与旧相机基本一致。
     * 暂时只支持 ini 格式的文件。
     * @param fileName
     */
    virtual bool saveSettingsToFile(QString fileName);

    /**
     * @brief loadSettingsFromFile 将相机的设置参数保存到文件中。
     * 该文件格式为通用格式，并不特定针对某一个相机。因此更换相机后还可以保证新相机的工作参数与旧相机基本一致。
     * @param fileName
     */
    virtual bool loadSettingsFromFile(QString fileName);

    /**
     * @brief close 关闭相机
     */
    virtual void close() = 0;

    /**
     * @brief autoBrightness 设置相机开启或关闭自动亮度调节功能
     * @param on
     * @return true 表示设置成功， false 表示设置失败
     */
    virtual bool setAutoBrightness(bool on);

    /**
     * @brief autoWhiteBalance 设置相机开启或关闭自动白平衡功能
     * @param on 0 表示关闭，1 表示连续
     */
    virtual bool setAutoWhiteBalance(bool on);

    /**
     * @brief setAutoExpoureTime 设置自动曝光
     * @param on
     * @return
     */
    virtual bool setAutoExpoureTime(bool on);

    /**
     * @brief setAutoGain 设置自动增益。通常不要设置开启自动增益。
     * @param on
     * @return
     */
    virtual bool setAutoGain(bool on);

    /**
     * @brief setBrightness 设置亮度值（只有当自动亮度调节关闭时才能设置成功）
     * @param value
     * @return true 表示设置成功， false 表示设置失败
     */
    virtual bool setBrightness(int value);
    virtual bool brightness(int &nValue);

    /**
     * @brief setExpoureTime 设置曝光时间，单位毫秒
     * @param us
     * @return true 表示设置成功， false 表示设置失败
     */
    virtual bool setExpoureTime(double us);
    virtual bool expoureTime(double &us);

    /**
     * @brief setFrameRate 设置帧率
     * @param fps
     * @return true 表示设置成功， false 表示设置失败
     */
    virtual bool setFrameRate(double fps);
    virtual bool frameRate(double &fps);

    /**
     * @brief setWhiteBalance 设置白平衡值（只有当自动白平衡关闭时才能设置成功）
     * @param red
     * @param blue
     * @return true 表示设置成功， false 表示设置失败
     */
    virtual bool setWhiteBalance(int red, int green, int blue);
    virtual bool whiteBalance(int &red, int &green, int &blue);


    virtual bool setGamma(double v);
    virtual bool gamma(double &v);
    virtual bool gammaEnable(bool &on);
    virtual bool setGammaEnable(bool on);

    virtual bool setGain(double fValue);
    virtual bool gain(double &fValue);

    virtual bool setSharpness(int nValue);
    virtual bool sharpness(int &nValue);
    virtual bool sharpnessEnable(bool &on);
    virtual bool setSharpnessEnable(bool on);

    virtual bool setBlackLevel(int nValue);
    virtual bool blackLevel(int &nValue);
    virtual bool blackLevelEnable(bool &on);
    virtual bool setBlackLevelEnable(bool on);

    virtual bool setContrast(int nValue);
    virtual bool contrast(int &nValue);
    virtual bool contrastEnable(bool &on);
    virtual bool setContrastEnable(bool on);

    virtual bool setSaturation(int nValue);
    virtual bool saturation(int &nValue);
    virtual bool saturationEnable(bool &on);
    virtual bool setSaturationEnable(bool on);

    virtual bool setDenoise(int nValue);
    virtual bool denoise(int &nValue);
    virtual bool denoiseEnable(bool &on);
    virtual bool setDenoiseEnable(bool on);

    virtual bool setHue(int nValue);
    virtual bool hue(int &nValue);
    virtual bool hueEnable(bool &on);
    virtual bool setHueEnable(bool on);

    /**
     * @brief showVCDPropertyPage 打开相机参数设置对话框
     * @return
     */
    virtual bool showVCDPropertyPage() = 0;

    int imageCount() {return m_imageCount;}
    void newImage(){m_imageCount ++;}

    QString deviceName() {return m_DeviceModelName;}
    QString deviceID() {return m_DeviceID;}
    QString deviceVendorName(){return m_DeviceVendorName;}
protected:
    int m_imageCount;
    QString m_DeviceVendorName;
    QString m_DeviceModelName;
    QString m_DeviceManufacturerInfo;
    QString m_DeviceVersion;
    QString m_DeviceFirmwareVersion;
    QString m_DeviceSerialNumber;
    QString m_DeviceID;
    QString m_DeviceUserID;
    QString m_UserID;

};

QDebug operator<<(QDebug dbg, const AVPixelFormat &format);
QString PixelFormat2QString(AVPixelFormat format);

} //namespace Qly

#endif // INDUSTRYCAMERA_H
