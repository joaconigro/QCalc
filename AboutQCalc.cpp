#include "AboutQCalc.h"
#include "ui_AboutQCalc.h"
#include "appinfo.h"

AboutQCalc::AboutQCalc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutQCalc)
{
    ui->setupUi(this);

    this->setWindowTitle("Acerca de " + AppInfo::appName);
    ui->labelName->setText(AppInfo::appName);
    ui->labelVersion->setText("Versión: " + AppInfo::version);
    ui->labelCompany->setText(AppInfo::company);
    ui->labelContact->setText(AppInfo::contact);
    ui->labelCopyright->setText(AppInfo::copyrigth);
    ui->labelAcknowledgment->setText(AppInfo::acknowledgment);

    QString qtv = QT_VERSION_STR;
    ui->labelQtVersion->setText("Basado en Qt " + qtv);

}

AboutQCalc::~AboutQCalc()
{
    delete ui;
}
