#include "OpenCVDshowCamera.h"
#include "PixelFormatConvert.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <QTime>

namespace Qly
{

OpenCVDShowCamera::~OpenCVDShowCamera()
{
//    m_run = false;
    m_pGrab->stop();
    capture.release();
    m_workerThread.quit();
    m_workerThread.wait();
}

CameraGrabbingObject::CameraGrabbingObject(OpenCVDShowCamera * pCamera)
    : m_pCamera(pCamera)
{
    pFrame = new VideoFrame();
    m_pMutex = new QMutex;
}

CameraGrabbingObject::~CameraGrabbingObject()
{
    delete pFrame;
    delete m_pMutex;
}

static AVPixelFormat cvMatPixelFormat(const cv::Mat &mat)
{
    AVPixelFormat format = AV_PIX_FMT_NONE;
    switch (mat.type())
    {
    case CV_8UC1:
        format = AV_PIX_FMT_GRAY8;
        break;
    case CV_8UC3:
        format = AV_PIX_FMT_BGR24;
        break;
    case CV_8UC4:
        format = AV_PIX_FMT_BGRA;
        break;
    }
    return format;
}

inline void allocImage(QImage & image, QSize size, QImage::Format format)
{
    if(image.size() != size || image.format() != format)
    {
        image = QImage(size, format);
    }
}

void CameraGrabbingObject::grabbing()
{
    uint32_t count = 0;
    m_run = true;
    cv::Mat mat;
    qDebug()<<"from CameraGrabbingObject::grabbing():" << QThread::currentThreadId();
    while(m_run)
    { 
        if(m_pCamera->grab(mat))
        {
            QSize size(mat.cols, mat.rows);
            AVPixelFormat format = cvMatPixelFormat(mat);

            //m_pMutex->lock();// 这里开始要写 m_image 内存了。
            allocImage(m_image, size, QImage::Format_RGB32);
            RGBtoRGB32(mat.data, static_cast<int>(mat.step), format, size, m_image);

            pFrame->setData(m_image.bits(),
                            m_image.bytesPerLine(),
                            m_image.size(),
                            toAVPixelFormat(m_image.format()), 0);
                            //m_pMutex);
            pFrame->setExtraInfo(count ++, QDateTime::currentDateTime());
            emit m_pCamera->videoFrameReady(*pFrame);
        }
        else
        {
            m_run = false;
            return;
        }
    }
}

OpenCVDShowCamera::OpenCVDShowCamera(QObject *parent)
    :Qly::IndustryCamera(parent)
{
    m_pGrab = new CameraGrabbingObject(this);
    m_pGrab->moveToThread(&m_workerThread);
    connect(this, &OpenCVDShowCamera::start, m_pGrab, &CameraGrabbingObject::grabbing);
    connect(this, &OpenCVDShowCamera::stop, m_pGrab, &CameraGrabbingObject::stop);
    m_workerThread.start();
}

bool OpenCVDShowCamera::openDefaultCamera()
{
    return capture.open(0 + cv::CAP_DSHOW);
}

QString OpenCVDShowCamera::queryInfo(IMoniker *pMoniker, LPCOLESTR pszPropName)
{
    IPropertyBag *pPropBag;
    HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
    if (FAILED(hr))
    {
        pMoniker->Release();
        return QString();// Null string
    }

    VARIANT var;
    VariantInit(&var);
    hr = pPropBag->Read(pszPropName, &var, 0);
    if(FAILED(hr))
    {
        return QString("");
    }

    QString str = QString::fromUtf16(reinterpret_cast<ushort*>(var.bstrVal));
    VariantClear(&var);
    return str;
}

QString OpenCVDShowCamera::description(IMoniker *pMoniker)
{
    return queryInfo(pMoniker, L"Description");
}

QString OpenCVDShowCamera::friendlyName(IMoniker *pMoniker)
{
    return queryInfo(pMoniker, L"FriendlyName");
}

QString OpenCVDShowCamera::devicePath(IMoniker *pMoniker)
{
    return queryInfo(pMoniker, L"DevicePath");
}

QStringList OpenCVDShowCamera::deviceInformation(IMoniker *pMoniker)
{
    QString Description = description(pMoniker);
    QString FriendlyName = friendlyName(pMoniker);
    QString DevicePath = devicePath(pMoniker);

    QStringList info;
    info << Description << FriendlyName << DevicePath;
    return info;
}

QStringList OpenCVDShowCamera::listAllDevicePath()
{
    QStringList list;
    CoInitialize(NULL);
    ICreateDevEnum *pDevEnum = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,
                                  NULL,
                                  CLSCTX_INPROC_SERVER,
                                  IID_ICreateDevEnum,
                                  reinterpret_cast<void**>(&pDevEnum));
    if (SUCCEEDED(hr))
    {
        IEnumMoniker *pEnum = NULL;
        hr = pDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEnum, 0);
        if (hr == S_OK)
        {
            IMoniker *pMoniker = NULL;
            while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
            {
                IPropertyBag *pPropBag;
                hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)(&pPropBag));
                if (FAILED(hr))
                {
                    pMoniker->Release();
                    pPropBag->Release();
                    continue;
                }
                QString info = devicePath(pMoniker);
                if(!info.isNull())
                {
                    list << info;
                }
                pMoniker->Release();
                pMoniker = NULL;
            }
            pDevEnum->Release();
            pDevEnum = NULL;
            pEnum->Release();
            pEnum = NULL;
        }
    }
    return list;
}

