#ifndef PIXELFORMATCOVERT_H
#define PIXELFORMATCOVERT_H


#include <QSize>
#include <QImage>
#include <QVideoFrame>

extern "C" {
//#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavutil/pixfmt.h"
}

namespace Qly {

/**
 * @brief YUV422toRGB888
 * @param pData
 * @param format  AV_PIX_FMT_YUYV422
 *                AV_PIX_FMT_UYVY422
 * @param size
 * @param image
 * @return
 */
bool YUV422toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image);
bool YUV422toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image);

/**
 * @brief BayertoRGB888
 * @param pData
 * @param format AV_PIX_FMT_BAYER_BGGR8,
 *               AV_PIX_FMT_BAYER_RGGB8,
 *               AV_PIX_FMT_BAYER_GBRG8,
 *               AV_PIX_FMT_BAYER_GRBG8
 * @param size
 * @param image
 * @return
 */
bool Bayer8toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image);
bool Bayer8toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image);
/**
 * @brief Bayer16toRGB888
 * @param pData
 * @param format AV_PIX_FMT_BAYER_BGGR16LE
 *               AV_PIX_FMT_BAYER_BGGR16BE
 *               AV_PIX_FMT_BAYER_RGGB16LE
 *               AV_PIX_FMT_BAYER_RGGB16BE
 *               AV_PIX_FMT_BAYER_GBRG16LE
 *               AV_PIX_FMT_BAYER_GBRG16BE
 *               AV_PIX_FMT_BAYER_GRBG16LE
 *               AV_PIX_FMT_BAYER_GRBG16BE
 * @param size
 * @param image
 * @return
 */
bool Bayer16toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image);
bool Bayer16toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image);
}
#endif // PIXELFORMATCOVERT_H
