#include <QCoreApplication>
#include "PageToolBar.h"

PageToolBar::PageToolBar(QWidget *parent)
    :QToolBar(parent)
{
    setWindowTitle(tr("PageToolBar"));
    setIconSize(QSize(32, 32));

    m_pGroup = new QActionGroup(this);
    m_pAction[0] = new QAction(tr("Show all camera image."), m_pGroup);
    m_pAction[0]->setCheckable(true);
    m_pGroup->addAction(m_pAction[0]);
    addAction(m_pAction[0]);

    for(int i = 1; i < 7; i++)
    {
        m_pAction[i] = new QAction(tr("Show CH%1 camera image.").arg(i), m_pGroup);
        m_pAction[i]->setCheckable(true);
        m_pGroup->addAction(m_pAction[i]);
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

void PageToolBar::retranslateUi()
{
    m_pAction[1]->setText(QCoreApplication::translate("PageToolBar", "Show CH1 camera image.", nullptr));
    m_pAction[2]->setText(QCoreApplication::translate("PageToolBar", "Show CH2 camera image.", nullptr));
    m_pAction[3]->setText(QCoreApplication::translate("PageToolBar", "Show CH3 camera image.", nullptr));
    m_pAction[4]->setText(QCoreApplication::translate("PageToolBar", "Show CH4 camera image.", nullptr));
    m_pAction[5]->setText(QCoreApplication::translate("PageToolBar", "Show CH5 camera image.", nullptr));
    m_pAction[6]->setText(QCoreApplication::translate("PageToolBar", "Show CH6 camera image.", nullptr));

    m_pAction[0]->setText(QCoreApplication::translate("PageToolBar", "Show All camera image.", nullptr));

    m_pActionZoomIn->setText(QCoreApplication::translate("PageToolBar", "Zoom In", nullptr));
    m_pActionZoomOut->setText(QCoreApplication::translate("PageToolBar", "Zoom Out", nullptr));
}

void PageToolBar::setPage(int n)
{
    m_pAction[n]->setChecked(true);
    emit pageChanged(n);
}

void PageToolBar::onAction(bool checked )
{
    Q_UNUSED(checked);
    for(int i = 0; i < 7; i++)
    {
        if(sender() == m_pAction[i])
        {
            emit pageChanged(i);
        }
    }
}
