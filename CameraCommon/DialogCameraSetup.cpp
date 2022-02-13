#include "DialogCameraSetup.h"
#include "ui_DialogCameraSetup.h"
#include "IndustryCamera.h"
#include <QTimer>
#include <QDebug>
#include <math.h>

inline int digCount(int value)
{
    int num = 0;
    while(value != 0)
    {
        value /= 10;
        num ++;
    }
    return num;
}

/**
 * @brief round 将整数取前 dig_num 位有效数字，向零四舍五入
 * @param value
 * @param dig_num
 * @return
 */
int round(int value, unsigned int dig_num)
{
    int value_abs = (value < 0) ? -value: value;
    int sign = (value < 0) ? -1: 1;
    int num = digCount(value);

    // 到这里 value 有 num 位数字。
    int div[10] = {1, 10, 100 ,1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
    if(num > dig_num) // 这时才需要取整
    {
        dig_num = num - dig_num;
        value_abs = (value_abs + div[dig_num] / 2) / div[dig_num] * div[dig_num];
    }
    return sign * value_abs;
}

namespace Qly {

DialogCameraSetup::DialogCameraSetup(IndustryCamera * camera, QWidget *parent) :
    m_pIndustryCamera(camera),
    QDialog(parent),
    ui(new Ui::DialogCameraSetup)
{
    ui->setupUi(this);
    setModal(true);

    initExposureTab();
    initColorTab();
    initImageTab();
    initROITab();

    enableBrightnessUI(false);
    enableContrastUI(false);
    enableBlackLevelUI(false);
    enableGainUI(false);

    enableExposureTimeUI(false);
    enableAutoExposureUI(false);
    enableGammaUI(false);
    enableHueUI(false);
    enableSaturationUI(false);
    enableHueUI(false);
    enableSaturationUI(false);
    enableSharpnessUI(false);
    enableDenoiseUI(false);
    enableAutoROIUI(false);

    connect(ui->pushButtonWhiteBalanceOnePush, SIGNAL(clicked(bool)), this, SLOT(onWhiteBalanceOnePush()));
    connect(ui->pushButtonExposureOnePush, SIGNAL(clicked(bool)), this, SLOT(onExposureOnePush()));

    connect(ui->checkBoxAutoExposure, SIGNAL(toggled(bool)), this, SLOT(changeAutoExposure(bool)));
    connect(ui->checkBoxAutoWhiteBalance, SIGNAL(toggled(bool)), this, SLOT(changeAutoWhiteBalance(bool)));

    connect(ui->horizontalSliderBrightness, SIGNAL(valueChanged(int)), this, SLOT(changeBrightness(int)));
    connect(ui->horizontalSliderContrast, SIGNAL(valueChanged(int)), this, SLOT(changeContrast(int)));
    connect(ui->horizontalSliderBlacklevel, SIGNAL(valueChanged(int)), this, SLOT(changeBlackLevel(int)));
    connect(ui->horizontalSliderGain, SIGNAL(valueChanged(int)), this, SLOT(changeGain(int)));

    connect(ui->horizontalSliderExposureTime, SIGNAL(valueChanged(int)), this, SLOT(changeExposureTime(int)));

    connect(ui->horizontalSliderHue, SIGNAL(valueChanged(int)), this, SLOT(changeHue(int)));
    connect(ui->horizontalSliderSaturation, SIGNAL(valueChanged(int)), this, SLOT(changeSaturation(int)));

    connect(ui->horizontalSliderRed, SIGNAL(valueChanged(int)), this, SLOT(changeWhiteBalanceRed(int)));
    connect(ui->horizontalSliderGreen, SIGNAL(valueChanged(int)), this, SLOT(changeWhiteBalanceGreen(int)));
    connect(ui->horizontalSliderBlue, SIGNAL(valueChanged(int)), this, SLOT(changeWhiteBalanceBlue(int)));
    connect(ui->horizontalSliderTemp, SIGNAL(valueChanged(int)), this, SLOT(changeWhiteBalanceTemperature(int)));
    connect(ui->horizontalSliderSharpness, SIGNAL(valueChanged(int)), this, SLOT(changeSharpness(int)));
    connect(ui->horizontalSliderGamma, SIGNAL(valueChanged(int)), this, SLOT(changeGamma(int)));
    connect(ui->horizontalSliderDenoise, SIGNAL(valueChanged(int)), this, SLOT(changeDenoise(int)));

    connect(ui->checkBoxHue, SIGNAL(toggled(bool)), this, SLOT(changeHueEnable(bool)));
    connect(ui->checkBoxSaturation, SIGNAL(toggled(bool)), this, SLOT(changeSaturationEnable(bool)));
    connect(ui->checkBoxSharpness, SIGNAL(toggled(bool)), this, SLOT(changeSharpnessEnable(bool)));
    connect(ui->checkBoxGamma, SIGNAL(toggled(bool)), this, SLOT(changeGammaEnable(bool)));
    connect(ui->checkBoxDenoise, SIGNAL(toggled(bool)), this, SLOT(changeDenoiseEnable(bool)));

}
void DialogCameraSetup::initSaturationUI()
{
    bool bValue;
    double dValue;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    bool ret1 = p->saturationEnable(bValue);
    bool ret2 = p->saturation(dValue);

    if(ret1 && ret2)
    {
        ui->checkBoxSaturation->setEnabled(true);
        ui->checkBoxSaturation->setChecked(bValue);
        setSaturationRange(-1, 1);
        setSaturation(dValue);
        enableSaturationUI(true);
        return;
    }
    ui->checkBoxSaturation->setEnabled(false);
    enableSaturationUI(false);
}

void DialogCameraSetup::initDenoiseUI()
{
    bool bValue;
    double dValue;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    bool ret1 = p->denoiseEnable(bValue);
    bool ret2 = p->denoise(dValue);

    if(ret1 && ret2)
    {
        ui->checkBoxDenoise->setEnabled(true);
        ui->checkBoxDenoise->setChecked(bValue);
        setDenoiseRange(-1, 1);
        setDenoise(dValue);
        enableDenoiseUI(true);
        return;
    }
    ui->checkBoxDenoise->setEnabled(false);
    enableDenoiseUI(false);
}

void DialogCameraSetup::initHueUI()
{
    bool bValue;
    double dValue;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    bool ret1 = p->hueEnable(bValue);
    bool ret2 = p->hue(dValue);
    if(ret1 && ret2)
    {
        ui->checkBoxHue->setEnabled(true);
        ui->checkBoxHue->setChecked(bValue);
        setHueRange(-1, 1);
        setHue(dValue);
        enableHueUI(true);
        return;
    }
    ui->checkBoxHue->setEnabled(false);
    enableHueUI(false);
}

void DialogCameraSetup::initSharpnessUI()
{
    bool bValue;
    double dValue;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    bool ret1 = p->sharpnessEnable(bValue);
    bool ret2 = p->sharpness(dValue);

    if(ret1 && ret2)
    {
        ui->checkBoxSharpness->setEnabled(true);
        ui->checkBoxSharpness->setChecked(bValue);
        setSharpnessRange(-1, 1);
        setSharpness(dValue);
        enableSharpnessUI(true);
        return;
    }
    ui->checkBoxSharpness->setEnabled(false);
    enableSharpnessUI(false);
}

void DialogCameraSetup::initGammaUI()
{
    bool bValue;
    double fMin, fMax, fValue;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    bool ret1 = p->gammaEnable(bValue);
    bool ret2 = p->gammaRange(fMin, fMax, fValue);

    if(ret1 && ret2)
    {
        ui->checkBoxGamma->setEnabled(true);
        ui->checkBoxGamma->setChecked(bValue);
        setGammaRange(fMin, fMax);
        setGamma(fValue);
        enableGammaUI(true);
        return;
    }
    ui->checkBoxGamma->setEnabled(false);
    enableGammaUI(false);
}

void DialogCameraSetup::initWhiteBalanceUI()
{
    bool on;
    int nMin, nMax, nValue;
    ui->checkBoxAutoWhiteBalance->setChecked(false);
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    CameraImageProcessing::WhiteBalanceMode mode;
    on = p->whiteBalanceMode(mode);
    qDebug() << "whiteBalanceMode = " << mode << " ret = " << on;
    if(on)
    {
        switch (mode)
        {
        case CameraImageProcessing::WhiteBalanceManual:
            ui->checkBoxAutoWhiteBalance->setChecked(false);
            break;
        case CameraImageProcessing::WhiteBalanceAutoContinuous:
            ui->checkBoxAutoWhiteBalance->setChecked(true);
            break;
        default:
            ui->checkBoxAutoWhiteBalance->setChecked(false);
            break;
        }
    }
    ui->checkBoxAutoWhiteBalance->setEnabled(on);
    ui->pushButtonWhiteBalanceOnePush->setEnabled(on);

    on = p->whiteBalanceRedRange(nMin, nMax, nValue);
    if(on)
    {
        ui->spinBoxRed->setRange(nMin, nMax);
        ui->horizontalSliderRed->setRange(nMin, nMax);
        ui->spinBoxRed->setValue(nValue);
    }
    ui->spinBoxRed->setEnabled(on);
    ui->horizontalSliderRed->setEnabled(on);

    on = p->whiteBalanceGreenRange(nMin, nMax, nValue);
    if(on)
    {
        ui->spinBoxGreen->setRange(nMin, nMax);
        ui->horizontalSliderGreen->setRange(nMin, nMax);
        ui->spinBoxGreen->setValue(nValue);
    }
    ui->spinBoxGreen->setEnabled(on);
    ui->horizontalSliderGreen->setEnabled(on);

    on = p->whiteBalanceBlueRange(nMin, nMax, nValue);
    if(on)
    {
        ui->spinBoxBlue->setRange(nMin, nMax);
        ui->horizontalSliderBlue->setRange(nMin, nMax);
        ui->spinBoxBlue->setValue(nValue);
    }
    ui->spinBoxBlue->setEnabled(on);
    ui->horizontalSliderBlue->setEnabled(on);

    ui->horizontalSliderTemp->setRange(2000, 12000);
    ui->spinBoxTemp->setRange(2000, 12000);
}

void DialogCameraSetup::setExposureTimeRange(int min, int max)
{
    //qDebug() << "begin setExposureTimeRange";
    ui->horizontalSliderExposureTime->setRange(min, max);
    ui->spinBoxExposureTime->setRange(min, max);
    //qDebug() << "end setExposureTimeRange";
}

void DialogCameraSetup::setExposureTime(int value)
{
    ui->spinBoxExposureTime->setValue(value);
}

void DialogCameraSetup::setAutoExposureTimeLowerLimitRange(int min, int max)
{
    ui->spinBoxAutoExposureMin->setRange(min, max);
}

void DialogCameraSetup::setAutoExposureTimeLowerLimit(int value)
{
    ui->spinBoxAutoExposureMin->setValue(value);
}

void DialogCameraSetup::setAutoExposureTimeUpperLimitRange(int min, int max)
{
    ui->spinBoxAutoExposureMax->setRange(min, max);
}

void DialogCameraSetup::setAutoExposureTimeUpperLimit(int value)
{
    ui->spinBoxAutoExposureMax->setValue(value);
}

void DialogCameraSetup::changeGain(int value)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setGain(value / 100.0);
}

void DialogCameraSetup::changeSharpness(int value)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setSharpness(value / 100.0);
}

