#include "MainVentana.h"
#include "ui_MainVentana.h"
#include "OnePoint.h"


MainVentana::MainVentana(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainVentana),
    onePoint(new mainWindow),
    manyPoints(new TableView)
{
    ui->setupUi(this);
    QGridLayout* gridOnePoint = new QGridLayout(ui->onePointTab);
    gridOnePoint->addWidget(onePoint);

    QGridLayout* gridManyPoints = new QGridLayout(ui->tableTab);
    gridManyPoints->addWidget(manyPoints);

    createActions();
    createMenus();
    ui->tabWidget->setTabText(0, "Puntual");
    ui->tabWidget->setTabText(1, "Tabla");
    ui->tabWidget->setCurrentIndex(0);


    connect(this, &MainVentana::geographicFormatChanged, onePoint, &mainWindow::setGeographicFormat);
    connect(this, &MainVentana::loadExcelFile, manyPoints, &TableView::openExcelFile);

    emit gmsFormatAction->triggered();

}

MainVentana::~MainVentana()
{
    delete ui;
}

void MainVentana::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Archivo"));
    fileMenu->addAction(openExcelFile);

    optionsMenu = menuBar()->addMenu(tr("&Opciones"));
    geographicFormatMenu = optionsMenu->addMenu("Formato de coordenadas geográficas...");
    geographicFormatMenu->addAction(decimalFormatAction);
    geographicFormatMenu->addAction(gmsFormatAction);

    helpMenu = menuBar()->addMenu(tr("&Ayuda"));

}

void MainVentana::createActions()
{
    openExcelFile = new QAction(tr("Abrir archivo &Excel..."), this);
    connect(openExcelFile, &QAction::triggered, this, &MainVentana::onExcelFileOpened);

    gmsFormatAction = new QAction(tr("&Grados minutos segundos"), this);
    gmsFormatAction->setCheckable(true);
    gmsFormatAction->setChecked(true);
    connect(gmsFormatAction, &QAction::triggered, this, &MainVentana::onGmsTriggered);

    decimalFormatAction = new QAction(tr("&Decimales"), this);
    decimalFormatAction->setCheckable(true);
    connect(decimalFormatAction, &QAction::triggered, this, &MainVentana::onDecimalTriggered);

    geographicFormatGroup = new QActionGroup(this);
    geographicFormatGroup->addAction(gmsFormatAction);
    geographicFormatGroup->addAction(decimalFormatAction);

}


void MainVentana::onGmsTriggered()
{
    emit geographicFormatChanged(ViewModel::GeographicFormat::GMS);

}

void MainVentana::onDecimalTriggered()
{
    emit geographicFormatChanged(ViewModel::GeographicFormat::Decimal);
}

void MainVentana::onExcelFileOpened()
{
    emit loadExcelFile();
}
