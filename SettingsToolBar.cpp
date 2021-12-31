#include <QApplication>
#include "SettingsToolBar.h"

SettingsToolBar::SettingsToolBar(QWidget *parent)
    :QToolBar(parent)
{
    setIconSize(QSize(32, 32));

    m_pActionCamera = new QAction(tr("Start/Stop Camera"), this);
    m_pActionCamera->setIcon(QIcon(":/res/camera.png"));
    m_pActionCamera->setCheckable(true);
    addAction(m_pActionCamera);
    connect(m_pActionCamera, &QAction::triggered, this, &SettingsToolBar::onAction);

    m_pActionSettings = new QAction("Settings", this);
    m_pActionSettings->setIcon(QIcon(":/res/settings.png"));
    //m_pActionSettings->setCheckable(true);
    addAction(m_pActionSettings);
    connect(m_pActionSettings, &QAction::triggered, this, &SettingsToolBar::onAction);

    m_pActionChLanguage = new QAction(tr("English/Chinese Language Select"), this);
    m_pActionChLanguage->setCheckable(true);
    m_pActionChLanguage->setIcon(QIcon(":/res/ChLanguage.png"));
    addAction(m_pActionChLanguage);
    connect(m_pActionChLanguage, &QAction::triggered, this, &SettingsToolBar::onAction);

    addSeparator();

    m_aboutJinghai = new QAction(tr("About Qt"), this);
    m_aboutJinghai->setIcon(QIcon(":/res/jh-logo.png"));
    addAction(m_aboutJinghai);
    connect(m_aboutJinghai, &QAction::triggered, this, &SettingsToolBar::aboutJinghai);

    m_aboutQt = new QAction(tr("About Qt"), this);
    m_aboutQt->setIcon(QIcon(":/res/qt-logo.png"));
    addAction(m_aboutQt);
    connect(m_aboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void SettingsToolBar::retranslateUi()
{
    m_pActionCamera->setText(QCoreApplication::translate("SettingsToolBar", "Start/Stop Camera", nullptr));
    m_pActionSettings->setText(QCoreApplication::translate("SettingsToolBar", "Settings", nullptr));
    m_pActionChLanguage->setText(QCoreApplication::translate("SettingsToolBar", "English/Chinese Language Select", nullptr));
}

void SettingsToolBar::onAction(bool checked)
{
    QAction *pAction = (QAction *)sender();
    if(pAction == m_pActionChLanguage)
    {
        emit translate(checked);
        return;
    }

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
