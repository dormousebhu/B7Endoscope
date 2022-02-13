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
#include <QSettings>
#include <QDateTime>
#include <QMutex>
extern "C" {
#include "libavutil/pixfmt.h"
}

namespace Qly {

/// 辅助类，用于 Camera 向 CameraImage 类传递数据
/// mutex 用于线程间同步，如果 Camera 和 CameraImage 在同一个线程，可以不用这个参数
/// 实际上还有另一种情况，就是 CameraImage 收到数据后是否需要把 imgBuf 所指向的内存空间释放了。
/// 这个功能还没想好怎么处理，所以在这版代码中暂时不考虑这个问题。
struct VideoFrame
{
    VideoFrame() {}
    VideoFrame(uint8_t *imgBuf, QSize size, AVPixelFormat format);
    VideoFrame(uint8_t *imgBuf, int stride, QSize size, AVPixelFormat format);
    VideoFrame(uint8_t *imgBuf, int stride, QSize size, AVPixelFormat format, QMutex *mutex);
    void setData(uint8_t *imgBuf, int stride, QSize size, AVPixelFormat format, QMutex *mutex);
    void setExtraInfo(uint32_t count, const QDateTime &t ) {frameCount = count; datetime = t;}
    AVPixelFormat format = AV_PIX_FMT_NONE;
    uint8_t *pData[4] = {nullptr, nullptr, nullptr, nullptr};
    int bytePerLine[4] = {0, 0, 0, 0};
    int width = 0;
    int height = 0;
    uint32_t frameCount = 0;
    QDateTime datetime;
    QMutex * pMutex = 0;
};

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

/// CameraImageProcessing class provides an interface for image processing related camera settings
class CameraImageProcessing
{
public:
    enum WhiteBalanceMode {
        WhiteBalanceManual = 0,
        WhiteBalanceAutoOnePush = 1,
        WhiteBalanceAutoContinuous = 2};
    enum GainMode{
        GainManual = 0,
        GainAutoOnePush = 1,
        GainAutoContinuous = 2};

    virtual ~CameraImageProcessing();
    virtual bool isAvailable() {return true;}

    virtual bool setWhiteBalanceMode(WhiteBalanceMode mode);
    virtual bool whiteBalanceMode(WhiteBalanceMode &mode);
    virtual bool setManualWhiteBalance(double colorTemperature);
    virtual bool manualWhiteBalance(double &colorTemperature);
    virtual bool setWhiteBalance(int red, int green, int blue);
    virtual bool whiteBalance(int &red, int &green, int &blue);

    virtual bool whiteBalanceRedRange(int &nMin, int &nMax, int &nValue);
    virtual bool whiteBalanceGreenRange(int &nMin, int &nMax, int &nValue);
    virtual bool whiteBalanceBlueRange(int &nMin, int &nMax, int &nValue);

    virtual bool setWhiteBalanceRed(int red);
    virtual bool setWhiteBalanceGreen(int green);
    virtual bool setWhiteBalanceBlue(int blue);

    virtual bool setGain(double fValue); // >=0 , default is 0
    virtual bool gain(double &fValue);
    virtual bool gainRange(double &min, double &max, double &value);
    virtual bool setGainMode(GainMode mode);
    virtual bool gainMode(GainMode &mode);

    virtual bool setBrightness(double value);//between -1.0 and 1.0, with a default of 0.
    virtual bool brightness(double &value); //between -1.0 and 1.0
    virtual bool brightnessEnable(bool &on);
    virtual bool setBrightnessEnable(bool on);

    virtual bool setGamma(double v); // > 0, default is 1.0
    virtual bool gamma(double &v);
    virtual bool gammaEnable(bool &on);
    virtual bool setGammaEnable(bool on);
    virtual bool gammaRange(double &min, double &max, double &value);

    virtual bool setSharpness(double value); //between -1.0 and 1.0, with a default of 0.
    virtual bool sharpness(double &value);
    virtual bool sharpnessEnable(bool &on);
    virtual bool setSharpnessEnable(bool on);

