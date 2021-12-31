#include "SettingsToolBar.h"

SettingsToolBar::SettingsToolBar(QWidget *parent)
    :QToolBar(parent)
{
    setIconSize(QSize(32, 32));

    m_pActionCamera = new QAction(tr("start/stop camera"), this);
    m_pActionCamera->setCheckable(true);
    addAction(m_pActionCamera);

    m_pActionSettings = new QAction("settings", this);
    //m_pActionSettings->setCheckable(true);
    addAction(m_pActionSettings);

    m_pActionCamera->setIcon(QIcon(":/res/camera.png"));
    m_pActionSettings->setIcon(QIcon(":/res/settings.png"));
    connect(m_pActionCamera, &QAction::triggered, this, &SettingsToolBar::onAction);
    connect(m_pActionSettings, &QAction::triggered, this, &SettingsToolBar::onAction);
}

void SettingsToolBar::onAction(bool checked)
{
    QAction *pAction = (QAction *)sender();
    if(pAction == m_pActionSettings)
    {
        emit showSettingsDialog();
        return;
    }
    if(checked)
    {
        emit startCamera();
    }
    else
    {
        emit stopCamera();
    }
}
