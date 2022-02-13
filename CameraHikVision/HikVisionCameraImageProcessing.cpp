#include "CameraHikVision.h"

namespace Qly {

inline double scaleFromStandardRange(double min, double max, double value)
{
    return min + value * (max - min) / 2.0;
}

inline double scaleToStandardRange(double min, double max, double value)
{
    return -1.0 + 2.0 *  (value - min) / (max - min);
}

bool HikVisionCameraImageProcessing::setWhiteBalanceMode(WhiteBalanceMode mode)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    int nValue = MV_BALANCEWHITE_AUTO_OFF;
    switch (mode) {
    case WhiteBalanceManual:
        nValue = MV_BALANCEWHITE_AUTO_OFF;
        break;
    case WhiteBalanceAutoOnePush:
        nValue = MV_BALANCEWHITE_AUTO_ONCE;
        break;
    case WhiteBalanceAutoContinuous:
        nValue = MV_BALANCEWHITE_AUTO_CONTINUOUS;
        break;
    }
    int nRet = MV_CC_SetBalanceWhiteAuto(m_hDevHandle, nValue);
    if (MV_OK != nRet)
    {
        qDebug() << "error: SetBalanceWhiteAuto fail";
        return false;
    }
    return true;
}

bool HikVisionCameraImageProcessing::setWhiteBalance(int red, int green, int blue)
{
    setWhiteBalanceRed(red);
    setWhiteBalanceGreen(green);
    return setWhiteBalanceBlue(blue);
}

bool HikVisionCameraImageProcessing::setWhiteBalanceRed(int red)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    int nRet = MV_CC_SetEnumValue(m_hDevHandle, "BalanceRatioSelector", 0);//RED
    if (MV_OK == nRet)
    {
        nRet = MV_CC_SetIntValue(m_hDevHandle, "BalanceRatio", red);
        if (MV_OK != nRet)
        {
            qWarning() << "error: Set BalanceRatio Red fail";
            return false;
        }
    }
    return true;
}

bool HikVisionCameraImageProcessing::setWhiteBalanceGreen(int green)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    int nRet = MV_CC_SetEnumValue(m_hDevHandle, "BalanceRatioSelector", 1);//Green
    if (MV_OK == nRet)
    {
        nRet = MV_CC_SetIntValue(m_hDevHandle, "BalanceRatio", green);
        if (MV_OK != nRet)
        {
            qWarning() << "error: Set BalanceRatio Red fail";
            return false;
        }
    }
    return true;
}

bool HikVisionCameraImageProcessing::setWhiteBalanceBlue(int blue)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    int nRet = MV_CC_SetEnumValue(m_hDevHandle, "BalanceRatioSelector", 2);//Blue
    if (MV_OK == nRet)
    {
        nRet = MV_CC_SetIntValue(m_hDevHandle, "BalanceRatio", blue);
        if (MV_OK != nRet)
        {
            qWarning() << "error: Set BalanceRatio Red fail";
            return false;
        }
    }
    return true;
}

bool HikVisionCameraImageProcessing::whiteBalance(int &red, int &green, int &blue)
{
    int min, max;
    whiteBalanceRedRange(min, max, red);
    whiteBalanceGreenRange(min, max, green);
    return whiteBalanceBlueRange(min, max, blue);
}

bool HikVisionCameraImageProcessing::whiteBalanceBlueRange(int &nMin, int &nMax, int &nValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //Get int parameter
    MVCC_INTVALUE struIntValue = {0};
    int nRet = MV_CC_SetEnumValue(m_hDevHandle, "BalanceRatioSelector", 2);//Blue
    if (MV_OK == nRet)
    {
        nRet = MV_CC_GetIntValue(m_hDevHandle, "BalanceRatio", &struIntValue);
        if (MV_OK == nRet)
        {
            nMin = struIntValue.nMin;
            nMax = struIntValue.nMax;
            nValue = struIntValue.nCurValue;
            qDebug() << "Get BalanceRatio Blue Success";
            return true;
        }
    }
    return false;
}

bool HikVisionCameraImageProcessing::whiteBalanceRedRange(int &nMin, int &nMax, int &nValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //Get int parameter
    MVCC_INTVALUE struIntValue = {0};
    int nRet = MV_CC_SetEnumValue(m_hDevHandle, "BalanceRatioSelector", 0);//RED
    if (MV_OK == nRet)
    {
        nRet = MV_CC_GetIntValue(m_hDevHandle, "BalanceRatio", &struIntValue);
        if (MV_OK == nRet)
        {
            nMin = struIntValue.nMin;
            nMax = struIntValue.nMax;
            nValue = struIntValue.nCurValue;
            qDebug() << "Get BalanceRatio Red Success";
            return true;
        }
    }
    qWarning() << "Get BalanceRatio Red Fail";
    return false;
}

