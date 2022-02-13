#ifndef QLYAVFORMAT_H
#define QLYAVFORMAT_H
#include <QDebug>
#include <QString>
#include <QMap>
#include <QVector>
#include <QList>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

#include "QlyAVPacket.h"
#include "QlyAVStream.h"



class QlyAVStream;
class QlyAVFormatContext
{
public:
    QlyAVFormatContext();
    ~QlyAVFormatContext();
    static QString license();
    static QString version() ;
    static QString configuration();

    void dumpFormat();

    QVector<QlyAVStream> allStreams();
    QVector<QlyAVStream> findStreams(QSet<AVMediaType> mediaType);
    void setStreams(QVector<QlyAVStream> streams);

    AVStream * rawStream(int i);
    int outStreamIndex(QlyAVPacket &pack);
    bool readFrame(QlyAVPacket &pack, int streamIndex);
    bool readFrame(QlyAVPacket &pack, QSet<AVMediaType> type);
    bool writeFrame(QlyAVPacket &pack, AVRational timeBase, bool useStreamMap);
    int streamCount() const;
    QMap<QString, QString> metadata();

    /**
     * @brief seekFrame
     * @param time      时间，以秒为单位
     * @param stream_index
     * @param seekFlag  可以是 AVSEEK_FLAG_BACKWARD
     *                        AVSEEK_FLAG_BYTE
     *                        AVSEEK_FLAG_ANY
     *                        AVSEEK_FLAG_FRAME
     * @return true 表示找到了，false 表示出错
     */
    bool seekFrame(double time, int stream_index, int seekFlag);// s 以秒为单位

    int openFile(QString url);
    int createFile(QString formatName, QString fileName);
    void closeInputFile();
    int closeOutputFile();
    int writeHeader();
    int writeTrailer();

    QString errorString() const;

private:
    AVFormatContext *pFormatCtx;
    QMap<unsigned int, unsigned int> streamMap;// 记录了 in_stream 到 out_stream 的映射关系
    QString m_url;
    int errorcode;
    int isOutput;
    char errstr[512];
    AVRounding m_rounding;
};

QDebug operator<<(QDebug dbg, enum AVMediaType codec_type);
QDebug operator<<(QDebug dbg, AVRational r);

#endif // QLYAVFORMAT_H
