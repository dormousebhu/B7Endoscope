#ifndef IMAGEDISPLAY_H
#define IMAGEDISPLAY_H
#include <QMutex>
#include <QWidget>
#include <QSize>
#include <QSet>
#include <QSettings>

namespace Qly
{
class ImageMonitor;

class ImageView : public QWidget
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = nullptr);
    ~ImageView();
    virtual void loadSettings(QSettings &settings);
    virtual void writeSettings(QSettings &settings);
public slots:

    bool setImage(QImage &image);
    /**
     * @brief saveImage 将图像保存到文件中
     * @param fileName 带路径的完整文件名
     * @return true 表示保存成功
     */
    bool saveImage(const QString &fileName);

public:
    QSize sizeHint() const Q_DECL_OVERRIDE{return m_size * m_zoom_factor / 100.0;}

    /**
     * @brief getImage 获取图像
     * @return
     */
    QImage getImage();
    /**
     * @brief getImageSize 获得图像的尺寸
     * @return
     */
    QSize imageSize() const {return m_size;}

    /**
     * @brief setImageMonitor 每次更新图像时调用 monitor->imageChanged(QImage &image) 函数。
     * 用于自动对焦等需要实时计算的功能，//deleted考虑到不要对系统造成太大的负担，只能加载一个 monitor（观察者模式）
     * @param monitor
     */
    void setImageMonitor(ImageMonitor *monitor);
    /**
     * @brief removeImageMonitor 移除这个 monitor
     * @param monitor
     */
    void removeImageMonitor(ImageMonitor * monitor);
    /**
     * @brief removeAllImageMonitors 去掉所有的 monitor
     */
    void removeAllImageMonitors();

protected:
    void paintEvent(QPaintEvent *event)  Q_DECL_OVERRIDE;
    void drawImage(QPainter &painter);
    void drawCross(QPainter &painter);
    void mouseDoubleClickEvent(QMouseEvent *event)  Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
signals:
    /**
     * @brief mouseDoubleClicked MousePositionPickMode 模式下，鼠标左键双击击时输出鼠标的位置。
     * @param point 这个点是以中心点为原点的，并且是在图像坐标系中的，不受图像缩放影响。
     */
    void mouseDoubleClicked(QPointF &point);

    /**
     * @brief mouseMoved
     * @param point 返回鼠标在图像上的位置，考虑了图像的放缩
     */
    void mouseMoved(QPointF &point);
    /**
     * @brief distance 输出鼠标距离中心点的距离
     * @param x
     * @param y
     */
    void distance(int x, int y);
    void zoomFactorChanged(double factor);

public slots:

    /**
     * @brief setZoomFactor 设置图像的缩放系数，[0.01, 10.0]
     * @param factor
     */
    void setZoomFactor(double factor);
    /**
     * @brief setZoomFactor 设置图像的缩放系数，[1, 1000]
     * @param factor 以百分比计的缩放系数
     */
    void setZoomFactorPCT(int factor);
    /**
     * @brief showCross 是否在图像中心位置显示一个十字
     * @param show
     */
    void showCross(bool show);

    void setRotate(double angle) {m_angle = angle; update();}
    /**
     * @brief setMousePositionPickMode 是否实时输出鼠标位置，鼠标左键点击时输出
     * @param on
     */
    void setMousePositionPickMode(bool on);

protected:
    QImage * m_pImage;
    bool m_show_cross;
    int m_zoom_factor;
    double m_angle; //十字线的旋转角度

    QSize m_size;
    bool m_mouse_pos_pick;
    bool m_wheelZoom = false;

private:
    void imageChangedHook();
    void paintEventHook(QPainter &painter);
    QSet<ImageMonitor *> m_monitor_set;
};

} //namespace Qly
#endif // IMAGEDISPLAY_H
