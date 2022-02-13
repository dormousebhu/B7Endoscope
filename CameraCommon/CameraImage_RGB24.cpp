﻿/****************************************************************************
** file: CameraImage_RGB24.cpp
** brief: CameraImage 类的派生类。内部存储图像类型为 RGB888。
** Copyright (C) LiYuan
** Author: LiYuan
** E-Mail: 18069211#qq(.)com
** Version 1.0.1
** Last modified: 2019.01.11
** Modified By: LiYuan
****************************************************************************/

#include "CameraImage_RGB24.h"
#include <QColor>
#include <QDebug>

#include "PixelFormatConvert.h"

namespace Qly {

static const int MAX_IMAGE_SIZE_X = 20000;
static const int MAX_IMAGE_SIZE_Y = 20000;
static const int DEFAULT_IMAGE_SIZE_X = 1920;
static const int DEFAULT_IMAGE_SIZE_Y = 1200;


struct rgb24
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

CameraImage_RGB24::CameraImage_RGB24(QObject *parent)
    : CameraImage(parent)
{
    m_image = QImage( QSize(DEFAULT_IMAGE_SIZE_X, DEFAULT_IMAGE_SIZE_Y), QImage::Format_RGB888 );
}


/**
 * @brief CameraView::flipImage  将图像翻转
 * @param image
 * @param flip_mode
 */
void CameraImage_RGB24::flipImage(QImage &image, FlipMode flip_mode)
{
    int width = image.width();
    int height = image.height();
    switch (flip_mode)
    {
    case BOTH:
        for(int i = 0; i < height / 2; i++)
        {
            rgb24 * line1 = (rgb24 *) image.scanLine(i);
            rgb24 * line2 = (rgb24 *) image.scanLine(height - i - 1);
            for(int j = 0; j < width; j ++)
            {
                std::swap(line1[j], line2[width - j - 1]);
            }
        }
        break;
    case HORIZONTAL_FLIP:
        for(int i = 0; i < height; i++)
        {
            rgb24 * line = (rgb24 *) image.scanLine(i);
            for(int j = 0; j < width / 2; j ++)
            {
                std::swap(line[j], line[width - j - 1]);
            }
        }
        break;
    case VERTICAL_FLIP:
        for(int i = 0; i < height / 2; i++)
        {
            rgb24 * line1 = (rgb24 *) image.scanLine(i);
            rgb24 * line2 = (rgb24 *) image.scanLine(height - i - 1);
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

void CameraImage_RGB24::setImage(const QImage &image)
{
    m_imageMutex.lock();
    m_image = image.convertToFormat(QImage::Format_RGB888);
    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
    m_imageMutex.unlock();
    imageChangedHook();
    emit imageChanged(m_image);
    //qDebug() << "emit imageChanged(m_image)";
}


void CameraImage_RGB24::setRGBImage(const uint8_t *imgBuf, int stride, AVPixelFormat format, QSize size)
{
    m_imageMutex.lock();
    if(m_image.size() != size)
    {
        m_image = QImage(size, QImage::Format_RGB888);
    }

    RGBtoRGB888(imgBuf, stride, format, size, m_image);
    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
    m_imageMutex.unlock();

    m_imageCount++;

    imageChangedHook();
    emit imageChanged(m_image);
}

//void CameraImage_RGB24::setMono8Image(const uint8_t *imgBuf, QSize size)
//{
//    int imageWidth = size.width();
//    int imageHeight = size.height();

//    m_imageMutex.lock();
//    if(m_image.size() != size)
//    {
//        m_image = QImage(size, QImage::Format_RGB888);
//    }
//    for (int row = 0; row < imageHeight; row++)
//    {
//        uint8_t *line = (uint8_t *)m_image.scanLine(row);
//        // 找到 CCD 数据一行的开头位置，一个像素占 1 字节
//        uint8_t * pixel = (uint8_t *) &imgBuf[row * imageWidth];
//        for (int col = 0; col < imageWidth; col ++)
//        {
//            line[3 * col] = pixel[col];
//            line[3 * col + 1] = pixel[col];
//            line[3 * col + 2] = pixel[col];
//        }
//    }
//    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
//    m_imageMutex.unlock();

//    m_imageCount++;
//    imageChangedHook();
//    emit imageChanged(m_image);
//    qDebug() << "emit imageChanged(m_image)";
//}

//void CameraImage_RGB24::setRGB24Image(const uint8_t *imgBuf, QSize size)
//{
//    int imageWidth = size.width();
//    int imageHeight = size.height();

//    m_imageMutex.lock();
//    if(m_image.size() != size)
//    {
//        m_image = QImage(size, QImage::Format_RGB888);
//    }
//    for (int row = 0; row < imageHeight; row++)
//    {
//        rgb24 *line = (rgb24 *)m_image.scanLine(row);
//        // 找到 CCD 数据一行的开头位置，一个像素占3字节
//        rgb24 * pixel = (rgb24 *) &imgBuf[3 * row * imageWidth];
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
//    //qDebug() << "emit imageChanged(m_image)";
//}

//void CameraImage_RGB24::setRGB32Image(const uint8_t *imgBuf, QSize size)
//{
//    int imageWidth = size.width();
//    int imageHeight = size.height();

//    m_imageMutex.lock();

//    if(m_image.size() != size)
//    {
//        m_image = QImage(size, QImage::Format_RGB888);
//    }

//    for (int row = 0; row < imageHeight; row++)
//    {
//        uint8_t * line = (uint8_t *)m_image.scanLine(row);
//        // 找到 CCD 数据一行的开头位置，一个像素占 4 字节
//        uint8_t * pixel = (uint8_t *) &imgBuf[4 * row * imageWidth];
//        for (int col = 0; col < imageWidth; col ++)
//        {
//            line[3 * col] = pixel[4 * col + 2];
//            line[3 * col + 1] = pixel[4 * col + 1];
//            line[3 * col + 2] = pixel[4 * col];
//        }
//    }
//    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
//    m_imageMutex.unlock();

//    m_imageCount++;
//    imageChangedHook();
//    emit imageChanged(m_image);
//    //qDebug() << "emit imageChanged(m_image)";
//}

void CameraImage_RGB24::setBayerImage(const uint8_t *imgBuf, QSize size)
{
    m_imageMutex.lock();
    if(m_image.size() != size)
    {
        m_image = QImage(size, QImage::Format_RGB888);
    }

    Bayer8toRGB32(imgBuf, m_inputPixelFormat, size, m_image);
    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
    m_imageMutex.unlock();

    m_imageCount++;

    imageChangedHook();
    emit imageChanged(m_image);
    //qDebug() << "emit imageChanged(m_image)";
}

void CameraImage_RGB24::setYUV422Image(const uint8_t *imgBuf, AVPixelFormat format, QSize size)
{
    m_imageMutex.lock();
    if(m_image.size() != size)
    {
        m_image = QImage(size, QImage::Format_RGB888);
    }

    YUV422toRGB888(imgBuf, format, size, m_image);

    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
    m_imageMutex.unlock();
    m_imageCount++;
    imageChangedHook();
    emit imageChanged(m_image);
}


//void CameraImage_RGB24::setYUY2Image(const uint8_t *imgBuf, QSize size)
//{
//    int imageWidth = size.width();
//    int imageHeight = size.height();

//    m_imageMutex.lock();
//    if(m_image.size() != size)
//    {
//        m_image = QImage(size, QImage::Format_RGB888);
//    }

//    for (int row = 0; row < imageHeight; row++)
//    {
//        uint8_t *line = (uint8_t *)m_image.scanLine(row);
//        // 找到 CCD 数据一行的开头位置，一个像素占 2 字节
//        uint8_t * pixel = (uint8_t *) &imgBuf[2 * row * imageWidth];
//        //m_yuvConverter.YUY2_to_RGB24(pixel, line, imageWidth);
//        m_yuvConverter.YUY2_to_BGR24(pixel, line, imageWidth);
//    }
//    flipImage(m_image, static_cast<FlipMode>(m_flipMode));
//    m_imageMutex.unlock();

//    m_imageCount++;
//    imageChangedHook();
//    emit imageChanged(m_image);
//    //qDebug() << "emit imageChanged(m_image)";
//}

//只用来测试
void CameraImage_RGB24::fillImageTestPattern()
{
    //只用来测试
    static char c = 0;

    int width = m_image.width();
    int height = m_image.height();

    for (int i = 0; i < height; i++)
    {
        uint8_t *line = (uint8_t *)m_image.scanLine(i);
        for (int j = 0; j < width; j ++)
        {
            line[3 * j] = c;
            line[3 * j + 1] = c;
            line[3 * j + 2] = c;
        }
        c++;
    }
    c++;
    flipImage(m_image, static_cast<FlipMode>(m_flipMode));

    m_imageCount++;
    imageChangedHook();
    emit imageChanged(m_image);
    //qDebug() << "emit imageChanged(m_image)";
}

void CameraImage_RGB24::imageChangedHook()
{
}

} //namespace Qly
