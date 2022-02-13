#ifndef PCCAMERA_H
#define PCCAMERA_H
#include <QCamera>
#include <QAbstractVideoSurface>
#include "IndustryCamera.h"

namespace Qly {


class PCCamera;

class PCCameraVideoSurface : public QAbstractVideoSurface
{
public:
    PCCameraVideoSurface(PCCamera *c) {camera = c;}
private:
    PCCamera * camera;
    void setVideoFrame(const QVideoFrame &frame);
    bool present(const QVideoFrame &frame) Q_DECL_OVERRIDE;
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const Q_DECL_OVERRIDE;

};

class PCCamera : public Qly::IndustryCamera
{
    Q_OBJECT
public:
    PCCamera(QObject *parent = 0);

    bool macroFocus();
    bool isLive() Q_DECL_OVERRIDE;
    void setFrameRate(double min, double max);
    void setResolutionIndex(int n);
    bool setResolution(QSize z);
    bool showVCDPropertyPage() Q_DECL_OVERRIDE;
    bool openCameraByName(QString deviceName);
    bool openCameraByInfo(QCameraInfo info);
    bool openDefaultCamera() Q_DECL_OVERRIDE;
    bool startGrabbing() Q_DECL_OVERRIDE;
    bool stopGrabbing() Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;
    bool setAutoExpoureTime(bool on) Q_DECL_OVERRIDE;
    bool setExpoureTime(double us) Q_DECL_OVERRIDE;
    bool expoureTime(double &us) Q_DECL_OVERRIDE;

    bool setAutoWhiteBalance(bool on) Q_DECL_OVERRIDE;
    bool setWhiteBalance(int red, int green, int blue) Q_DECL_OVERRIDE;
    bool whiteBalance(int &red, int &green, int &blue) Q_DECL_OVERRIDE;

    bool setSaturation(int nValue) Q_DECL_OVERRIDE;
    bool saturation(int &nValue) Q_DECL_OVERRIDE;
    bool saturationEnable(bool &on) Q_DECL_OVERRIDE;
    bool setSaturationEnable(bool on) Q_DECL_OVERRIDE;

    bool setContrast(int nValue) Q_DECL_OVERRIDE;
    bool contrast(int &nValue) Q_DECL_OVERRIDE;
    bool contrastEnable(bool &on) Q_DECL_OVERRIDE;
    bool setContrastEnable(bool on) Q_DECL_OVERRIDE;

    bool setDenoise(int nValue) Q_DECL_OVERRIDE;
    bool denoise(int &nValue) Q_DECL_OVERRIDE;
    bool denoiseEnable(bool &on) Q_DECL_OVERRIDE;
    bool setDenoiseEnable(bool on) Q_DECL_OVERRIDE;

    bool setBrightness(int value) Q_DECL_OVERRIDE;
    bool brightness(int &nValue) Q_DECL_OVERRIDE;

    bool setSharpness(int nValue) Q_DECL_OVERRIDE;
    bool sharpness(int &nValue) Q_DECL_OVERRIDE;
    bool sharpnessEnable(bool &on) Q_DECL_OVERRIDE;
    bool setSharpnessEnable(bool on) Q_DECL_OVERRIDE;
private slots:
    void setResolution(QCamera::State state);
private:
    QCamera * m_camera = nullptr;
    PCCameraVideoSurface * m_surface = nullptr;
    QSize m_resolution;
};

} //namespace Qly
#endif // PCCAMERA_H
