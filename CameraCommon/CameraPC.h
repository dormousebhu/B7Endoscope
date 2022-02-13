#ifndef PCCAMERA_H
#define PCCAMERA_H
#include <QCamera>
#include <QAbstractVideoSurface>
#include "IndustryCamera.h"
#include "DialogCameraSetup.h"

namespace Qly {


class PCCamera;

class PCCameraVideoSurface : public QAbstractVideoSurface
{
//    friend class PCCamera;
public:
    PCCameraVideoSurface(PCCamera *c) {pPCCamera = c;}
private:
    PCCamera * pPCCamera;
    void setVideoFrame(const QVideoFrame &frame);
    bool present(const QVideoFrame &frame) Q_DECL_OVERRIDE;
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const Q_DECL_OVERRIDE;

};

class PCCameraImageProcessing: public CameraImageProcessing
{
public:
    friend class PCCamera;

    bool setWhiteBalanceMode(WhiteBalanceMode mode) Q_DECL_OVERRIDE;
    bool whiteBalanceMode(WhiteBalanceMode &mode) Q_DECL_OVERRIDE;
    bool setManualWhiteBalance(double colorTemperature) Q_DECL_OVERRIDE;
    bool manualWhiteBalance(double &colorTemperature) Q_DECL_OVERRIDE;

    bool setSaturation(double value) Q_DECL_OVERRIDE;
    bool saturation(double &value) Q_DECL_OVERRIDE;
    bool saturationEnable(bool &on) Q_DECL_OVERRIDE;
    bool setSaturationEnable(bool on) Q_DECL_OVERRIDE;

    bool setContrast(double value) Q_DECL_OVERRIDE;
    bool contrast(double &value) Q_DECL_OVERRIDE;
    bool contrastEnable(bool &on) Q_DECL_OVERRIDE;
    bool setContrastEnable(bool on) Q_DECL_OVERRIDE;

    bool setDenoise(double value) Q_DECL_OVERRIDE;
    bool denoise(double &value) Q_DECL_OVERRIDE;
    bool denoiseEnable(bool &on) Q_DECL_OVERRIDE;
    bool setDenoiseEnable(bool on) Q_DECL_OVERRIDE;

    bool setBrightness(double value) Q_DECL_OVERRIDE;
    bool brightness(double &value) Q_DECL_OVERRIDE;

    bool setSharpness(double value) Q_DECL_OVERRIDE;
    bool sharpness(double &value) Q_DECL_OVERRIDE;
    bool sharpnessEnable(bool &on) Q_DECL_OVERRIDE;
    bool setSharpnessEnable(bool on) Q_DECL_OVERRIDE;
private:
    QCamera * m_pQCamera = nullptr;
};

class PCCamera : public Qly::IndustryCamera
{
    friend class PCCameraVideoSurface;
public:
    PCCamera();
    ~PCCamera() Q_DECL_OVERRIDE;
    bool isLive() Q_DECL_OVERRIDE;
    bool showVCDPropertyPage() Q_DECL_OVERRIDE;
    bool openCameraByName(QString deviceName);
    bool openCameraByInfo(QCameraInfo info);
    bool openDefaultCamera() Q_DECL_OVERRIDE;
    bool startGrabbing() Q_DECL_OVERRIDE;
    bool stopGrabbing() Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;
//    bool setAutoExposureTime(bool on) Q_DECL_OVERRIDE;
    bool setExposureTimeMode(AutoExposureMode mode) Q_DECL_OVERRIDE;
    bool setExposureTime(double us) Q_DECL_OVERRIDE;
    bool exposureTime(double &us) Q_DECL_OVERRIDE;
    bool exposureTimeMode(AutoExposureMode &mode) Q_DECL_OVERRIDE;
    bool exposureTimeRange(double &min, double &max, double &us) Q_DECL_OVERRIDE;
    bool setResolution(QSize size);
private:
    QCamera * m_pQCamera = nullptr;
    DialogCameraSetup * m_dialog = nullptr;
    PCCameraVideoSurface * m_surface = nullptr;
};

} //namespace Qly
#endif // PCCAMERA_H
