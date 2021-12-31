#ifndef SETTINGSTOOLBAR_H
#define SETTINGSTOOLBAR_H

#include <QToolBar>
#include <QAction>

class SettingsToolBar : public QToolBar
{
    Q_OBJECT
public:
    SettingsToolBar(QWidget *parent = nullptr);
    void retranslateUi();
signals:
    void startCamera();
    void stopCamera();
    void showSettingsDialog();
    void translate(bool on);
    void aboutJinghai();

private slots:
    void onAction(bool checked);
private:
    QAction *m_pActionCamera;
    QAction *m_pActionSettings;
    QAction *m_pActionChLanguage;
    QAction *m_aboutJinghai;
    QAction *m_aboutQt;
};

#endif // SETTINGSTOOLBAR_H
