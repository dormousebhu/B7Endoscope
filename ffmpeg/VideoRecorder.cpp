/****************************************************************************
** file: VideoRecorder.cpp
** brief: 利用 ffmpeg 实现视频录制
** Copyright (C) LiYuan
** Author: LiYuan
** E-Mail: 18069211#qq(.)com
** Version 1.0.1
** Last modified: 2021.12.28
** Modified By: LiYuan
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
** THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
****************************************************************************/

#include <QDebug>
#include "VideoRecorder.h"

namespace Qly {

static enum AVPixelFormat toAVPixelFormat(QImage::Format format)
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

VideoRecorder::VideoRecorder(QObject *parent) : QObject(parent)
{
    m_pFormatCtx = nullptr;
    m_pPacket = av_packet_alloc();
    if(!m_pPacket)
    {
        qWarning() << "VideoRecorder::VideoRecorder av_packet_alloc failed.";
    }
    m_pFrame = av_frame_alloc();
    if (!m_pFrame)
    {
        qWarning() << "VideoRecorder::VideoRecorder av_frame_alloc failed.";
    }

    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &Qly::VideoRecorder::timeout);
}

VideoRecorder::~VideoRecorder()
{
    avcodec_free_context(&m_pCodecCtx);
    av_frame_free(&m_pFrame);
    av_packet_free(&m_pPacket);
    if(m_pFormatCtx)
    {
        avformat_free_context(m_pFormatCtx);
    }
}

int VideoRecorder::openFile(QString url)
{
    if(m_recording)
    {
        return -4;
    }
    m_startTime = QTime(); //将 m_startTime 复原到原始状态
    if(url.isNull() || url.isEmpty())
    {
        qWarning() << "VideoRecorder::openFile failed, url is Invalid(empty)";
        return -1;
    }
    m_url = url;
    if(m_pFormatCtx)
    {
        avformat_free_context(m_pFormatCtx);
    }
    m_errorcode = avformat_alloc_output_context2(&m_pFormatCtx, nullptr,
                                               nullptr,
                                               url.toLocal8Bit().constData());
    if(m_errorcode < 0)
    {
        qWarning() << "In VideoRecorder::openFile avformat_alloc_output_context2 failed";
        return -2;
    }
    qDebug() << "avformat_alloc_output_context2 success";
    if (!(m_pFormatCtx->flags & AVFMT_NOFILE))
    {
        m_errorcode = avio_open(&m_pFormatCtx->pb, m_url.toLocal8Bit().constData(), AVIO_FLAG_READ_WRITE);
        if(m_errorcode < 0)
        {
            qWarning() << "in VideoRecorder::openFile avio_open failed";
            return -3;
        }
    }
    qDebug() << "avio_open success";
    m_recording = true;
    return 0;
}

void VideoRecorder::initStreamParameters(AVStream * stream)
{
    stream->time_base.den = m_time_base.den;
    stream->time_base.num = m_time_base.num;
    stream->id = m_pFormatCtx->nb_streams -1;
    stream->index = m_pFormatCtx->nb_streams -1;
    stream->codecpar->codec_tag = 0;
    stream->codecpar->codec_type = m_pCodec->type;
    stream->codecpar->codec_id = m_pCodec->id;
    stream->codecpar->format = m_format;
    stream->codecpar->width = m_width;
    stream->codecpar->height = m_height;
    stream->codecpar->bit_rate = m_bit_rate;
}

