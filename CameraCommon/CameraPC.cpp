#include "CameraPC.h"
#include <QCameraInfo>
#include <QList>

namespace Qly {


PCCamera::PCCamera()
{
    m_cameraImageProcessing = new PCCameraImageProcessing;
    m_dialog = new DialogCameraSetup(this);
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
    int stride = frame.bytesPerLine();

    //qDebug() << "PCCameraVideoSurface::setVideoFrame";
    qDebug() << frame.pixelFormat();
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
    VideoFrame videoframe((uint8_t *)frame.bits(), stride, frame.size(), format);
    videoframe.setExtraInfo(pPCCamera->m_imageCount ++, QDateTime::currentDateTime());
    emit pPCCamera->videoFrameReady(videoframe);
    //emit camera->ImageDataChanged(frame.bits(), stride, frame.size(), format);
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
    if(!m_pQCamera) return false;
    //m_dialog->m_pQCamera = m_pQCamera;
    m_dialog->initNodeInfo();
    m_dialog->show();
    return true;
}

bool PCCamera::isLive()
{
    return (m_pQCamera->status() == QCamera::ActiveStatus);
}

bool PCCamera::setResolution(QSize size)
{
    if(m_pQCamera)
    {
        //m_pQCamera->load();
        QCameraViewfinderSettings  settings = m_pQCamera->viewfinderSettings();
        settings.setResolution(size);
        m_pQCamera->setViewfinderSettings(settings);
        return true;
    }
    return false;
}

bool PCCamera::openCameraByInfo(QCameraInfo info)
{
    qDebug() << "PCCamera::openCameraByInfo(QCameraInfo info)";
    if(m_pQCamera)
    {
        delete m_pQCamera;
    }
    if(!info.isNull())
    {
        m_pQCamera = new QCamera(info);
        PCCameraImageProcessing * p = static_cast<PCCameraImageProcessing *>(m_cameraImageProcessing);
        p->m_pQCamera = m_pQCamera;
        if(m_surface)
        {
            delete m_surface;
        }
        m_surface = new PCCameraVideoSurface(this);   
        m_pQCamera->setViewfinder(m_surface);

//        QList<QSize> s = m_camera->supportedViewfinderResolutions();
//        QCameraViewfinderSettings  settings = m_camera->viewfinderSettings();
//        settings.setResolution(s.last());
//        m_camera->setViewfinderSettings(settings);
//        qDebug() << "Resolution: " << s;

        m_DeviceModelName = info.description();
        m_pQCamera->setCaptureMode(QCamera::CaptureVideo);
        return true;
    }
    return false;
}

bool PCCamera::openDefaultCamera()
{
    if(m_pQCamera)
    {
        delete m_pQCamera;
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

bool PCCamera::setExposureTime(double us)
{
    if(m_pQCamera)
    {
        QCameraExposure * exp = m_pQCamera->exposure();
        exp->setManualShutterSpeed(us / 1000.0 / 1000.0);
        qDebug() << "setManualShutterSpeed = " << us / 1000.0 / 1000.0;
        return true;
    }
    return false;
}

bool PCCamera::exposureTimeMode(AutoExposureMode &mode)
{
    if(m_pQCamera)
    {
        QCameraExposure * exp = m_pQCamera->exposure();

        QCameraExposure::ExposureMode m = exp->exposureMode();

        switch (m) {
        case QCameraExposure::ExposureAuto:
            mode = IndustryCamera::CONTINUOUS;
            break;
        case QCameraExposure::ExposureManual:
            mode = IndustryCamera::OFF;
            break;
        default:
            mode = IndustryCamera::OFF;
            break;
        }
        return true;
    }
    return false;
}

bool PCCamera::setExposureTimeMode(AutoExposureMode mode)
{
    if(m_pQCamera)
    {
        QCameraExposure * exp = m_pQCamera->exposure();
        switch (mode) {
        case IndustryCamera::OFF:
            exp->setExposureMode(QCameraExposure::ExposureManual);
            qDebug() << "setExposureMode(QCameraExposure::ExposureManual);";
            break;
        case IndustryCamera::CONTINUOUS:
            exp->setExposureMode(QCameraExposure::ExposureAuto);
            qDebug() << "setExposureMode(QCameraExposure::ExposureAuto);";
            break;
        case IndustryCamera::ONCE:
            //exp->setExposureMode(QCameraExposure::ExposureAuto);
            break;
        }
        return true;
    }
    return false;
}

bool PCCamera::exposureTime(double &us)
{
    if(m_pQCamera)
    {
        QCameraExposure * exp = m_pQCamera->exposure();
        us = exp->shutterSpeed() * 1000000.00;
        return true;
    }
    return false;
}

bool PCCamera::exposureTimeRange(double &min, double &max, double &us)
{
    if(m_pQCamera)
    {
        QCameraExposure * exp = m_pQCamera->exposure();
        us = exp->shutterSpeed() * 1000000.00;
        min = 1;
        max = 10000000;
        return true;
    }
    return false;
}

bool PCCamera::startGrabbing()
{
    if(m_pQCamera)
    {
        m_imageCount = 0;
        m_pQCamera->start();

//        setResolution(QSize(1920, 1080));
//        qDebug() << m_pQCamera->supportedViewfinderResolutions();
//        QList<QCameraViewfinderSettings > ViewSets = m_pQCamera->supportedViewfinderSettings();
//        qDebug() << ViewSets.last().resolution();
//        qDebug() << ViewSets.last().pixelFormat();
//        qDebug() << ViewSets.last().maximumFrameRate();
//        m_pQCamera->setViewfinderSettings(ViewSets.last());
        return true;
    }
    return false;
}

bool PCCamera::stopGrabbing()
{
    if(m_pQCamera)
    {
        m_pQCamera->stop();
        return true;
    }
    return false;
}

void PCCamera::close()
{
    if(m_pQCamera)
    {
        m_pQCamera->stop();
        delete m_pQCamera;
        delete m_surface;
        m_pQCamera = nullptr;
        m_surface = nullptr;
        PCCameraImageProcessing * p = static_cast<PCCameraImageProcessing *>(m_cameraImageProcessing);
        p->m_pQCamera = nullptr;
    }
}

PCCamera::~PCCamera()
{
    delete m_cameraImageProcessing;
}

bool PCCameraImageProcessing::setSaturation(double value)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        imageProcessing->setSaturation(value);
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::whiteBalanceMode(WhiteBalanceMode &mode)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        QCameraImageProcessing::WhiteBalanceMode m = imageProcessing->whiteBalanceMode();
        if(m == QCameraImageProcessing::WhiteBalanceAuto)
        {
            mode = CameraImageProcessing::WhiteBalanceAutoContinuous;
        }
        else
        {
            mode = CameraImageProcessing::WhiteBalanceManual;

        }
        return true;
    }
    return false;
}
bool PCCameraImageProcessing::setWhiteBalanceMode(WhiteBalanceMode mode)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        QCameraImageProcessing::WhiteBalanceMode m = imageProcessing->whiteBalanceMode();
        switch (mode) {
        case CameraImageProcessing::WhiteBalanceAutoContinuous:
            imageProcessing->setWhiteBalanceMode(QCameraImageProcessing::WhiteBalanceAuto);
            break;
        case CameraImageProcessing::WhiteBalanceManual:
            imageProcessing->setWhiteBalanceMode(QCameraImageProcessing::WhiteBalanceManual);
            break;
        case CameraImageProcessing::WhiteBalanceAutoOnePush:
            break;
        }
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::setManualWhiteBalance(double colorTemperature)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();
    imageProcessing->setManualWhiteBalance(colorTemperature);
    return true;
}

