/****************************************************************************
** file: CameraImage_RGB32.cpp
** brief: CameraImage 类的派生类。内部存储图像类型为 RGB888。
** Copyright (C) LiYuan
** Author: LiYuan
** E-Mail: 18069211#qq(.)com
** Version 1.0.1
** Last modified: 2019.01.11
** Modified By: LiYuan
****************************************************************************/

#include "CameraImage_RGB32.h"
#include <QColor>
#include <QDebug>
#include "PixelFormatConvert.h"

namespace Qly {

static const int MAX_IMAGE_SIZE_X = 20000;
static const int MAX_IMAGE_SIZE_Y = 20000;
static const int DEFAULT_IMAGE_SIZE_X = 1920;
static const int DEFAULT_IMAGE_SIZE_Y = 1080;

struct rgb24
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

CameraImage_RGB32::CameraImage_RGB32(QObject *parent)
    : CameraImage(parent)
{
    m_image = QImage( QSize(DEFAULT_IMAGE_SIZE_X, DEFAULT_IMAGE_SIZE_Y), QImage::Format_RGB32 );
}

/**
 * @brief CameraView::flipImage  将图像翻转
 * @param image
 * @param flip_mode
 */
void CameraImage_RGB32::flipImage(QImage &image, FlipMode flip_mode)
{
    int width = image.width();
    int height = image.height();
    switch (flip_mode)
    {
    case BOTH:
        for(int i = 0; i < height / 2; i++)
        {
            QRgb * line1 = (QRgb *) image.scanLine(i);
            QRgb * line2 = (QRgb *) image.scanLine(height - i - 1);
            for(int j = 0; j < width; j ++)
            {
                std::swap(line1[j], line2[width - j - 1]);
            }
        }
        break;
    case HORIZONTAL_FLIP:
        for(int i = 0; i < height; i++)
        {
            QRgb * line = (QRgb *) image.scanLine(i);
            for(int j = 0; j < width / 2; j ++)
            {
                std::swap(line[j], line[width - j - 1]);
            }
        }
        break;
    case VERTICAL_FLIP:
        for(int i = 0; i < height / 2; i++)
        {
            QRgb * line1 = (QRgb *) image.scanLine(i);
            QRgb * line2 = (QRgb *) image.scanLine(height - i - 1);
            for(int j = 0; j < width; j ++)
            {
                std::swap(line1[j], line2[j]);
            }
        }
        break;
    default:
        break;
    }
}

void CameraImage_RGB32::setRGBImage(const uint8_t *imgBuf, int stride, AVPixelFormat format, QSize size)
{
    m_imageMutex.lock();
    if(m_image.size() != size)
    {
        m_image = QImage(size, QImage::Format_RGB32);
    }

    RGBtoRGB32(imgBuf, stride, format, size, m_image);

    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
    m_imageMutex.unlock();
    m_imageCount++;
    imageChangedHook();
    emit imageChanged(m_image);
}


void CameraImage_RGB32::setImage(const QImage &image)
{
    m_imageMutex.lock();
    m_image = image.convertToFormat(QImage::Format_RGB32);
    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
    m_imageMutex.unlock();
    imageChangedHook();
    emit imageChanged(m_image);
}


//void CameraImage_RGB32::setMono8Image(const uint8_t *imgBuf, QSize size)
//{
//    int imageWidth = size.width();
//    int imageHeight = size.height();

//    m_imageMutex.lock();
//    if(m_image.size() != size)
//    {
//        m_image = QImage(size, QImage::Format_RGB32);
//    }
//    for (int row = 0; row < imageHeight; row++)
//    {
//        QRgb *line = (QRgb *)m_image.scanLine(row);
//        // 找到 CCD 数据一行的开头位置，一个像素占 1 字节
//        uint8_t * pixel = (uint8_t *) &imgBuf[row * imageWidth];
//        for (int col = 0; col < imageWidth; col ++)
//        {
//            line[col] = qRgb(pixel[col], pixel[col], pixel[col]);
//        }
//    }
//    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
//    m_imageMutex.unlock();

//    m_imageCount++;
//    imageChangedHook();
//    emit imageChanged(m_image);
//}

//void CameraImage_RGB32::setRGB24Image(const uint8_t *imgBuf, QSize size)
//{
//    int imageWidth = size.width();
//    int imageHeight = size.height();

//    m_imageMutex.lock();
//    if(m_image.size() != size)
//    {
//        m_image = QImage(size, QImage::Format_RGB32);
//    }
//    for (int row = 0; row < imageHeight; row++)
//    {
//        QRgb *line = (QRgb *)m_image.scanLine(row);
//        // 找到 CCD 数据一行的开头位置，一个像素占3字节
//        rgb24 * pixel = (rgb24 *) &imgBuf[3 * row * imageWidth];
//        for (int col = 0; col < imageWidth; col ++)
//        {
//            line[col] = qRgb(pixel[col].r, pixel[col].g, pixel[col].b);
//        }
//    }

//    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
//    m_imageMutex.unlock();

//    m_imageCount++;
//    imageChangedHook();
//    emit imageChanged(m_image);
//}

//void CameraImage_RGB32::setRGB32Image(const uint8_t *imgBuf, QSize size)
//{
//    int imageWidth = size.width();
//    int imageHeight = size.height();

//    m_imageMutex.lock();

//    if(m_image.size() != size)
//    {
//        m_image = QImage(size, QImage::Format_RGB32);
//    }

//    for (int row = 0; row < imageHeight; row++)
//    {
//        QRgb * line = (QRgb *)m_image.scanLine(row);
//        // 找到 CCD 数据一行的开头位置，一个像素占 4 字节
//        QRgb * pixel = (QRgb *) &imgBuf[4 * row * imageWidth];
//        for (int col = 0; col < imageWidth; col ++)
//        {
//            line[col] = pixel[col];
//        }
//    }
//    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
//    m_imageMutex.unlock();

//    m_imageCount++;
//    imageChangedHook();
//    emit imageChanged(m_image);
//}

void CameraImage_RGB32::setBayerImage(const uint8_t *imgBuf, QSize size)
{
    m_imageMutex.lock();
    if(m_image.size() != size)
    {
        m_image = QImage(size, QImage::Format_RGB32);
    }

    Bayer8toRGB32(imgBuf, m_inputPixelFormat, size, m_image);
//    QRgb *rgb32 = (QRgb *)m_image.scanLine(0);
//    bayer_to_RGB32_NearestNeighbor(imgBuf, rgb32, imageWidth, imageHeight, m_bayerPattern);

    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
    m_imageMutex.unlock();

    m_imageCount++;
    imageChangedHook();
    emit imageChanged(m_image);
}

void CameraImage_RGB32::setYUV422Image(const uint8_t *imgBuf, AVPixelFormat format, QSize size)
{
    m_imageMutex.lock();
    if(m_image.size() != size)
    {
        m_image = QImage(size, QImage::Format_RGB32);
    }

    YUV422toRGB32(imgBuf, format, size, m_image);

    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
    m_imageMutex.unlock();
    m_imageCount++;
    imageChangedHook();
    emit imageChanged(m_image);
}


//只用来测试
void CameraImage_RGB32::fillImageTestPattern()
{
    //只用来测试
    static char c = 0;

    int width = m_image.width();
    int height = m_image.height();

    for (int i = 0; i < height; i++)
    {
        QRgb *line = (QRgb *)m_image.scanLine(i);
        for (int j = 0; j < width; j ++)
        {
            line[j] = qRgb(c, c, c);
        }
        c++;
    }
    c++;
    flipImage(m_image, static_cast<FlipMode>(m_flipMode));

    m_imageCount++;
    imageChangedHook();
    emit imageChanged(m_image);
}

void CameraImage_RGB32::imageChangedHook()
{
}

} // namespace Qly
