#ifndef QLYAVPACKET_H
#define QLYAVPACKET_H

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

class QlyAVPacket
{
public:
    QlyAVPacket();
    ~QlyAVPacket();
    unsigned int streamIndex() {return m_packet.stream_index;}
    void setStreamIndex(unsigned int index);
    void changeTimeBase(AVRational bq, AVRational cq, enum AVRounding rnd);

    /**
     * @brief compare_ts
     * @param timestamp
     * @return  -1 表示当前帧的时间小于 timestamp， 1 表示大于， 0 表示相等
     */
    int compare_ts(double timestamp);
    void adjustTime(double timestamp);
    void unref();
    AVPacket *ptr() {return &m_packet;}
    AVPacket m_packet;
    AVRational m_timeBase;
};

#endif // QLYAVPACKET_H