void DialogCameraSetup::changeBlackLevel(int value)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setBlackLevel(value / 100.0);
}

void DialogCameraSetup::changeContrast(int value)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setContrast(value / 100.0);
}

void DialogCameraSetup::changeGamma(int value)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setGamma(value / 10.0);
}

void DialogCameraSetup::changeDenoise(int value)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setDenoise(value);
}

void DialogCameraSetup::changeExposureTime(int value)
{
    if(!m_update) return;
    m_pIndustryCamera->setExposureTime(value);
}

void DialogCameraSetup::changeWhiteBalanceRed(int value)
{
    if(!m_update) return;
    qDebug() << "setWhiteBalanceRed = " << value;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setWhiteBalanceRed(value);
}

void DialogCameraSetup::changeWhiteBalanceGreen(int value)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setWhiteBalanceGreen(value);
}

void DialogCameraSetup::changeWhiteBalanceBlue(int value)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setWhiteBalanceBlue(value);
}

void DialogCameraSetup::changeWhiteBalanceTemperature(int t)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setWhiteBalanceMode(CameraImageProcessing::WhiteBalanceManual);
    p->setManualWhiteBalance(t);
    qDebug() << "changeWhiteBalanceTemperature";
}

void DialogCameraSetup::changeAutoGain(bool on)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    if(on)
    {
        p->setGainMode(CameraImageProcessing::GainAutoContinuous);
        return ;
    }
    p->setGainMode(CameraImageProcessing::GainManual);
}

