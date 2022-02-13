#include "QlyAVFormatContext.h"
#include "QlyAVStream.h"

void msg(const char * str, int ret)
{
    static char err[512];
    if(ret < 0)
    {
        av_strerror(ret, err, 512);
        qWarning() << str << " error:" << err;
        exit(ret);
    }
    else
    {
        qDebug() << str << " : success";
    }
}

QDebug operator<<(QDebug dbg, enum AVMediaType codec_type)
{
    switch (codec_type) {
    case AVMEDIA_TYPE_UNKNOWN:
        dbg.nospace() << "AVMEDIA_TYPE_UNKNOWN";
        break;
    case AVMEDIA_TYPE_VIDEO:
        dbg.nospace() << "AVMEDIA_TYPE_VIDEO";
        break;
    case AVMEDIA_TYPE_AUDIO:
        dbg.nospace() << "AVMEDIA_TYPE_AUDIO";
        break;
    case AVMEDIA_TYPE_DATA:
        dbg.nospace() << "AVMEDIA_TYPE_DATA";
        break;
    case AVMEDIA_TYPE_SUBTITLE:
        dbg.nospace() << "AVMEDIA_TYPE_SUBTITLE";
        break;
    case AVMEDIA_TYPE_ATTACHMENT:
        dbg.nospace() << "AVMEDIA_TYPE_ATTACHMENT";
        break;
    case AVMEDIA_TYPE_NB:
        dbg.nospace() << "AVMEDIA_TYPE_NB";
        break;
    }
    return dbg.maybeSpace();
}


QDebug operator<<(QDebug dbg, AVRational r)
{
    dbg.nospace() << r.num << "/" << r.den;
    return dbg.maybeSpace();
}

QlyAVFormatContext::QlyAVFormatContext()
    :pFormatCtx(nullptr),
      isOutput(false),
      m_rounding((AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX))
{
    //av_register_all();
    //avformat_network_init();
}

void QlyAVFormatContext::dumpFormat()
{
    av_dump_format(pFormatCtx, 0, m_url.toLocal8Bit().constData(), isOutput);
}

bool QlyAVFormatContext::seekFrame(double s, int stream_index, int seekFlag)
{
    int64_t timestamp = 0;
    if(stream_index == -1)
    {
        timestamp = s * AV_TIME_BASE;
    }
    else
    {
        AVStream *in_stream = pFormatCtx->streams[stream_index];
        timestamp =  s / av_q2d(in_stream->time_base);
    }
    errorcode = av_seek_frame(pFormatCtx, stream_index, timestamp, seekFlag);
    return (errorcode >= 0);
}

QlyAVFormatContext::~QlyAVFormatContext()
{
    if(isOutput && pFormatCtx)
    {
        closeOutputFile();
    }
    else if(pFormatCtx)
    {
        avformat_close_input(&pFormatCtx);
    }
    avformat_free_context(pFormatCtx);
}

int QlyAVFormatContext::writeTrailer()
{
    errorcode = av_write_trailer(pFormatCtx);
    msg("QlyAVFormatContext::writeTrailer av_write_trailer", errorcode);
    return errorcode;
}

int QlyAVFormatContext::writeHeader()
{
    qDebug() << "in QlyAVFormatContext::writeHeader()";
    if (!(pFormatCtx->flags & AVFMT_NOFILE))
    {
        errorcode = avio_open(&pFormatCtx->pb, m_url.toLocal8Bit().constData(), AVIO_FLAG_WRITE);
        msg("QlyAVFormatContext::writeTrailer avio_open", errorcode);
    }
    errorcode = avformat_write_header(pFormatCtx, nullptr);
    msg("QlyAVFormatContext::writeHeader avformat_write_header", errorcode);

    return errorcode;
}

int QlyAVFormatContext::outStreamIndex(QlyAVPacket &pack)
{
    unsigned int index = pack.m_packet.stream_index;
    if(!streamMap.contains(index)) return -1;
    return streamMap[index];
}

bool QlyAVFormatContext::readFrame(QlyAVPacket &pack, QSet<AVMediaType> type)
{
    if(!pFormatCtx)
    {
        errorcode = 0;
        return false;
    }
    AVPacket * pPacket = pack.ptr();
    while(1)
    {
        errorcode = av_read_frame(pFormatCtx, pPacket);
        if(errorcode < 0) return false;
        AVStream *in_stream = pFormatCtx->streams[pPacket->stream_index];
        AVCodecParameters *in_codecpar = in_stream->codecpar;

        if (!type.contains(in_codecpar->codec_type))
        {
            av_packet_unref(pPacket);
            continue;
        }
        pack.m_timeBase = pFormatCtx->streams[pPacket->stream_index]->time_base;
        return true;
    }
}

bool QlyAVFormatContext::readFrame(QlyAVPacket &pack, int streamIndex)
{
    if(!pFormatCtx)
    {
        errorcode = 0;
        return false;
    }
    AVPacket * pPacket = pack.ptr();
    while(1)
    {
        errorcode = av_read_frame(pFormatCtx, pPacket);
        qDebug() << "av_read_frame:errorcode = " << errorcode;
        if(errorcode < 0) return false;
        if (pPacket->stream_index != streamIndex || streamIndex != -1)
        {
            av_packet_unref(pPacket);
            continue;
        }
        qDebug() << "find a Packet";
        pack.m_timeBase = pFormatCtx->streams[pPacket->stream_index]->time_base;
        return true;
    }
}

