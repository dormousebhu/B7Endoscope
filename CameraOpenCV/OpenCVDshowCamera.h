#ifndef CVCAMERA_H
#define CVCAMERA_H
#include "IndustryCamera.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <QImage>
#include <QThread>
#include <dshow.h>
#include <strmif.h>
namespace Qly{

class OpenCVDShowCamera;
class CameraGrabbingObject: public QObject
{
    Q_OBJECT
public:
    CameraGrabbingObject(OpenCVDShowCamera * pCamera);
    ~CameraGrabbingObject();
    bool isRunning() {return m_run;}
public slots:
    void grabbing();
    void stop() {m_run = false;}
private:
    OpenCVDShowCamera * m_pCamera;
    QImage m_image;
    VideoFrame *pFrame;
    QMutex *m_pMutex;
    bool m_run = false;
};

class OpenCVDShowCamera: public Qly::IndustryCamera
{
    Q_OBJECT
public:
    OpenCVDShowCamera(QObject *parent);
    ~OpenCVDShowCamera();
    static QStringList listAllDevicePath();
    static QStringList listAllDeviceName();
    static QStringList listAllDeviceDescription();
    bool openDefaultCamera() Q_DECL_OVERRIDE;
    bool openCameraByName(QString devicePath);
    //cv::Mat grabOne();
    bool grab(cv::Mat &mat);
    bool grab(QImage &image);
    bool startGrabbing() Q_DECL_OVERRIDE;
    bool stopGrabbing() Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;
    bool showVCDPropertyPage() Q_DECL_OVERRIDE;
    bool isLive() Q_DECL_OVERRIDE;
    bool setResolution(QSize size);
signals:
      void start();
      void stop();
//protected:
//    void run();
private:
    static QString queryInfo(IMoniker *pMoniker, LPCOLESTR pszPropName);
    static QString description(IMoniker *pMoniker);
    static QString friendlyName(IMoniker *pMoniker);
    static QString devicePath(IMoniker *pMoniker);
    static QStringList deviceInformation(IMoniker *pMoniker);
    static int cameraIndex(QString devicePath);
    QThread m_workerThread;
    cv::VideoCapture capture;
    CameraGrabbingObject * m_pGrab = nullptr;

//    int m_interval_ms = 50;
//    bool m_run = false;
};

}
#endif // CVCAMERA_H