void DialogCameraSetup::changeAutoExposure(bool on)
{
    if(!m_update) return;  
    if(on)
    {
        m_pIndustryCamera->setExposureTimeMode(IndustryCamera::CONTINUOUS);
        enableExposureTimeUI(false);
    }
    else
    {
        m_pIndustryCamera->setExposureTimeMode(IndustryCamera::OFF);
        double value;
        m_update = false;
        m_pIndustryCamera->exposureTime(value);
        setExposureTime(value);
        enableExposureTimeUI(true);
        m_update = true;
    }
}

void DialogCameraSetup::changeAutoWhiteBalance(bool on)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    if(on)
    {
        p->setWhiteBalanceMode(CameraImageProcessing::WhiteBalanceAutoContinuous);
        // 开启自动白平衡后就不能获取白平衡数据了
        ui->horizontalSliderBlue->setEnabled(false);
        ui->horizontalSliderGreen->setEnabled(false);
        ui->horizontalSliderRed->setEnabled(false);

        ui->spinBoxRed->setEnabled(false);
        ui->spinBoxGreen->setEnabled(false);
        ui->spinBoxBlue->setEnabled(false);

        ui->spinBoxTemp->setEnabled(false);
        ui->horizontalSliderTemp->setEnabled(false);

        ui->pushButtonWhiteBalanceOnePush->setEnabled(false);
    }
    else
    {
        p->setWhiteBalanceMode(CameraImageProcessing::WhiteBalanceManual);
        m_update = false;
        int nMin, nMax, nValue;
        on = p->whiteBalanceRedRange(nMin, nMax, nValue);
        if(on)
        {
            ui->spinBoxRed->setRange(nMin, nMax);
            ui->horizontalSliderRed->setRange(nMin, nMax);
            ui->spinBoxRed->setValue(nValue);
        }
        ui->spinBoxRed->setEnabled(on);
        ui->horizontalSliderRed->setEnabled(on);
        on = p->whiteBalanceGreenRange(nMin, nMax, nValue);
        if(on)
        {
            ui->spinBoxGreen->setRange(nMin, nMax);
            ui->horizontalSliderGreen->setRange(nMin, nMax);
            ui->spinBoxGreen->setValue(nValue);
        }
        ui->spinBoxGreen->setEnabled(on);
        ui->horizontalSliderGreen->setEnabled(on);
        on = p->whiteBalanceBlueRange(nMin, nMax, nValue);
        if(on)
        {
            ui->spinBoxBlue->setRange(nMin, nMax);
            ui->horizontalSliderBlue->setRange(nMin, nMax);
            ui->spinBoxBlue->setValue(nValue);
        }
        ui->spinBoxBlue->setEnabled(on);
        ui->horizontalSliderBlue->setEnabled(on);

        ui->pushButtonWhiteBalanceOnePush->setEnabled(true);
        double t;
        on = p->manualWhiteBalance(t);
        if(on)
        {
            ui->spinBoxTemp->setEnabled(true);
            ui->horizontalSliderTemp->setEnabled(true);
            ui->spinBoxTemp->setValue(t);
        }
        m_update = true;
    }
}

