#include "OnePoint.h"
#include "ui_OnePoint.h"

mainWindow::mainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);

    ui->initialZLineEdit->setValidator(new QDoubleValidator);
    viewModel = new ViewModel();
    ui->initialSystemComboBox->addItems(viewModel->availableSystems());
    ui->finalSystemComboBox->addItems(viewModel->availableSystems());
    ui->initialDatumComboBox->addItems(viewModel->availableDatums());
    ui->finalDatumComboBox->addItems(viewModel->availableDatums());


    //Conections
    connect(ui->convertButton, &QPushButton::clicked, this->viewModel, &ViewModel::startConverter);
    connect(ui->initialXLineEdit, &QLineEdit::textChanged, this->viewModel, &ViewModel::initialXChanged);
    connect(ui->initialYLineEdit, &QLineEdit::textChanged, this->viewModel, &ViewModel::initialYChanged);
    connect(ui->initialZLineEdit, &QLineEdit::textChanged, this->viewModel, &ViewModel::initialZChanged);
    connect(ui->initialSystemComboBox, &QComboBox::currentTextChanged, this, &mainWindow::inputSystemChanged);
    connect(ui->finalSystemComboBox, &QComboBox::currentTextChanged, this, &mainWindow::outputSystemChanged);
    connect(ui->initialSystemComboBox, &QComboBox::currentTextChanged, this->viewModel, &ViewModel::selectedInputSystemChanged);
    connect(ui->finalSystemComboBox, &QComboBox::currentTextChanged, this->viewModel, &ViewModel::selectedOutputSystemChanged);



    connect(viewModel, &ViewModel::updateInputZoneList, this, &mainWindow::updateInputZones);
    connect(viewModel, &ViewModel::updateOutputZoneList, this, &mainWindow::updateOutputZones);
    connect(viewModel, &ViewModel::updateFinalX, ui->finalXLineEdit, &QLineEdit::setText);
    connect(viewModel, &ViewModel::updateFinalY, ui->finalYLineEdit, &QLineEdit::setText);
    connect(viewModel, &ViewModel::updateFinalZ, ui->finalZLineEdit, &QLineEdit::setText);


    connect(ui->initialZoneComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), viewModel, &ViewModel::selectedInputZoneChanged);
    connect(ui->finalZoneComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), viewModel, &ViewModel::selectedOutputZoneChanged);

    connect(ui->initialDatumComboBox, &QComboBox::currentTextChanged, viewModel, &ViewModel::selectedInputDatumChanged);
    connect(ui->finalDatumComboBox, &QComboBox::currentTextChanged, viewModel, &ViewModel::selectedOutputDatumChanged);

    connect(this, &mainWindow::changeGeographicFormat, viewModel, &ViewModel::setGeographicFormat);

    emit ui->initialSystemComboBox->currentTextChanged(ui->initialSystemComboBox->currentText());
    emit ui->finalSystemComboBox->currentTextChanged(ui->finalSystemComboBox->currentText());
    emit ui->initialDatumComboBox->currentTextChanged(ui->initialDatumComboBox->currentText());
    emit ui->finalDatumComboBox->currentTextChanged(ui->finalDatumComboBox->currentText());


}

mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::updateFinalX(const QString xValue)
{
    ui->finalXLineEdit->setText(xValue);
}

void mainWindow::updateFinalY(const QString yValue)
{
    ui->finalYLineEdit->setText(yValue);
}

void mainWindow::updateFinalZ(const QString zValue)
{
    ui->finalZLineEdit->setText(zValue);
}

void mainWindow::inputSystemChanged(const QString name)
{
    if (name == "Geográficas"){
        ui->initialXLabel->setText("Longitud: ");
        ui->initialYLabel->setText("Latitud: ");
        ui->initialZoneComboBox->setVisible(false);
        ui->initialZoneLabel->setVisible(false);
        isGeographic = true;
    } else{
        ui->initialXLabel->setText("X: ");
        ui->initialYLabel->setText("Y: ");
        ui->initialZoneComboBox->setVisible(true);
        ui->initialZoneLabel->setVisible(true);
        isGeographic = false;
    }

    setGeographicFormat(geographicFormat);
    emit ui->initialDatumComboBox->currentTextChanged(ui->initialDatumComboBox->currentText());
}

void mainWindow::outputSystemChanged(const QString name)
{
    if (name == "Geográficas"){
        ui->finalXLabel->setText("Longitud: ");
        ui->finalYLabel->setText("Latitud: ");
        ui->finalZoneComboBox->setVisible(false);
        ui->finalZoneLabel->setVisible(false);
    } else{
        ui->finalXLabel->setText("X: ");
        ui->finalYLabel->setText("Y: ");
        ui->finalZoneComboBox->setVisible(true);
        ui->finalZoneLabel->setVisible(true);
    }
    emit ui->finalDatumComboBox->currentTextChanged(ui->finalDatumComboBox->currentText());
}

void mainWindow::updateInputZones(const QStringList list)
{
    ui->initialZoneComboBox->clear();
    ui->initialZoneComboBox->addItems(list);
    emit ui->initialZoneComboBox->currentIndexChanged(ui->initialZoneComboBox->currentIndex());
}

void mainWindow::updateOutputZones(const QStringList list)
{
    ui->finalZoneComboBox->clear();
    ui->finalZoneComboBox->addItems(list);
    emit ui->finalZoneComboBox->currentIndexChanged(ui->finalZoneComboBox->currentIndex());
}

void mainWindow::setGeographicFormat(const ViewModel::GeographicFormat format)
{
    emit changeGeographicFormat(format);
    geographicFormat = format;
    if (isGeographic){
        if (format == ViewModel::GeographicFormat::Decimal) {
            ui->initialXLineEdit->setValidator(new QDoubleValidator);
            ui->initialYLineEdit->setValidator(new QDoubleValidator);
        } else {
            ui->initialXLineEdit->setValidator(nullptr);
            ui->initialYLineEdit->setValidator(nullptr);
        }
    } else {
        ui->initialXLineEdit->setValidator(new QDoubleValidator);
        ui->initialYLineEdit->setValidator(new QDoubleValidator);
    }

}
