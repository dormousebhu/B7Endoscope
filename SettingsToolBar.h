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
    void resolutionChanged(QSize z);

private slots:
    void onAction(bool checked);
    void onResolutionChanged(bool checked);
private:
    QAction *m_pActionCamera;
    QAction *m_pActionSettings;
    QAction *m_pActionChLanguage;
    QAction *m_aboutJinghai;
    QAction *m_aboutQt;

    QActionGroup *m_actionGroup;
    QAction *m_action480p;
    QAction *m_action720p;
    QAction *m_action1080p;
};

#endif // SETTINGSTOOLBAR_H