void DialogCameraSetup::changeHue(int value)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setHue(value);
}

void DialogCameraSetup::changeSaturation(int value)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setSaturation(value / 100.0);
}

void DialogCameraSetup::changeBrightness(int value)
{
    if(!m_update) return;
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setBrightness(value / 100.0);
}

void DialogCameraSetup::changeHueEnable(bool on)
{
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    if( p->setHueEnable(on) )
    {
        ui->horizontalSliderHue->setEnabled(on);
        ui->spinBoxHue->setEnabled(on);
    }
    if(!on) return;

    double value;
    if(p->hue(value))
    {
        setHue(value);
    }
}

void DialogCameraSetup::changeSaturationEnable(bool on)
{
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    if( p->setSaturationEnable(on) )
    {
        ui->horizontalSliderSaturation->setEnabled(on);
        ui->spinBoxSaturation->setEnabled(on);
    }
    if(!on) return;

    double value;
    if(p->saturation(value))
    {
        setSaturation(value);
    }
}

void DialogCameraSetup::changeSharpnessEnable(bool on)
{
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    if( p->setSharpnessEnable(on) )
    {
        ui->horizontalSliderSharpness->setEnabled(on);
        ui->spinBoxSharpness->setEnabled(on);
    }
    else
    {
        ui->horizontalSliderSharpness->setEnabled(false);
        ui->spinBoxSharpness->setEnabled(false);
    }
    if(!on) return;

    double value;
    if(p->sharpness(value))
    {
        setSharpness(value);
    }
}

