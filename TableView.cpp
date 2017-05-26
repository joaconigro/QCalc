#include "TableView.h"
#include "ui_TableView.h"
#include "xlsxworksheet.h"
#include "xlsxcellrange.h"
#include "xlsxsheetmodel.h"
#include "xlsxcell.h"
#include <QFileDialog>
#include <QPushButton>
#include <QVariant>
#include "SelectFromListDialog.h"
#include "ColumnSelectDialog.h"
#include <QMessageBox>
#include <QFile>

using namespace QXlsx;

TableView::TableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableView)
{
    ui->setupUi(this);

    viewModel = new ViewModel();
    ui->initialSystemComboBox->addItems(viewModel->availableSystems());
    ui->finalSystemComboBox->addItems(viewModel->availableSystems());
    ui->initialDatumComboBox->addItems(viewModel->availableDatums());
    ui->finalDatumComboBox->addItems(viewModel->availableDatums());
    columnX = columnY = 0;
    columnZ = -1;
    columnsConfigured = false;

    //Connections
    connect(this, &TableView::convertPoint, this->viewModel, &ViewModel::startConverter);
    connect(this, &TableView::updateInitialX, this->viewModel, &ViewModel::initialXChanged);
    connect(this, &TableView::updateInitialY, this->viewModel, &ViewModel::initialYChanged);
    connect(this, &TableView::updateInitialZ, this->viewModel, &ViewModel::initialZChanged);
    connect(ui->initialSystemComboBox, &QComboBox::currentTextChanged, this, &TableView::inputSystemChanged);
    connect(ui->finalSystemComboBox, &QComboBox::currentTextChanged, this, &TableView::outputSystemChanged);
    connect(ui->initialSystemComboBox, &QComboBox::currentTextChanged, this->viewModel, &ViewModel::selectedInputSystemChanged);
    connect(ui->finalSystemComboBox, &QComboBox::currentTextChanged, this->viewModel, &ViewModel::selectedOutputSystemChanged);



    connect(viewModel, &ViewModel::updateInputZoneList, this, &TableView::updateInputZones);
    connect(viewModel, &ViewModel::updateOutputZoneList, this, &TableView::updateOutputZones);
    connect(viewModel, &ViewModel::updateFinalX, this, &TableView::updateFinalX);
    connect(viewModel, &ViewModel::updateFinalY, this, &TableView::updateFinalY);
    connect(viewModel, &ViewModel::updateFinalZ, this, &TableView::updateFinalZ);


    connect(ui->initialZoneComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), viewModel, &ViewModel::selectedInputZoneChanged);
    connect(ui->finalZoneComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), viewModel, &ViewModel::selectedOutputZoneChanged);

    connect(ui->initialDatumComboBox, &QComboBox::currentTextChanged, viewModel, &ViewModel::selectedInputDatumChanged);
    connect(ui->finalDatumComboBox, &QComboBox::currentTextChanged, viewModel, &ViewModel::selectedOutputDatumChanged);

    connect(this, &TableView::changeGeographicFormat, viewModel, &ViewModel::setGeographicFormat);

    emit ui->initialSystemComboBox->currentTextChanged(ui->initialSystemComboBox->currentText());
    emit ui->finalSystemComboBox->currentTextChanged(ui->finalSystemComboBox->currentText());
    emit ui->initialDatumComboBox->currentTextChanged(ui->initialDatumComboBox->currentText());
    emit ui->finalDatumComboBox->currentTextChanged(ui->finalDatumComboBox->currentText());



}

TableView::~TableView()
{
    delete ui;
}