int VideoRecorder::initFile(AVCodecID codecID, QSize size)
{
    qDebug() << "IN VideoRecorder::initFile";
    m_width = size.width();
    m_height = size.height();
    m_codecID = codecID;
    m_pCodec = avcodec_find_encoder(codecID);

    if (!m_pCodec)
    {
        qWarning() << "VideoRecorder::initFile avcodec_find_encoder failed.";
        return -2;
    }
    qDebug() << "avcodec_find_encoder success, codecID = " << codecID ;
    AVStream *pStream = avformat_new_stream(m_pFormatCtx, m_pCodec);
    if(pStream == nullptr)
    {
        qWarning() << "VideoRecorder::initFile avformat_new_stream failed.";
        return -3;
    }
    qDebug() << "avformat_new_stream success";

    initStreamParameters(pStream);
    //m_pCodecCtx = pStream->codec;

    qDebug() << "initStreamParameters success";
    if(m_pCodecCtx)
    {
        qDebug() << "avcodec_free_context";
        avcodec_free_context(&m_pCodecCtx);
    }

    qDebug() << "m_pCodecCtx = " << m_pCodecCtx;
    m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
    if(!m_pCodecCtx)
    {
       qWarning() << "VideoRecorder::initFile avcodec_alloc_context3 failed.";
       return -4;
    }
    qDebug() << "avcodec_alloc_context3 success";
    m_pCodecCtx->codec_id = m_pCodec->id;
    m_pCodecCtx->time_base = pStream->time_base;
    m_pCodecCtx->gop_size = 10;
    m_pCodecCtx->max_b_frames = 0;

    //qDebug() << "max_b_frames";

    if (codecID == AV_CODEC_ID_H264)
    {
     av_opt_set(m_pCodecCtx->priv_data, "preset", "fast", 0);
     //av_opt_set(pCodecCtx->priv_data, "tune", "zerolatency", 0);
     //av_opt_set(pCodecCtx->priv_data, "profile", "main", 0);

    }
    else if(codecID == AV_CODEC_ID_H265)
    {
     av_opt_set(m_pCodecCtx->priv_data, "preset", "fast", 0);
     //av_opt_set(pCodecCtx->priv_data, "tune", "zerolatency", 0);
     //av_opt_set(pCodecCtx->priv_data, "profile", "main", 0);
    }

    qDebug() << "av_opt_set";
    /* Some formats want stream headers to be separate. */
    if (m_pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER)
    {
     m_pFormatCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    avcodec_parameters_to_context(m_pCodecCtx, pStream->codecpar);
    m_errorcode = avcodec_open2(m_pCodecCtx, m_pCodec, nullptr);
    if(m_errorcode < 0)
    {
        qWarning() << "VideoRecorder::initFile avcodec_open2 failed.";
        return -5;
    }
    qDebug() << "avcodec_open2 success";
    m_pFrame->format = (int)m_pCodecCtx->pix_fmt;
    m_pFrame->width  = m_pCodecCtx->width;
    m_pFrame->height = m_pCodecCtx->height;

    if( av_frame_get_buffer(m_pFrame, 0) < 0 )
    {
        qWarning() << "VideoRecorder::initFile av_frame_get_buffer() failed.";
        return -6;
    }
     qDebug() << "av_frame_get_buffer success";
    return 0;
}

void VideoRecorder::buildFrameFromImage(AVFrame *pFrame, const QImage &image, int pts)
{
    //qDebug() << "IN VideoRecorder::buildFrameFromImage";
    /* make sure the frame data is writable */
    if (av_frame_make_writable(pFrame) < 0)
    {
        qWarning() << "in VideoRecorder::buildFrameFromImage av_frame_make_writable(pFrame) failed";
        return;
    }

    int width = image.width();
    int height = image.height();
    AVPixelFormat imgFmt = toAVPixelFormat(image.format());
    SwsContext * pContext = sws_getContext(width, height, imgFmt,
                                           width, height, (AVPixelFormat)pFrame->format, SWS_POINT, nullptr, nullptr, nullptr);
    if(!pContext) return;

    const uint8_t *in_data[1];
    int in_linesize[1];

    in_data[0] = image.bits();
    in_linesize[0] = image.bytesPerLine();

    sws_scale(pContext, in_data, in_linesize, 0, height,
              pFrame->data, pFrame->linesize);
    sws_freeContext(pContext);

    pFrame->pts = pts;
}

bool VideoRecorder::setImage(const QImage &image)
{
    return setImage2(image, -1);
}

bool VideoRecorder::setImage2(const QImage &image, int pts)
{
    //qDebug() << "IN VideoRecorder::setImage";
    if(!m_recording)
    {
        //qDebug() << "in VideoRecorder::setImage m_recording = false";
        return false;
    }
    QTime t = QTime::currentTime();
    if( pts < 0 ) // 说明这时要用真实的时间来做为 pts
    {
        if(m_startTime.isNull())
        {
            m_startTime = t; // 说明这是第一帧。需要初始化起始时间。
        }
        int oldpts = m_startTime.msecsTo(t);

        pts = av_rescale_q_rnd(oldpts, AVRational({1, 1000}), m_time_base, AV_ROUND_NEAR_INF);
        //qDebug() << "oldpts = " << oldpts << ", pts = " << pts;
    }
    //qDebug() << "pts = " << pts;
    if(m_width == 0) // 说明这是第一个帧
    {
        initFile(m_codecID, image.size());
        writeHeader();
        av_dump_format(m_pFormatCtx, 0, m_url.toLocal8Bit().constData(), true);
        m_timer.start(m_timeout);
        //qDebug() << QString("m_timer.start(%1);").arg(m_timeout);
    }
    buildFrameFromImage(m_pFrame, image, pts);
    return writeFrame(m_pFrame);
}

void VideoRecorder::timeout()
{
    qDebug() << " VideoRecorder::timeout()";
    close();
    emit recordStoped();
}

void VideoRecorder::setAVCodecID(AVCodecID id)
{
    m_codecID = id;
    m_pCodec = avcodec_find_encoder(id);
    if(m_pCodec)
    {
        const enum AVPixelFormat * pFormat = m_pCodec->pix_fmts;
        if(pFormat)
        {
            while (*pFormat != AV_PIX_FMT_NONE)
            {
                if(*pFormat == m_format)
                {
                    return;
                }
                pFormat ++;
            }
            // 到这里说明 m_format 不在当前 codec 支持的 format 里
            pFormat = m_pCodec->pix_fmts;
            m_format = *pFormat; // 默认使用 codec 支持的第一个 format
        }
    }
}

int VideoRecorder::writeHeader()
{
    m_errorcode = avformat_write_header(m_pFormatCtx, nullptr);
    if(m_errorcode < 0)
    {
        qWarning() << "in VideoRecorder::writeHeader avformat_write_header failed";
        return -2;
    }
    return 0;
}

int VideoRecorder::writeTrailer()
{
    m_errorcode = av_write_trailer(m_pFormatCtx);
    if(m_errorcode < 0)
    {
        qWarning() << "in VideoRecorder::writeTrailer av_write_trailer failed";
        return -1;
    }
    return 0;
}

bool VideoRecorder::writeFrame(const AVFrame *pFrame)
{
    //qDebug() << "IN VideoRecorder::writeFrame";
    m_errorcode = avcodec_send_frame(m_pCodecCtx, pFrame);
    if(m_errorcode < 0)
    {
        qWarning() << "in VideoRecorder::writeFrame avcodec_send_frame failed";
        return false;
    }

    while (m_errorcode >= 0)
    {
        m_errorcode = avcodec_receive_packet(m_pCodecCtx, m_pPacket);
        if (m_errorcode == AVERROR(EAGAIN) || m_errorcode == AVERROR_EOF)
        {
            return true;
        }
        else if (m_errorcode < 0)
        {
            qWarning() << "in VideoRecorder::writeFrame avcodec_receive_packet failed";
            return false;
        }
        m_pPacket->stream_index = 0;
        AVRational out_timebase = m_pFormatCtx->streams[0]->time_base;

        m_pPacket->pts = av_rescale_q_rnd(m_pPacket->pts, m_time_base, out_timebase, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        m_pPacket->dts = av_rescale_q_rnd(m_pPacket->dts, m_time_base, out_timebase,  (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        m_pPacket->duration = av_rescale_q(m_pPacket->duration, m_time_base, out_timebase);
        m_pPacket->pos = -1;

        m_errorcode = av_interleaved_write_frame(m_pFormatCtx, m_pPacket);
        if (m_errorcode < 0)
        {
            qWarning() << "in VideoRecorder::writeFrame av_interleaved_write_frame failed";
            return false;
        }
        av_packet_unref(m_pPacket);
    }
    return true;
}

bool VideoRecorder::close()
{
    if(!m_recording) return false;
    m_recording = false;
    m_timer.stop();

    if(m_width != 0 && m_pCodecCtx && m_pCodec && m_pFormatCtx && m_pFormatCtx->nb_streams != 0) // 视频中已经有一些帧了
    {
        writeFrame(nullptr);
        writeTrailer();
    }

    if (m_pFormatCtx && !(m_pFormatCtx->flags & AVFMT_NOFILE))
    {
        m_errorcode = avio_closep(&m_pFormatCtx->pb);
    }

    m_width = 0;
    m_height = 0;
    return true;
}
}//namespace Qly