void DialogCameraSetup::changeGammaEnable(bool on)
{
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    if( p->setGammaEnable(on) )
    {
        ui->horizontalSliderGamma->setEnabled(on);
        ui->spinBoxGamma->setEnabled(on);
    }
    else
    {
        ui->horizontalSliderGamma->setEnabled(false);
        ui->spinBoxGamma->setEnabled(false);
    }
    if(!on) return;

    double min, max, value;
    if(p->gammaRange(min, max, value))
    {
        setGammaRange(min, max);
        setGamma(value);
    }
}

void DialogCameraSetup::changeDenoiseEnable(bool on)
{
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    if( p->setDenoiseEnable(on) )
    {
        ui->horizontalSliderDenoise->setEnabled(on);
        ui->spinBoxDenoise->setEnabled(on);
    }
    if(!on) return;

    double value;
    if(p->denoise(value))
    {
        setDenoise(value);
    }
}

void DialogCameraSetup::onWhiteBalanceOnePush()
{
    qDebug() << "onWhiteBalanceOnePush()";
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    p->setWhiteBalanceMode(CameraImageProcessing::WhiteBalanceAutoOnePush);
    QTimer::singleShot(500, this, SLOT(updateWhiteBalanceValue()));
}

void DialogCameraSetup::onExposureOnePush()
{
    qDebug() << "onExposureOnePush()";
    m_pIndustryCamera->setExposureTimeMode(IndustryCamera::ONCE);
    QTimer::singleShot(1000, this, SLOT(updateAutoExposureValue()));
}

void DialogCameraSetup::setContrastRange(double min, double max)
{
    ui->horizontalSliderContrast->setRange(min * 100, max * 100);
    ui->spinBoxContrast->setRange(min * 100, max * 100);
}

void DialogCameraSetup::setContrast(double value)
{
    ui->horizontalSliderContrast->setValue(value * 100);
}

void DialogCameraSetup::setGammaRange(double min, double max)
{
    ui->horizontalSliderGamma->setRange(min * 10, max * 10);
    ui->spinBoxGamma->setRange(min * 10, max * 10);
}

void DialogCameraSetup::setGamma(double value)
{
    ui->horizontalSliderGamma->setValue(value * 10);
}

void DialogCameraSetup::setBrightnessRange(double min, double max)
{
    ui->horizontalSliderBrightness->setRange(min * 100, max * 100);
    ui->spinBoxBrightness->setRange(min * 100, max * 100);
}

void DialogCameraSetup::setBrightness(double value)
{
    ui->horizontalSliderBrightness->setValue(value * 100);
}

void DialogCameraSetup::setGainRange(double min, double max)
{
    ui->horizontalSliderGain->setRange(min * 10, max * 10);
    ui->spinBoxGain->setRange(min * 10, max * 10);
}

void DialogCameraSetup::setGain(double value)
{
    ui->horizontalSliderGain->setValue(value * 10);
}

void DialogCameraSetup::setBlackLevelRange(double min, double max)
{
    ui->spinBoxBlacklevel->setRange(min * 100, max * 100);
    ui->horizontalSliderBlacklevel->setRange(min * 100, max * 100);
}

void DialogCameraSetup::setBlackLevel(double value)
{
    ui->spinBoxBlacklevel->setValue(value * 100);
}

void DialogCameraSetup::setHueRange(double min, double max)
{
    ui->spinBoxHue->setRange(min * 100, max * 100);
    ui->horizontalSliderHue->setRange(min * 100, max * 100);
}

void DialogCameraSetup::setHue(double value)
{
    ui->spinBoxHue->setValue(value * 100);
}

void DialogCameraSetup::setSaturationRange(double min, double max)
{
    ui->spinBoxSaturation->setRange(min * 100, max * 100);
    ui->horizontalSliderSaturation->setRange(min * 100, max * 100);
}

