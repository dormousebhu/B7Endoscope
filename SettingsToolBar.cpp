#include <QApplication>
#include "SettingsToolBar.h"

SettingsToolBar::SettingsToolBar(QWidget *parent)
    :QToolBar(parent)
{
    setWindowTitle("SettingsToolBar");
    setIconSize(QSize(32, 32));

    m_pActionCamera = new QAction(tr("Start/Stop Camera"), this);
    m_pActionCamera->setIcon(QIcon(":/res/camera.png"));
    m_pActionCamera->setCheckable(true);
    addAction(m_pActionCamera);
    connect(m_pActionCamera, &QAction::triggered, this, &SettingsToolBar::onAction);

    m_action480p = new QAction(tr("480p"), this);
    m_action480p->setIcon(QIcon(":/res/480.png"));
    m_action480p->setCheckable(true);
    addAction(m_action480p);
    connect(m_action480p, &QAction::triggered, this, &SettingsToolBar::onResolutionChanged);

    m_action720p = new QAction(tr("720p"), this);
    m_action720p->setIcon(QIcon(":/res/720.png"));
    m_action720p->setCheckable(true);
    addAction(m_action720p);
    connect(m_action720p, &QAction::triggered, this, &SettingsToolBar::onResolutionChanged);

    m_action1080p = new QAction(tr("1080p"), this);
    m_action1080p->setIcon(QIcon(":/res/1080.png"));
    m_action1080p->setCheckable(true);
    m_action1080p->setChecked(true);
    addAction(m_action1080p);
    connect(m_action1080p, &QAction::triggered, this, &SettingsToolBar::onResolutionChanged);

    m_actionGroup = new QActionGroup(this);
    m_actionGroup->addAction(m_action480p);
    m_actionGroup->addAction(m_action720p);
    m_actionGroup->addAction(m_action1080p);

    addSeparator();

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

void SettingsToolBar::onResolutionChanged(bool checked)
{
    Q_UNUSED(checked);
    QAction *pAction = (QAction *)sender();
    if(pAction == m_action480p)
    {
        emit resolutionChanged(QSize(640, 480));
        return;
    }
    if(pAction == m_action720p)
    {
        emit resolutionChanged(QSize(1280, 720));
        return;
    }
    if(pAction == m_action1080p)
    {
        emit resolutionChanged(QSize(1920, 1080));
        return;
    }
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