bool HikVisionCameraImageProcessing::whiteBalanceGreenRange(int &nMin, int &nMax, int &nValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //Get int parameter
    MVCC_INTVALUE struIntValue = {0};
    int nRet = MV_CC_SetEnumValue(m_hDevHandle, "BalanceRatioSelector", 1);//Green
    if (MV_OK == nRet)
    {
        nRet = MV_CC_GetIntValue(m_hDevHandle, "BalanceRatio", &struIntValue);
        if (MV_OK == nRet)
        {
            nMin = struIntValue.nMin;
            nMax = struIntValue.nMax;
            nValue = struIntValue.nCurValue;
            qDebug() << "Get BalanceRatio Green Success";
            return true;
        }
    }
    return false;
}

bool HikVisionCameraImageProcessing::autoWhiteBalance(bool &on)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    MVCC_ENUMVALUE struEnumValue = {0};
    on  = false;
    int nRet = MV_CC_GetEnumValue(m_hDevHandle, "BalanceWhiteAuto", &struEnumValue);
    if (MV_OK == nRet)
    {
        on = (struEnumValue.nCurValue == MV_BALANCEWHITE_AUTO_CONTINUOUS);
        qDebug() << "Get BalanceWhiteAuto Success";
        return true;
    }
    qWarning() << "Get BalanceWhiteAuto Fail";
    return false;
}

bool HikVisionCameraImageProcessing::brightness(double &value)
{
    int min, max, nValue;
    if( brightnessRange(min, max, nValue) )
    {
        value = scaleToStandardRange(min, max, nValue);
        return true;
    }
    return false;
}

bool HikVisionCameraImageProcessing::setBrightness(double value)
{
    //获取相机图像的亮度信息
    MVCC_INTVALUE struIntValue = {0};
    int nRet = MV_CC_GetBrightness(m_hDevHandle, &struIntValue);
    if (MV_OK != nRet)
    {
        qDebug() << "error: GetBrightness fail";
        return false;
    }

    //设置相机图像的亮度
    value = qBound(-1.0, value, 1.0);//设置为最大值
    unsigned int nValue = scaleFromStandardRange(struIntValue.nMin, struIntValue.nMax, value);

    nRet = MV_CC_SetBrightness(m_hDevHandle, nValue);
    if (MV_OK != nRet)
    {
        qDebug() << "error: SetBrightness fail";
        return false;
    }
    return true;
}


bool HikVisionCameraImageProcessing::brightnessRange(int &min, int &max, int &nValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //Get int parameter
    MVCC_INTVALUE struIntValue = {0};

    int nRet = MV_CC_GetIntValue(m_hDevHandle, "Brightness", &struIntValue);
    if (MV_OK != nRet)
    {
        qWarning() << "error: GetBrightness fail";
        return false;
    }
    min = struIntValue.nMin;
    max = struIntValue.nMax;
    nValue = struIntValue.nCurValue;
    return true;
}

bool HikVisionCameraImageProcessing::gammaRange(double &min, double &max, double &fValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //获取相机Gamma值
    MVCC_FLOATVALUE struFloatValue = {0};

    int nRet = MV_CC_GetGamma(m_hDevHandle, &struFloatValue);
    if (MV_OK != nRet)
    {
        qDebug() << "error: GetGamma fail";
        return false;
    }
    min = struFloatValue.fMin;
    max = struFloatValue.fMax;
    fValue = struFloatValue.fCurValue;
    return true;
}

bool HikVisionCameraImageProcessing::gamma(double &v)
{
    double min, max;
    return gammaRange(min, max, v);
}

bool HikVisionCameraImageProcessing::setGamma(double v)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    int nRet = MV_CC_SetFloatValue(m_hDevHandle, "Gamma", v);
    if (MV_OK != nRet)
    {
        qWarning() << "error: Set Gamma fail";
        return false;
    }
    return true;
}

bool HikVisionCameraImageProcessing::gammaEnable(bool &on)
{
    if( m_hDevHandle == nullptr ) return false;
    return MV_CC_GetBoolValue(m_hDevHandle, "GammaEnable", &on);
}