void DialogCameraSetup::setSaturation(double value)
{
    ui->spinBoxSaturation->setValue(value * 100);
}

void DialogCameraSetup::setSharpnessRange(double min, double max)
{
    ui->spinBoxSharpness->setRange(min * 100, max * 100);
    ui->horizontalSliderSharpness->setRange(min * 100, max * 100);
}

void DialogCameraSetup::setSharpness(double value)
{
    ui->spinBoxSharpness->setValue(value * 100);
}

void DialogCameraSetup::setDenoiseRange(double min, double max)
{
    ui->spinBoxDenoise->setRange(min * 100, max * 100);
    ui->horizontalSliderDenoise->setRange(min * 100, max * 100);
}

void DialogCameraSetup::setDenoise(double value)
{
    ui->spinBoxDenoise->setValue(value * 100);
}

void DialogCameraSetup::enableBrightnessUI(bool on)
{
    ui->spinBoxBrightness->setEnabled(on);
    ui->horizontalSliderBrightness->setEnabled(on);
}

void DialogCameraSetup::enableContrastUI(bool on)
{
    ui->spinBoxContrast->setEnabled(on);
    ui->horizontalSliderContrast->setEnabled(on);
}

void DialogCameraSetup::enableBlackLevelUI(bool on)
{
    ui->spinBoxBlacklevel->setEnabled(on);
    ui->horizontalSliderBlacklevel->setEnabled(on);
}

void DialogCameraSetup::enableGainUI(bool on)
{
    ui->spinBoxGain->setEnabled(on);
    ui->horizontalSliderGain->setEnabled(on);
}

void DialogCameraSetup::enableExposureTimeUI(bool on)
{
    ui->spinBoxExposureTime->setEnabled(on);
    ui->horizontalSliderExposureTime->setEnabled(on);
}

void DialogCameraSetup::enableAutoExposureUI(bool on)
{
    ui->spinBoxAutoExposureMin->setEnabled(on);
    ui->spinBoxAutoExposureMax->setEnabled(on);
    ui->checkBoxAutoExposure->setEnabled(on);
    ui->pushButtonExposureOnePush->setEnabled(on);
}

void DialogCameraSetup::enableGammaUI(bool on)
{
    ui->spinBoxGamma->setEnabled(on);
    ui->horizontalSliderGamma->setEnabled(on);
}

void DialogCameraSetup::enableHueUI(bool on)
{
    ui->spinBoxHue->setEnabled(on);
    ui->horizontalSliderHue->setEnabled(on);
}

void DialogCameraSetup::enableSaturationUI(bool on)
{
    ui->spinBoxSaturation->setEnabled(on);
    ui->horizontalSliderSaturation->setEnabled(on);
}

void DialogCameraSetup::enableSharpnessUI(bool on)
{
    ui->spinBoxSharpness->setEnabled(on);
    ui->horizontalSliderSharpness->setEnabled(on);
}

void DialogCameraSetup::enableDenoiseUI(bool on)
{
    ui->spinBoxDenoise->setEnabled(on);
    ui->horizontalSliderDenoise->setEnabled(on);
}

void DialogCameraSetup::enableAutoROIUI(bool on)
{
    ui->horizontalSliderLeft->setEnabled(on);
    ui->horizontalSliderTop->setEnabled(on);
    ui->horizontalSliderHeight->setEnabled(on);
    ui->horizontalSliderWidth->setEnabled(on);
}

void DialogCameraSetup::initExposureTab()
{
    connect(ui->spinBoxBrightness,SIGNAL(valueChanged(int)), ui->horizontalSliderBrightness, SLOT(setValue(int)));
    connect(ui->horizontalSliderBrightness, SIGNAL(valueChanged(int)), ui->spinBoxBrightness, SLOT(setValue(int)));

    connect(ui->spinBoxContrast,SIGNAL(valueChanged(int)), ui->horizontalSliderContrast, SLOT(setValue(int)));
    connect(ui->horizontalSliderContrast, SIGNAL(valueChanged(int)), ui->spinBoxContrast, SLOT(setValue(int)));

    connect(ui->spinBoxBlacklevel,SIGNAL(valueChanged(int)), ui->horizontalSliderBlacklevel, SLOT(setValue(int)));
    connect(ui->horizontalSliderBlacklevel, SIGNAL(valueChanged(int)), ui->spinBoxBlacklevel, SLOT(setValue(int)));

    connect(ui->spinBoxExposureTime,SIGNAL(valueChanged(int)), ui->horizontalSliderExposureTime, SLOT(setValue(int)));
    connect(ui->horizontalSliderExposureTime, SIGNAL(valueChanged(int)), ui->spinBoxExposureTime, SLOT(setValue(int)));

    connect(ui->spinBoxGain,SIGNAL(valueChanged(int)), ui->horizontalSliderGain, SLOT(setValue(int)));
    connect(ui->horizontalSliderGain, SIGNAL(valueChanged(int)), ui->spinBoxGain, SLOT(setValue(int)));

}

