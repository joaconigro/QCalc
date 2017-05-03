#include "ColumnSelectDialog.h"
#include "ui_ColumnSelectDialog.h"

ColumnSelectDialog::ColumnSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnSelectDialog)
{
    ui->setupUi(this);
}

ColumnSelectDialog::~ColumnSelectDialog()
{
    delete ui;
}
