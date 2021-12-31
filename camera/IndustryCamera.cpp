#include "IndustryCamera.h"

namespace Qly {

IndustryCamera::IndustryCamera(QObject *parent) : QObject(parent)
{
    m_imageCount = 0;
    qRegisterMetaType<AVPixelFormat>("AVPixelFormat");
}

QString PixelFormat2QString(AVPixelFormat format)
{
    QString str;
    switch (format) {
    case AV_PIX_FMT_NONE :
        str = "NONE";
        break;
    case AV_PIX_FMT_YUV420P:
        str = "YUV420P";
        break;
    case AV_PIX_FMT_YUYV422:
        str = "YUYV422";
        break;
    case AV_PIX_FMT_UYVY422:
        str = "UYVY422";
        break;
    case AV_PIX_FMT_RGB24:
        str = "RGB24";
        break;
    case AV_PIX_FMT_BGR24:
        str = "BGR24";
        break;
    case AV_PIX_FMT_YUV422P:
        str = "YUV422P";
        break;
    case AV_PIX_FMT_YUV444P:
        str = "YUV444P";
        break;
    case AV_PIX_FMT_YUV410P:
        str = "YUV410P";
        break;
    case AV_PIX_FMT_YUV411P:
        str = "YUV411P";
        break;
    case AV_PIX_FMT_GRAY8:
        str = "GRAY8";
        break;
    case AV_PIX_FMT_ARGB:
        str = "ARGB";
        break;
    case AV_PIX_FMT_RGBA:
        str = "RGBA";
        break;
    case AV_PIX_FMT_ABGR:
        str = "ABGR";
        break;
    case AV_PIX_FMT_BGRA:
        str = "BGRA";
        break;
    case AV_PIX_FMT_BAYER_BGGR8:
        str = "BAYER_BGGR8";
        break;
    case AV_PIX_FMT_BAYER_RGGB8:
        str = "BAYER_RGGB8";
        break;
    case AV_PIX_FMT_BAYER_GRBG8:
        str = "BAYER_GRBG8";
        break;
    case AV_PIX_FMT_BAYER_GBRG8:
        str = "BAYER_GBRG8";
        break;
    case AV_PIX_FMT_BAYER_BGGR16LE:
        str = "BAYER_BGGR16LE";
        break;
    case AV_PIX_FMT_BAYER_RGGB16LE:
        str = "BAYER_RGGB16LE";
        break;
    case AV_PIX_FMT_BAYER_GBRG16LE:
        str = "BAYER_GBRG16LE";
        break;
    case AV_PIX_FMT_BAYER_GRBG16LE:
        str = "BAYER_GRBG16LE";
        break;
    default:
        str = "AV_PIX_FMT_UNKNOWN";
        break;
    }
    return str;
}

QDebug operator<<(QDebug dbg, const AVPixelFormat &format)
{
    dbg.nospace() << PixelFormat2QString(format);
    return dbg.maybeSpace();
}

bool IndustryCamera::setPixelType(AVPixelFormat type)
{
    Q_UNUSED(type);
    return false;
}

bool IndustryCamera::gammaEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::setGammaEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::sharpnessEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::setSharpnessEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::blackLevelEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::setBlackLevelEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::contrastEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::setContrastEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::saturationEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::setSaturationEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::denoiseEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::setDenoiseEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::hueEnable(bool &on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::setHueEnable(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::saveSettingsToFile(QString fileName)
{
    Q_UNUSED(fileName);
    return false;
}
bool IndustryCamera::loadSettingsFromFile(QString fileName)
{
    Q_UNUSED(fileName);
    return false;
}

bool IndustryCamera::setAutoBrightness(bool on)
{
    Q_UNUSED(on);
    return false;
}
bool IndustryCamera::setAutoWhiteBalance(bool on)
{
    Q_UNUSED(on);
    return false;
}
bool IndustryCamera::setAutoExpoureTime(bool on)
{
    Q_UNUSED(on);
    return false;
}
bool IndustryCamera::setAutoGain(bool on)
{
    Q_UNUSED(on);
    return false;
}

bool IndustryCamera::setBrightness(int value)
{
    Q_UNUSED(value);
    return false;
}
bool IndustryCamera::brightness(int &nValue)
{
    nValue = 0;
    return false;
}

bool IndustryCamera::setExpoureTime(double us)
{
    Q_UNUSED(us);
    return false;
}
bool IndustryCamera::expoureTime(double &us)
{
    Q_UNUSED(us);
    return false;
}

bool IndustryCamera::setFrameRate(double fps)
{
    Q_UNUSED(fps);
    return false;
}
bool IndustryCamera::frameRate(double &fps)
{
    Q_UNUSED(fps);
    return false;
}

bool IndustryCamera::setWhiteBalance(int red, int green, int blue)
{
    Q_UNUSED(red);
    Q_UNUSED(green);
    Q_UNUSED(blue);
    return false;
}
bool IndustryCamera::whiteBalance(int &red, int &green, int &blue)
{
    Q_UNUSED(red);
    Q_UNUSED(green);
    Q_UNUSED(blue);
    return false;
}

bool IndustryCamera::setGamma(double v)
{
    Q_UNUSED(v);
    return false;
}
bool IndustryCamera::gamma(double &v)
{
    Q_UNUSED(v);
    return false;
}

bool IndustryCamera::setGain(double fValue)
{
    Q_UNUSED(fValue);
    return false;
}
bool IndustryCamera::gain(double &fValue)
{
    Q_UNUSED(fValue);
    return false;
}

bool IndustryCamera::setSharpness(int nValue)
{
    Q_UNUSED(nValue);
    return false;
}
bool IndustryCamera::sharpness(int &nValue)
{
    Q_UNUSED(nValue);
    return false;
}

bool IndustryCamera::setBlackLevel(int nValue)
{
    Q_UNUSED(nValue);
    return false;
}
bool IndustryCamera::blackLevel(int &nValue)
{
    Q_UNUSED(nValue);
    return false;
}

bool IndustryCamera::setContrast(int nValue)
{
    Q_UNUSED(nValue);
    return false;
}
bool IndustryCamera::contrast(int &nValue)
{
    Q_UNUSED(nValue);
    return false;
}

bool IndustryCamera::setSaturation(int nValue)
{
    Q_UNUSED(nValue);
    return false;
}
bool IndustryCamera::saturation(int &nValue)
{
    Q_UNUSED(nValue);
    return false;
}

bool IndustryCamera::setDenoise(int nValue)
{
    Q_UNUSED(nValue);
    return false;
}
bool IndustryCamera::denoise(int &nValue)
{
    Q_UNUSED(nValue);
    return false;
}

bool IndustryCamera::setHue(int nValue)
{
    Q_UNUSED(nValue);
    return false;
}
bool IndustryCamera::hue(int &nValue)
{
    Q_UNUSED(nValue);
    return false;
}

}//namespace Qly
