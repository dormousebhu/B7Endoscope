#ifndef DIALOGCAMERASETUP_H
#define DIALOGCAMERASETUP_H
#include <QDialog>
#include <QPair>

/**
 * 相机辅助类，提供一个相机基本参数调节的对话框。
 * 这个类不直接操作相机，而是通过 signal-slot 方式传递命令传递命令。
 * 因此，这个对话框可以用于任意的相机参数控制。
 */

namespace Ui {
class DialogCameraSetup;
}

namespace Qly {

class IndustryCamera;
class DialogCameraSetup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCameraSetup(IndustryCamera * camera, QWidget *parent = 0);
    ~DialogCameraSetup();

    void initNodeInfo();

private slots:
    void onWhiteBalanceOnePush();
    void onExposureOnePush();

    void changeBrightness(int value);//
    void changeContrast(int value);//
    void changeBlackLevel(int value);//
    void changeGain(int value);//

    void changeExposureTime(int value);//

    void changeHue(int value);//
    void changeSaturation(int value);//

    void changeWhiteBalanceRed(int value);//
    void changeWhiteBalanceGreen(int value);//
    void changeWhiteBalanceBlue(int value);//

    void changeWhiteBalanceTemperature(int t);

    void changeSharpness(int value);//
    void changeGamma(int value);//
    void changeDenoise(int value);//

    void changeAutoGain(bool on);//
    void changeAutoExposure(bool on);//
    void changeAutoWhiteBalance(bool on);//

    void changeHueEnable(bool on);//
    void changeSaturationEnable(bool on);//
    void changeSharpnessEnable(bool on);//
    void changeGammaEnable(bool on);//
    void changeDenoiseEnable(bool on);//
private:
    void initExposureTab();//
    void initImageTab();//
    void initROITab();//
    void initColorTab();//

    void initHueUI();//
    void initSaturationUI();//
    void initSharpnessUI();//
    void initDenoiseUI();//
    void initGammaUI();//
    void initWhiteBalanceUI();//

    void setExposureTimeRange(int min, int max);//
    void setExposureTime(int value);//

    void setAutoExposureTimeLowerLimitRange(int min, int max);//
    void setAutoExposureTimeLowerLimit(int value);//

    void setAutoExposureTimeUpperLimitRange(int min, int max);//
    void setAutoExposureTimeUpperLimit(int value);//

    void setContrastRange(double min, double max);//
    void setContrast(double value);//

    void setGammaRange(double min, double max);//
    void setGamma(double value);//

    void setBrightnessRange(double min, double max);//
    void setBrightness(double value);//

    void setGainRange(double min, double max);//
    void setGain(double value);//

    void setBlackLevelRange(double min, double max);//
    void setBlackLevel(double value);//

    void setHueRange(double min, double max);//
    void setHue(double value);//

    void setSaturationRange(double min, double max);//
    void setSaturation(double value);//

    void setDenoiseRange(double min, double max);//
    void setDenoise(double value);//

    void setSharpnessRange(double min, double max);//
    void setSharpness(double value);//

    void enableBrightnessUI(bool on);//
    void enableContrastUI(bool on);//
    void enableBlackLevelUI(bool on);//
    void enableGainUI(bool on);//
    void enableExposureTimeUI(bool on);//
    void enableAutoExposureUI(bool on);//

    void enableHueUI(bool on);//
    void enableSaturationUI(bool on);//

    void enableWhiteBalanceUI(bool on);//

    void enableSharpnessUI(bool on);//
    void enableGammaUI(bool on);//
    void enableDenoiseUI(bool on);//

    void enableAutoROIUI(bool on);//
private:
    Ui::DialogCameraSetup *ui;
    bool m_update = false;

    IndustryCamera *m_pIndustryCamera = nullptr;
};

}


#endif // DIALOGCAMERASETUP_H
