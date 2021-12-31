#include <QCoreApplication>
#include "ImageSaveToolBar.h"

ImageSaveToolBar::ImageSaveToolBar(QWidget *parent)
    :QToolBar(parent)
{
    setWindowTitle(tr("Save Image"));
    setIconSize(QSize(32, 32));
    m_pAction[0] = new QAction(tr("Save All images to files."), this);
    addAction(m_pAction[0]);
    for(int i = 1; i < 7; i++)
    {
        m_pAction[i] = new QAction(tr("Save CH%1 image to file.").arg(i), this);
        addAction(m_pAction[i]);
    }
    m_pAction[1]->setIcon(QIcon(":/res/filesave1.png"));
    m_pAction[2]->setIcon(QIcon(":/res/filesave2.png"));
    m_pAction[3]->setIcon(QIcon(":/res/filesave3.png"));
    m_pAction[4]->setIcon(QIcon(":/res/filesave4.png"));
    m_pAction[5]->setIcon(QIcon(":/res/filesave5.png"));
    m_pAction[6]->setIcon(QIcon(":/res/filesave6.png"));
    m_pAction[0]->setIcon(QIcon(":/res/filesaveAll.png"));
    connect(m_pAction[0], &QAction::triggered, this, &ImageSaveToolBar::onAction);
    connect(m_pAction[1], &QAction::triggered, this, &ImageSaveToolBar::onAction);
    connect(m_pAction[2], &QAction::triggered, this, &ImageSaveToolBar::onAction);
    connect(m_pAction[3], &QAction::triggered, this, &ImageSaveToolBar::onAction);
    connect(m_pAction[4], &QAction::triggered, this, &ImageSaveToolBar::onAction);
    connect(m_pAction[5], &QAction::triggered, this, &ImageSaveToolBar::onAction);
    connect(m_pAction[6], &QAction::triggered, this, &ImageSaveToolBar::onAction);
}

void ImageSaveToolBar::retranslateUi()
{
    m_pAction[1]->setText(QCoreApplication::translate("ImageSaveToolBar", "Save CH1 image to file.", nullptr));
    m_pAction[2]->setText(QCoreApplication::translate("ImageSaveToolBar", "Save CH2 image to file.", nullptr));
    m_pAction[3]->setText(QCoreApplication::translate("ImageSaveToolBar", "Save CH3 image to file.", nullptr));
    m_pAction[4]->setText(QCoreApplication::translate("ImageSaveToolBar", "Save CH4 image to file.", nullptr));
    m_pAction[5]->setText(QCoreApplication::translate("ImageSaveToolBar", "Save CH5 image to file.", nullptr));
    m_pAction[6]->setText(QCoreApplication::translate("ImageSaveToolBar", "Save CH6 image to file.", nullptr));

    m_pAction[0]->setText(QCoreApplication::translate("ImageSaveToolBar", "Save All images to files.", nullptr));
}

void ImageSaveToolBar::onAction(bool checked)
{
    Q_UNUSED(checked);
    QAction *pAction = (QAction *)sender();

    for(int i = 0; i < 7; i++)
    {
        if(pAction == m_pAction[i])
        {
            emit saveImage(i);
            return;
        }
    }
}