bool HikVisionCameraImageProcessing::setGammaEnable(bool on)
{
    if( m_hDevHandle == nullptr ) return false;
    return MV_CC_SetBoolValue(m_hDevHandle, "GammaEnable", on);
}

bool HikVisionCameraImageProcessing::gainRange(double &fMin, double &fMax, double &fValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //Get int parameter
    MVCC_FLOATVALUE struFloatValue = {0};

    int nRet = MV_CC_GetFloatValue(m_hDevHandle, "Gain", &struFloatValue);
    if (MV_OK != nRet)
    {
        qWarning() << "error: GetGain fail";
        return false;
    }
    fMin = struFloatValue.fMin;
    fMax = struFloatValue.fMax;
    fValue= struFloatValue.fCurValue;
    return true;
}

bool HikVisionCameraImageProcessing::gain(double &fValue)
{
    double min, max;
    return gainRange(min, max, fValue);
}

bool HikVisionCameraImageProcessing::setGain(double fValue)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }

    int nRet = MV_CC_SetFloatValue(m_hDevHandle, "Gain", fValue);
    if (MV_OK != nRet)
    {
        qWarning() << "error: Set Gain fail";
        return false;
    }
    return true;
}

bool HikVisionCameraImageProcessing::setGainMode(GainMode mode)
{
    unsigned int nValue = MV_GAIN_MODE_OFF;
    switch (mode) {
    case GainManual:
        nValue = MV_GAIN_MODE_OFF;
        break;
    case GainAutoOnePush:
        nValue = MV_GAIN_MODE_ONCE;
        break;
    case GainAutoContinuous:
        nValue = MV_GAIN_MODE_CONTINUOUS;
        break;
    }
    int nRet = MV_CC_SetGainMode(m_hDevHandle, nValue);
    if (MV_OK != nRet)
    {
        qDebug() << "error: MV_CC_SetGainMode fail";
        return false;
    }
    return true;
}

bool HikVisionCameraImageProcessing::autoGain(bool &on)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    MVCC_ENUMVALUE struEnumValue = {0};
    on  = false;
    int nRet = MV_CC_GetEnumValue(m_hDevHandle, "GainAuto", &struEnumValue);
    if (MV_OK == nRet)
    {
        on = (struEnumValue.nCurValue == MV_GAIN_MODE_CONTINUOUS);
        qDebug() << "Get GainAuto Success";
        return true;
    }
    qWarning() << "Get GainAuto Fail";
    return false;
}

bool HikVisionCameraImageProcessing::autoGainLowerLimitRange(double &fMin, double &fMax, double &fValue)
{
    MVCC_FLOATVALUE struFloatValue = {0};
    int nRet = MV_CC_GetFloatValue(m_hDevHandle, "AutoGainLowerLimit", &struFloatValue);
    if (MV_OK == nRet)
    {
        fMin = struFloatValue.fMin;
        fMax = struFloatValue.fMax;
        fValue = struFloatValue.fCurValue;
        return true;
    }
    return false;
}

bool HikVisionCameraImageProcessing::autoGainUpperLimitRange(double &fMin, double &fMax, double &fValue)
{
    if( m_hDevHandle == nullptr ) return false;
    MVCC_FLOATVALUE struFloatValue = {0};
    int nRet = MV_CC_GetFloatValue(m_hDevHandle, "AutoGainUpperLimit", &struFloatValue);
    if (MV_OK == nRet)
    {
        fMin = struFloatValue.fMin;
        fMax = struFloatValue.fMax;
        fValue = struFloatValue.fCurValue;
        return true;
    }
    return false;
}

bool HikVisionCameraImageProcessing::autoGainLowerLimit(double &fValue)
{
    double min, max;
    return autoGainLowerLimitRange(min, max, fValue);
}

bool HikVisionCameraImageProcessing::autoGainUpperLimit(double &fValue)
{
    double min, max;
    return autoGainUpperLimitRange(min, max, fValue);
}

bool HikVisionCameraImageProcessing::sharpnessEnable(bool &on)
{
    if( m_hDevHandle == nullptr ) return false;
    bool ret = MV_CC_GetBoolValue(m_hDevHandle, "SharpnessEnable", &on);

    if(ret)
    {
        qDebug() << "Get SharpnessEnable Success, State = " << on;
    }
    else qWarning() << "Get SharpnessEnable Failed!";

    return ret;
}