void DialogCameraSetup::initImageTab()
{
    connect(ui->spinBoxSharpness,SIGNAL(valueChanged(int)), ui->horizontalSliderSharpness, SLOT(setValue(int)));
    connect(ui->horizontalSliderSharpness, SIGNAL(valueChanged(int)), ui->spinBoxSharpness, SLOT(setValue(int)));

    connect(ui->spinBoxGamma,SIGNAL(valueChanged(int)), ui->horizontalSliderGamma, SLOT(setValue(int)));
    connect(ui->horizontalSliderGamma, SIGNAL(valueChanged(int)), ui->spinBoxGamma, SLOT(setValue(int)));

    connect(ui->spinBoxDenoise,SIGNAL(valueChanged(int)), ui->horizontalSliderDenoise, SLOT(setValue(int)));
    connect(ui->horizontalSliderDenoise, SIGNAL(valueChanged(int)), ui->spinBoxDenoise, SLOT(setValue(int)));

    connect(ui->horizontalSliderTemp,SIGNAL(valueChanged(int)), ui->spinBoxTemp, SLOT(setValue(int)));
    connect(ui->spinBoxTemp,SIGNAL(valueChanged(int)), ui->horizontalSliderTemp, SLOT(setValue(int)));
}

void DialogCameraSetup::initROITab()
{
    connect(ui->spinBoxLeft,SIGNAL(valueChanged(int)), ui->horizontalSliderLeft, SLOT(setValue(int)));
    connect(ui->horizontalSliderLeft, SIGNAL(valueChanged(int)), ui->spinBoxLeft, SLOT(setValue(int)));

    connect(ui->spinBoxTop,SIGNAL(valueChanged(int)), ui->horizontalSliderTop, SLOT(setValue(int)));
    connect(ui->horizontalSliderTop, SIGNAL(valueChanged(int)), ui->spinBoxTop, SLOT(setValue(int)));

    connect(ui->spinBoxWidth,SIGNAL(valueChanged(int)), ui->horizontalSliderWidth, SLOT(setValue(int)));
    connect(ui->horizontalSliderWidth, SIGNAL(valueChanged(int)), ui->spinBoxWidth, SLOT(setValue(int)));

    connect(ui->spinBoxHeight,SIGNAL(valueChanged(int)), ui->horizontalSliderHeight, SLOT(setValue(int)));
    connect(ui->horizontalSliderHeight, SIGNAL(valueChanged(int)), ui->spinBoxHeight, SLOT(setValue(int)));
}

void DialogCameraSetup::initColorTab()
{
    connect(ui->spinBoxHue,SIGNAL(valueChanged(int)), ui->horizontalSliderHue, SLOT(setValue(int)));
    connect(ui->horizontalSliderHue, SIGNAL(valueChanged(int)), ui->spinBoxHue, SLOT(setValue(int)));

    connect(ui->spinBoxSaturation,SIGNAL(valueChanged(int)), ui->horizontalSliderSaturation, SLOT(setValue(int)));
    connect(ui->horizontalSliderSaturation, SIGNAL(valueChanged(int)), ui->spinBoxSaturation, SLOT(setValue(int)));

    connect(ui->spinBoxRed,SIGNAL(valueChanged(int)), ui->horizontalSliderRed, SLOT(setValue(int)));
    connect(ui->horizontalSliderRed, SIGNAL(valueChanged(int)), ui->spinBoxRed, SLOT(setValue(int)));

    connect(ui->spinBoxGreen,SIGNAL(valueChanged(int)), ui->horizontalSliderGreen, SLOT(setValue(int)));
    connect(ui->horizontalSliderGreen, SIGNAL(valueChanged(int)), ui->spinBoxGreen, SLOT(setValue(int)));

    connect(ui->spinBoxBlue,SIGNAL(valueChanged(int)), ui->horizontalSliderBlue, SLOT(setValue(int)));
    connect(ui->horizontalSliderBlue, SIGNAL(valueChanged(int)), ui->spinBoxBlue, SLOT(setValue(int)));
}


