#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "CameraCommon/CameraImage_RGB32.h"
#include "CameraOpenCV/OpenCVDshowCamera.h"
#include "ffmpeg/VideoRecorder.h"
#include "CamerasDisplayWidget.h"
#include "RemoteControl.h"
#include "SettingDialog.h"
#include "AboutDialog.h"

#include "FocusToolBar.h"

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
    void showAbout();

    void setResolution(QSize z);
    void showPage(int i);
private:

    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void initRemoteControl();
    void initCamera2();
    void initToolBar();
    void loadSettings();
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void defaultCompanyInfo();
    Ui::MainWindow *ui = nullptr;
    CamerasDisplayWidget * m_pCentreWidget = nullptr;

    //摄像头对象
    Qly::OpenCVDShowCamera *m_pCamera[6];
    QStringList m_cameraNames;
    Qly::CameraImage_RGB32 *m_pImage[6];
    Qly::VideoRecorder m_videoRecorder[6];

    QString m_imagePath[6]; // 保存图像的路径
    QString m_videoPath[6]; // 保存视频的路径
    int m_videoLength[6];

    RemoteControl m_remoteControl;

    QLabel * m_pLabelConnectionStatus = nullptr;
    AboutDialog *m_aboutDialog = nullptr;

    /// 以下是公司信息，在派生类里会设置这些内容
    QString m_appName;
    QString m_companyName;
    QString m_author;
    QString m_website;
    QString m_email;
    QString m_version;
    QString m_phoneNumber;
    QIcon m_appIcon;

    FocusToolBar *m_focusToolBar = nullptr;

};
#endif // MAINWINDOW_H
