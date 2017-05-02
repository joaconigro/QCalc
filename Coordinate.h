#ifndef COORDINATE_H
#define COORDINATE_H

#include <QWidget>
#include <QString>
#include <QLocale>
#include "Datum.h"
#include "CoordinateSystem.h"

class Coordinate : public QWidget
{
    Q_OBJECT

public:
    enum class GeographicType{Decimal, GMS};

private:
    double initialX;
    double initialY;
    double initialZ;
    double finalX;
    double finalY;
    double finalZ;
    double latitude;
    double longitude;
    double elevation;
    Datum * initialDatum;
    Datum * destinationDatum;
    CoordinateSystem * initialSystem;
    CoordinateSystem * destinationSystem;
    GeographicType geographicFormat;

    Datum *setDatum(const QString name, double factor);    //configura el datum de origen y de destino.


    void changeDatum();
    void geographicToTransverseMercator(bool isFromOrigin);
    void transverseMercatorToGeographic(bool isFromOrigin);

public:
    explicit Coordinate(QWidget *parent = 0);  //Constructor por defecto
    explicit Coordinate(const double &x, const double &y, const double &z = 0.0);    //Constructor a partir de dos coordenadas planas

    double getInitialX();
    double getInitialY();
    double getInitialZ();
    void convertCoordinate();
    ~Coordinate();


signals:
    void xCalculated(const double x);
    void yCalculated(const double y);
    void zCalculated(const double z);

public slots:
    void setInitialDatum(const QString name);
    void setDestinationDatum(const QString name);
    Datum *getInitialDatum();
    Datum *getDestinationDatum();
    void setInitialSystem(const QString &name);
    void setDestinationSystem(const QString &name);
    void setInitialZone(const double &region, const CoordinateSystem::HemisphereType &hem = CoordinateSystem::HemisphereType::South);
    void setDestinationZone(const double &region, const CoordinateSystem::HemisphereType &hem = CoordinateSystem::HemisphereType::South);
    void setInitialX(const double x);
    void setInitialY(const double y);
    void setInitialZ(const double z = 0.0);
    void setInitialLatitude(const double lat);
    void setInitialLongitude(const double lng);
    void setInitialElevation(const double elev);
    void setGeographicFormat(const GeographicType &format);



};

#endif // COORDINATE_H
