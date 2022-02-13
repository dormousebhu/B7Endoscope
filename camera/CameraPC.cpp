#include "CameraPC.h"
#include <QCameraInfo>
#include <QList>

namespace Qly {


PCCamera::PCCamera(QObject *parent)
    :Qly::IndustryCamera(parent)
{

}

bool PCCamera::macroFocus()
{
    qDebug() << "PCCamera::macroFocus()";
    if(m_camera)
    {
        QCameraFocus *focus = m_camera->focus();
        qDebug() <<"isFocusPointModeSupported(FocusPointAuto)" << focus->isFocusPointModeSupported(QCameraFocus::QCameraFocus::FocusPointAuto);
        qDebug() <<"isFocusPointModeSupported(FocusPointCenter)" << focus->isFocusPointModeSupported(QCameraFocus::QCameraFocus::FocusPointCenter);
        qDebug() <<"isFocusPointModeSupported(FocusPointCustom)" << focus->isFocusPointModeSupported(QCameraFocus::QCameraFocus::FocusPointCustom);

        focus->setFocusPointMode(QCameraFocus::FocusPointCustom);


        qDebug() <<"isFocusModeSupported(ManualFocus)" << focus->isFocusModeSupported(QCameraFocus::QCameraFocus::ManualFocus);
        qDebug() <<"isFocusModeSupported(HyperfocalFocus)" << focus->isFocusModeSupported(QCameraFocus::QCameraFocus::HyperfocalFocus);
        qDebug() <<"isFocusModeSupported(InfinityFocus)" << focus->isFocusModeSupported(QCameraFocus::QCameraFocus::InfinityFocus);
        qDebug() <<"isFocusModeSupported(AutoFocus)" << focus->isFocusModeSupported(QCameraFocus::QCameraFocus::AutoFocus);
        qDebug() <<"isFocusModeSupported(ContinuousFocus)" << focus->isFocusModeSupported(QCameraFocus::QCameraFocus::ContinuousFocus);
        if(focus->isFocusModeSupported(QCameraFocus::QCameraFocus::AutoFocus))
        {
            focus->setFocusMode(QCameraFocus::MacroFocus);
            return true;
        }
    }
    return false;
}

QList<QVideoFrame::PixelFormat> PCCameraVideoSurface::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    QList<QVideoFrame::PixelFormat> list;
    list << QVideoFrame::Format_RGB32;
    list << QVideoFrame::Format_ARGB32;
    list << QVideoFrame::Format_RGB24;
    list << QVideoFrame::Format_UYVY;
    list << QVideoFrame::Format_Y8;
    list << QVideoFrame::Format_YUYV;
    return list;
}

void PCCameraVideoSurface::setVideoFrame(const QVideoFrame &frame)
{
//    qDebug() << frame.pixelFormat();
    AVPixelFormat format = AV_PIX_FMT_NONE;

    //qDebug() << "PCCameraVideoSurface::setVideoFrame";
    switch (frame.pixelFormat())
    {
    case QVideoFrame::Format_RGB24:
        format = AV_PIX_FMT_RGB24;
        //emit camera->RGB24ImageChanged(frame.bits(), frame.size());
        break;
    case QVideoFrame::Format_RGB32:
    case QVideoFrame::Format_ARGB32:
    case QVideoFrame::Format_ARGB32_Premultiplied:
        format = AV_PIX_FMT_BGRA;
        //emit camera->RGB32ImageChanged(frame.bits(), frame.size());
        break;
    case QVideoFrame::Format_Y8:
        format = AV_PIX_FMT_GRAY8;
        //camera->Bayer8ImageChanged(frame.bits(), frame.size());
        break;
    case QVideoFrame::Format_YUYV:
        format = AV_PIX_FMT_YUYV422;
        //camera->YUYVImageChanged(frame.bits(), frame.size());
        break;
    case QVideoFrame::Format_UYVY:
        format = AV_PIX_FMT_UYVY422;
        //camera->UYVYImageChanged(frame.bits(), frame.size());
        break;
    default:
        break;
    }
    camera->newImage();
    emit camera->ImageDataChanged(frame.bits(), frame.size(), format);
}

bool PCCameraVideoSurface::present(const QVideoFrame &frame)
{
    //qDebug() << "in PCCameraVideoSurface::present()";
    if(frame.isMapped())
    {
        setVideoFrame(frame);
    }
    else
    {
        QVideoFrame f = frame;
        f.map(QAbstractVideoBuffer::ReadOnly);
        setVideoFrame(f);
    }
    return true;
}

