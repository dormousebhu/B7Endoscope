#include "QlyAVPacket.h"

QlyAVPacket::QlyAVPacket()
{

}

QlyAVPacket::~QlyAVPacket()
{
    av_packet_unref(&m_packet);
}

void QlyAVPacket::unref()
{
    av_packet_unref(&m_packet);
}

int QlyAVPacket::compare_ts(double timestamp)
{
    AVRational av_time_base_q = {1, AV_TIME_BASE};
    return av_compare_ts(m_packet.pts, m_timeBase, timestamp * AV_TIME_BASE, av_time_base_q);
}

void QlyAVPacket::adjustTime(double timestamp)
{
    int64_t ts = timestamp * m_timeBase.den / m_timeBase.num;
    m_packet.pts = m_packet.pts - ts;
    m_packet.dts = m_packet.dts - ts;
}

void QlyAVPacket::changeTimeBase(AVRational bq, AVRational cq, enum AVRounding rnd)
{
    m_packet.pts = av_rescale_q_rnd(m_packet.pts, bq, cq, rnd);
    m_packet.dts = av_rescale_q_rnd(m_packet.dts, bq, cq, rnd);
    m_packet.duration = av_rescale_q(m_packet.duration, bq, cq);
    m_packet.pos = -1;
}

void QlyAVPacket::setStreamIndex(unsigned int index)
{
    m_packet.stream_index = index;
}
