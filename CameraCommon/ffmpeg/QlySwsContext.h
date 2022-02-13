#ifndef QLYSWSCONTEXT_H
#define QLYSWSCONTEXT_H

#include <QSize>
#include <QImage>
#include <QVideoFrame>

extern "C" {
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}
class QlySwsContext
{
public:
    QlySwsContext(QSize srcSize, enum AVPixelFormat srcFormat, QSize dstSize, enum AVPixelFormat dstFormat, int flag);
    ~QlySwsContext();

    static bool convert(QVideoFrame &frame, QImage &image);
    static bool scale(const QImage &inImage, QImage &outImage, double scaleX, double scaleY); //
    static bool YUV422toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image);

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
    static bool Bayer8toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image);
    static bool Bayer8toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image);
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
    static bool Bayer16toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image);
    static bool Bayer16toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image);

protected:
    SwsContext * m_pContext;
    QSize m_srcSize;
    QSize m_dstSize;
    AVPixelFormat m_srcFormat;
    AVPixelFormat m_dstFormat;

    int m_flag;

};

#endif // QLYSWSCONTEXT_H
