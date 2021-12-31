#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class RemoteControl : public QObject
{
    Q_OBJECT
public:
    explicit RemoteControl(QObject *parent = nullptr);
    void sendOK();
    void sendFAIL();

signals:
    void saveImage(int channel);
    void startRecordVideo(int channel);
    void stopRecordVideo(int channel);
    void queryStatus();
    void connectionSetup(QString ip);
    void connectionLost();
private slots:
    void on_newConnection();
    void receiveCommand();
private:

    void parser(char * pCmd);
    void wrongCommand();
    void ack();
    QTcpServer *tcpServer = nullptr;
    QTcpSocket *clientConnection = nullptr;
};

#endif // REMOTECONTROL_H
