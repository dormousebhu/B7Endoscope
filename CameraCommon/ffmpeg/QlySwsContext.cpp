#include "QlySwsContext.h"

QlySwsContext::QlySwsContext(QSize srcSize, enum AVPixelFormat srcFormat, QSize dstSize, enum AVPixelFormat dstFormat, int flag)
    :m_srcSize(srcSize),
      m_dstSize(dstSize),
      m_srcFormat(srcFormat),
      m_dstFormat(dstFormat),
      m_flag(flag)
{
    m_pContext = sws_getContext(srcSize.width(), srcSize.height(), srcFormat, dstSize.width(), dstSize.height(), dstFormat, flag, nullptr, nullptr, nullptr);
}

QlySwsContext::~QlySwsContext()
{
    sws_freeContext(m_pContext);
}

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
        return AV_PIX_FMT_BGRA;
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

enum AVPixelFormat toAVPixelFormat(QVideoFrame::PixelFormat format)
{
    switch (format) {
    case QVideoFrame::Format_ARGB32:
    case QVideoFrame::Format_ARGB32_Premultiplied:
    case QVideoFrame::Format_RGB32:
        return AV_PIX_FMT_RGBA;
    case QVideoFrame::Format_RGB24:
        return AV_PIX_FMT_RGB24;
    case QVideoFrame::Format_RGB565:
    case QVideoFrame::Format_ARGB8565_Premultiplied:
        return AV_PIX_FMT_RGB565LE;
    case QVideoFrame::Format_RGB555:
        return AV_PIX_FMT_RGB555LE;
    case QVideoFrame::Format_BGRA32:
    case QVideoFrame::Format_BGRA32_Premultiplied:
    case QVideoFrame::Format_ABGR32:
    case QVideoFrame::Format_BGR32:
        return AV_PIX_FMT_BGRA;
    case QVideoFrame::Format_BGR24:
        return AV_PIX_FMT_BGR24;
    case QVideoFrame::Format_BGR565:
        return AV_PIX_FMT_BGR565;
    case QVideoFrame::Format_BGR555:
        return AV_PIX_FMT_BGR555;
    case QVideoFrame::Format_BGRA5658_Premultiplied:
        return AV_PIX_FMT_BGR565;
    case QVideoFrame::Format_AYUV444:
    case QVideoFrame::Format_AYUV444_Premultiplied:
        return AV_PIX_FMT_BGR555;
    case QVideoFrame::Format_YUV444:
        return AV_PIX_FMT_BGR555;
    case QVideoFrame::Format_YUV420P:
        return AV_PIX_FMT_BGR555;
    case QVideoFrame::Format_YV12:
        return AV_PIX_FMT_BGR555;
    case QVideoFrame::Format_UYVY:
        return AV_PIX_FMT_BGR555;
    case QVideoFrame::Format_YUYV:
        return AV_PIX_FMT_BGR555;
    case QVideoFrame::Format_NV12:
        return AV_PIX_FMT_BGR555;
    }
}

bool QlySwsContext::YUV422toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
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

bool QlySwsContext::Bayer16toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
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

bool QlySwsContext::Bayer8toRGB888(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
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

bool QlySwsContext::Bayer8toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
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

    SwsContext * pContext = sws_getContext(srcW, srcH, format,
                                           desW, desH, AV_PIX_FMT_BGRA, SWS_POINT, nullptr, nullptr, nullptr);
    if(!pContext) return false;
    sws_scale(pContext, in_data, in_linesize, 0, srcH,
              out_data, out_linesize);
    sws_freeContext(pContext);
    return true;

}

bool QlySwsContext::Bayer16toRGB32(const uchar *pData, AVPixelFormat format, QSize size, QImage &image)
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

    SwsContext * pContext = sws_getContext(srcW, srcH, format,
                                           desW, desH, AV_PIX_FMT_BGRA, SWS_POINT, nullptr, nullptr, nullptr);
    if(!pContext) return false;
    sws_scale(pContext, in_data, in_linesize, 0, srcH,
              out_data, out_linesize);
    sws_freeContext(pContext);
    return true;

}

bool QlySwsContext::scale(const QImage &inImage, QImage &outImage, double scaleX, double scaleY)
{
    int srcW = inImage.width();
    int srcH = inImage.height();
    int desW = srcW * scaleX;
    int desH = srcH * scaleY;
    if(outImage.size() != QSize(desW, desH) || outImage.format() != inImage.format())
    {
        outImage = QImage(QSize(desW, desH), inImage.format());
    }
    AVPixelFormat srcFormat = toAVPixelFormat(inImage.format());

    uint8_t *in_data[4];
    int in_linesize[4];
    in_data[0] = (uint8_t *) inImage.bits();
    in_linesize[0] = inImage.bytesPerLine();
    //av_image_fill_arrays(in_data, in_linesize, inImage.bits(), AV_PIX_FMT_YUYV422, srcW, srcH, 1);

    uint8_t *out_data[4];
    int out_linesize[4];
    out_data[0] = outImage.bits();
    out_linesize[0] = outImage.bytesPerLine();

    SwsContext * pContext = sws_getContext(srcW, srcH, srcFormat,
                                           desW, desH, srcFormat, SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
    if(!pContext) return false;
    sws_scale(pContext, in_data, in_linesize, 0, srcH,
              out_data, out_linesize);
    sws_freeContext(pContext);
    return true;
}

bool QlySwsContext::convert(QVideoFrame &frame, QImage &image)
{
    int srcW = frame.width();
    int srcH = frame.height();
    int desW = image.width();
    int desH = image.height();

    AVPixelFormat srcFormat = toAVPixelFormat(frame.pixelFormat());
    AVPixelFormat dstFormat = toAVPixelFormat(image.format());

    uint8_t *in_data[4];
    int in_linesize[4];
    frame.map(QAbstractVideoBuffer::ReadOnly);//
    in_data[0] = (uint8_t *) frame.bits();
    in_linesize[0] = frame.bytesPerLine();


    uint8_t *out_data[4];
    int out_linesize[4];
    out_data[0] = image.bits();
    out_linesize[0] = image.bytesPerLine();

    SwsContext * pContext = sws_getContext(srcW, srcH, srcFormat,
                                           desW, desH, dstFormat, SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
    if(!pContext) return false;
    sws_scale(pContext, in_data, in_linesize, 0, srcH,
              out_data, out_linesize);
    sws_freeContext(pContext);
    return true;
}
