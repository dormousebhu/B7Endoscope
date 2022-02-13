#ifndef TOOLBARIMAGEFILP_H
#define TOOLBARIMAGEFILP_H
#include <QComboBox>
#include <QLabel>
#include <QToolBar>
#include <QSettings>
#include "CameraImage.h"

namespace Qly
{
class ToolBarCamera : public QToolBar
{
    Q_OBJECT
public:
    ToolBarCamera(const QString &title, QWidget *parent = Q_NULLPTR);
    ToolBarCamera(QWidget *parent = Q_NULLPTR);
    void attach(CameraImage *cameraImage, bool flipFunc = true);
public slots:
    void defaultCameraOpened();
    void defaultCameraFailed();
    void selectCameraSuccessed();
    void selectCameraFailed();
    void cameraLost();
    void setBayerPattern(AVPixelFormat pattern);
    void setHorizontalFlip(bool on);
    void setVerticalFlip(bool on);
    void setFlipMode(CameraImage::FlipMode mode);
    void hideBayerSettings(bool on);
    void hideFlipSettings(bool on);
    void loadSettings(QSettings &settings);
    void writeSettings(QSettings &settings);

    void trigDefaultCamera();
    void trigStartCamera();
    void trigCloseCamera();
    void toggleStartStopCamera();
signals:
    void defaultCamera();
    void selectCamera();
    void closeCamera();
    void startCamera();
    void suspendCamera();
    void setupCamera();
    void bayerPatternChanged(AVPixelFormat pattern); /// 发送给CameraImage 类，用于 Bayer 解码。
    void verticalFlipImage(bool on);
    void horizontalFlipImage(bool on);
    //void flipModeChanged(CameraImage::FlipMode mode);  ///Camera、CameraImage、DisplayWidget 都可以翻转图像。
public:
    QAction *actionImageHFlip;
    QAction *actionImageVFlip;
    QAction *actionSelectCamera;
    QAction *actionDefaultCamera;
    QAction *actionStartLive;
    QAction *actionSetupCamera;
private:
    void initContent();
    void initConnection();
    QLabel *m_bayerLabel;
    QComboBox *m_comboxColorFilter;
    QAction *m_actionBayerLabel;
    QAction *m_actionColorFilter;
private slots:
    void on_actionSelectCamera_triggered(bool on);
    void on_actionDefaultCamera_triggered(bool on);
    void on_actionStartLive_triggered(bool on);
    void on_actionSetupCamera_triggered(bool on);
    void on_selectColorFilter(int n);
private:
    CameraImage *m_cameraImage;
};
} //namespace
#endif // TOOLBARIMAGEFILP_H
