#include "GoogleMapsPointView.h"
#include "ui_GoogleMapsPointView.h"


void GoogleMapsPointView::configureViewModel(const QString x, const QString y, const QString z, const QString coordinateSystem, const QString datum, int zone)
{
    emit updateInitialSystem(coordinateSystem);
    emit updateInitialDatum(datum);
    emit updateInitialZone(zone);
    emit updateGeographicFormat(ViewModel::GeographicFormat::Decimal);
    emit updateFinalSystem("Geográficas");
    emit updateFinalDatum("WGS84");
    emit updateFinalZone(0);
    emit updateInitialX(x);
    emit updateInitialY(y);
    emit updateInitialZ(z);
    emit convertPoint();
}

void GoogleMapsPointView::loadMap(const QString coordinateString)
{



    QFile sourcedHtml(":/map.txt");    //contains urls for sourced tenders.
    if(!sourcedHtml.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox_2;
        msgBox_2.setText("No se puede cargar el contenido de GoogleMaps");
        msgBox_2.exec();
    }
    QTextStream inStream(&sourcedHtml);
    QString tempHtml = inStream.readAll();

    tempHtml = tempHtml.replace("reemplazar", coordinateString);

    view->setHtml(tempHtml);


}

GoogleMapsPointView::GoogleMapsPointView(const QString x, const QString y, const QString z, const QString coordinateSystem, const QString datum, int zone, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GoogleMapsPointView)
{
    ui->setupUi(this);
    view = new QWebEngineView(this);

    QGridLayout *qgrid = new QGridLayout(this);
    qgrid->addWidget(view);
    this->setLayout(qgrid);

    viewModel = new ViewModel;

    connect(this, &GoogleMapsPointView::updateInitialX, viewModel, &ViewModel::initialXChanged);
    connect(this, &GoogleMapsPointView::updateInitialY, viewModel, &ViewModel::initialYChanged);
    connect(this, &GoogleMapsPointView::updateInitialZ, viewModel, &ViewModel::initialZChanged);

    connect(this, &GoogleMapsPointView::updateInitialSystem, viewModel, &ViewModel::selectedInputSystemChanged);
    connect(this, &GoogleMapsPointView::updateFinalSystem, viewModel, &ViewModel::selectedOutputSystemChanged);
    connect(this, &GoogleMapsPointView::updateInitialDatum, viewModel, &ViewModel::selectedInputDatumChanged);
    connect(this, &GoogleMapsPointView::updateFinalDatum, viewModel, &ViewModel::selectedOutputDatumChanged);
    connect(this, &GoogleMapsPointView::updateInitialZone, viewModel, &ViewModel::selectedInputZoneChanged);
    connect(this, &GoogleMapsPointView::updateFinalZone, viewModel, &ViewModel::selectedOutputZoneChanged);
    connect(this, &GoogleMapsPointView::updateGeographicFormat, viewModel, &ViewModel::setGeographicFormat);
    connect(this, &GoogleMapsPointView::convertPoint, viewModel, &ViewModel::startConverter);
    connect(viewModel, &ViewModel::updateFinalX, this, &GoogleMapsPointView::longitudeCalculated);
    connect(viewModel, &ViewModel::updateFinalY, this, &GoogleMapsPointView::latitudeCalculated);
    configureViewModel(x, y, z, coordinateSystem, datum, zone);


    if (!lat.isEmpty() && !lng.isEmpty()){
        QString tempCoord = lat + ", " + lng;
        loadMap(tempCoord);
    }
}

GoogleMapsPointView::~GoogleMapsPointView()
{
    delete ui;
}

void GoogleMapsPointView::latitudeCalculated(const QString value)
{
    QString temp = value;
    temp = temp.replace(',', '.');
    lat = "lat: " + temp;
}

void GoogleMapsPointView::longitudeCalculated(const QString value)
{
    QString temp = value;
    temp = temp.replace(',', '.');
    lng = "lng: " + temp;
}
