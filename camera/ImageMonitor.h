#ifndef VIDEOIMAGEMONITOR_H
#define VIDEOIMAGEMONITOR_H

#include <QObject>
#include <QPainter>

namespace Qly
{
class ImageMonitor : public QObject
{
    Q_OBJECT
public:
    explicit ImageMonitor(QObject *parent = 0);
    ~ImageMonitor();
    /**
     * @brief imageChanged 每次图像发生变化是调用
     * @param image
     */
    virtual void imageChanged(QImage &image) = 0;
    /**
     * @brief paintHook 在窗口坐标系中绘制些内容。
     * @param painter
     * @param zoom_factor 图像坐标系相对窗口坐标系的缩放系数。
     */
    virtual void paintHook(QPainter &painter, double zoom_factor) = 0;
//    virtual void drawMark(QPainter &painter) = 0;
signals:

public slots:
protected:
    double m_zoom_factor;
};

} //namespace Qly
#endif // VIDEOIMAGEMONITOR_H
