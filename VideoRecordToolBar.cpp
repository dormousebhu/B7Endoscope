#include "VideoRecordToolBar.h"

VideoRecordToolBar::VideoRecordToolBar(QWidget *parent)
    :QToolBar(parent)
{
    setWindowTitle("Record Video");
    setIconSize(QSize(32, 32));
    for(int i = 0; i < 6; i++)
    {
        m_pAction[i] = new QAction(tr("Record CH%1 video to file").arg(i + 1), this);
        m_pAction[i]->setCheckable(true);
        addAction(m_pAction[i]);
    }
    m_pAction[0]->setIcon(QIcon(":/res/video1.png"));
    m_pAction[1]->setIcon(QIcon(":/res/video2.png"));
    m_pAction[2]->setIcon(QIcon(":/res/video3.png"));
    m_pAction[3]->setIcon(QIcon(":/res/video4.png"));
    m_pAction[4]->setIcon(QIcon(":/res/video5.png"));
    m_pAction[5]->setIcon(QIcon(":/res/video6.png"));

    connect(m_pAction[0], &QAction::triggered, this, &VideoRecordToolBar::onAction);
    connect(m_pAction[1], &QAction::triggered, this, &VideoRecordToolBar::onAction);
    connect(m_pAction[2], &QAction::triggered, this, &VideoRecordToolBar::onAction);
    connect(m_pAction[3], &QAction::triggered, this, &VideoRecordToolBar::onAction);
    connect(m_pAction[4], &QAction::triggered, this, &VideoRecordToolBar::onAction);
    connect(m_pAction[5], &QAction::triggered, this, &VideoRecordToolBar::onAction);
}

void VideoRecordToolBar::videoRecordStoped(int channel)
{
    if(channel > 0 && channel < 7)
    {
        m_pAction[channel - 1]->setChecked(false);
    }
}

void VideoRecordToolBar::videoRecordStarted(int channel)
{
    if(channel > 0 && channel < 7)
    {
        m_pAction[channel - 1]->setChecked(true);
    }
}

void VideoRecordToolBar::onAction(bool checked)
{
    QAction *pAction = (QAction *)sender();
    for(int i = 0; i < 6; i++)
    {
        if(pAction == m_pAction[i])
        {
            if(checked)
            {
                emit startRecordVideo(i + 1);
                return;
            }
            else
            {
                emit stopRecordVideo(i + 1);
                return;
            }
        }
    }
}