bool HikVisionCameraImageProcessing::setSharpnessEnable(bool on)
{
    if( m_hDevHandle == nullptr ) return false;
    return MV_CC_SetBoolValue(m_hDevHandle, "SharpnessEnable", on);
}

bool HikVisionCameraImageProcessing::setSharpness(double value)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    int min, max, nValue;
    if( sharpnessRange(min, max, nValue) )
    {
        nValue = scaleFromStandardRange(min, max, value);
        int nRet = MV_CC_SetIntValue(m_hDevHandle, "Sharpness", nValue);
        if (MV_OK != nRet)
        {
            qWarning() << "error: Set Sharpness fail";
            return false;
        }
    }
    return true;
}

bool HikVisionCameraImageProcessing::sharpness(double &value)
{
    int min, max, nValue;
    if( sharpnessRange(min, max, nValue) )
    {
        value = scaleToStandardRange(min, max, nValue);
        return true;
    }
    return false;
}

bool HikVisionCameraImageProcessing::sharpnessRange(int &min, int &max, int &nValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //Get int parameter
    MVCC_INTVALUE struIntValue = {0};

    int nRet = MV_CC_GetIntValue(m_hDevHandle, "Sharpness", &struIntValue);
    if (MV_OK != nRet)
    {
        qWarning() << "error: GetSharpness fail";
        return false;
    }
    min = struIntValue.nMin;
    max = struIntValue.nMax;
    nValue = struIntValue.nCurValue;
    return true;
}

bool HikVisionCameraImageProcessing::blackLevel(double &value)
{
    int min, max, nValue;
    if( blackLevelRange(min, max, nValue) )
    {
        value = scaleToStandardRange(min, max, nValue);
        return true;
    }
    return false;
}

bool HikVisionCameraImageProcessing::setBlackLevel(double value)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    int min, max, nValue;
    if( blackLevelRange(min, max, nValue) )
    {
        nValue = scaleFromStandardRange(min, max, value);
        int nRet = MV_CC_SetIntValue(m_hDevHandle, "BlackLevel", nValue);
        if (MV_OK != nRet)
        {
            qWarning() << "error: Set BlackLevel fail";
            return false;
        }
    }
    return true;
}

bool HikVisionCameraImageProcessing::blackLevelRange(int &nMin, int &nMax, int &nValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //Get int parameter
    MVCC_INTVALUE struIntValue = {0};

    int nRet = MV_CC_GetIntValue(m_hDevHandle, "BlackLevel", &struIntValue);
    if (MV_OK != nRet)
    {
        qWarning() << "error: GetBlackLevel fail";
        return false;
    }
    nMin = struIntValue.nMin;
    nMax = struIntValue.nMax;
    nValue = struIntValue.nCurValue;
    return true;
}

bool HikVisionCameraImageProcessing::contrastRange(int &nMin, int &nMax, int &nValue)
{
    // 海康相机暂时不支持 Contrast 参数
    nMin = -1;
    nMax = 1;
    nValue = 0;
    return false;
}

bool HikVisionCameraImageProcessing::contrast(double &value)
{
    int min, max, nValue;
    if( contrastRange(min, max, nValue) )
    {
        value = scaleToStandardRange(min, max, nValue);
        return true;
    }
    return false;
}

bool HikVisionCameraImageProcessing::setContrast(double value)
{
    Q_UNUSED(value);
    return false;
}

bool HikVisionCameraImageProcessing::saturation(double &value)
{
    int min, max, nValue;
    if( saturationRange(min, max, nValue) )
    {
        value = scaleToStandardRange(min, max, nValue);
        return true;
    }
    return false;
}

bool HikVisionCameraImageProcessing::setSaturation(double value)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    int min, max, nValue;
    if( saturationRange(min, max, nValue) )
    {
        nValue = scaleFromStandardRange(min, max, value);
        int nRet = MV_CC_SetIntValue(m_hDevHandle, "Saturation", nValue);
        if (MV_OK != nRet)
        {
            qWarning() << "error: Set Saturation fail";
            return false;
        }
    }
    return true;
}

bool HikVisionCameraImageProcessing::saturationRange(int &nMin, int &nMax, int &nValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //Get int parameter
    MVCC_INTVALUE struIntValue = {0};

    int nRet = MV_CC_GetIntValue(m_hDevHandle, "Saturation", &struIntValue);
    if (MV_OK != nRet)
    {
        qWarning() << "error: GetSaturation fail";
        return false;
    }
    nMin = struIntValue.nMin;
    nMax = struIntValue.nMax;
    nValue = struIntValue.nCurValue;
    return true;
}

