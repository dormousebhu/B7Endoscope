#include "PixelFormatConvert.h"

namespace Qly {
bool YUV422toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
{
    int srcW = size.width();
    int srcH = size.height();
    int desW = srcW;
    int desH = srcH;
    if(image.size() != size || image.format() != QImage::Format_RGB888)
    {
        image = QImage(size, QImage::Format_RGB888);
    }

    uint8_t *in_data[4];
    int in_linesize[4];
    in_data[0] = (uint8_t *) pData;
    in_linesize[0] = srcW * 2;
    //av_image_fill_arrays(in_data, in_linesize, inImage.bits(), AV_PIX_FMT_YUYV422, srcW, srcH, 1);

    uint8_t *out_data[4];
    int out_linesize[4];
    out_data[0] = image.bits();
    out_linesize[0] = image.bytesPerLine();

    SwsContext * pContext = sws_getContext(srcW, srcH, format,
                                           desW, desH, AV_PIX_FMT_RGB24, SWS_POINT, nullptr, nullptr, nullptr);
    if(!pContext) return false;
    sws_scale(pContext, in_data, in_linesize, 0, srcH, out_data, out_linesize);
    sws_freeContext(pContext);
    return true;
}

bool YUV422toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
{
    int srcW = size.width();
    int srcH = size.height();
    int desW = srcW;
    int desH = srcH;
    if(image.size() != size || image.format() != QImage::Format_RGB32)
    {
        image = QImage(size, QImage::Format_RGB32);
    }

    uint8_t *in_data[4];
    int in_linesize[4];
    in_data[0] = (uint8_t *) pData;
    in_linesize[0] = srcW * 2;
    //av_image_fill_arrays(in_data, in_linesize, inImage.bits(), AV_PIX_FMT_YUYV422, srcW, srcH, 1);

    uint8_t *out_data[4];
    int out_linesize[4];
    out_data[0] = image.bits();
    out_linesize[0] = image.bytesPerLine();

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    SwsContext * pContext = sws_getContext(srcW, srcH, format,
                                           desW, desH, AV_PIX_FMT_BGRA, SWS_POINT, nullptr, nullptr, nullptr);
#endif
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    SwsContext * pContext = sws_getContext(srcW, srcH, format,
                                           desW, desH, AV_PIX_FMT_ARGB, SWS_POINT, nullptr, nullptr, nullptr);
#endif

    if(!pContext) return false;
    sws_scale(pContext, in_data, in_linesize, 0, srcH, out_data, out_linesize);
    sws_freeContext(pContext);
    return true;

}

bool Bayer16toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
{
    int srcW = size.width();
    int srcH = size.height();
    int desW = srcW;
    int desH = srcH;
    if(image.size() != size || image.format() != QImage::Format_RGB888)
    {
        image = QImage(size, QImage::Format_RGB888);
    }

    uint8_t *in_data[4];
    int in_linesize[4];
    in_data[0] = (uint8_t *) pData;
    in_linesize[0] = srcW * 4;
    //av_image_fill_arrays(in_data, in_linesize, inImage.bits(), AV_PIX_FMT_YUYV422, srcW, srcH, 1);

    uint8_t *out_data[4];
    int out_linesize[4];
    out_data[0] = image.bits();
    out_linesize[0] = image.bytesPerLine();

    SwsContext * pContext = sws_getContext(srcW, srcH, format,
                                           desW, desH, AV_PIX_FMT_RGB24, SWS_POINT, nullptr, nullptr, nullptr);
    if(!pContext) return false;
    sws_scale(pContext, in_data, in_linesize, 0, srcH,
              out_data, out_linesize);
    sws_freeContext(pContext);
    return true;

}

bool Bayer8toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
{
    int srcW = size.width();
    int srcH = size.height();
    int desW = srcW;
    int desH = srcH;
    if(image.size() != size || image.format() != QImage::Format_RGB888)
    {
        image = QImage(size, QImage::Format_RGB888);
    }

    uint8_t *in_data[4];
    int in_linesize[4];
    in_data[0] = (uint8_t *) pData;
    in_linesize[0] = srcW * 2;
    //av_image_fill_arrays(in_data, in_linesize, inImage.bits(), AV_PIX_FMT_YUYV422, srcW, srcH, 1);

    uint8_t *out_data[4];
    int out_linesize[4];
    out_data[0] = image.bits();
    out_linesize[0] = image.bytesPerLine();

    SwsContext * pContext = sws_getContext(srcW, srcH, format,
                                           desW, desH, AV_PIX_FMT_RGB24, SWS_POINT, nullptr, nullptr, nullptr);
    if(!pContext) return false;
    sws_scale(pContext, in_data, in_linesize, 0, srcH,
              out_data, out_linesize);
    sws_freeContext(pContext);
    return true;
}

bool Bayer8toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
{
    int srcW = size.width();
    int srcH = size.height();
    int desW = srcW;
    int desH = srcH;
    if(image.size() != size || image.format() != QImage::Format_RGB888)
    {
        image = QImage(size, QImage::Format_RGB32);
    }

    uint8_t *in_data[4];
    int in_linesize[4];
    in_data[0] = (uint8_t *) pData;
    in_linesize[0] = srcW * 2;
    //av_image_fill_arrays(in_data, in_linesize, inImage.bits(), AV_PIX_FMT_YUYV422, srcW, srcH, 1);

    uint8_t *out_data[4];
    int out_linesize[4];
    out_data[0] = image.bits();
    out_linesize[0] = image.bytesPerLine();

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    SwsContext * pContext = sws_getContext(srcW, srcH, format,
                                           desW, desH, AV_PIX_FMT_BGRA, SWS_POINT, nullptr, nullptr, nullptr);
#endif
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    SwsContext * pContext = sws_getContext(srcW, srcH, format,
                                           desW, desH, AV_PIX_FMT_ARGB, SWS_POINT, nullptr, nullptr, nullptr);
#endif

    if(!pContext) return false;
    sws_scale(pContext, in_data, in_linesize, 0, srcH,
              out_data, out_linesize);
    sws_freeContext(pContext);
    return true;

}

bool Bayer16toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
{
    int srcW = size.width();
    int srcH = size.height();
    int desW = srcW;
    int desH = srcH;
    if(image.size() != size || image.format() != QImage::Format_RGB32)
    {
        image = QImage(size, QImage::Format_RGB888);
    }

    uint8_t *in_data[4];
    int in_linesize[4];
    in_data[0] = (uint8_t *) pData;
    in_linesize[0] = srcW * 2;
    //av_image_fill_arrays(in_data, in_linesize, inImage.bits(), AV_PIX_FMT_YUYV422, srcW, srcH, 1);

    uint8_t *out_data[4];
    int out_linesize[4];
    out_data[0] = image.bits();
    out_linesize[0] = image.bytesPerLine();

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    SwsContext * pContext = sws_getContext(srcW, srcH, format,
                                           desW, desH, AV_PIX_FMT_BGRA, SWS_POINT, nullptr, nullptr, nullptr);
#endif
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    SwsContext * pContext = sws_getContext(srcW, srcH, format,
                                           desW, desH, AV_PIX_FMT_ARGB, SWS_POINT, nullptr, nullptr, nullptr);
#endif

    if(!pContext) return false;
    sws_scale(pContext, in_data, in_linesize, 0, srcH,
              out_data, out_linesize);
    sws_freeContext(pContext);
    return true;

}
}