void TableView::readExcelFile()
{

    Worksheet *sheet = dynamic_cast<Worksheet *>(xlsx->sheet(sheetName));

    QMessageBox *message = new QMessageBox(this);
    message->setWindowTitle("Encabezado");
    message->setText("¿La hoja seleccionada contiene un encabezado en la primera fila?");
    message->setStandardButtons(QMessageBox::Yes);
    message->addButton(QMessageBox::No);
    message->setDefaultButton(QMessageBox::Yes);
    message->button(QMessageBox::Yes)->setText("Sí");

    int startRow;
    if (message->exec()==QMessageBox::Yes){
        startRow = 1;
    } else {
        startRow = 0;
    }

    if (sheet) {
        int columns = sheet->dimension().columnCount();
        int rows = sheet->dimension().rowCount();

        ui->multiPointTable->setColumnCount(columns);
        ui->multiPointTable->setRowCount(rows - startRow);

        for (int i = startRow; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                QTableWidgetItem *pCell = ui->multiPointTable->item(i-startRow, j);
                if(!pCell)
                {
                    pCell = new QTableWidgetItem;
                    ui->multiPointTable->setItem(i-startRow, j, pCell);
                }
                pCell->setData(Qt::DisplayRole, sheet->cellAt(i+1, j+1)->value().toString());
                pCell->setTextAlignment(Qt::AlignRight);
            }
        }

        //Crea los encabezados de las columnas para
        //poder elegirlos
        headers.clear();
        if (startRow>0){
            for (int j = 0; j < columns; j++) {
                headers.append(sheet->cellAt(1, j+1)->value().toString());
            }
            ui->multiPointTable->setHorizontalHeaderLabels(headers);
        } else {
            for (int j = 0; j < columns; j++) {
                headers.append(QString::number(j+1));
            }
        }
    }
}

void TableView::saveFileAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Guardar archivo de Excel", QString(), "Archivos de Microsoft Excel (*.xlsx)");
    if (!filePath.isEmpty()){
        if (!filePath.endsWith(".xlsx")){
            filePath += ".xlsx";
        }

        QXlsx::Document xlsxToSave;
        xlsxToSave.addSheet("Conversión");


        for (int i = 0; i < headers.count(); i++){
            xlsxToSave.write(1, i+1, QVariant (headers.at(i)));
        }

        QLocale qLoc;
        for (int i = 0; i < ui->multiPointTable->rowCount(); i++) {
            for (int j = 0; j < ui->multiPointTable->columnCount(); j++) {
                QTableWidgetItem *someCell = ui->multiPointTable->item(i, j);
                QString tempValue = someCell->data(Qt::DisplayRole).toString();
                bool ok;
                double tempDouble = qLoc.toDouble(tempValue, &ok);
                if (ok){
                    xlsxToSave.write(i+2, j+1, tempDouble);
                } else {
                    xlsxToSave.write(i+2, j+1, tempValue);
                }
                }
        }

         xlsxToSave.saveAs(filePath);

    }

}

void TableView::openExcelFile()
{
    //Muestra el cuadro de apertura del archivo Excel
    QString filePath = QFileDialog::getOpenFileName(this, "Abrir archivo de Excel", QString(), "Archivos de Microsoft Excel (*.xlsx)");
    QFile file;
    file.setFileName(filePath);

    //Si el archivo existe
    if (file.exists()){
        if (!filePath.isEmpty()){
            xlsx = new Document(filePath);

            //Si hay mas de una hoja muestra el dialogo de seleccion
            if (xlsx->sheetNames().count() > 1){
                SelectFromListDialog *listDialog = new SelectFromListDialog(xlsx->sheetNames(), this);
                connect(listDialog, &SelectFromListDialog::selectedSheetChanged, this, &TableView::sheetListAccepted);

                listDialog->show();
            } else {
                //Sino selecciona automaticamente la unica hoja
                sheetName = xlsx->currentWorksheet()->sheetName();
                readExcelFile();
            }
            emit fileOpened(true);
            return;
        }
    }
    emit fileOpened(false);

}

void TableView::sheetListAccepted(const QString sheet)
{
    sheetName = sheet;
    readExcelFile();
}

void TableView::updateFinalX(const QString xValue)
{
    QTableWidgetItem *pCell = ui->multiPointTable->item(currentRow, columnX);
    pCell->setData(Qt::DisplayRole, xValue);
}

void TableView::updateFinalY(const QString yValue)
{
    QTableWidgetItem *pCell = ui->multiPointTable->item(currentRow, columnY);
    pCell->setData(Qt::DisplayRole, yValue);
}

