#include "ImageView.h"
#include "ImageMonitor.h"
#include <QPen>
#include <QColor>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QGuiApplication>

namespace Qly {

const int DEFAULT_IMAGE_SIZE_X = 1280;
const int DEFAULT_IMAGE_SIZE_Y = 720;

void ImageView::loadSettings(QSettings &settings)
{
    m_size = settings.value("ImageView/Size", QSize(DEFAULT_IMAGE_SIZE_X, DEFAULT_IMAGE_SIZE_Y)).toSize();
    bool show = settings.value("ImageView/ShowCross", false).toBool();
    int zoom = settings.value("ImageView/ZoomFactor", 100).toInt();
    double angle = settings.value("ImageView/Angle", 0).toDouble();
    showCross(show);
    setZoomFactorPCT(zoom);
    setRotate(angle);
}
void ImageView::writeSettings(QSettings &settings)
{
    settings.setValue("ImageView/Size", m_size);
    settings.setValue("ImageView/ShowCross", m_show_cross);
    settings.setValue("ImageView/ZoomFactor", m_zoom_factor);
    settings.setValue("ImageView/Angle", m_angle);
}

ImageView::ImageView(QWidget *parent) :
        QWidget(parent),
        m_pImage(nullptr),
        m_show_cross(false),
        m_zoom_factor(100),
        m_angle(0),
        m_size(DEFAULT_IMAGE_SIZE_X, DEFAULT_IMAGE_SIZE_Y),   
        m_mouse_pos_pick(false)

{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize( m_size * m_zoom_factor / 100.0);
    update();
}

void ImageView::setMousePositionPickMode(bool on)
{
    m_mouse_pos_pick = on;
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    if(!m_wheelZoom)
    {
        return QWidget::wheelEvent(event);
    }
    bool isCtrlPressed = QGuiApplication::keyboardModifiers() & Qt::ControlModifier;
    bool isAltPressed = QGuiApplication::keyboardModifiers() & Qt::AltModifier;
    if(isCtrlPressed | isAltPressed)
    {
        event->ignore();
        return;
    }
    int delta = event->angleDelta().y() > 0 ? 1 : -1;

    int factor = m_zoom_factor + delta;
    factor = qBound(1, factor, 1000);
    //qDebug() << "factor = " << factor << ", delta = " << delta;
    setZoomFactorPCT(factor);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "ImageView::mouseMoveEvent";
    QWidget::mouseMoveEvent(event);
    double x = event->pos().x();
    double y = event->pos().y();
    QPointF point(x / m_zoom_factor / 100.0, y / m_zoom_factor / 100.0);
    emit mouseMoved(point);

}

void ImageView::mouseDoubleClickEvent(QMouseEvent *event)
{
//    qDebug() <<  "ImageView::mouseDoubleClickEvent";
    if(m_mouse_pos_pick && event->button() == Qt::RightButton)
    {
        double x = event->pos().x() - width() / 2.0;
        double y = event->pos().y() - height() / 2.0;
        QPointF point(x / m_zoom_factor / 100.0, y / m_zoom_factor / 100.0);
        emit mouseDoubleClicked(point);
    }
    QWidget::mouseDoubleClickEvent(event);
}

ImageView::~ImageView()
{

}

QImage ImageView::getImage()
{
    QImage image;
    if(m_pImage)
    {
        image = m_pImage->copy();
    }
    return image;
}

bool ImageView::setImage(QImage &image)
{
    m_pImage = &image;
    m_size = image.size();
    setFixedSize( m_size * m_zoom_factor / 100.0 );

    update();
    return true;
}

bool ImageView::saveImage( const QString &fileName )
{
    if(m_pImage)
    {
        QImage image = m_pImage->copy();
        return image.save(fileName);
    }
    return false;
}

void ImageView::setZoomFactor(double factor)
{
    int f = factor * 100.0;
    setZoomFactorPCT(f);
}

void ImageView::setZoomFactorPCT(int factor)
{
    factor = qBound(1, factor, 1000);
    //qDebug() << "in setZoomFactorPCT, factor = " << factor;
    if(m_zoom_factor != factor)
    {
        m_zoom_factor = factor;
        //resize(m_size * m_zoom_factor / 100.0);
        setFixedSize(m_size * m_zoom_factor / 100.0);
        update();
        emit zoomFactorChanged(factor / 100.0);
    }
}

void ImageView::setImageMonitor(ImageMonitor *monitor)
{
    if(monitor)
    {
        m_monitor_set.insert(monitor);
    }
}

void ImageView::removeAllImageMonitors()
{
    m_monitor_set.clear();
}

void ImageView::removeImageMonitor(ImageMonitor * monitor)
{
    m_monitor_set.remove(monitor);
}

void ImageView::imageChangedHook()
{
    QSetIterator<ImageMonitor *> iter(m_monitor_set);
    while (iter.hasNext())
    {
        ImageMonitor * p = iter.next();
        p->imageChanged(*m_pImage);
    }
}

void ImageView::paintEventHook(QPainter &painter)
{
    QSetIterator<ImageMonitor *> iter(m_monitor_set);
    while (iter.hasNext())
    {
        ImageMonitor * p = iter.next();
        p->paintHook(painter, m_zoom_factor / 100.0);
    }
}

void ImageView::showCross(bool show)
{
    m_show_cross = show;
    update();
}

inline void ImageView::drawImage(QPainter &painter)
{
    painter.save();
    painter.scale(m_zoom_factor / 100.0, m_zoom_factor / 100.0);
    if(m_pImage)
    {
        painter.drawImage ( 0, 0, *m_pImage );
    }
    else
    {
        //painter.setBrush(QGradient(QGradient::BurningSpring));
        painter.setBrush(QImage(":/res/microscope01.jpg"));
        painter.drawRect(QRect(0, 0, m_size.width(), m_size.height()));
    }
    painter.restore();
}

inline void ImageView::drawCross(QPainter &painter)
{
    int mid_x = size().width() / 2;
    int mid_y = size().height() / 2;
    painter.save();
    painter.translate(mid_x, mid_y);
    painter.rotate(m_angle);
    QPen pen(Qt::red);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(-2 * mid_x, 0, 2 * mid_x, 0);
    painter.drawLine(0, -2 * mid_y, 0, 2 * mid_y);

    painter.restore();
}

void ImageView::paintEvent( QPaintEvent * event )
{
    (void) event;
    QPainter painter(this);
    painter.setRenderHint ( QPainter::Antialiasing, true );
    drawImage(painter);
    paintEventHook(painter);
    if (m_show_cross)
    {
        drawCross(painter);
    }
}


} //namespace Qly
