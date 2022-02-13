#ifndef FOCUSTOOLBAR_H
#define FOCUSTOOLBAR_H
#include <QToolBar>
#include <QAction>

class FocusToolBar : public QToolBar
{
    Q_OBJECT
public:
    FocusToolBar(QWidget *parent = nullptr);
signals:
    void showPage(int i);
private slots:
    void onAction(bool checked);
private:
    QAction *m_pAction[6];
};

#endif // FOCUSTOOLBAR_H
