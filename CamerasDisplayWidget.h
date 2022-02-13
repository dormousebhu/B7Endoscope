﻿#ifndef CAMERASDISPLAYWIDGET_H
#define CAMERASDISPLAYWIDGET_H

#include <QStackedWidget>
#include <QScrollArea>
#include <QSettings>
#include "ImageView.h"
#include "CameraCommon/CameraImage_RGB32.h"

class CamerasDisplayWidget : public QStackedWidget
{
    Q_OBJECT
public:
    CamerasDisplayWidget(QWidget * parent = nullptr);
    void loadSettings(QSettings &settings);
    void writeSettings(QSettings &settings);
public slots:
    void zoomIn();
    void zoomOut();
    void setPage(int n);
    void setZoomFactorPCT(int channel, int pct);
    void attch(Qly::CameraImage_RGB32 * pImage[6]);
private:
    Qly::ImageView * m_pThumbnails[6];
    Qly::ImageView * m_pView[6];
    QScrollArea * m_pScrollArea[7];

    int m_zoomFactorIndex[7];
};

#endif // CAMERASDISPLAYWIDGET_H
