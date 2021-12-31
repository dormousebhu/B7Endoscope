#ifndef IMAGESAVETOOLBAR_H
#define IMAGESAVETOOLBAR_H
#include <QToolBar>
#include <QAction>

class ImageSaveToolBar : public QToolBar
{
    Q_OBJECT
public:
    ImageSaveToolBar(QWidget *parent = nullptr);
signals:
    void saveImage(int n);
private slots:
    void onAction(bool checked);
private:
    QAction *m_pAction[7];
};

#endif // IMAGESAVETOOLBAR_H
