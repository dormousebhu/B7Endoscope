/****************************************************************************
** file: CameraImage_RGB32.h
** brief: CameraImage 类的派生类。内部存储图像类型为 RGB888。
** Copyright (C) LiYuan
** Author: LiYuan
** E-Mail: 18069211#qq(.)com
** Version 1.0.1
** Last modified: 2019.01.11
** Modified By: LiYuan
****************************************************************************/


#ifndef CAMERAIMAGE_RGB32_H
#define CAMERAIMAGE_RGB32_H

#include "CameraImage.h"

namespace Qly
{
class CameraImage_RGB32 : public CameraImage
{
    Q_OBJECT
public:
    explicit CameraImage_RGB32(QObject *parent = 0);
public slots:
    void setImage(const QImage &image) Q_DECL_OVERRIDE;
    void setYUV422Image(const uint8_t *imgBuf, AVPixelFormat format, QSize size) Q_DECL_OVERRIDE;
    void setRGB24Image(const uint8_t *imgBuf, QSize size) Q_DECL_OVERRIDE;
    void setRGB32Image(const uint8_t *imgBuf, QSize size) Q_DECL_OVERRIDE;
    void setMono8Image(const uint8_t *imgBuf, QSize size) Q_DECL_OVERRIDE;
    void setBayerImage(const uint8_t *imgBuf, QSize size) Q_DECL_OVERRIDE;
    void fillImageTestPattern() Q_DECL_OVERRIDE;
private:
    void flipImage(QImage &image, FlipMode flip_mode) Q_DECL_OVERRIDE;
    void imageChangedHook();
};

} //namespace
#endif // CAMERAIMAGE_H
