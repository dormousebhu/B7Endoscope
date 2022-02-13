#ifndef QLYAVSTREAM_H
#define QLYAVSTREAM_H
#include<QtGlobal>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

class QlyAVStream
{
    friend class QlyAVFormatContext;
public:
    QlyAVStream() = default;
    QlyAVStream(AVStream *stream, int inStreamIndex = -1, int outStreamIndex = -1);
    QlyAVStream(const QlyAVStream &source);
    QlyAVStream & operator= (const QlyAVStream & source);
    AVCodecParameters * codecpar() {return m_stream->codecpar;}

    AVStream *m_stream;
    int m_inStreamIndex; //在 AVFormatContext 中的 index
    int m_outStreamIndex;
};

Q_DECLARE_TYPEINFO(QlyAVStream,
                   Q_PRIMITIVE_TYPE);


#endif // QLYAVSTREAM_H
