#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "camera/CameraImage_RGB32.h"
#include "camera/CameraPC.h"
#include "ffmpeg/VideoRecorder.h"
#include "CamerasDisplayWidget.h"
#include "PageToolBar.h"
#include "ImageSaveToolBar.h"
#include "VideoRecordToolBar.h"
#include "RemoteControl.h"
#include "SettingDialog.h"
#include "SettingsToolBar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /// n = 0 表示保存全部 6 个图像， n = 1..6 表示保存特定相机的图像
    void saveImage(int n);
    bool startRecordVideo(int n);// n = 1..6 表示保存特定相机的视频
    void stopRecordVideo(int n);
    void recordVideoTimeout();

    void status();
    void showSettingsDialog();
    bool startCamera();
    bool stopCamera();

    void connectionSetup(QString ip);
    void connectionLost();

    void translate(bool on);
private:

    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void initRemoteControl();
    void initCamera();
    void initToolBar();
    void loadSettings();
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    Ui::MainWindow *ui;
    CamerasDisplayWidget * m_pCentreWidget;
    PageToolBar * m_pPageToolBar;
    ImageSaveToolBar * m_pImageSaveToolBar;
    VideoRecordToolBar * m_pVideoRecordToolBar;
    SettingsToolBar * m_pSettingsToolBar;

    Qly::PCCamera * m_pCamera[6];         //摄像头对象
    QStringList m_cameraName;
    Qly::CameraImage_RGB32 *m_pImage[6];
    Qly::VideoRecorder m_videoRecorder[6];

    QString m_imagePath[6]; // 保存图像的路径
    QString m_videoPath[6]; // 保存视频的路径
    int m_videoLength[6];

    RemoteControl m_remoteControl;

    QLabel * m_pLabelConnectionStatus;

};
#endif // MAINWINDOW_H
