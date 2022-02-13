#include "PixelFormatConvert.h"

namespace Qly {

enum AVPixelFormat toAVPixelFormat(QImage::Format format)
{
    switch (format) {
    case QImage::Format_Invalid:
        return AV_PIX_FMT_NONE;
    case QImage::Format_Mono:
        return AV_PIX_FMT_MONOBLACK;
    case QImage::Format_MonoLSB:
        return AV_PIX_FMT_NONE;
    case QImage::Format_Indexed8:
        return AV_PIX_FMT_PAL8;
    case QImage::Format_Alpha8:
    case QImage::Format_Grayscale8:
        return AV_PIX_FMT_GRAY8;
    case QImage::Format_Grayscale16:
        return AV_PIX_FMT_GRAY16LE;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        //return AV_PIX_FMT_BGRA;
        return AV_PIX_FMT_BGR0;
    case QImage::Format_RGB16:
    case QImage::Format_ARGB8565_Premultiplied:
        return AV_PIX_FMT_RGB565LE;
    case QImage::Format_RGB666:
    case QImage::Format_ARGB6666_Premultiplied:
        return AV_PIX_FMT_NONE;
    case QImage::Format_RGB555:
    case QImage::Format_ARGB8555_Premultiplied:
        return AV_PIX_FMT_BGR555LE;
    case QImage::Format_RGB888:
        return AV_PIX_FMT_RGB24;
    case QImage::Format_RGB444:
    case QImage::Format_ARGB4444_Premultiplied:
        return AV_PIX_FMT_RGB444LE;
    case QImage::Format_RGBX8888:
    case QImage::Format_RGBA8888:
    case QImage::Format_RGBA8888_Premultiplied:
        return AV_PIX_FMT_RGBA;
    case QImage::Format_BGR30:
    case QImage::Format_A2BGR30_Premultiplied:
    case QImage::Format_RGB30:
    case QImage::Format_A2RGB30_Premultiplied:
        return AV_PIX_FMT_NONE;
    case QImage::Format_RGBX64:
    case QImage::Format_RGBA64:
    case QImage::Format_RGBA64_Premultiplied:
        return AV_PIX_FMT_RGBA64LE;
    case QImage::Format_BGR888:
        return AV_PIX_FMT_BGR24;
    default:
        return AV_PIX_FMT_NONE;
    }
    return AV_PIX_FMT_NONE;
}


bool RGBtoRGB888(const uchar *pData, int stride, AVPixelFormat format, QSize size, QImage &image)
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
    in_linesize[0] = stride;
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

bool RGBtoRGB32(const uchar *pData, int stride, AVPixelFormat format, QSize size, QImage &image)
{
    int srcW = size.width();
    int srcH = size.height();
    int desW = srcW;
    int desH = srcH;
   // qDebug() << "in RGBtoRGB32, image.size() = " << image.size() << ", size = " << size;
    if(image.size() != size || image.format() != QImage::Format_RGB32)
    {
        image = QImage(size, QImage::Format_RGB32);
    }

    uint8_t *in_data[4];
    int in_linesize[4];
    in_data[0] = (uint8_t *) pData;
    in_linesize[0] = stride;
    //av_image_fill_arrays(in_data, in_linesize, inImage.bits(), AV_PIX_FMT_YUYV422, srcW, srcH, 1);
    //qDebug() << "stride = " << stride;

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

bool YUV422toRGB888(const uchar *pData, int stride, AVPixelFormat format, QSize size, QImage &image)
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
    in_linesize[0] = stride;
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

bool YUV422toRGB32(const uchar *pData, int stride, AVPixelFormat format, QSize size, QImage &image)
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
    in_linesize[0] = stride;
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

bool YUV422toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
{
    return YUV422toRGB888(pData, size.width() * 2, format, size, image);
}

bool YUV422toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
{
    return YUV422toRGB32(pData, size.width() * 2, format, size, image);
}

bool Bayer8toRGB888(const uchar *pData, int stride, AVPixelFormat format, QSize size, QImage &image)
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
    in_linesize[0] = stride;
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

bool Bayer8toRGB32(const uchar *pData, int stride, AVPixelFormat format, QSize size, QImage &image)
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
    in_linesize[0] = stride;
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

bool Bayer8toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
{
    return Bayer8toRGB888(pData, size.width() * 2, format, size, image);
}

bool Bayer8toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
{
    return Bayer8toRGB32(pData, size.width() * 2, format, size, image);
}

bool Bayer16toRGB888(const uchar *pData, int stride, AVPixelFormat format, QSize size, QImage &image)
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
    in_linesize[0] = stride;
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

bool Bayer16toRGB32(const uchar *pData, int stride, AVPixelFormat format, QSize size, QImage &image)
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
    in_linesize[0] = stride;
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

bool Bayer16toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
{
    return Bayer16toRGB888(pData, size.width() * 4, format, size, image);
}

bool Bayer16toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
{
    return Bayer16toRGB32(pData, size.width() * 4, format, size, image);
}
}
