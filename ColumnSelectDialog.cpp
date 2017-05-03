#include "ColumnSelectDialog.h"
#include "ui_ColumnSelectDialog.h"
#include <QPushButton>

ColumnSelectDialog::ColumnSelectDialog(const QStringList columList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnSelectDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Aceptar");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Cancelar");
    ui->XComboBox->addItems(columList);
    ui->YComboBox->addItems(columList);
    ui->ZComboBox->addItems(columList);
    ui->ZComboBox->insertItem(0, "Omitir");

}

ColumnSelectDialog::~ColumnSelectDialog()
{
    delete ui;
}

void ColumnSelectDialog::on_XComboBox_currentIndexChanged(int index)
{
    emit updateXColumn(index);
}

void ColumnSelectDialog::on_YComboBox_currentIndexChanged(int index)
{
    emit updateYColumn(index);
}

void ColumnSelectDialog::on_ZComboBox_currentIndexChanged(int index)
{
    if (ui->ZComboBox->currentText() == "Omitir"){
        emit updateZColumn(-1);
    } else {
        emit updateZColumn(index);
    }

}

void ColumnSelectDialog::on_buttonBox_accepted()
{
    emit configureColumns(true);
}

void ColumnSelectDialog::on_buttonBox_rejected()
{
    emit configureColumns(false);
}
