﻿#ifndef SETTINGSTOOLBAR_H
#define SETTINGSTOOLBAR_H

#include <QToolBar>
#include <QAction>

class SettingsToolBar : public QToolBar
{
    Q_OBJECT
public:
    SettingsToolBar(QWidget *parent = nullptr);
signals:
    void startCamera();
    void stopCamera();
    void showSettingsDialog();

private slots:
    void onAction(bool checked);
private:
    QAction *m_pActionCamera;
    QAction *m_pActionSettings;
};

#endif // SETTINGSTOOLBAR_H
