#include "ToolBarCamera.h"
#include "CameraImage_RGB32.h"
#include "CameraImage_RGB24.h"
#include <QDebug>
#include <QApplication>

namespace Qly {

ToolBarCamera::ToolBarCamera(const QString &title, QWidget *parent)
    :QToolBar(title, parent)
{
    setIconSize(QSize(32, 32));
    setObjectName(QStringLiteral("ToolBarCamera"));
    initContent();
    initConnection();
    m_cameraImage = nullptr;
    hideBayerSettings(true);
}
ToolBarCamera::ToolBarCamera(QWidget *parent)
    :QToolBar(parent)
{
    setIconSize(QSize(32, 32));
    setObjectName(QStringLiteral("ToolBarCamera"));
    initContent();
    initConnection();
    m_cameraImage = nullptr;
    hideBayerSettings(true);
}

void ToolBarCamera::hideBayerSettings(bool on)
{
    m_actionColorFilter->setVisible(!on);
    m_actionBayerLabel->setVisible(!on);
//    if(on)
//    {
//        //qDebug() << "hideBayerSettings(true)";

//        m_comboBayerMethod->setFixedWidth(0);
//        m_comboxColorFilter->setFixedWidth(0);
//        m_bayerLabel->setFixedWidth(0);
//    }
//    else
//    {
//        m_comboBayerMethod->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//        m_comboxColorFilter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//        m_bayerLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    }
}
void ToolBarCamera::hideFlipSettings(bool on)
{
    actionImageHFlip->setVisible(!on);
    actionImageVFlip->setVisible(!on);
}

void ToolBarCamera::attach(CameraImage *cameraImage, bool flipFunc)
{
    m_cameraImage = cameraImage;
    AVPixelFormat p = m_cameraImage->bayerPattern();
    qDebug() << " m_cameraImage->bayerPattern() = " << p;
    setBayerPattern(p);
    connect(this, SIGNAL(bayerPatternChanged(BAYER_PATTERN)), m_cameraImage, SLOT(setBayerPattern(BAYER_PATTERN)));
    connect(m_cameraImage, SIGNAL(bayerPatternChanged(BAYER_PATTERN)), this, SLOT(setBayerPattern(BAYER_PATTERN)));

    if(flipFunc)
    {
        //connect(this, SIGNAL(flipModeChanged(CameraImage::FlipMode)), m_cameraImage, SLOT(setFlipMode(CameraImage::FlipMode)));
        connect(this, SIGNAL(verticalFlipImage(bool)), m_cameraImage, SLOT(setVerticalFlip(bool)));
        connect(this, SIGNAL(horizontalFlipImage(bool)), m_cameraImage, SLOT(setHorizontalFlip(bool)));
        connect(m_cameraImage, SIGNAL(flipModeChanged(CameraImage::FlipMode)), this, SLOT(setFlipMode(CameraImage::FlipMode)));
    }
    actionImageHFlip->setChecked(false);
    actionImageVFlip->setChecked(false);
}

void ToolBarCamera::setBayerPattern(AVPixelFormat pattern)
{
    int n = m_comboxColorFilter->currentIndex();
    if(pattern != n + AV_PIX_FMT_BAYER_BGGR8)
    {
        m_comboxColorFilter->setCurrentIndex(pattern - AV_PIX_FMT_BAYER_BGGR8);
    }
}


void ToolBarCamera::setFlipMode(CameraImage::FlipMode mode)
{
    actionImageHFlip->setChecked(mode & CameraImage::HORIZONTAL_FLIP);
    actionImageVFlip->setChecked(mode & CameraImage::VERTICAL_FLIP);
}

void ToolBarCamera::trigDefaultCamera()
{
    actionDefaultCamera->setChecked(true);
    on_actionDefaultCamera_triggered(true);
}

void ToolBarCamera::trigStartCamera()
{
    actionStartLive->setChecked(true);
    on_actionStartLive_triggered(true);
}

void ToolBarCamera::toggleStartStopCamera()
{
    if(actionStartLive->isEnabled() == false)
    {
        return;
    }
    if(actionStartLive->isChecked())
    {
        actionStartLive->setChecked(false);
        on_actionStartLive_triggered(false);

    }
    else
    {
        actionStartLive->setChecked(true);
        on_actionStartLive_triggered(true);
    }
}

void ToolBarCamera::trigCloseCamera()
{
    actionDefaultCamera->setChecked(false);
    on_actionDefaultCamera_triggered(false);
}

void ToolBarCamera::loadSettings(QSettings &settings)
{
    bool on = settings.value("CameraView/VerticalFilp", false).toBool();
    qDebug() << "CameraView/VerticalFilp = " << on;
    setVerticalFlip(on);
    on = settings.value("CameraView/HorizontalFilp", false).toBool();
    qDebug() << "CameraView/HorizontalFilp = " << on;
    setHorizontalFlip(on);
    on = settings.value("CameraView/HideBayerSettings", true).toBool();
    hideBayerSettings(on);
    AVPixelFormat p = (AVPixelFormat)settings.value("CameraView/BayerPattern", AV_PIX_FMT_BAYER_BGGR8).toInt();
    setBayerPattern(p);
}

void ToolBarCamera::writeSettings(QSettings &settings)
{
    bool hf = actionImageHFlip->isChecked();
    bool vf = actionImageVFlip->isChecked();
    settings.setValue("CameraView/VerticalFilp", vf);
    settings.setValue("CameraView/HorizontalFilp", hf);
}

void ToolBarCamera::setHorizontalFlip(bool on)
{
    actionImageHFlip->setChecked(on);
}

void ToolBarCamera::setVerticalFlip(bool on)
{
    actionImageVFlip->setChecked(on);
}

void ToolBarCamera::on_selectColorFilter(int n)
{
    emit bayerPatternChanged(static_cast<AVPixelFormat> (n + AV_PIX_FMT_BAYER_BGGR8));
}


void ToolBarCamera::on_actionSelectCamera_triggered(bool on)
{
    if(on)
    {
        emit selectCamera();
    }
    else
    {
        actionDefaultCamera->setEnabled(true);
        actionDefaultCamera->setChecked(false);
        actionSelectCamera->setChecked(false);
        actionStartLive->setEnabled(false);
        actionStartLive->setChecked(false);
        actionSetupCamera->setEnabled(false);
        emit closeCamera();
    }
}

void ToolBarCamera::on_actionDefaultCamera_triggered(bool on)
{
    qDebug() << "on_actionDefaultCamera_triggered()";
    if(on)
    {
        emit defaultCamera();
    }
    else
    {
        actionDefaultCamera->setEnabled(true);
        actionDefaultCamera->setChecked(false);
        actionSelectCamera->setEnabled(true);
        actionSelectCamera->setChecked(false);
        actionStartLive->setEnabled(false);
        actionStartLive->setChecked(false);
        actionSetupCamera->setEnabled(false);
        emit closeCamera();
    }
}

void ToolBarCamera::on_actionStartLive_triggered(bool on)
{
    if(on)
    {
        emit startCamera();
    }
    else
    {
        emit suspendCamera();
    }
}

void ToolBarCamera::on_actionSetupCamera_triggered(bool on)
{
    Q_UNUSED(on);
    qDebug() << "on_actionSetupCamera_triggered";
    emit setupCamera();
}

void ToolBarCamera::initContent()
{
    setObjectName(QStringLiteral("ToolBarCamera"));
    setWindowTitle("Camera Toolbar");
    actionSelectCamera = new QAction(tr("Select Camera"), this);
    actionSelectCamera->setObjectName(QStringLiteral("actionSelectCamera"));
    actionSelectCamera->setCheckable(true);
    QIcon iconSelectCamera;
    iconSelectCamera.addFile(QStringLiteral(":/CameraCommon/res/camera.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSelectCamera->setIcon(iconSelectCamera);
    addAction(actionSelectCamera);
    actionSelectCamera->setVisible(false);

    actionDefaultCamera = new QAction(tr("Find Default Camera"), this);
    actionDefaultCamera->setObjectName(QStringLiteral("actionDefaultCamera"));
    actionDefaultCamera->setCheckable(true);
    QIcon iconDefaultCamera;
    iconDefaultCamera.addFile(QStringLiteral(":/CameraCommon/res/IC-Camera.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionDefaultCamera->setIcon(iconDefaultCamera);
    addAction(actionDefaultCamera);


    actionStartLive = new QAction(tr("Start/Stop"), this);
    actionStartLive->setObjectName(QStringLiteral("actionStartLive"));
    actionStartLive->setCheckable(true);
    actionStartLive->setEnabled(false);
    QIcon iconStartLive;
    iconStartLive.addFile(QStringLiteral(":/CameraCommon/res/start.png"), QSize(), QIcon::Normal, QIcon::Off);
    iconStartLive.addFile(QStringLiteral(":/CameraCommon/res/suspend.png"), QSize(), QIcon::Normal, QIcon::On);
    actionStartLive->setIcon(iconStartLive);
    addAction(actionStartLive);


    actionSetupCamera = new QAction(tr("Setup..."), this);
    actionSetupCamera->setObjectName(QStringLiteral("actionSetupCamera"));
    actionSetupCamera->setEnabled(false);
    QIcon iconSetupCamera;
    iconSetupCamera.addFile(QStringLiteral(":/CameraCommon/res/setup.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSetupCamera->setIcon(iconSetupCamera);
    addAction(actionSetupCamera);
    addSeparator();

    actionImageHFlip = new QAction(tr("Flip Horizontal"), this);
    actionImageHFlip->setObjectName(QStringLiteral("actionImageHFlip"));
    actionImageHFlip->setCheckable(true);
    QIcon iconImageHFlip;
    iconImageHFlip.addFile(QStringLiteral(":/CameraCommon/res/HFlip.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionImageHFlip->setIcon(iconImageHFlip);

    actionImageVFlip = new QAction(tr("Flip Vertical"), this);
    actionImageVFlip->setObjectName(QStringLiteral("actionImageVFlip"));
    actionImageVFlip->setCheckable(true);
    QIcon iconImageVFlip;
    iconImageVFlip.addFile(QStringLiteral(":/CameraCommon/res/VFlip.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionImageVFlip->setIcon(iconImageVFlip);

    addAction(actionImageHFlip);
    addAction(actionImageVFlip);

    addSeparator();
    m_bayerLabel = new QLabel(tr("Bayer Method:"), this);
    m_actionBayerLabel = addWidget(m_bayerLabel);

    m_comboxColorFilter = new QComboBox(this);
    m_comboxColorFilter->addItem(QIcon(":/CameraCommon/res/ColorFilter_BG.png"), "");
    m_comboxColorFilter->addItem(QIcon(":/CameraCommon/res/ColorFilter_RG.png"), "");
    m_comboxColorFilter->addItem(QIcon(":/CameraCommon/res/ColorFilter_GB.png"), "");
    m_comboxColorFilter->addItem(QIcon(":/CameraCommon/res/ColorFilter_GR.png"), "");

    m_comboxColorFilter->setIconSize(QSize(32, 32));
    m_comboxColorFilter->setFixedWidth(56);
    m_actionColorFilter = addWidget(m_comboxColorFilter);
}

void ToolBarCamera::initConnection()
{
    connect(actionSelectCamera, SIGNAL(triggered(bool)), this, SLOT(on_actionSelectCamera_triggered(bool)));
    connect(actionDefaultCamera, SIGNAL(triggered(bool)), this, SLOT(on_actionDefaultCamera_triggered(bool)));
    connect(actionStartLive, SIGNAL(triggered(bool)), this, SLOT(on_actionStartLive_triggered(bool)));
    connect(actionSetupCamera, SIGNAL(triggered(bool)), this, SLOT(on_actionSetupCamera_triggered(bool)));
    connect(m_comboxColorFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(on_selectColorFilter(int)));

    connect(actionImageHFlip, SIGNAL(triggered(bool)), this, SIGNAL(horizontalFlipImage(bool)));
    connect(actionImageVFlip, SIGNAL(triggered(bool)), this, SIGNAL(verticalFlipImage(bool)));
}

void ToolBarCamera::selectCameraSuccessed()
{
    actionDefaultCamera->setEnabled(false);
    actionStartLive->setEnabled(true);
    actionSetupCamera->setEnabled(true);
}

void ToolBarCamera::selectCameraFailed()
{
    actionSelectCamera->setChecked(false);
    actionStartLive->setEnabled(false);
    actionStartLive->setChecked(false);
    actionSetupCamera->setEnabled(false);
}

void ToolBarCamera::cameraLost()
{
    actionDefaultCamera->setChecked(false);
    actionSelectCamera->setChecked(false);
    actionSetupCamera->setEnabled(false);
    actionStartLive->setEnabled(false);
}

void ToolBarCamera::defaultCameraOpened()
{
    actionSelectCamera->setEnabled(false);
    actionSelectCamera->setChecked(false);

    actionStartLive->setEnabled(true);
    actionStartLive->setChecked(false);

    actionSetupCamera->setEnabled(true);
    actionSetupCamera->setChecked(false);
}

void ToolBarCamera::defaultCameraFailed()
{
    actionDefaultCamera->setChecked(false);

    actionSelectCamera->setEnabled(true);
    actionSelectCamera->setChecked(false);

    actionStartLive->setEnabled(false);
    actionStartLive->setChecked(false);

    actionSetupCamera->setEnabled(false);
    actionSetupCamera->setChecked(false);
}
} //namespace Qly
