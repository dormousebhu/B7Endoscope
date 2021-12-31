#ifndef VIDEORECORDTOOLBAR_H
#define VIDEORECORDTOOLBAR_H

#include <QToolBar>
#include <QAction>

class VideoRecordToolBar : public QToolBar
{
    Q_OBJECT
public:
    VideoRecordToolBar(QWidget *parent = nullptr);
public slots:
    void videoRecordStoped(int channel);// n = 1..6
    void videoRecordStarted(int channel);// n = 1..6
signals:
    void startRecordVideo(int channel); // n = 1..6
    void stopRecordVideo(int channel); // n = 1..6
private slots:
    void onAction(bool checked);
private:
    QAction *m_pAction[6];
};

#endif // VIDEORECORDTOOLBAR_H