bool PCCamera::showVCDPropertyPage()
{
    return false;
}

bool PCCamera::isLive()
{
    return (m_camera->status() == QCamera::ActiveStatus);
}

bool PCCamera::openCameraByInfo(QCameraInfo info)
{
    qDebug() << "PCCamera::openCameraByInfo(QCameraInfo info)";
    if(m_camera)
    {
        delete m_camera;
    }
    if(!info.isNull())
    {
        m_camera = new QCamera(info);
        if(m_surface)
        {
            delete m_surface;
        }
        m_surface = new PCCameraVideoSurface(this);   
        m_camera->setViewfinder(m_surface);
        m_DeviceModelName = info.description();

       // connect(m_camera, &QCamera::stateChanged, this, &PCCamera::setResolution);

        return true;
    }
    return false;
}

void PCCamera::setResolution(QCamera::State state)
{
    if(state == QCamera::ActiveState)
    {
//        QList<QSize> s = m_camera->supportedViewfinderResolutions();
//        qDebug() << "supportedViewfinderResolutions = " << s;

//        QCameraViewfinderSettings  settings = m_camera->viewfinderSettings();
//        settings.setResolution(s.at(0));
//        m_camera->setViewfinderSettings(settings);
    }
}

void PCCamera::setFrameRate(double min, double max)
{
    if(m_camera)
    {
        QCameraViewfinderSettings  settings = m_camera->viewfinderSettings();
        QList<QCamera::FrameRateRange> range = m_camera->supportedViewfinderFrameRateRanges(settings) ;
        qDebug() << u8"支持的帧率数量 = " <<  range.size();
        if(range.size() != 0)
        {
            qDebug() << u8"最小 FrameRate = " <<  range.at(0).minimumFrameRate;
            qDebug() << u8"最大 FrameRate = " <<  range.at(0).maximumFrameRate;
        }
//    settings.setMaximumFrameRate(max);
//    settings.setMinimumFrameRate(min);
//    m_camera->setViewfinderSettings(settings);
    }
}
void PCCamera::setResolutionIndex(int n)
{
    if(m_camera)
    {
        qDebug() << "in PCCamera::setResolutionIndex(int n)";
        QList<QSize> s = m_camera->supportedViewfinderResolutions();
        if(s.isEmpty()) return;
        qDebug() << "supportedViewfinderResolutions = " << s;
        n = qBound(0, n, s.size() - 1);
        QCameraViewfinderSettings  settings = m_camera->viewfinderSettings();
        settings.setResolution(s.at(n));
        m_camera->setViewfinderSettings(settings);
        qDebug() << "Resolution: " << s.at(n);
    }
}

bool PCCamera::setResolution(QSize z)
{
    qDebug() << "in PCCamera::setResolution(QSize z)";
    m_resolution = z;
    if(m_camera && isLive())
    {
        QCameraViewfinderSettings  settings = m_camera->viewfinderSettings();
        settings.setResolution(z);
        m_camera->setViewfinderSettings(settings);
        qDebug() << "setResolution: " << z;
    }
    return true;
}

bool PCCamera::openDefaultCamera()
{
    if(m_camera)
    {
        delete m_camera;
    }
    QCameraInfo info = QCameraInfo::defaultCamera();
    return openCameraByInfo(info);
}

bool PCCamera::openCameraByName(QString name)
{
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
      for (const QCameraInfo &cameraInfo : cameras)
      {
          if (cameraInfo.deviceName() == name)
          {
             return openCameraByInfo(cameraInfo);
          }
      }
      return false;
}

bool PCCamera::setExpoureTime(double us)
{
    if(m_camera)
    {
        QCameraExposure * exp = m_camera->exposure();
        exp->setManualShutterSpeed(us / 1000.0 / 1000.0);
        return true;
    }
    return false;
}

bool PCCamera::setAutoExpoureTime(bool on)
{
    if(m_camera)
    {
        QCameraExposure * exp = m_camera->exposure();
        if(on)
        {
            exp->setExposureMode(QCameraExposure::ExposureAuto);
        }
        else
        {
            exp->setExposureMode(QCameraExposure::ExposureManual);
        }
        return true;
    }
    return false;
}

