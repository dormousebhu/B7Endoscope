/****************************************************************************
** file: CameraImage.h
** brief: 提供了相机数据解析为 QImage 功能的接口。这个类及派生类实现的功能很多。
** 包括解析 Bayer 格式,解析 YUV 格式，图像的水平、竖直翻转等。
** Copyright (C) LiYuan
** Author: LiYuan
** E-Mail: 18069211#qq(.)com
** Version 2.0.1
** Last modified: 2019.01.11
** Modified By: LiYuan
****************************************************************************/


#ifndef CAMERAIMAGE_H
#define CAMERAIMAGE_H

#include <QObject>
#include <QImage>
#include <QSize>
#include <QMutex>
#include <QVideoFrame>
#include <QAbstractVideoSurface>
#include "IndustryCamera.h"
extern "C" {
#include "libavutil/pixfmt.h"
}

namespace Qly
{
/**
 * @brief The CameraImage class 这个类是纯虚类，提供了相机原始图像数据到 QImage 类的转换功能。
 */
class CameraImage : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const Q_DECL_OVERRIDE;

    enum FlipMode{ NONE = 0, HORIZONTAL_FLIP = 1, VERTICAL_FLIP = 2, BOTH = 3};
    explicit CameraImage(QObject *parent = 0);
    AVPixelFormat bayerPattern() const {return m_inputPixelFormat;}
    int imageCount() {return m_imageCount;}
    /**
     * @brief getImage 获取图像
     * @return
     */
    QImage getImage();

    /**
    * @brief 加载文件中的图像
    * @param fileName 带路径的完整文件名
    * @return true 表示加载成功，false 表示加载失败
    */
    bool loadImage(const QString &fileName);

    /**
     * @brief saveImage 将图像保存到文件中
     * @param fileName 带路径的完整文件名
     * @return true 表示保存成功
     */
    bool saveImage(const QString &fileName);
signals:
    /**
     * @brief flipModeChanged 图像翻转模式改变时发送这个信号
     * @param mode
     */
    void flipModeChanged(CameraImage::FlipMode mode);
    /**
     * @brief bayerPatternChanged Color Filter 起始图案更改时发送这个信号
     * @param pattern
     */
    void bayerPatternChanged(AVPixelFormat pattern);

    /**
     * @brief imageChanged 当图像发生变化时发送这个信号
     * @param image
     */
    void imageChanged(QImage &image);
public slots:
    void setVideoFrame(const QVideoFrame &frame);
    void setImageData(const uint8_t *imgBuf, QSize size, AVPixelFormat format);
    virtual void setImage(const QImage &image) = 0;
    virtual void setRGB24Image(const uint8_t *imgBuf, QSize size) = 0;
    virtual void setRGB32Image(const uint8_t *imgBuf, QSize size) = 0;
    virtual void setMono8Image(const uint8_t *imgBuf, QSize size) = 0;
    virtual void setYUV422Image(const uint8_t *imgBuf, AVPixelFormat format, QSize size) = 0;
    virtual void setBayerImage(const uint8_t *imgBuf, QSize size) = 0;
    virtual void fillImageTestPattern() = 0;
    /**
     * @brief setFlipMode 设置图像翻转的模式。每次更新图像时，可以对图像进行左右翻转或上下翻转，也可同时翻转。
     * @param flipMode
     */
    void setFlipMode(CameraImage::FlipMode flipMode);
    /**
     * @brief setHorizontalFlip 设置图像是否左右翻转
     * @param on true 表示开启图像左右翻转
     */
    void setHorizontalFlip(bool on);
    /**
     * @brief setVerticalFlip 设置图像是否上下翻转
     * @param on true 表示开启图像上下翻转
     */
    void setVerticalFlip(bool on);
    /**
     * @brief setBayerPattern 制定 Color Filter 的起始图案
     * @param pattern
     */
    void setBayerPattern(AVPixelFormat pattern);

protected:
    int m_flipMode; // 这里故意设为int类型，不用FlipMode类型
    AVPixelFormat m_inputPixelFormat;
    //YUV2RGBConverter m_yuvConverter;
    QImage m_image;
    int m_imageCount;
    QMutex m_imageMutex; ///< m_image 会被多个线程访问，需用 mutex 保护
private:
    virtual void flipImage(QImage &image, FlipMode flip_mode) = 0;
private slots:
    bool present(const QVideoFrame &frame) Q_DECL_OVERRIDE;
};

} //namespac Qly
#endif // CAMERAIMAGE_H
