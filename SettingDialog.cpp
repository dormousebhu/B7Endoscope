#include "SettingDialog.h"
#include <QStandardPaths>
#include <QSettings>
#include <QFileDialog>
SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(0, tr("Image path"));
    ui->tabWidget->setTabText(1, tr("Video path"));
    ui->tab->setWindowTitle(tr("Image path"));
    ui->tab_2->setWindowTitle(tr("Video path"));

    QSpinBox *tempSpinBox[6] = {ui->spinBoxCH_1, ui->spinBoxCH_2, ui->spinBoxCH_3,
                                ui->spinBoxCH_4, ui->spinBoxCH_5, ui->spinBoxCH_6};

    QLabel * tempLabelImageCH[6] = {ui->labelImageCH_1, ui->labelImageCH_2, ui->labelImageCH_3,
                                ui->labelImageCH_4, ui->labelImageCH_5, ui->labelImageCH_6};

    QLabel *tempLabelVideoCH[6] = {ui->labelVideoCH_1, ui->labelVideoCH_2, ui->labelVideoCH_3,
                               ui->labelVideoCH_4, ui->labelVideoCH_5, ui->labelVideoCH_6};

    QPushButton * tempButtonVideoCH[6] {ui->pushButtonVideoCH_1, ui->pushButtonVideoCH_2, ui->pushButtonVideoCH_3,
                               ui->pushButtonVideoCH_4, ui->pushButtonVideoCH_5, ui->pushButtonVideoCH_6};

    QPushButton * tempButtonImageCH[6] = {ui->pushButtonImageCH_1, ui->pushButtonImageCH_2, ui->pushButtonImageCH_3,
                                      ui->pushButtonImageCH_4, ui->pushButtonImageCH_5, ui->pushButtonImageCH_6};

    for(int i = 0; i < 6; i++)
    {
        pSpinBoxCH[i] = tempSpinBox[i];
        pLabelImageCH[i] = tempLabelImageCH[i];
        pLabelVideoCH[i] = tempLabelVideoCH[i];
        pButtonVideoCH[i] = tempButtonVideoCH[i];
        pButtonImageCH[i] = tempButtonImageCH[i];

        connect(pButtonImageCH[i], &QPushButton::clicked, this, &SettingDialog::setImageFilePath);
        connect(pButtonVideoCH[i], &QPushButton::clicked, this, &SettingDialog::setVideoFilePath);
        connect(pSpinBoxCH[i], SIGNAL(valueChanged(int)), this, SLOT(setVideoLength(int)));
    }
    loadSettings();
}

void SettingDialog::setImageFilePath()
{
    QPushButton * btn = (QPushButton * ) sender();

    for(int i = 0; i < 6; i++)
    {
        if(btn == pButtonImageCH[i])
        {
            QString path = QFileDialog::getExistingDirectory(this, tr("image path"), m_imagePath[i]);
            if(!path.isEmpty())
            {
                m_imagePath[i] = path;
                pLabelImageCH[i]->setText(m_imagePath[i]);
            }
            return;
        }
    }

}
void SettingDialog::setVideoFilePath()
{
    QPushButton * btn = (QPushButton * ) sender();

    for(int i = 0;i < 6; i++)
    {
        if( btn == pButtonVideoCH[i] )
        {
            QString path = QFileDialog::getExistingDirectory(this, tr("video path"), m_videoPath[i]);
            if(!path.isEmpty())
            {
                m_videoPath[i] = path;
                pLabelVideoCH[i]->setText(m_videoPath[i]);
            }
            return ;
        }
    }
}

void SettingDialog::setVideoLength(int n)
{
    Q_UNUSED(n);
    QSpinBox * spin = (QSpinBox *) sender();
    for(int i = 0; i < 6; i++)
    {
        if( spin == pSpinBoxCH[i] )
        {
            m_videoLength[i] = spin->value();
            return ;
        }
    }
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::writeSettings()
{
    QSettings settings("JingHai", "JHEndoscope", this);
    for(int i = 0; i < 6; i++)
    {
        settings.setValue(QString("ImagePath/CH%1").arg(i + 1), m_imagePath[i]);
        settings.setValue(QString("VideoPath/CH%1").arg(i + 1), m_videoPath[i]);
        settings.setValue(QString("Video/Length%1").arg(i + 1), m_videoLength[i]);
    }
}

void SettingDialog::loadSettings()
{
    QSettings settings("JingHai", "JHEndoscope", this);

    QString imagePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QString videoPath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

    for(int i = 0; i < 6; i++)
    {
        m_imagePath[i] = settings.value(QString("ImagePath/CH%1").arg(i + 1), imagePath).toString();
        m_videoPath[i] = settings.value(QString("VideoPath/CH%1").arg(i + 1), videoPath).toString();
        m_videoLength[i] = settings.value(QString("Video/Length%1").arg(i + 1), 10).toInt();

        pLabelImageCH[i]->setText(m_imagePath[i]);
        pLabelVideoCH[i]->setText(m_videoPath[i]);
        pSpinBoxCH[i]->setValue(m_videoLength[i]);
    }
}