    virtual bool setBlackLevel(double value); //between -1.0 and 1.0, with a default of 0.
    virtual bool blackLevel(double &value);
    virtual bool blackLevelEnable(bool &on);
    virtual bool setBlackLevelEnable(bool on);

    virtual bool setContrast(double value); //between -1.0 and 1.0, with a default of 0.
    virtual bool contrast(double &value);
    virtual bool contrastEnable(bool &on);
    virtual bool setContrastEnable(bool on);

    virtual bool setSaturation(double value); //between -1.0 and 1.0, with a default of 0.
    virtual bool saturation(double &value);
    virtual bool saturationEnable(bool &on);
    virtual bool setSaturationEnable(bool on);

    virtual bool setDenoise(double value); //between -1.0 and 1.0, with a default of 0.
    virtual bool denoise(double &value);
    virtual bool denoiseEnable(bool &on);
    virtual bool setDenoiseEnable(bool on);

    virtual bool setHue(double value); //between -1.0 and 1.0, with a default of 0.
    virtual bool hue(double &value);
    virtual bool hueEnable(bool &on);
    virtual bool setHueEnable(bool on);

    virtual bool loadWhiteBalanceSettings(QSettings &settings);
    virtual bool writeBalanceSettings(QSettings &settings);

    virtual bool loadWhiteGainSettings(QSettings &settings);
    virtual bool writeGainSettings(QSettings &settings);

    bool loadSettings(QSettings &settings);
    bool writeSettings(QSettings &settings);
};

/// 工业相机的接口
class IndustryCamera : public QObject
{
    Q_OBJECT
public:
    enum AutoExposureMode {OFF, ONCE, CONTINUOUS};
    explicit IndustryCamera(QObject *parent = 0);
    virtual ~IndustryCamera() {}
    virtual bool isLive() = 0;
signals:
    void imageDataReady(const uint8_t *imgBuf, int stride, QSize size, AVPixelFormat format);
    void videoFrameReady(VideoFrame videoFrame);
    void deviceLost();

public slots:
    // 以下为相机设置的通用接口。除这些接口之外，相机还可以有其他特殊的接口。
    virtual bool openDefaultCamera() = 0;
    virtual bool startGrabbing() = 0; //启动实时视频采集
    virtual bool stopGrabbing() = 0; //停止实时视频采集

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
    virtual void close() = 0; //关闭相机

    virtual bool setExposureTimeMode(AutoExposureMode mode);
    virtual bool exposureTimeMode(AutoExposureMode &mode);
//    virtual bool setAutoExposureTime(bool on);
    virtual bool autoExposureTime(bool &on);
    virtual bool autoExposureTimeLowerLimitRange(int &nMin, int &nMax, int &nValue);
    virtual bool autoExposureTimeUpperLimitRange(int &nMin, int &nMax, int &nValue);

    virtual bool setExposureTime(double us); //设置曝光时间，单位毫秒
    virtual bool exposureTime(double &us);
    virtual bool exposureTimeRange(double &min, double &max, double &us);
    virtual bool setFrameRate(double fps); //设置帧率
    virtual bool frameRate(double &fps);

    CameraImageProcessing * imageProcessing();
    virtual bool showVCDPropertyPage() = 0; //打开相机参数设置对话框

    int imageCount() const{return m_imageCount;}
    //void newImage(){m_imageCount ++;}

    QString deviceName() const {return m_DeviceModelName;}
    QString deviceID() const {return m_DeviceID;}
    QString deviceVendorName() const {return m_DeviceVendorName;}
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

    CameraImageProcessing * m_cameraImageProcessing = nullptr;

};

QDebug operator<<(QDebug dbg, const AVPixelFormat &format);
QString PixelFormat2QString(AVPixelFormat format);

} //namespace Qly

#endif // INDUSTRYCAMERA_H
