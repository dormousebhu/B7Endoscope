#include "QlyAVStream.h"

QlyAVStream::QlyAVStream(AVStream *stream, int inStreamIndex, int outStreamIndex)
{
    m_stream = stream;
    m_inStreamIndex = inStreamIndex;
    m_outStreamIndex = outStreamIndex;
}

QlyAVStream::QlyAVStream(const QlyAVStream &source)
{
    m_stream = source.m_stream;
    m_inStreamIndex = source.m_inStreamIndex;
    m_outStreamIndex = source.m_outStreamIndex;
}

QlyAVStream & QlyAVStream::operator= (const QlyAVStream & source)
{
    m_stream = source.m_stream;
    m_inStreamIndex = source.m_inStreamIndex;
    m_outStreamIndex = source.m_outStreamIndex;
    return (*this);
}