bool PCCameraImageProcessing::manualWhiteBalance(double &colorTemperature)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();
    colorTemperature = imageProcessing->manualWhiteBalance();
    return true;
}

bool PCCameraImageProcessing::saturation(double &value)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        value = imageProcessing->saturation();
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::saturationEnable(bool &on)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        on = (imageProcessing->saturation() == 0.0);
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::setSaturationEnable(bool on)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        if(!on) imageProcessing->setSaturation(0.0);
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::setContrast(double value)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        imageProcessing->setContrast(value);
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::contrast(double &value)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        value = imageProcessing->contrast();
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::contrastEnable(bool &on)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        on = (imageProcessing->contrast() == 0.0);
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::setContrastEnable(bool on)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        if(!on) imageProcessing->setContrast(0.0);
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::setDenoise(double value)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        imageProcessing->setDenoisingLevel(value);
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::denoise(double &value)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        value = imageProcessing->denoisingLevel();
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::denoiseEnable(bool &on)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        on = (imageProcessing->denoisingLevel() == 0.0);
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::setDenoiseEnable(bool on)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        if(!on) imageProcessing->setDenoisingLevel(0.0);
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::setBrightness(double value)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        imageProcessing->setBrightness(value);
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::brightness(double &value)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        value = imageProcessing->brightness();
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::setSharpness(double value)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        imageProcessing->setSharpeningLevel(value);
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::sharpness(double &value)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        value = imageProcessing->sharpeningLevel();
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::sharpnessEnable(bool &on)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        on = (imageProcessing->sharpeningLevel() == 0.0);
        return true;
    }
    return false;
}

bool PCCameraImageProcessing::setSharpnessEnable(bool on)
{
    if(!m_pQCamera) return false;
    QCameraImageProcessing *imageProcessing = m_pQCamera->imageProcessing();

    if (imageProcessing->isAvailable())
    {
        if(!on) imageProcessing->setSharpeningLevel(0.0);
        return true;
    }
    return false;
}

} //namespace
