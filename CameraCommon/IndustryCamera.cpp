#include "IndustryCamera.h"

namespace Qly {


VideoFrame::VideoFrame(uint8_t *imgBuf, QSize size, AVPixelFormat f)
{
    pData[0] = imgBuf;
    width = size.width();
    height = size.height();
    format = f;
    switch (format) {
    case AV_PIX_FMT_PAL8:
    case AV_PIX_FMT_GRAY8:
        bytePerLine[0] = width;
        break;
    case AV_PIX_FMT_GRAY16:
        bytePerLine[0] = width * 2;
        break;
    case AV_PIX_FMT_BGR0:
    case AV_PIX_FMT_ABGR:
    case AV_PIX_FMT_0BGR:
    case AV_PIX_FMT_BGRA:
    case AV_PIX_FMT_0RGB:
    case AV_PIX_FMT_RGBA:
    case AV_PIX_FMT_ARGB:
    case AV_PIX_FMT_RGB0:
        bytePerLine[0] = width * 4;
        break;
    case AV_PIX_FMT_RGB24:
    case AV_PIX_FMT_BGR24:
        bytePerLine[0] = width * 3;
        break;
    case AV_PIX_FMT_BAYER_BGGR8:
    case AV_PIX_FMT_BAYER_GBRG8:
    case AV_PIX_FMT_BAYER_GRBG8:
    case AV_PIX_FMT_BAYER_RGGB8:
        bytePerLine[0] = width * 2;
        break;
    case AV_PIX_FMT_BAYER_BGGR16BE:
    case AV_PIX_FMT_BAYER_BGGR16LE:
    case AV_PIX_FMT_BAYER_GBRG16BE:
    case AV_PIX_FMT_BAYER_GBRG16LE:
    case AV_PIX_FMT_BAYER_GRBG16BE:
    case AV_PIX_FMT_BAYER_GRBG16LE:
    case AV_PIX_FMT_BAYER_RGGB16BE:
    case AV_PIX_FMT_BAYER_RGGB16LE:
        bytePerLine[0] = width * 4;
        break;
    case AV_PIX_FMT_YUYV422:
    case AV_PIX_FMT_UYVY422:
        bytePerLine[0] = width * 2;
        break;
    }
}
VideoFrame::VideoFrame(uint8_t *imgBuf, int stride, QSize size, AVPixelFormat f)
{
    pData[0] = imgBuf;
    bytePerLine[0] = stride;
    width = size.width();
    height = size.height();
    format = f;
}

void VideoFrame::setData(uint8_t *imgBuf, int stride, QSize size, AVPixelFormat f, QMutex *mutex)
{
    pData[0] = imgBuf;
    bytePerLine[0] = stride;
    width = size.width();
    height = size.height();
    format = f;
    pMutex = mutex;
}

VideoFrame::VideoFrame(uint8_t *imgBuf, int stride, QSize size, AVPixelFormat f, QMutex *mutex)
    :VideoFrame(imgBuf, stride, size, f)
{
    pMutex = mutex;
}

IndustryCamera::IndustryCamera(QObject *parent) : QObject(parent)
{
    m_imageCount = 0;
    qRegisterMetaType<AVPixelFormat>("AVPixelFormat");
    qRegisterMetaType<VideoFrame>("VideoFrame");
}

QString PixelFormat2QString(AVPixelFormat format)
{
    QString str;
    switch (format) {
    case AV_PIX_FMT_NONE :
        str = "NONE";
        break;
    case AV_PIX_FMT_YUV420P:
        str = "YUV420P";
        break;
    case AV_PIX_FMT_YUYV422:
        str = "YUYV422";
        break;
    case AV_PIX_FMT_UYVY422:
        str = "UYVY422";
        break;
    case AV_PIX_FMT_RGB24:
        str = "RGB24";
        break;
    case AV_PIX_FMT_BGR24:
        str = "BGR24";
        break;
    case AV_PIX_FMT_YUV422P:
        str = "YUV422P";
        break;
    case AV_PIX_FMT_YUV444P:
        str = "YUV444P";
        break;
    case AV_PIX_FMT_YUV410P:
        str = "YUV410P";
        break;
    case AV_PIX_FMT_YUV411P:
        str = "YUV411P";
        break;
    case AV_PIX_FMT_GRAY8:
        str = "GRAY8";
        break;
    case AV_PIX_FMT_ARGB:
        str = "ARGB";
        break;
    case AV_PIX_FMT_RGBA:
        str = "RGBA";
        break;
    case AV_PIX_FMT_ABGR:
        str = "ABGR";
        break;
    case AV_PIX_FMT_BGRA:
        str = "BGRA";
        break;
    case AV_PIX_FMT_BAYER_BGGR8:
        str = "BAYER_BGGR8";
        break;
    case AV_PIX_FMT_BAYER_RGGB8:
        str = "BAYER_RGGB8";
        break;
    case AV_PIX_FMT_BAYER_GRBG8:
        str = "BAYER_GRBG8";
        break;
    case AV_PIX_FMT_BAYER_GBRG8:
        str = "BAYER_GBRG8";
        break;
    case AV_PIX_FMT_BAYER_BGGR16LE:
        str = "BAYER_BGGR16LE";
        break;
    case AV_PIX_FMT_BAYER_RGGB16LE:
        str = "BAYER_RGGB16LE";
        break;
    case AV_PIX_FMT_BAYER_GBRG16LE:
        str = "BAYER_GBRG16LE";
        break;
    case AV_PIX_FMT_BAYER_GRBG16LE:
        str = "BAYER_GRBG16LE";
        break;
    default:
        str = "AV_PIX_FMT_UNKNOWN";
        break;
    }
    return str;
}

QDebug operator<<(QDebug dbg, const AVPixelFormat &format)
{
    dbg.nospace() << PixelFormat2QString(format);
    return dbg.maybeSpace();
}

CameraImageProcessing * IndustryCamera::imageProcessing()
{
    return m_cameraImageProcessing;
}

bool IndustryCamera::setPixelType(AVPixelFormat type)
{
    Q_UNUSED(type);
    return false;
}

bool IndustryCamera::saveSettingsToFile(QString fileName)
{
    Q_UNUSED(fileName);
    return false;
}
bool IndustryCamera::loadSettingsFromFile(QString fileName)
{
    Q_UNUSED(fileName);
    return false;
}

bool IndustryCamera::setExposureTimeMode(AutoExposureMode mode)
{
    Q_UNUSED(mode);
    return false;
}

bool IndustryCamera::exposureTimeMode(AutoExposureMode &mode)
{
    Q_UNUSED(mode);
    return false;
}

//bool IndustryCamera::setAutoExposureTime(bool on)
//{
//    Q_UNUSED(on);
//    return false;
//}

bool IndustryCamera::autoExposureTime(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::setExposureTime(double us)
{
    Q_UNUSED(us);
    return false;
}
bool IndustryCamera::exposureTime(double &us)
{
    Q_UNUSED(us);
    return false;
}

bool IndustryCamera::autoExposureTimeLowerLimitRange(int &nMin, int &nMax, int &nValue)
{
    Q_UNUSED(nValue);
    Q_UNUSED(nMin);
    Q_UNUSED(nMax);
    return false;
}

bool IndustryCamera::autoExposureTimeUpperLimitRange(int &nMin, int &nMax, int &nValue)
{
    Q_UNUSED(nValue);
    Q_UNUSED(nMin);
    Q_UNUSED(nMax);
    return false;
}

bool IndustryCamera::exposureTimeRange(double &min, double &max, double &us)
{
    Q_UNUSED(us);
    Q_UNUSED(min);
    Q_UNUSED(max);
    return false;
}

bool IndustryCamera::setFrameRate(double fps)
{
    Q_UNUSED(fps);
    return false;
}
bool IndustryCamera::frameRate(double &fps)
{
    Q_UNUSED(fps);
    return false;
}

}//namespace Qly