bool PCCamera::expoureTime(double &us)
{
    if(m_camera)
    {
        QCameraExposure * exp = m_camera->exposure();
        us = exp->shutterSpeed() * 1000000.00;
        return true;
    }
    return false;
}

bool PCCamera::startGrabbing()
{
    if(m_camera)
    {
        m_imageCount = 0;

        m_camera->load();
        if(!m_resolution.isEmpty())
        {
            QCameraViewfinderSettings  settings = m_camera->viewfinderSettings();
            settings.setResolution(m_resolution);
            m_camera->setViewfinderSettings(settings);
        }
        m_camera->start();
        return true;
    }
    return false;
}

bool PCCamera::stopGrabbing()
{
    if(m_camera)
    {
        m_camera->stop();
        return true;
    }
    return false;
}

void PCCamera::close()
{
    if(m_camera)
    {
        m_camera->stop();
        delete m_camera;
        delete m_surface;
        m_camera = nullptr;
        m_surface = nullptr;
    }
}

bool PCCamera::setAutoWhiteBalance(bool on)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        if(on)
        {
            imageProcessing->setWhiteBalanceMode(QCameraImageProcessing::WhiteBalanceAuto);
        }
        else
        {
            imageProcessing->setWhiteBalanceMode(QCameraImageProcessing::WhiteBalanceManual);
        }
        return true;
    }
    return false;
}

bool PCCamera::setWhiteBalance(int red, int green, int blue)
{
    return false;
}

bool PCCamera::whiteBalance(int &red, int &green, int &blue)
{
    return false;
}

bool PCCamera::setSaturation(int nValue)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        imageProcessing->setSaturation(nValue / 100.0);
        return true;
    }
    return false;
}

bool PCCamera::saturation(int &nValue)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        nValue = 100 * imageProcessing->saturation();
        return true;
    }
    return false;
}

bool PCCamera::saturationEnable(bool &on)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        on = (imageProcessing->saturation() == 0.0);
        return true;
    }
    return false;
}

bool PCCamera::setSaturationEnable(bool on)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        if(!on) imageProcessing->setSaturation(0.0);
        return true;
    }
    return false;
}


bool PCCamera::setContrast(int nValue)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        imageProcessing->setContrast(nValue / 100.0);
        return true;
    }
    return false;
}

bool PCCamera::contrast(int &nValue)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        nValue = 100 * imageProcessing->contrast();
        return true;
    }
    return false;
}

bool PCCamera::contrastEnable(bool &on)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        on = (imageProcessing->contrast() == 0.0);
        return true;
    }
    return false;
}

bool PCCamera::setContrastEnable(bool on)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        if(!on) imageProcessing->setContrast(0.0);
        return true;
    }
    return false;
}

bool PCCamera::setDenoise(int nValue)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        imageProcessing->setDenoisingLevel(nValue / 100.0);
        return true;
    }
    return false;
}

bool PCCamera::denoise(int &nValue)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        nValue = 100 * imageProcessing->denoisingLevel();
        return true;
    }
    return false;
}

bool PCCamera::denoiseEnable(bool &on)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        on = (imageProcessing->denoisingLevel() == 0.0);
        return true;
    }
    return false;
}

bool PCCamera::setDenoiseEnable(bool on)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        if(!on) imageProcessing->setDenoisingLevel(0.0);
        return true;
    }
    return false;
}

bool PCCamera::setBrightness(int value)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        imageProcessing->setBrightness(value / 100.0);
        return true;
    }
    return false;
}

bool PCCamera::brightness(int &nValue)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        nValue = 100 * imageProcessing->brightness();
        return true;
    }
    return false;
}

bool PCCamera::setSharpness(int nValue)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        imageProcessing->setSharpeningLevel(nValue / 100.0);
        return true;
    }
    return false;
}

bool PCCamera::sharpness(int &nValue)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        nValue = 100 * imageProcessing->sharpeningLevel();
        return true;
    }
    return false;
}

bool PCCamera::sharpnessEnable(bool &on)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        on = (imageProcessing->sharpeningLevel() == 0.0);
        return true;
    }
    return false;
}

bool PCCamera::setSharpnessEnable(bool on)
{
    if(!m_camera) return false;
    QCameraImageProcessing *imageProcessing = m_camera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        if(!on) imageProcessing->setSharpeningLevel(0.0);
        return true;
    }
    return false;
}

} //namespace