void DialogCameraSetup::initNodeInfo()
{
    CameraImageProcessing * p = m_pIndustryCamera->imageProcessing();
    m_update = false;
    setWindowTitle(m_pIndustryCamera->deviceName());
    double value;
    bool on = p->brightness(value);
    if(on )
    {
        setBrightnessRange(-1, 1);
        setBrightness(value);
    }
    enableBrightnessUI(on);

    on = p->contrast(value);
    if(on )
    {
        setContrastRange(-1, 1);
        setContrast(value);
    }
    enableContrastUI(on);

    on = p->blackLevel(value);
    if(on)
    {
        setBlackLevelRange(-1, 1);
        setBlackLevel(value);
    }
    enableBlackLevelUI(on);

    double dMin, dMax, dValue;
    on = p->gainRange(dMin, dMax, dValue);
    if(on)
    {
        setGainRange(dMin, dMax);
        setGain(dValue);
    }
    enableGainUI(on);
    on = m_pIndustryCamera->exposureTimeRange(dMin, dMax, dValue);
    if(on)
    {
        if(dMax > 1000000) dMax = 1000000;
        if(dMin < 10) dMin = 10;
        setExposureTimeRange(dMin, dMax);
        setExposureTime(dValue);
    }

    on = p->blackLevel(value);
    if(on)
    {
        setBlackLevelRange(-1, 1);
        setBlackLevel(value);
    }
    enableBlackLevelUI(on);
    IndustryCamera::AutoExposureMode mode;
    on = m_pIndustryCamera->exposureTimeMode(mode);
    enableAutoExposureUI(on);
    if(on)
    {
        int nMin, nMax, nValue;
        on = m_pIndustryCamera->autoExposureTimeLowerLimitRange(nMin, nMax, nValue);
        if(on)
        {
            setAutoExposureTimeLowerLimitRange(nMin, nMax);
            setAutoExposureTimeLowerLimit(nValue);
        }
        on = m_pIndustryCamera->autoExposureTimeUpperLimitRange(nMin, nMax, nValue);
        if(on)
        {
            setAutoExposureTimeUpperLimitRange(nMin, nMax);
            setAutoExposureTimeUpperLimit(nValue);
        }
        if(mode == IndustryCamera::OFF)
        {
            enableExposureTimeUI(true);
            ui->checkBoxAutoExposure->setChecked(false);
        }
        else if(mode == IndustryCamera::CONTINUOUS)
        {
            enableExposureTimeUI(false);
            ui->checkBoxAutoExposure->setChecked(true);
        }
    }

//----------------------------------------第二页-------------------------------------------

    initHueUI();
    initSaturationUI();
    initWhiteBalanceUI();

//--------------------------第三页----------------------------------

    initSharpnessUI();
    initDenoiseUI();
    initGammaUI();

    ui->checkBoxHFlip->setEnabled(false);// 海康相机 SDK 现阶段不支持图像翻转
    ui->checkBoxVFlip->setEnabled(false);

    //--------------------------第四页---------------------------
    // 暂时还没写 ROI 相关的功能，先禁用
    ui->checkBoxAutoFunctionROIEnable->setEnabled(false);
    ui->comboBoxAutoFunctionPreset->setEnabled(false);

    ui->horizontalSliderLeft->setEnabled(false);
    ui->spinBoxLeft->setEnabled(false);
    ui->horizontalSliderTop->setEnabled(false);
    ui->spinBoxTop->setEnabled(false);
    ui->horizontalSliderHeight->setEnabled(false);
    ui->spinBoxHeight->setEnabled(false);
    ui->horizontalSliderWidth->setEnabled(false);
    ui->spinBoxWidth->setEnabled(false);

    m_update = true;
}


DialogCameraSetup::~DialogCameraSetup()
{
    delete ui;
}

}
