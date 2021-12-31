#include "RemoteControl.h"
#include <QtDebug>

RemoteControl::RemoteControl(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    if(!tcpServer->listen(QHostAddress::AnyIPv4, quint16(8888)))
    {
            return;
    }
    connect(tcpServer, &QTcpServer::newConnection, this, &RemoteControl::on_newConnection);
}

void RemoteControl::on_newConnection()
{
    qDebug() << "RemoteControl::on_newConnection()";
    if(tcpServer->hasPendingConnections())
    {
        clientConnection = tcpServer->nextPendingConnection();
        if(!clientConnection->isValid())
        {
            return;
        }
        connect(clientConnection, &QTcpSocket::readyRead, this, &RemoteControl::receiveCommand);
        connect(clientConnection, &QTcpSocket::disconnected, this, &RemoteControl::connectionLost);
        connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);

        QString ip = clientConnection->localAddress().toString();
        emit connectionSetup(ip);
    }
}

void RemoteControl::receiveCommand()
{

    char buf[128];
    while ( clientConnection->readLine(buf, 128) > 0)
    {
        qDebug() << "RemoteControl::on_newConnection() : "  << buf;
        parser(buf);
    }
}

void RemoteControl::ack()
{
    clientConnection->write("OK\n");
}
void RemoteControl::parser(char * pCmd)
{
    if (strstr(pCmd, "STATUS") )
    {
        emit queryStatus();
        return;
    }
    if (strstr(pCmd, "SAVE_IMAGE_") )
    {
        int channel = pCmd[11] - '0';
        if(channel < 0 || channel > 6)
        {
            qWarning() << "Fail: " << pCmd;
            wrongCommand();
            return;
        }
        emit saveImage(channel);
        //ack();
        return;
    }
    if (strstr(pCmd, "SAVE_VIDEO_") )
    {
        int channel = pCmd[11] - '0';
        if(channel < 0 || channel > 6)
        {
            qWarning() << "Fail: " << pCmd;
            wrongCommand();
            return;
        }
        emit startRecordVideo(channel);
        //ack();
        return;
    }
    if (strstr(pCmd, "STOP_VIDEO_") )
    {
        int channel = pCmd[11] - '0';
        if(channel < 0 || channel > 6)
        {
            qWarning() << "Fail: " << pCmd;
            wrongCommand();
            return;
        }
        emit stopRecordVideo(channel);
        //ack();
        return;
    }
    wrongCommand();
}

void RemoteControl::sendOK()
{
    clientConnection->write( QByteArray("OK\n") );
}

void RemoteControl::sendFAIL()
{
    clientConnection->write( QByteArray("FAIL\n") );
}

void RemoteControl::wrongCommand()
{
    qDebug() << "RemoteControl::wrongCommand()";
    clientConnection->write( QByteArray("INVALID_COMMAND\n") );
}
