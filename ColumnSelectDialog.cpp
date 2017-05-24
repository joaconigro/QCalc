#include "ColumnSelectDialog.h"
#include "ui_ColumnSelectDialog.h"
#include <QPushButton>

ColumnSelectDialog::ColumnSelectDialog(const QStringList list, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnSelectDialog)
{
    for(int i = 0; i < list.count(); i++){
        columnList << list.at(i);
    }

    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Aceptar");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Cancelar");
    ui->XComboBox->addItems(columnList);
    ui->YComboBox->addItems(columnList);
    ui->ZComboBox->addItems(columnList);
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
    emit updateZColumn(index);
}

void ColumnSelectDialog::on_buttonBox_accepted()
{
    emit configureColumns(true);
}

void ColumnSelectDialog::on_buttonBox_rejected()
{
    emit configureColumns(false);
}

void ColumnSelectDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    autoSelectColumns();
}

void ColumnSelectDialog::autoSelectColumns()
{
    ui->ZComboBox->setCurrentIndex(0);

    for(int i = 0; i < columnList.count(); i++){
        QString item = columnList.at(i);
        if(item.contains("x", Qt::CaseInsensitive) || item.contains("long", Qt::CaseInsensitive) || item.contains("lng", Qt::CaseInsensitive)){
            ui->XComboBox->setCurrentIndex(i);
            emit updateXColumn(i);
        }
        if(item.contains("y", Qt::CaseInsensitive) || item.contains("lat", Qt::CaseInsensitive)){
            ui->YComboBox->setCurrentIndex(i);
            emit updateYColumn(i);
        }
        if(item.contains("z", Qt::CaseInsensitive) || item.contains("cota", Qt::CaseInsensitive) || item.contains("elev", Qt::CaseInsensitive)){
            ui->ZComboBox->setCurrentIndex(i+1);
            emit updateZColumn(i+1);
        }
    }
}
