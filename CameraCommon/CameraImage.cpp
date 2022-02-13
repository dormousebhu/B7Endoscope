#include "CameraImage.h"
#include <QDebug>
#include <QThread>

namespace Qly
{
CameraImage::CameraImage(QObject *parent):
    QObject(parent),
    m_flipMode(NONE),
    m_inputPixelFormat(AV_PIX_FMT_BAYER_RGGB8),
    m_imageCount(0)
{

}

void CameraImage::setVideoFrame(VideoFrame frame)
{
    QSize size(frame.width, frame.height);
    setImageData(frame.pData[0], frame.bytePerLine[0], size, frame.format);
    //qDebug()<<"from CameraImage::setVideoFrame(VideoFrame frame):" << QThread::currentThreadId();
    if(frame.pMutex)
    {
        frame.pMutex->unlock();
    }
}

void CameraImage::setImageData(const uint8_t *imgBuf, int stride, QSize size, AVPixelFormat format)
{
    if(format == AV_PIX_FMT_GRAY8) //针对映美晶相机打的一个补丁。映美晶相机无法确认 BAYER 的具体格式。
    {
        switch (m_inputPixelFormat)
        {
        case AV_PIX_FMT_BAYER_BGGR8:
        case AV_PIX_FMT_BAYER_RGGB8:
        case AV_PIX_FMT_BAYER_GBRG8:
        case AV_PIX_FMT_BAYER_GRBG8:
             format = m_inputPixelFormat;
            break;
        default:
            break;
        }
    }
    m_imageCount ++;
    switch (format)
    {
    case AV_PIX_FMT_GRAY8:
    case AV_PIX_FMT_RGB24:
    case AV_PIX_FMT_BGR24:
    case AV_PIX_FMT_BGRA:
    case AV_PIX_FMT_BGR0:
    case AV_PIX_FMT_ABGR:
    case AV_PIX_FMT_ARGB:
    //case AV_PIX_FMT_RGBA:
        //qDebug() << "setRGBImage";
        setRGBImage(imgBuf, stride, format, size);
        break;
    case AV_PIX_FMT_BAYER_BGGR8:
    case AV_PIX_FMT_BAYER_RGGB8:
    case AV_PIX_FMT_BAYER_GBRG8:
    case AV_PIX_FMT_BAYER_GRBG8:
        m_inputPixelFormat = format;
        setBayerImage(imgBuf, size);
        break;
    case AV_PIX_FMT_YUYV422:
    case AV_PIX_FMT_UYVY422:
        setYUV422Image(imgBuf, format, size);
        break;
    default:
        break;
    }

}

//void CameraImage::setImageData(const uint8_t *imgBuf, QSize size, AVPixelFormat format)
//{
//    //qDebug() << format;
//    if(format == AV_PIX_FMT_GRAY8) //针对映美晶相机打的一个补丁。映美晶相机无法确认 BAYER 的具体格式。
//    {
//        switch (m_inputPixelFormat)
//        {
//        case AV_PIX_FMT_BAYER_BGGR8:
//        case AV_PIX_FMT_BAYER_RGGB8:
//        case AV_PIX_FMT_BAYER_GBRG8:
//        case AV_PIX_FMT_BAYER_GRBG8:
//             format = m_inputPixelFormat;
//            break;
//        default:
//            break;
//        }
//    }
//    m_imageCount ++;
//    switch (format)
//    {
//    case AV_PIX_FMT_GRAY8:
//        setMono8Image(imgBuf, size);
//        break;
//    case AV_PIX_FMT_RGB24:
//        setRGB24Image(imgBuf, size);
//        break;
//    case AV_PIX_FMT_BGRA:
//        setRGB32Image(imgBuf, size);
//        break;
//    case AV_PIX_FMT_BAYER_BGGR8:
//    case AV_PIX_FMT_BAYER_RGGB8:
//    case AV_PIX_FMT_BAYER_GBRG8:
//    case AV_PIX_FMT_BAYER_GRBG8:
//        m_inputPixelFormat = format;
//        setBayerImage(imgBuf, size);
//        break;
//    case AV_PIX_FMT_YUYV422:
//        setYUV422Image(imgBuf, AV_PIX_FMT_YUYV422, size);
//        break;
//    case AV_PIX_FMT_UYVY422:
//        setYUV422Image(imgBuf, AV_PIX_FMT_UYVY422, size);
//        break;
//    default:
//        break;
//    }
//}

//QList<QVideoFrame::PixelFormat> CameraImage::supportedPixelFormats(
//        QAbstractVideoBuffer::HandleType handleType) const
//{
//    Q_UNUSED(handleType);
//    QList<QVideoFrame::PixelFormat> list;
//    list << QVideoFrame::Format_RGB32;
//    list << QVideoFrame::Format_ARGB32;
//    list << QVideoFrame::Format_RGB24;
//    list << QVideoFrame::Format_UYVY;
//    list << QVideoFrame::Format_Y8;
//    list << QVideoFrame::Format_YUYV;
//    return list;
//}

//bool CameraImage::present(const QVideoFrame &frame)
// {
//    // 处理捕获的帧
//    if(frame.isMapped())
//    {
//        setVideoFrame(frame);
//    }
//    else
//    {
//        QVideoFrame f = frame;
//        f.map(QAbstractVideoBuffer::ReadOnly);
//        setVideoFrame(f);
//    }
//    return true;
// }

//void CameraImage::setVideoFrame(const QVideoFrame &frame)
//{
////    qDebug() << "setVideoFrame";
////    qDebug() << "height = " << frame.height();
////    qDebug() << "width = " << frame.width();
////    qDebug() << frame.pixelFormat();
//    switch (frame.pixelFormat())
//    {
//    case QVideoFrame::Format_RGB24:
//        setRGBImage(frame.bits(), frame.bytesPerLine(), AV_PIX_FMT_RGB24, frame.size());
//        //setRGB24Image(frame.bits(), frame.size());
//        break;
//    case QVideoFrame::Format_RGB32:
//    case QVideoFrame::Format_ARGB32:
//    case QVideoFrame::Format_ARGB32_Premultiplied:
//        setRGBImage(frame.bits(), frame.bytesPerLine(), AV_PIX_FMT_ARGB, frame.size());
//        //setRGB32Image(frame.bits(), frame.size());
//        break;
//    case QVideoFrame::Format_Y8:
//        setBayerImage(frame.bits(), frame.size());
//        break;
//    case QVideoFrame::Format_YUYV:
//        setYUV422Image(frame.bits(), AV_PIX_FMT_YUYV422, frame.size());
//        //setYUY2Image(frame.bits(), frame.size());
//        break;
//    case QVideoFrame::Format_UYVY:
//        setYUV422Image(frame.bits(), AV_PIX_FMT_UYVY422, frame.size());
//        //setUYVYImage(frame.bits(), frame.size());
//        break;
//    default:
//        break;
//    }
//}

void CameraImage::setFlipMode(enum FlipMode flipMode)
{
    if(flipMode == m_flipMode)
    {
        return;
    }
    m_flipMode = flipMode;
    emit flipModeChanged(flipMode);
}

void CameraImage::setHorizontalFlip(bool on)
{
    qDebug() << "setHorizontalFlip = " << on;
    int flip_mode;
    on ? flip_mode = m_flipMode | 0x01: flip_mode = m_flipMode & 0x02;
    setFlipMode(static_cast<FlipMode> (flip_mode));
}

void CameraImage::setVerticalFlip(bool on)
{
    qDebug() << "setVerticalFlip = " << on;
    int flip_mode;
    on ? flip_mode = m_flipMode | 0x02: flip_mode = m_flipMode & 0x01;
    setFlipMode(static_cast<FlipMode> (flip_mode));
}

void CameraImage::setBayerPattern(AVPixelFormat pattern)
{
    if(pattern == m_inputPixelFormat)
    {
        return;
    }
    m_inputPixelFormat = pattern;
    qDebug() << "m_bayer_pattern = " << m_inputPixelFormat;
    emit bayerPatternChanged(pattern);

}

bool CameraImage::loadImage(const QString &fileName)
{
    qDebug() << "loadImage";
    QImage image;
    image.load(fileName);

    if(image.isNull())
    {
        qDebug() << "image.isNull() == true";
        return false;
    }
    setImage(image);
    return true;
}

bool CameraImage::saveImage( const QString &fileName )
{
    m_imageMutex.lock();
    QImage image = m_image.copy();
    m_imageMutex.unlock();
    return image.save(fileName);
}


QImage CameraImage::getImage()
{
    QImage image;
    // 防止在获取图像时有其他的线程更新图像，这里要加锁
    m_imageMutex.lock();
    image = m_image.copy();
    m_imageMutex.unlock();
    return image;
}
} //namespace Qly
