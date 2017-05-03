#include "SelectFromListDialog.h"
#include "ui_SelectFromListDialog.h"
#include <QList>
#include <QPushButton>
#include "TableView.h"

SelectFromListDialog::SelectFromListDialog(const QStringList list, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectFromListDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Aceptar");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Cancelar");

    for(int i = 0; i<list.count(); i++){
        sheetsList.append(list.at(i));
    }

    ui->listWidget->addItems(sheetsList);

}

SelectFromListDialog::~SelectFromListDialog()
{
    delete ui;
}


void SelectFromListDialog::on_listWidget_itemSelectionChanged()
{
    QList<QListWidgetItem*> sheets = ui->listWidget->selectedItems();
    selectedSheet = sheets.at(0)->text();
}

void SelectFromListDialog::on_buttonBox_accepted()
{
    emit selectedSheetChanged(selectedSheet);
}