QStringList OpenCVDShowCamera::listAllDeviceName()
{
    QStringList list;
    CoInitialize(NULL);
    ICreateDevEnum *pDevEnum = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,
                                  NULL,
                                  CLSCTX_INPROC_SERVER,
                                  IID_ICreateDevEnum,
                                  reinterpret_cast<void**>(&pDevEnum));
    if (SUCCEEDED(hr))
    {
        IEnumMoniker *pEnum = NULL;
        hr = pDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEnum, 0);
        if (hr == S_OK)
        {
            IMoniker *pMoniker = NULL;
            while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
            {
                IPropertyBag *pPropBag;
                hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)(&pPropBag));
                if (FAILED(hr))
                {
                    pMoniker->Release();
                    pPropBag->Release();
                    continue;
                }
                QString info = friendlyName(pMoniker);
                if(!info.isNull())
                {
                    list << info;
                }
                pMoniker->Release();
                pMoniker = NULL;
            }
            pDevEnum->Release();
            pDevEnum = NULL;
            pEnum->Release();
            pEnum = NULL;
        }
    }
    return list;
}

QStringList OpenCVDShowCamera::listAllDeviceDescription()
{
    QStringList list;
    CoInitialize(NULL);
    ICreateDevEnum *pDevEnum = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,
                                  NULL,
                                  CLSCTX_INPROC_SERVER,
                                  IID_ICreateDevEnum,
                                  reinterpret_cast<void**>(&pDevEnum));
    if (SUCCEEDED(hr))
    {
        IEnumMoniker *pEnum = NULL;
        hr = pDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEnum, 0);
        if (hr == S_OK)
        {
            IMoniker *pMoniker = NULL;
            while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
            {
                IPropertyBag *pPropBag;
                hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)(&pPropBag));
                if (FAILED(hr))
                {
                    pMoniker->Release();
                    pPropBag->Release();
                    continue;
                }
                QString info = description(pMoniker);
                if(!info.isNull())
                {
                    list << info;
                }
                pMoniker->Release();
                pMoniker = NULL;
            }
            pDevEnum->Release();
            pDevEnum = NULL;
            pEnum->Release();
            pEnum = NULL;
        }
    }
    return list;
}

bool OpenCVDShowCamera::isLive()
{
    return m_pGrab->isRunning();
}

int OpenCVDShowCamera::cameraIndex(QString devicePath)
{
    QStringList list = listAllDevicePath();
    return(list.indexOf(devicePath));
}

bool OpenCVDShowCamera::openCameraByName(QString devicePath)
{
    QStringList list = listAllDevicePath();
    //qDebug() << "listAllDevicePath : " << list;
    //qDebug() << "devicePath = " << devicePath;
    int index = list.indexOf(devicePath);
    if(index != -1)
    {
        return capture.open(index + cv::CAP_DSHOW);
    }
    return false;
}

bool OpenCVDShowCamera::setResolution(QSize size)
{
    capture.set(cv::CAP_PROP_FRAME_WIDTH, size.width());
    return capture.set(cv::CAP_PROP_FRAME_HEIGHT, size.height());
}


static QImage cvMat2QImage(const cv::Mat& mat, bool clone, bool rb_swap)
{
    const uchar *pSrc = (const uchar*)mat.data;
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        //QImage image(mat.cols, mat.rows, QImage::Format_Grayscale8);
        QImage image(pSrc, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8);
        if(clone) return image.copy();
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Create QImage with same dimensions as input Mat
        QImage::Format format = QImage::Format_BGR888;
        if(rb_swap) format = QImage::Format_RGB888;
        QImage image(pSrc, mat.cols, mat.rows, static_cast<int>(mat.step), format);
        if(clone)
        {
            if(rb_swap) return image.rgbSwapped();
            return image.copy();
        }
        else
        {
            if(rb_swap)
            {
                cv::cvtColor(mat, mat,cv::COLOR_BGR2RGB);
            }
            return image;
        }

    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        QImage image(pSrc, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_ARGB32);
        if(clone) return image.copy();
        return image;
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

bool OpenCVDShowCamera::grab(cv::Mat &mat)
{
    if(capture.isOpened())
    {
        capture >> mat;
        return !mat.empty();
    }
    return false;
}

bool OpenCVDShowCamera::grab(QImage &image)
{
    cv::Mat mat;
    if(capture.isOpened())
    {
        capture >> mat;
        if ( !mat.empty() )
        {
            QSize size(mat.cols, mat.rows);
            AVPixelFormat format = cvMatPixelFormat(mat);
            allocImage(image, size, QImage::Format_RGB32);
            RGBtoRGB32(mat.data, static_cast<int>(mat.step), format, size, image);
            return true;
        }
    }
    return false;
}

bool OpenCVDShowCamera::startGrabbing()
{
    if(capture.isOpened())
    {
        emit start();
        return true;
    }
    return false;
}

bool OpenCVDShowCamera::showVCDPropertyPage()
{
    capture.set(cv::CAP_PROP_SETTINGS, 1);
    return true;
}

bool OpenCVDShowCamera::stopGrabbing()
{
//    m_run = false;
    m_pGrab->stop();
    //emit stop();
    return true;
}

void OpenCVDShowCamera::close()
{
    emit stop();
    capture.release();
}


}
