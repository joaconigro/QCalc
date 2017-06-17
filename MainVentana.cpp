#include "MainVentana.h"
#include "ui_MainVentana.h"
#include "OnePoint.h"
#include "AboutQCalc.h"

MainVentana::MainVentana(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainVentana),
    onePoint(new mainWindow),
    manyPoints(new TableView)
{
    ui->setupUi(this);

    //Agrega el widget de conversión puntual
    QGridLayout* gridOnePoint = new QGridLayout(ui->onePointTab);
    gridOnePoint->addWidget(onePoint);
    ui->tabWidget->setTabText(0, "Conversión Puntual");

    //Agrega el widget de conversión por tabla
    QGridLayout* gridManyPoints = new QGridLayout(ui->tableTab);
    gridManyPoints->addWidget(manyPoints);
    ui->tabWidget->setTabText(1, "Conversión por Tabla");

    //Crea los menúes
    createActions();
    createMenus();

    //Elige el tab de conversión puntual
    ui->tabWidget->setCurrentIndex(0);

    //Conexiones
    connect(this, &MainVentana::geographicFormatChanged, onePoint, &mainWindow::setGeographicFormat);
    connect(this, &MainVentana::geographicFormatChanged, manyPoints, &TableView::setGeographicFormat);
    connect(this, &MainVentana::loadExcelFile, manyPoints, &TableView::openExcelFile);
    connect(manyPoints, &TableView::fileOpened, this, &MainVentana::tableOpened);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainVentana::tabChanged);
    emit gmsFormatAction->triggered();
}

MainVentana::~MainVentana()
{
    delete ui;
}

void MainVentana::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Archivo"));
    fileMenu->addAction(quitAction);

    onePointMenu = menuBar()->addMenu(tr("Conversión &puntual"));
    onePointMenu->addAction(viewGoogleMapsAction);

    tableMenu = menuBar()->addMenu(tr("Conversión por &tabla"));
    tableMenu->addAction(openExcelFileAction);
    tableMenu->addAction(viewOriginalDataAction);
    tableMenu->addSeparator();
    tableMenu->addAction(saveTableAction);

    optionsMenu = menuBar()->addMenu(tr("&Opciones"));
    geographicFormatMenu = optionsMenu->addMenu("Formato de coordenadas geográficas...");
    geographicFormatMenu->addAction(decimalFormatAction);
    geographicFormatMenu->addAction(gmsFormatAction);

    helpMenu = menuBar()->addMenu(tr("Ayuda"));
    helpMenu->addAction(viewHelpAction);
    helpMenu->addAction(aboutAction);
}

void MainVentana::createActions()
{
    //Acción salir
    quitAction = new QAction(tr("Salir"), this);
    connect(quitAction, &QAction::triggered, this, &MainVentana::close);

    //Acción abrir Excel
    openExcelFileAction = new QAction(tr("Abrir archivo &Excel..."), this);
    connect(openExcelFileAction, &QAction::triggered, this, &MainVentana::onExcelFileOpened);

    //Acción ver datos originales
    viewOriginalDataAction = new QAction(tr("Ver &datos originales"), this);
    connect(viewOriginalDataAction, &QAction::triggered, manyPoints, &TableView::readExcelFile);
    viewOriginalDataAction->setEnabled(false);

    //Acción ver punto en Google Maps
    viewGoogleMapsAction = new QAction(tr("Ver punto en Google Maps"), this);
    connect(viewGoogleMapsAction, &QAction::triggered, onePoint, &mainWindow::viewOnGoogleMaps);

    //Acción configurar formato de grados minutos y segundos
    gmsFormatAction = new QAction(tr("&Grados minutos segundos"), this);
    gmsFormatAction->setCheckable(true);
    gmsFormatAction->setChecked(true);
    connect(gmsFormatAction, &QAction::triggered, this, &MainVentana::onGmsTriggered);

    //Acción configurar formato de grados decimales
    decimalFormatAction = new QAction(tr("&Decimales"), this);
    decimalFormatAction->setCheckable(true);
    connect(decimalFormatAction, &QAction::triggered, this, &MainVentana::onDecimalTriggered);

    geographicFormatGroup = new QActionGroup(this);
    geographicFormatGroup->addAction(gmsFormatAction);
    geographicFormatGroup->addAction(decimalFormatAction);

    //Acción guardar tabla en Excel
    saveTableAction = new QAction(tr("Guardar resultados..."), this);
    connect(saveTableAction, &QAction::triggered, manyPoints, &TableView::saveFileAs);
    saveTableAction->setEnabled(false);

    //Acción ver ayuda
    viewHelpAction = new QAction(tr("Ver ayuda"), this);
    connect(viewHelpAction, &QAction::triggered, this, &MainVentana::viewHelp);

    //Acción Acerca de...
    aboutAction = new QAction(tr("Acerca de..."), this);
    connect(aboutAction, &QAction::triggered, this, &MainVentana::aboutQCalc);
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
    ui->tabWidget->setCurrentIndex(1);
}

void MainVentana::viewHelp()
{
    QString str(QCoreApplication::applicationDirPath());
    str.append("/Manual.pdf");
    QFile file(str);
    if (file.exists()){
        QDesktopServices::openUrl(QUrl("file:///" + str));
    } else{
        QMessageBox::information(this, "Error", "No se puede encontrar el archivo de ayuda.", QDialogButtonBox::Ok);
    }
}

void MainVentana::aboutQCalc()
{
    AboutQCalc *about = new AboutQCalc();
    about->show();
}

void MainVentana::tabChanged(int index)
{
    if (index == 0){
        ui->tabWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        ui->tabWidget->resize(ui->tabWidget->minimumSizeHint());
    } else{
        ui->tabWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    }

    ui->tabWidget->adjustSize();
    this->adjustSize();
}

void MainVentana::tableOpened(bool opened)
{
    saveTableAction->setEnabled(opened);
    viewOriginalDataAction->setEnabled(opened);
}
