#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

    void setAppName(QString name);
    void setPhoneNumber(QString tel);
    void setWebSite(QString website);
    void setCompanyName(QString companyName);
    void setIcon(QIcon icon);
private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
