#include <QGridLayout>
#include "CamerasDisplayWidget.h"
const int ZOOMFACTOR[20] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200};

CamerasDisplayWidget::CamerasDisplayWidget(QWidget *parent)
    :QStackedWidget(parent)
{
    m_zoomFactorIndex[0] = 5;
    for(int i = 0; i < 6; i++)
    {
        m_pThumbnails[i] = new Qly::ImageView(this);
        m_pThumbnails[i]->setZoomFactorPCT(ZOOMFACTOR[m_zoomFactorIndex[0]]);
        m_pView[i] = new Qly::ImageView(this);
        m_zoomFactorIndex[i + 1] = 4;
        m_pView[i]->setZoomFactorPCT(ZOOMFACTOR[m_zoomFactorIndex[i + 1]]);
    }

    QGridLayout * layout = new QGridLayout;
    layout->addWidget(m_pThumbnails[0], 0, 0);
    layout->addWidget(m_pThumbnails[1], 0, 1);
    layout->addWidget(m_pThumbnails[2], 0, 2);
    layout->addWidget(m_pThumbnails[3], 1, 0);
    layout->addWidget(m_pThumbnails[4], 1, 1);
    layout->addWidget(m_pThumbnails[5], 1, 2);
    layout->setSizeConstraint(QLayout::SetFixedSize);
//    QBoxLayout * boxlayout = new QBoxLayout(QBoxLayout::TopToBottom);
//    boxlayout->addStretch();
//    boxlayout->addLayout(layout);
//    boxlayout->addStretch();
    QWidget * pPageAll = new QWidget;
    pPageAll->setLayout(layout);
    m_pScrollArea[6] = new QScrollArea(this);
    m_pScrollArea[6]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_pScrollArea[6]->setWidget(pPageAll);
    addWidget(m_pScrollArea[6]);

    for(int i = 0; i < 6; i++)
    {
        m_pScrollArea[i] = new QScrollArea;
        m_pScrollArea[i]->setWidget(m_pView[i]);
        addWidget(m_pScrollArea[i]);
    }
}



void CamerasDisplayWidget::zoomIn()
{
    int page = currentIndex();
    m_zoomFactorIndex[page] = qBound(0, m_zoomFactorIndex[page] + 1, 19);
    qDebug() << "Page = " << page << ", m_zoomFactorIndex[page] = " << m_zoomFactorIndex[page];
    if(page == 0)
    {
        for(int i = 0; i < 6; i++)
        {
            m_pThumbnails[i]->setZoomFactorPCT(ZOOMFACTOR[m_zoomFactorIndex[page]]);
        }
    }
    else
    {
        m_pView[page - 1]->setZoomFactorPCT(ZOOMFACTOR[m_zoomFactorIndex[page]]);
    }
}

void CamerasDisplayWidget::zoomOut()
{
    int page = currentIndex();
    m_zoomFactorIndex[page] = qBound(0, m_zoomFactorIndex[page] - 1, 19);
    qDebug() << "Page = " << page << ", m_zoomFactorIndex[page] = " << m_zoomFactorIndex[page];
    if(page == 0)
    {
        for(int i = 0; i < 6; i++)
        {
            m_pThumbnails[i]->setZoomFactorPCT(ZOOMFACTOR[m_zoomFactorIndex[page]]);
        }
    }
    else
    {
        m_pView[page - 1]->setZoomFactorPCT(ZOOMFACTOR[m_zoomFactorIndex[page]]);
    }
}

void CamerasDisplayWidget::setPage(int n)
{
    setCurrentIndex(n);
}

void CamerasDisplayWidget::attch(Qly::CameraImage_RGB32 * pImage[6])
{
    for(int i = 0; i < 6; i++)
    {
        connect(pImage[i], &Qly::CameraImage::imageChanged, m_pThumbnails[i], &Qly::ImageView::setImage);
        connect(pImage[i], &Qly::CameraImage::imageChanged, m_pView[i], &Qly::ImageView::setImage);
    }
}

void CamerasDisplayWidget::setZoomFactorPCT(int channel, int pct)
{
    channel = qBound(0, channel, 6);
    if(channel == 0)
    {
        for(int i = 0; i < 6; i++)
        {
            m_pThumbnails[i]->setZoomFactorPCT(pct);
        }
    }
    else
    {
        m_pView[channel - 1]->setZoomFactorPCT(pct);
    }
    update();
}
