#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include "ui_SettingDialog.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();
    void loadSettings();
    void writeSettings();

private slots:
    void setImageFilePath();
    void setVideoFilePath();
    void setVideoLength(int n);
private:
    Ui::SettingDialog *ui;
    QString m_imagePath[6]; // 保存图像的路径
    QString m_videoPath[6]; // 保存视频的路径
    int m_videoLength[6];

    QSpinBox * pSpinBoxCH[6];
    QLabel * pLabelImageCH[6];
    QLabel *pLabelVideoCH[6];
    QPushButton * pButtonVideoCH[6];
    QPushButton * pButtonImageCH[6];

};

#endif // SETTINGDIALOG_H