void TableView::updateFinalZ(const QString zValue)
{
    if (columnZ >= 0){
        QTableWidgetItem *pCell = ui->multiPointTable->item(currentRow, columnZ);
        pCell->setData(Qt::DisplayRole, zValue);
    }
}

void TableView::inputSystemChanged(const QString name)
{
    if (name == "Geográficas"){
               ui->initialZoneComboBox->setVisible(false);
        ui->initialZoneLabel->setVisible(false);
        isGeographic = true;
    } else{
                ui->initialZoneComboBox->setVisible(true);
        ui->initialZoneLabel->setVisible(true);
        isGeographic = false;
    }

    setGeographicFormat(geographicFormat);
    emit ui->initialDatumComboBox->currentTextChanged(ui->initialDatumComboBox->currentText());
}

void TableView::outputSystemChanged(const QString name)
{
    if (name == "Geográficas"){
               ui->finalZoneComboBox->setVisible(false);
        ui->finalZoneLabel->setVisible(false);
    } else{
              ui->finalZoneComboBox->setVisible(true);
        ui->finalZoneLabel->setVisible(true);
    }
    emit ui->finalDatumComboBox->currentTextChanged(ui->finalDatumComboBox->currentText());
}

void TableView::updateInputZones(const QStringList list)
{
    ui->initialZoneComboBox->clear();
    ui->initialZoneComboBox->addItems(list);
    emit ui->initialZoneComboBox->currentIndexChanged(ui->initialZoneComboBox->currentIndex());
}

void TableView::updateOutputZones(const QStringList list)
{
    ui->finalZoneComboBox->clear();
    ui->finalZoneComboBox->addItems(list);
    emit ui->finalZoneComboBox->currentIndexChanged(ui->finalZoneComboBox->currentIndex());
}

void TableView::setGeographicFormat(const ViewModel::GeographicFormat format)
{
    emit changeGeographicFormat(format);
    geographicFormat = format;
}


//Muestra el diálogo de configuración de las columnas
void TableView::on_columnsButton_clicked()
{
    ColumnSelectDialog *dialog = new ColumnSelectDialog(headers, this);
    connect(dialog, &ColumnSelectDialog::updateXColumn, this, &TableView::onXColumnChanged);
    connect(dialog, &ColumnSelectDialog::updateYColumn, this, &TableView::onYColumnChanged);
    connect(dialog, &ColumnSelectDialog::updateZColumn, this, &TableView::onZColumnChanged);
    connect(dialog, &ColumnSelectDialog::configureColumns, this, &TableView::onColumnsConfigured);

    dialog->show();
}

void TableView::onXColumnChanged(int value)
{
    columnX = value;
}

void TableView::onYColumnChanged(int value)
{
    columnY = value;
}

void TableView::onZColumnChanged(int value)
{
    columnZ = value - 1;
}

void TableView::onColumnsConfigured(bool value)
{
    columnsConfigured = value;
}

void TableView::on_convertButton_clicked()
{

    if (columnsConfigured){
        for (int i = 0; i < ui->multiPointTable->rowCount(); i++) {
            currentRow = i;

            //Carga la coordenada X en
            QTableWidgetItem *xCell = ui->multiPointTable->item(i, columnX);
            emit updateInitialX(xCell->data(Qt::DisplayRole).toString());

            QTableWidgetItem *yCell = ui->multiPointTable->item(i, columnY);
            emit updateInitialY(yCell->data(Qt::DisplayRole).toString());

            if (columnZ >= 0){
                QTableWidgetItem *zCell = ui->multiPointTable->item(i, columnZ);
                emit updateInitialZ(zCell->data(Qt::DisplayRole).toString());
            }

            emit convertPoint();
        }
    } else {
        QMessageBox *message = new QMessageBox(this);
        message->setWindowTitle("Columnas");
        message->setText("No se configuró qué columna corresponde a qué dato.");
        message->setStandardButtons(QMessageBox::Ok);
        message->setDefaultButton(QMessageBox::Ok);
        message->button(QMessageBox::Ok)->setText("Aceptar");

        message->show();
    }



}

void TableView::on_SaveButton_clicked()
{
    saveFileAs();
}