QString QlyAVFormatContext::license()
{
    return QString(avformat_license());
}

QMap<QString, QString> QlyAVFormatContext::metadata()
{
    QMap<QString, QString> data;
    AVDictionaryEntry *tag = nullptr;
    while ((tag = av_dict_get(pFormatCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
    {
        data.insert(QString(tag->key), QString(tag->value));
    }
    return data;
}

int QlyAVFormatContext::createFile(QString formatName, QString fileName)
{
    m_url = fileName;
    isOutput = true;
    if(pFormatCtx) avformat_free_context(pFormatCtx);
    //pFormatCtx = avformat_alloc_context();
    if(formatName.isNull() || formatName.isEmpty())
    {
        errorcode = avformat_alloc_output_context2(&pFormatCtx, nullptr,
                                                   nullptr,
                                                   fileName.toLocal8Bit().constData());
    }
    else
    {
        errorcode = avformat_alloc_output_context2(&pFormatCtx, nullptr,
                                                   formatName.toLocal8Bit().constData(),
                                                   fileName.toLocal8Bit().constData());
    }

    msg("QlyAVFormatContext::createFile avformat_alloc_output_context2", errorcode);
    return errorcode;
}

int QlyAVFormatContext::closeOutputFile()
{
    /* close output */
    if (pFormatCtx && !(pFormatCtx->flags & AVFMT_NOFILE))
    {
        errorcode = avio_closep(&pFormatCtx->pb);
    }
    return errorcode;
}

QVector<QlyAVStream> QlyAVFormatContext::allStreams()
{
    QVector<QlyAVStream> list;
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        AVStream *in_stream = pFormatCtx->streams[i];
        list << QlyAVStream(in_stream, i, -1);
    }
    return list;
}

bool QlyAVFormatContext::writeFrame(QlyAVPacket &pack, AVRational timeBase, bool useStreamMap)
{
    AVPacket *pPacket = pack.ptr();
    if(useStreamMap && streamMap.contains(pPacket->stream_index))
    {
        pPacket->stream_index = streamMap[pPacket->stream_index];
    }
    AVRational out_timebase = pFormatCtx->streams[pPacket->stream_index]->time_base;

    pPacket->pts = av_rescale_q_rnd(pPacket->pts, timeBase, out_timebase, m_rounding);
    pPacket->dts = av_rescale_q_rnd(pPacket->dts, timeBase, out_timebase, m_rounding);
    pPacket->duration = av_rescale_q(pPacket->duration, timeBase, out_timebase);
    pPacket->pos = -1;

    errorcode = av_interleaved_write_frame(pFormatCtx, pPacket);
    //msg("QlyAVFormatContext::writeFrame av_interleaved_write_frame", errorcode);
    return (errorcode == 0);
}

QVector<QlyAVStream> QlyAVFormatContext::findStreams(QSet<AVMediaType> mediaType)
{
    QVector<QlyAVStream> list;
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        AVStream *in_stream = pFormatCtx->streams[i];
        AVCodecParameters *in_codecpar = in_stream->codecpar;
        if(mediaType.contains(in_codecpar->codec_type))
        {
            qDebug() << "in_stream->index = " << in_stream->index;
            list << QlyAVStream(in_stream, i, -1);
        }
    }
    return list;
}

void QlyAVFormatContext::setStreams(QVector<QlyAVStream> streams)
{
    streamMap.clear();
    unsigned int stream_index = 0;
    QVectorIterator<QlyAVStream> iter(streams);
    while(iter.hasNext())
    {
        QlyAVStream st = iter.next();
        AVStream * out_stream = avformat_new_stream(pFormatCtx, nullptr);
        avcodec_parameters_copy(out_stream->codecpar, st.codecpar());
        out_stream->codecpar->codec_tag = 0;
        streamMap[st.m_inStreamIndex] = stream_index;
        qDebug() << "streamMap[" << st.m_inStreamIndex << "] = " << stream_index;
        stream_index ++;
    }
    qDebug() << "QlyAVFormatContext::setStreams success, stream_index = " << stream_index - 1;
}

AVStream * QlyAVFormatContext::rawStream(int i)
{
    if(pFormatCtx) return pFormatCtx->streams[i];
    return 0;
}

int QlyAVFormatContext::streamCount() const
{
    if(pFormatCtx) return pFormatCtx->nb_streams;
    return 0;
}

QString QlyAVFormatContext::version()
{
    return QString(avformat_version());
}

QString QlyAVFormatContext::configuration()
{
    return QString(avformat_configuration());
}

void QlyAVFormatContext::closeInputFile()
{
    if(pFormatCtx)
        avformat_close_input(&pFormatCtx);
}

int QlyAVFormatContext::openFile(QString url)
{
    m_url = url;
    isOutput = false;
    if(pFormatCtx) avformat_free_context(pFormatCtx);
    pFormatCtx = avformat_alloc_context();
    errorcode = avformat_open_input(&pFormatCtx, url.toLocal8Bit().constData(), nullptr, nullptr);
    if (errorcode < 0) return errorcode;
    errorcode = avformat_find_stream_info(pFormatCtx, nullptr);
    return errorcode;
}


QString QlyAVFormatContext::errorString() const
{
    char err[512];
    av_strerror(errorcode, err, 1024);
    return QString(err);
}
