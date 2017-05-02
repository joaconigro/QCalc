#include "TableView.h"
#include "ui_TableView.h"
#include "xlsxworksheet.h"
#include "xlsxcellrange.h"
#include "xlsxsheetmodel.h"
#include "xlsxcell.h"
#include <QFileDialog>
#include <QVariant>
#include "SelectFromListDialog.h"
#include <QMessageBox>

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


    //Conections
    connect(ui->convertButton, &QPushButton::clicked, this->viewModel, &ViewModel::startConverter);
//    connect(ui->initialXLineEdit, &QLineEdit::textChanged, this->viewModel, &ViewModel::initialXChanged);
//    connect(ui->initialYLineEdit, &QLineEdit::textChanged, this->viewModel, &ViewModel::initialYChanged);
//    connect(ui->initialZLineEdit, &QLineEdit::textChanged, this->viewModel, &ViewModel::initialZChanged);
    connect(ui->initialSystemComboBox, &QComboBox::currentTextChanged, this, &TableView::inputSystemChanged);
    connect(ui->finalSystemComboBox, &QComboBox::currentTextChanged, this, &TableView::outputSystemChanged);
    connect(ui->initialSystemComboBox, &QComboBox::currentTextChanged, this->viewModel, &ViewModel::selectedInputSystemChanged);
    connect(ui->finalSystemComboBox, &QComboBox::currentTextChanged, this->viewModel, &ViewModel::selectedOutputSystemChanged);



    connect(viewModel, &ViewModel::updateInputZoneList, this, &TableView::updateInputZones);
    connect(viewModel, &ViewModel::updateOutputZoneList, this, &TableView::updateOutputZones);
//    connect(viewModel, &ViewModel::updateFinalX, ui->finalXLineEdit, &QLineEdit::setText);
//    connect(viewModel, &ViewModel::updateFinalY, ui->finalYLineEdit, &QLineEdit::setText);
//    connect(viewModel, &ViewModel::updateFinalZ, ui->finalZLineEdit, &QLineEdit::setText);


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

void TableView::openExcelFile()
{
    QString filePath = QFileDialog::getOpenFileName(0, "Abrir archivo de Excel", QString(), "*.xlsx");


    if (!filePath.isEmpty()){
        xlsx = new Document(filePath);

        if (xlsx->sheetNames().count() > 1){
            SelectFromListDialog *listDialog = new SelectFromListDialog(xlsx->sheetNames(), this);

            connect(listDialog, &SelectFromListDialog::selectedSheetChanged, this, &TableView::sheetListAccepted);

            listDialog->show();
        } else {
            QString sheetName = xlsx->currentWorksheet()->sheetName();
            sheetListAccepted(sheetName);
        }
    }
}

void TableView::sheetListAccepted(const QString sheetName)
{

    Worksheet *sheet = dynamic_cast<Worksheet *>(xlsx->sheet(sheetName));

    QMessageBox *message = new QMessageBox(this);
    message->setWindowTitle("Encabezado");
    message->setText("¿La hoja seleccionada contiene un encabezado en la primera fila?");
    message->setStandardButtons(QMessageBox::Yes);
    message->addButton(QMessageBox::No);
    message->setDefaultButton(QMessageBox::Yes);

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
                pCell->setData(Qt::DisplayRole, sheet->cellAt(i+1, j+1)->value());
            }
        }

        if (startRow>0){
            QStringList headers;
            for (int j = 0; j < columns; j++) {
                headers.append(sheet->cellAt(1, j+1)->value().toString());
            }
            ui->multiPointTable->setHorizontalHeaderLabels(headers);
        }
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
//    if (isGeographic){
//        if (format == ViewModel::GeographicFormat::Decimal) {
//            ui->initialXLineEdit->setValidator(new QDoubleValidator);
//            ui->initialYLineEdit->setValidator(new QDoubleValidator);
//        } else {
//            ui->initialXLineEdit->setValidator(nullptr);
//            ui->initialYLineEdit->setValidator(nullptr);
//        }
//    } else {
//        ui->initialXLineEdit->setValidator(new QDoubleValidator);
//        ui->initialYLineEdit->setValidator(new QDoubleValidator);
//    }

}
