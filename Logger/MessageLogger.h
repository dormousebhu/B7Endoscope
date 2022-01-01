/****************************************************************************
** file: MessageLogger.h
** brief: 将 qDebug() 等的输出重定向到文件中
** Copyright (C) LiYuan
** Author: LiYuan
** E-Mail: 18069211#qq(.)com
** Version 1.0.1
** Last modified: 2022.1.1
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

#ifndef MESSAGELOGGER_H
#define MESSAGELOGGER_H

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QDebug>
#include <QtCore/QMessageLogContext>


namespace Qly {

class FileLogger
{
public:
    FileLogger(QString name = "stderr", bool debug = false, bool info = true, bool warning = true, bool critical= true, bool fatal = true);
    virtual ~FileLogger();
    /**
     * @brief setFileName 设置日志存储的文件名
     * @param name 日志存储的文件名，如果为 "stderr" 则输出到 stderr
     * @return
     */
    bool setFileName(QString name = "stderr");

    /**
     * @brief setLogLevel 设置哪些级别的信息要输出到文件
     * @param debug  true 表示 qDebug 信息输出到日志
     * @param info   true 表示 qInfo 信息输出到日志
     * @param warning  true 表示 qWarning 信息输出到日志
     * @param critical true 表示 qCritical 信息输出到日志
     * @param fatal    true 表示 qFatal 信息输出到日志
     */
    void setLogLevel(bool debug = false, bool info = true, bool warning = true, bool critical= true, bool fatal = true);
    virtual void writeLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);
private:
    QString messageType(QtMsgType type);
    FileLogger(FileLogger &f) {Q_UNUSED(f);} // 不能被拷贝
    FileLogger& operator=( FileLogger &f) {Q_UNUSED(f);} // 不能被拷贝
private:
    QFile m_file;
    QMap<QtMsgType, bool> m_level;
    bool m_where;
};

class LoggerController
{
public:
    LoggerController(){}
    ~LoggerController();
    void startLogging();
    void attach(FileLogger *m_currentLogger);
    void detach(FileLogger *m_currentLogger);
private:
    static void writeLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static LoggerController * m_currentLogger;
    QList<FileLogger *> m_list;
};

} //namespace Qly
#endif // MESSAGELOGGER_H