bool HikVisionCameraImageProcessing::setSaturationEnable(bool on)
{
    if( m_hDevHandle == nullptr ) return false;
    return MV_CC_SetBoolValue(m_hDevHandle, "SaturationEnable", on);
}

bool HikVisionCameraImageProcessing::saturationEnable(bool &on)
{
    if( m_hDevHandle == nullptr ) return false;
    return MV_CC_GetBoolValue(m_hDevHandle, "SaturationEnable", &on);
}

bool HikVisionCameraImageProcessing::denoiseEnable(bool &on)
{
    if( m_hDevHandle == nullptr ) return false;
    bool ret = MV_CC_GetBoolValue(m_hDevHandle, "NoiseReductionEnable", &on);

    if(ret)
    {
        qDebug() << "Get NoiseReductionEnable Success, State = " << on;
    }
    else qWarning() << "Get NoiseReductionEnable Failed!";

    return ret;
}

bool HikVisionCameraImageProcessing::setDenoiseEnable(bool on)
{
    if( m_hDevHandle == nullptr ) return false;
    return MV_CC_SetBoolValue(m_hDevHandle, "NoiseReductionEnable", on);
}

bool HikVisionCameraImageProcessing::denoise(double &value)
{
    int min, max, nValue;
    if( denoiseRange(min, max, nValue) )
    {
        value = scaleToStandardRange(min, max, nValue);
        return true;
    }
    return false;
}

bool HikVisionCameraImageProcessing::setDenoise(double value)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    int min, max, nValue;
    if( denoiseRange(min, max, nValue) )
    {
        nValue = scaleFromStandardRange(min, max, value);
        int nRet = MV_CC_SetIntValue(m_hDevHandle, "NoiseReduction", nValue);
        if (MV_OK != nRet)
        {
            qWarning() << "error: Set NoiseReduction fail";
            return false;
        }
    }
    return true;
}

bool HikVisionCameraImageProcessing::denoiseRange(int &nMin, int &nMax, int &nValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //Get int parameter
    MVCC_INTVALUE struIntValue = {0};

    int nRet = MV_CC_GetIntValue(m_hDevHandle, "NoiseReduction", &struIntValue);
    if (MV_OK != nRet)
    {
        qWarning() << "error: GetNoiseReduction fail";
        return false;
    }
    nMin = struIntValue.nMin;
    nMax = struIntValue.nMax;
    nValue = struIntValue.nCurValue;
    return true;
}

bool HikVisionCameraImageProcessing::hueEnable(bool &on)
{
    if( m_hDevHandle == nullptr ) return false;
    bool ret = MV_CC_GetBoolValue(m_hDevHandle, "HueEnable", &on);

    if(ret)
    {
        qDebug() << "Get HueEnable Success, State = " << on;
    }
    else qWarning() << "Get HueEnable Failed!";

    return ret;
}

bool HikVisionCameraImageProcessing::setHueEnable(bool on)
{
    if( m_hDevHandle == nullptr ) return false;
    return MV_CC_SetBoolValue(m_hDevHandle, "HueEnable", on);
}

bool HikVisionCameraImageProcessing::setHue(double value)
{
    if(m_hDevHandle == nullptr)
    {
        return false;
    }
    int min, max, nValue;
    if( hueRange(min, max, nValue) )
    {
        nValue = scaleFromStandardRange(min, max, value);
        int nRet = MV_CC_SetIntValue(m_hDevHandle, "Hue", nValue);
        if (MV_OK != nRet)
        {
            qWarning() << "error: Set Hue fail";
            return false;
        }
    }
    return true;
}

bool HikVisionCameraImageProcessing::hue(double &value)
{
    int min, max, nValue;
    if( hueRange(min, max, nValue) )
    {
        value = scaleToStandardRange(min, max, nValue);
        return true;
    }
    return false;
}

bool HikVisionCameraImageProcessing::hueRange(int &nMin, int &nMax, int &nValue)
{
    if (NULL == m_hDevHandle)
    {
         return false;
    }
    //Get int parameter
    MVCC_INTVALUE struIntValue = {0};

    int nRet = MV_CC_GetIntValue(m_hDevHandle, "Hue", &struIntValue);
    if (MV_OK != nRet)
    {
        qWarning() << "error: GetHue fail";
        return false;
    }
    nMin = struIntValue.nMin;
    nMax = struIntValue.nMax;
    nValue = struIntValue.nCurValue;
    return true;

}


} //namespace Qly
