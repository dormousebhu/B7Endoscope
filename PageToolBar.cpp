﻿#include "PageToolBar.h"

PageToolBar::PageToolBar(QWidget *parent)
    :QToolBar(parent)
{
    setIconSize(QSize(32, 32));
    m_pAction[0] = new QAction(tr("Show all camera image"), this);
    m_pAction[0]->setCheckable(true);
    addAction(m_pAction[0]);

    for(int i = 1; i < 7; i++)
    {
        m_pAction[i] = new QAction(tr("Show CH%1 camera image").arg(i), this);
        m_pAction[i]->setCheckable(true);
        addAction(m_pAction[i]);
    }
    m_pAction[0]->setIcon(QIcon(":/res/CHall.png"));
    m_pAction[1]->setIcon(QIcon(":/res/ch1.png"));
    m_pAction[2]->setIcon(QIcon(":/res/ch2.png"));
    m_pAction[3]->setIcon(QIcon(":/res/ch3.png"));
    m_pAction[4]->setIcon(QIcon(":/res/ch4.png"));
    m_pAction[5]->setIcon(QIcon(":/res/ch5.png"));
    m_pAction[6]->setIcon(QIcon(":/res/ch6.png"));

    m_pAction[0]->setChecked(true);

    connect(m_pAction[0], &QAction::triggered, this, &PageToolBar::onAction);
    connect(m_pAction[1], &QAction::triggered, this, &PageToolBar::onAction);
    connect(m_pAction[2], &QAction::triggered, this, &PageToolBar::onAction);
    connect(m_pAction[3], &QAction::triggered, this, &PageToolBar::onAction);
    connect(m_pAction[4], &QAction::triggered, this, &PageToolBar::onAction);
    connect(m_pAction[5], &QAction::triggered, this, &PageToolBar::onAction);
    connect(m_pAction[6], &QAction::triggered, this, &PageToolBar::onAction);

    m_pActionZoomIn = new QAction(tr("Zoom In"), this);
    m_pActionZoomOut = new QAction(tr("Zoom Out"), this);
    m_pActionZoomIn->setIcon(QIcon(":/res/zoomin.png"));
    m_pActionZoomOut->setIcon(QIcon(":/res/zoomout.png"));
    addAction(m_pActionZoomIn);
    addAction(m_pActionZoomOut);
    connect(m_pActionZoomIn, &QAction::triggered, this, &PageToolBar::zoomIn);
    connect(m_pActionZoomOut, &QAction::triggered, this, &PageToolBar::zoomOut);
}

void PageToolBar::setPage(int n)
{
    clearAll();
    m_pAction[n]->setChecked(true);
    emit pageChanged(n);
}

void PageToolBar::clearAll()
{
    for(int i = 0; i < 7; i++)
    {
        m_pAction[i]->setChecked(false);
    }
}
void PageToolBar::onAction(bool checked )
{
    if(checked == false)
    {
        QAction * pa = (QAction *)sender();
        pa->setChecked(true);
        return;
    }
    clearAll();
    for(int i = 0; i < 7; i++)
    {
        if(sender() == m_pAction[i])
        {
            m_pAction[i]->setChecked(true);
            emit pageChanged(i);
        }
    }
}
