#include "IndustryCamera.h"

namespace Qly {


CameraImageProcessing::~CameraImageProcessing()
{

}

bool CameraImageProcessing::manualWhiteBalance(double &colorTemperature)
{
    Q_UNUSED(colorTemperature);
    return false;
}
bool CameraImageProcessing::setManualWhiteBalance(double colorTemperature)
{
    Q_UNUSED(colorTemperature);
    return false;
}

bool CameraImageProcessing::whiteBalanceMode(WhiteBalanceMode &mode)
{
    Q_UNUSED(mode);
    return false;
}

bool CameraImageProcessing::setWhiteBalance(int red, int green, int blue)
{
    Q_UNUSED(red);
    Q_UNUSED(green);
    Q_UNUSED(blue);
    return false;
}
bool CameraImageProcessing::whiteBalanceRedRange(int &nMin, int &nMax, int &nValue)
{
    Q_UNUSED(nMin);
    Q_UNUSED(nMax);
    Q_UNUSED(nValue);
    return false;
}

bool CameraImageProcessing::whiteBalanceGreenRange(int &nMin, int &nMax, int &nValue)
{
    Q_UNUSED(nMin);
    Q_UNUSED(nMax);
    Q_UNUSED(nValue);
    return false;
}

bool CameraImageProcessing::whiteBalanceBlueRange(int &nMin, int &nMax, int &nValue)
{
    Q_UNUSED(nMin);
    Q_UNUSED(nMax);
    Q_UNUSED(nValue);
    return false;
}

bool CameraImageProcessing::setWhiteBalanceRed(int red)
{
    Q_UNUSED(red);
    return false;
}

bool CameraImageProcessing::setWhiteBalanceGreen(int green)
{
    Q_UNUSED(green);
    return false;
}

bool CameraImageProcessing::setWhiteBalanceBlue(int blue)
{
    Q_UNUSED(blue);
    return false;
}

bool CameraImageProcessing::whiteBalance(int &red, int &green, int &blue)
{
    Q_UNUSED(red);
    Q_UNUSED(green);
    Q_UNUSED(blue);
    return false;
}

bool CameraImageProcessing::setWhiteBalanceMode(WhiteBalanceMode mode)
{
    Q_UNUSED(mode);
    return false;
}

bool CameraImageProcessing::setBrightness(double value)
{
    Q_UNUSED(value);
    return false;
}
bool CameraImageProcessing::brightness(double &value)
{
    value = 0;
    return false;
}

bool CameraImageProcessing::brightnessEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setBrightnessEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::gammaEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setGammaEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}
bool CameraImageProcessing::gammaRange(double &min, double &max, double &value)
{
    min = 0;
    max = 10;
    value = 1;
    return false;
}

bool CameraImageProcessing::setGamma(double v)
{
    Q_UNUSED(v);
    return false;
}

bool CameraImageProcessing::gamma(double &v)
{
    Q_UNUSED(v);
    return false;
}


bool CameraImageProcessing::setGain(double fValue)
{
    Q_UNUSED(fValue);
    return false;
}
bool CameraImageProcessing::gain(double &fValue)
{
    Q_UNUSED(fValue);
    return false;
}

bool CameraImageProcessing::gainRange(double &min, double &max, double &value)
{
    min = 0;
    max = 100;
    value = 0;
    return false;
}

bool CameraImageProcessing::setGainMode(GainMode mode)
{
    Q_UNUSED(mode);
    return false;
}

bool CameraImageProcessing::gainMode(GainMode &mode)
{
    Q_UNUSED(mode);
    return false;
}

bool CameraImageProcessing::sharpnessEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setSharpnessEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setSharpness(double value)
{
    Q_UNUSED(value);
    return false;
}

bool CameraImageProcessing::sharpness(double &value)
{
    Q_UNUSED(value);
    return false;
}

bool CameraImageProcessing::blackLevelEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setBlackLevelEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setBlackLevel(double value)
{
    Q_UNUSED(value);
    return false;
}

bool CameraImageProcessing::blackLevel(double &value)
{
    Q_UNUSED(value);
    return false;
}

bool CameraImageProcessing::contrastEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setContrastEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setContrast(double value)
{
    Q_UNUSED(value);
    return false;
}

bool CameraImageProcessing::contrast(double &value)
{
    Q_UNUSED(value);
    return false;
}
bool CameraImageProcessing::saturationEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setSaturationEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setSaturation(double value)
{
    Q_UNUSED(value);
    return false;
}

bool CameraImageProcessing::saturation(double &value)
{
    Q_UNUSED(value);
    return false;
}

bool CameraImageProcessing::denoiseEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setDenoiseEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setDenoise(double value)
{
    Q_UNUSED(value);
    return false;
}

bool CameraImageProcessing::denoise(double &value)
{
    Q_UNUSED(value);
    return false;
}

bool CameraImageProcessing::hueEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setHueEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool CameraImageProcessing::setHue(double value)
{
    Q_UNUSED(value);
    return false;
}
bool CameraImageProcessing::hue(double &value)
{
    Q_UNUSED(value);
    return false;
}

