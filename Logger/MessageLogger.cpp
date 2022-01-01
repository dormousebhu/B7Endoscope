/****************************************************************************
** file: MessageLogger.cpp
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

#include "MessageLogger.h"
#include <QtCore/QDateTime>
#include <QtCore/QTextStream>

namespace Qly {
FileLogger::FileLogger(QString name, bool debug, bool info, bool warning, bool critical, bool fatal)
    :m_where(true)
{
    setFileName(name);
    setLogLevel(debug, info, warning, critical, fatal);
}


FileLogger::~FileLogger()
{
    m_file.close();
}

bool FileLogger::setFileName(QString name)
{
    m_file.close();
    if(name == "stderr")
    {
        return m_file.open(stderr, QIODevice::WriteOnly);
    }
    else
    {
        m_file.setFileName(name);
        return m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    }
}

void FileLogger::setLogLevel(bool debug, bool info, bool warning, bool critical, bool fatal)
{
    m_level[QtDebugMsg] = debug;
    m_level[QtInfoMsg] = info;
    m_level[QtWarningMsg] = warning;
    m_level[QtCriticalMsg] = critical;
    m_level[QtFatalMsg] = fatal;
}

void FileLogger::writeLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(!m_level[type])
    {
        return;
    }
    QString strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QTextStream logfile(&m_file);

    logfile << strTime << ", ";
    logfile << messageType(type) << ", ";
    logfile << msg;
    if(context.file && m_where)
    {
        logfile <<  ", (" << context.file << ":" << context.line << ", " << context.function << ")\n";
    }
    else
    {
        logfile << endl;
    }

    if(type == QtFatalMsg)
    {
        abort();
    }
}

QString FileLogger::messageType(QtMsgType type)
{
    QString str;
    switch (type)
    {
    case QtDebugMsg:
        str = "[Debug]   ";
        break;
    case QtInfoMsg:
        str = "[Info]    ";
        break;
    case QtWarningMsg:
        str = "[Warning] ";
        break;
    case QtCriticalMsg:
        str = "[Critical]";
        break;
    case QtFatalMsg:
        str = "[Fatal]   ";
    }
    return str;
}

void LoggerController::attach(FileLogger *logger)
{
    m_list.append(logger);
}

void LoggerController::detach(FileLogger *logger)
{
    if(logger)
    {
        m_list.removeOne(logger);
        delete logger;
    }
}

void LoggerController::startLogging()
{
    m_currentLogger = this;
    qInstallMessageHandler(LoggerController::writeLog);
}

LoggerController::~LoggerController()
{
    qDeleteAll(m_list);
}
LoggerController* LoggerController::m_currentLogger = nullptr;

void LoggerController::writeLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if( m_currentLogger )
    {
        QList<FileLogger *> &list = m_currentLogger->m_list;

        QList<FileLogger *>::const_iterator i;
        for (i = list.cbegin(); i != list.cend(); ++i)
        {
            (*i)->writeLog(type, context, msg);
        }
    }
}

}//namespace Qly
