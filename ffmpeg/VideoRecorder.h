/****************************************************************************
** file: VideoRecorder.h
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

#ifndef VIDEORECORDER_H
#define VIDEORECORDER_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <QSize>
#include <QImage>
#include <QQueue>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}

namespace Qly {

class VideoRecorder : public QObject
{
    Q_OBJECT
public:
    explicit VideoRecorder(QObject *parent = nullptr);
    ~VideoRecorder();

    void setMaxRecordTime(int ms) {m_timeout = ms;}
    /**
     * @brief setAVCodecID 设置编码类型。默认是 MPEG4
     * @param id
     */
    void setAVCodecID(AVCodecID id);

    /**
     * @brief setTimeBase 设置视频文件的time base, 默认是 1/1000。 也就是 1ms 为基本单位。
     * @param timebase
     */
    void setTimeBase(AVRational timebase) {m_time_base = timebase;}

    /**
     * @brief openFile 建立视频文件
     * @param url
     * @return
     */
    int openFile(QString url);
    int errorcode() const {return m_errorcode;}
public slots:
    /**
    * @brief close 关闭视频文件
    * @return
    */
    bool close();
    /**
     * @brief setImage 将图像插入到视频中，以当前时间自动计算 pts
     * @param image
     * @return
     */
    bool setImage(const QImage &image);
    /**
     * @brief setImage 将图像插入到视频中
     * @param image
     * @param pts 时间戳，以 time base 为基本单位。第一张图像默认 pts 为 0。 如果 pts = -1 则根据当前时间自动计算 pts.
     * @return
     */
    bool setImage2(const QImage &image, int pts);
signals:
    void recordStoped();

private slots:
    void timeout();
protected:
    int writeHeader();
    int writeTrailer();
    bool writeFrame(const AVFrame *m_pFrame);
    int initFile(AVCodecID codecID, QSize size);
    void initStreamParameters(AVStream *stream);
    void buildFrameFromImage(AVFrame *m_pFrame, const QImage &image, int pts);

    AVFormatContext *m_pFormatCtx = nullptr;
    const AVCodec *m_pCodec = nullptr;
    AVCodecContext *m_pCodecCtx = nullptr;
    AVFrame *m_pFrame = nullptr;
    AVPacket *m_pPacket = nullptr;

    AVCodecID m_codecID = AV_CODEC_ID_MPEG4;
    AVPixelFormat m_format = AV_PIX_FMT_YUV420P;
    AVRational m_time_base = {1, 1000};

    int64_t m_bit_rate = 10000000;
    int m_width = 0;
    int m_height = 0;
    QString m_url;
private:
    int m_errorcode = 0;
    bool m_recording = false;
    QTime m_startTime;
    QTimer m_timer;
    int m_timeout = 1000 * 1800; //默认半个小时
};

}


#endif // VIDEORECORDER_H
