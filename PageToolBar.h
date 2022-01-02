#ifndef CHTOOLBAR_H
#define CHTOOLBAR_H

#include <QToolBar>
#include <QAction>

class PageToolBar : public QToolBar
{
    Q_OBJECT

public:
    PageToolBar(QWidget *parent = nullptr);
    void setPage(int n);
    void retranslateUi();
signals:
    void pageChanged(int n);
    void zoomIn();
    void zoomOut();
private slots:
    void onAction(bool checked);
private:
    QAction *m_pAction[7];
    QAction *m_pActionZoomIn;
    QAction *m_pActionZoomOut;
    QActionGroup *m_pGroup;
};

#endif // CHTOOLBAR_H
