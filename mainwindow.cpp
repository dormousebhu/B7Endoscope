﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCameraInfo>
#include <QtDebug>
#include <QFileDialog>
#include <QPainter>
#include <QKeyEvent>
#include <QStandardPaths>
#include <QTranslator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{  
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    setWindowTitle(tr("JHEndoscope, Beijing Jinghai technoloy Co.Ltd"));

    initCamera();
    initToolBar();
    initRemoteControl();

    loadSettings();
}

void MainWindow::initToolBar()
{

    m_pPageToolBar = new PageToolBar(this);
    addToolBar(m_pPageToolBar);
    connect(m_pPageToolBar, &PageToolBar::pageChanged, m_pCentreWidget, &CamerasDisplayWidget::setPage);
    connect(m_pPageToolBar, &PageToolBar::zoomIn, m_pCentreWidget, &CamerasDisplayWidget::zoomIn);
    connect(m_pPageToolBar, &PageToolBar::zoomOut, m_pCentreWidget, &CamerasDisplayWidget::zoomOut);


    m_pImageSaveToolBar = new ImageSaveToolBar(this);
    addToolBar(m_pImageSaveToolBar);
    connect(m_pImageSaveToolBar, &ImageSaveToolBar::saveImage, this, &MainWindow::saveImage);

    m_pVideoRecordToolBar = new VideoRecordToolBar(this);
    addToolBar(m_pVideoRecordToolBar);
    connect(m_pVideoRecordToolBar, &VideoRecordToolBar::startRecordVideo, this, &MainWindow::startRecordVideo);
    connect(m_pVideoRecordToolBar, &VideoRecordToolBar::stopRecordVideo, this, &MainWindow::stopRecordVideo);


    m_pSettingsToolBar = new SettingsToolBar(this);
    addToolBar(m_pSettingsToolBar);
    connect(m_pSettingsToolBar, &SettingsToolBar::showSettingsDialog, this, &MainWindow::showSettingsDialog);
    connect(m_pSettingsToolBar, &SettingsToolBar::startCamera, this, &MainWindow::startCamera);
    connect(m_pSettingsToolBar, &SettingsToolBar::stopCamera, this, &MainWindow::stopCamera);
    connect(m_pSettingsToolBar, &SettingsToolBar::translate, this, &MainWindow::translate);
}

void MainWindow::initCamera()
{
    for(int i = 0; i < 6; i++)
    {
        m_pCamera[i] = new Qly::PCCamera(this);
        m_pImage[i] = new Qly::CameraImage_RGB32(this);
        m_pImage[i]->setVerticalFlip(true);
        connect(m_pCamera[i], &Qly::IndustryCamera::ImageDataChanged, m_pImage[i], &Qly::CameraImage_RGB32::setImageData);
        connect(m_pImage[i], &Qly::CameraImage_RGB32::imageChanged, &m_videoRecorder[i], &Qly::VideoRecorder::setImage);
        connect(&m_videoRecorder[i], &Qly::VideoRecorder::recordStoped, this, &MainWindow::recordVideoTimeout);
    }

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras)
    {
        m_cameraName << cameraInfo.deviceName();
        qDebug() << "deviceName: " << cameraInfo.deviceName();
        qDebug() << "description: "<< cameraInfo.description();
    }

    for(int i = 0; i < m_cameraName.size(); i ++)
    {
        if(i > 5) break;
        m_pCamera[i] ->openCameraByName(m_cameraName.at(i));
    }
    m_pCentreWidget = new CamerasDisplayWidget(this);
    m_pCentreWidget->attch(m_pImage);
    setCentralWidget(m_pCentreWidget);

}

void MainWindow::initRemoteControl()
{
    connect(&m_remoteControl, &RemoteControl::saveImage, this, &MainWindow::saveImage);
    connect(&m_remoteControl, &RemoteControl::startRecordVideo, this, &MainWindow::startRecordVideo);
    connect(&m_remoteControl, &RemoteControl::stopRecordVideo, this, &MainWindow::stopRecordVideo);
    connect(&m_remoteControl, &RemoteControl::queryStatus, this, &MainWindow::status);
    connect(&m_remoteControl, &RemoteControl::connectionSetup, this, &MainWindow::connectionSetup);
    connect(&m_remoteControl, &RemoteControl::connectionLost, this, &MainWindow::connectionLost);

    m_pLabelConnectionStatus = new QLabel(this);
    m_pLabelConnectionStatus->setText("Waiting for connection.");
    statusBar()->addWidget(m_pLabelConnectionStatus);
}

void MainWindow::translate(bool on)
{
    QString qmFilename;
    static QTranslator* translator;
    if (translator != nullptr)
    {
        qApp->removeTranslator(translator);
        delete translator;
        translator = nullptr;
    }
    translator = new QTranslator;

    QString runPath = QCoreApplication::applicationDirPath();       //获取文件运行路径

    if(on)
    {
        qmFilename = runPath + "/B7Endoscope_zh_CN.qm";
    }
    if (translator->load(qmFilename))
    {
        qApp->installTranslator(translator);
    }
    ui->retranslateUi(this);
    m_pImageSaveToolBar->retranslateUi();
    m_pVideoRecordToolBar->retranslateUi();
    m_pPageToolBar->retranslateUi();
    m_pSettingsToolBar->retranslateUi();
}