bool CameraImageProcessing::loadWhiteBalanceSettings(QSettings &settings)
{
    WhiteBalanceMode wb_mode = (WhiteBalanceMode)settings.value("AnalogControl/WhiteBalanceMode", false).toInt();
    setWhiteBalanceMode(wb_mode);
    if(wb_mode == WhiteBalanceManual)
    {
//        int nValue = settings.value("AnalogControl/WhiteBalanceRed", -1).toInt();
//        if(nValue != -1) setWhiteBalanceRed(nValue);

//        nValue = settings.value("AnalogControl/WhiteBalanceBlue", -1).toInt();
//        if(nValue != -1) setWhiteBalanceGreen(nValue);

//        nValue = settings.value("AnalogControl/WhiteBalanceGreen", -1).toInt();
//        if(nValue != -1) setWhiteBalanceBlue(nValue);
    }
    return false;
}

bool CameraImageProcessing::writeBalanceSettings(QSettings &settings)
{
    Q_UNUSED(settings);
    return false;
}

bool CameraImageProcessing::loadWhiteGainSettings(QSettings &settings)
{
    GainMode mode = (GainMode) settings.value("AnalogControl/GainMode", 0).toInt();
    setGainMode(mode);
    if(mode == GainManual)
    {
        double value = settings.value("AnalogControl/Gain", 0).toDouble();
        setGain(value);
    }

    return false;
}

bool CameraImageProcessing::writeGainSettings(QSettings &settings)
{
    double fValue;
    GainMode mode;
    if(gainMode(mode))
    {
        settings.setValue("AnalogControl/GainMode", mode);
        if(gain(fValue))
        {
            settings.setValue("AnalogControl/Gain", fValue);
        }
    }

    return false;
}

bool CameraImageProcessing::loadSettings(QSettings &settings)
{

    loadWhiteBalanceSettings(settings);
    loadWhiteGainSettings(settings);

    bool on = settings.value("AnalogControl/BrightnessEnable", false).toBool();
    setBrightnessEnable(on);
    if(on)
    {
        double value = settings.value("AnalogControl/Brightness", 0).toDouble();
        setBrightness(value);
    }

    on = settings.value("AnalogControl/GammaEnable", false).toBool();
    setGammaEnable(on);
    if(on)
    {
        double value = settings.value("AnalogControl/Gamma", 0).toDouble();
        setGamma(value);
    }

    on = settings.value("AnalogControl/SharpnessEnable", false).toBool();
    setSharpnessEnable(on);
    if(on)
    {
        double value = settings.value("AnalogControl/Sharpness", 0).toDouble();
        setSharpness(value);
    }

    on = settings.value("AnalogControl/BlackLevelEnable", false).toBool();
    setBlackLevelEnable(on);
    if(on)
    {
        double value = settings.value("AnalogControl/BlackLevel", 0).toDouble();
        setBlackLevel(value);
    }

    on = settings.value("AnalogControl/ContrastEnable", false).toBool();
    setContrastEnable(on);
    if(on)
    {
        double value = settings.value("AnalogControl/Contrast", 0).toDouble();
        setContrast(value);
    }

    on = settings.value("AnalogControl/SaturationEnable", false).toBool();
    setSaturationEnable(on);
    if(on)
    {
        double value = settings.value("AnalogControl/Saturation", 0).toDouble();
        setSaturation(value);
    }

    on = settings.value("AnalogControl/DenoiseEnable", false).toBool();
    setDenoiseEnable(on);
    if(on)
    {
        double value = settings.value("AnalogControl/Denoise", 0).toDouble();
        setDenoise(value);
    }

    on = settings.value("AnalogControl/HueEnable", false).toBool();
    setHueEnable(on);
    if(on)
    {
        double value = settings.value("AnalogControl/Hue", 0).toDouble();
        setDenoise(value);
    }
    return true;
}

bool CameraImageProcessing::writeSettings(QSettings &settings)
{
    writeBalanceSettings(settings);
    writeGainSettings(settings);

    double fValue;
    if( brightness(fValue))
    {
        settings.setValue("AnalogControl/Brightness", fValue);
    }

    bool bValue = false;

    if(gammaEnable(bValue))
    {
        settings.setValue("AnalogControl/GammaEnable", bValue);
        if(gamma(fValue))
        {
            settings.setValue("AnalogControl/Gamma", fValue);
        }
    }

    if(sharpnessEnable(bValue))
    {
        settings.setValue("AnalogControl/SharpnessEnable", bValue);
        if(sharpness(fValue))
        {
            settings.setValue("AnalogControl/Sharpness", fValue);
        }
    }

    if(blackLevelEnable(bValue))
    {
        settings.setValue("AnalogControl/BlackLevelEnable", bValue);
        if( blackLevel(fValue))
        {
            settings.setValue("AnalogControl/BlackLevel", fValue);
        }
    }

    if(contrastEnable(bValue))
    {
        settings.setValue("AnalogControl/ContrastEnable", bValue);
        if( contrast(fValue))
        {
            settings.setValue("AnalogControl/Contrast", fValue);
        }
    }

    if(saturationEnable(bValue))
    {
        settings.setValue("AnalogControl/SaturationEnable", bValue);
        if(saturation(fValue))
        {
            settings.setValue("AnalogControl/Saturation", fValue);
        }
    }

    if(denoiseEnable(bValue))
    {
        settings.setValue("AnalogControl/DenoiseEnable", bValue);
        if(denoise(fValue))
        {
            settings.setValue("AnalogControl/Denoise", fValue);
        }
    }

    if(hueEnable(bValue))
    {
        settings.setValue("AnalogControl/HueEnable", bValue);
        if(hue(fValue))
        {
            settings.setValue("AnalogControl/Hue", fValue);
        }
    }

    return true;
}

}//namespace Qly
