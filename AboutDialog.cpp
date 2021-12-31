#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    ui->labelWebSite->setOpenExternalLinks(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(ui->pushButtonClose, SIGNAL(clicked()),this, SLOT(close()));
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

void AboutDialog::setIcon(QIcon icon)
{
    ui->labelIcon->setPixmap(icon.pixmap(QSize(100, 100)));
}

void AboutDialog::setAppName(QString name)
{
    ui->labelAppName->setText(name);
    setWindowTitle(name);
}

void AboutDialog::setCompanyName(QString companyName)
{
    ui->labelCompanyName->setText(companyName);
}

void AboutDialog::setPhoneNumber(QString tel)
{
    QString text(tr("tel: %1"));
    ui->labelPhoneNumber->setText(text.arg(tel));
}

void AboutDialog::setWebSite(QString website)
{
    //<html><a href="http://www.xianweijing123.com">www.xianweijing123.com</a></html>
    QString text = "<html><a href=\"" + website +"\">" + website + "</a></html>";
    ui->labelWebSite->setText(text);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