void MainWindow::connectionSetup(QString ip)
{
    m_pLabelConnectionStatus->setText(QString("Connection setup. ip = %1").arg(ip));
}

void MainWindow::connectionLost()
{
    m_pLabelConnectionStatus->setText(QString("Connection Lost, Waiting for new connection."));
}

void MainWindow::showSettingsDialog()
{
    SettingDialog dialog;
    if(dialog.exec())
    {
        dialog.writeSettings();
        loadSettings();
    }
}


void MainWindow::loadSettings()
{
    QSettings settings("JingHai", "JHEndoscope", this);
    QString imagePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QString videoPath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

    for(int i = 0; i < 6; i++)
    {
        m_imagePath[i] = settings.value(QString("ImagePath/CH%1").arg(i + 1), imagePath).toString();
        m_videoPath[i] = settings.value(QString("VideoPath/CH%1").arg(i + 1), videoPath).toString();
        m_videoLength[i] = settings.value(QString("Video/Length%1").arg(i + 1), 10).toInt();
    }

    m_pCentreWidget->loadSettings(settings);
    restoreGeometry(settings.value("Geometry/MainWindow").toByteArray());
    restoreState(settings.value("Geometry/windowState").toByteArray());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_F1:
    case Qt::Key_F2:
    case Qt::Key_F3:
    case Qt::Key_F4:
    case Qt::Key_F5:
    case Qt::Key_F6:
        m_pPageToolBar->setPage(event->key() - Qt::Key_F1 + 1);
        break;
    case Qt::Key_Escape:
        m_pPageToolBar->setPage(0);
        break;
    case Qt::Key_Minus:
        m_pCentreWidget->zoomOut();
        break;
    case Qt::Key_Plus:
        m_pCentreWidget->zoomIn();
        break;
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

bool MainWindow::stopCamera()
{
    for(int i = 0; i < 6; i++)
    {
        m_pCamera[i]->stopGrabbing();
    }
    return true;
}

bool MainWindow::startCamera()
{
    for(int i = 0; i < 6; i++)
    {
        m_pCamera[i]->startGrabbing();
    }

    return true;
}

void MainWindow::saveImage(int n)
{
    bool ret = false;
    qDebug() << QString("MainWindow::saveImage(int %1)").arg(n);
    QDateTime t = QDateTime::currentDateTime();
    if(n > 0 && n < 7)
    {
        QString fileName = m_imagePath[n - 1] + QString("/CH%1_%2.jpg").arg(n).arg(t.toString("yyyyMMdd_hhmmss"));
        qDebug() << fileName;
        ret = m_pImage[n - 1]->saveImage(fileName);
    }
    else if(n == 0)
    {
        for(int i = 1; i <= 6; i++)
        {
            QString fileName = m_imagePath[i - 1] + QString("/CH%1_%2.jpg").arg(i).arg(t.toString("yyyyMMdd_hhmmss"));
            qDebug() << fileName;
            ret = m_pImage[i - 1]->saveImage(fileName);
        }
    }
    RemoteControl *pControl = qobject_cast<RemoteControl *>(sender());
    if(pControl && pControl == &m_remoteControl)
    {
        if(ret)
        {
            pControl->sendOK();
        }
        else
        {
            pControl->sendFAIL();
        }
    }
}

void MainWindow::status()
{
    RemoteControl *pControl = qobject_cast<RemoteControl *>(sender());
    if(pControl && pControl == &m_remoteControl)
    {
        pControl->sendOK();
    }

}

void MainWindow::recordVideoTimeout()
{
    qDebug() << "void MainWindow::recordVideoTimeout()";
    Qly::VideoRecorder * pSender = (Qly::VideoRecorder *) sender();
    for(int i = 0; i < 6; i++)
    {
        if(pSender == &m_videoRecorder[i])
        {
            m_pVideoRecordToolBar->videoRecordStoped(i + 1);
        }
    }
}

bool MainWindow::startRecordVideo(int n)
{
    qDebug() << QString("startRecordVideo(%1)").arg(n);
    const int index = n - 1;
    QDateTime t = QDateTime::currentDateTime();
    QString fileName = m_videoPath[index] + QString("/CH%1_%2.avi").arg(n).arg(t.toString("yyyyMMdd_hhmmss"));
    m_videoRecorder[index].setMaxRecordTime(m_videoLength[index] * 1000);
    int ret = m_videoRecorder[index].openFile(fileName);

    RemoteControl *pControl = qobject_cast<RemoteControl *>(sender());
    if(pControl && pControl == &m_remoteControl)
    {
        if(ret >= 0)
        {
            pControl->sendOK();
        }
        else
        {
            pControl->sendFAIL();
        }
    }
    return true;
}

void MainWindow::stopRecordVideo(int n)
{
    const int index = n - 1;
    bool ret = m_videoRecorder[index].close();
    m_pVideoRecordToolBar->videoRecordStoped(n);
    RemoteControl *pControl = qobject_cast<RemoteControl *>(sender());
    if(pControl && pControl == &m_remoteControl)
    {
        if(ret)
        {
            pControl->sendOK();
        }
        else
        {
            pControl->sendFAIL();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("JingHai", "JHEndoscope", this);
    m_pCentreWidget->writeSettings(settings);
    settings.setValue("Geometry/MainWindow", saveGeometry());
    settings.setValue("Geometry/windowState", saveState());
}

MainWindow::~MainWindow()
{
    delete ui;
}

