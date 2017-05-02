#include "Coordinate.h"
#include "QtMath"

Datum *Coordinate::setDatum(const QString name, double factor)
{
    return new Datum(name, factor);
}

void Coordinate::changeDatum()
{

    //Parámetros de la transformacion
    double  dx, dy, dz, rx, ry, rz, s; //Parámetros del datum de origen
    double a, b;    //Parámetros del elipsoide de origen
    double a2, b2, e2, ep2; //Parámetros del elipsoide de salida
    initialDatum->extractData(dx, dy, dz, rx, ry, rz, s, a, b, a2, b2, e2, ep2, destinationDatum);
    e2 = pow(e2, 2.0);

    //Lista de variables
    double phi, lambda;
    double X, Y, Z, N;
    double Xn, Yn, Zn;
    double p, t;


    //Pasa la lat y long de grados a radianes
    phi = latitude * M_PI / 180.0;
    lambda = longitude * M_PI / 180.0;

    //Se calculan las coordenadas rectangulares
    N = pow(a, 2.0) / sqrt(pow(a, 2.0) * pow(cos(phi), 2.0) + pow(b, 2.0) * pow(sin(phi), 2.0));
    X = (N + elevation) * cos(phi) * cos(lambda);
    Y = (N + elevation) * cos(phi) * sin(lambda);
    Z = ((pow(b, 2.0) / pow(a, 2.0)) * N + elevation) * sin(phi);

    //Se transforma el datum (versión de Helmert)
    Xn = dx + (1.0 + s) * (X + rz * Y - ry * Z);
    Yn = dy + (1.0 + s) * (Y - rz * X + rx * Z);
    Zn = dz + (1.0 + s) * (ry * X - rx * Y + Z);

    //Se vuelve a coordenadas geográficas con el nuevo datum
    p = sqrt(pow(Xn, 2.0) + pow(Yn, 2.0));
    t = atan((Zn * a2) / (p * b2));
    latitude = (atan((Zn + ep2 * b2 * pow(sin(t), 3.0)) / (p - e2 * a2 * pow(cos(t), 3.0)))) * 180.0 / M_PI;
    longitude = (atan(Yn / Xn)) * 180.0 / M_PI;
    //La conversión de altura no funciona sin
    //tener la curvatura del geoide
    //elevation = (p * cos(latitude * 180.0 / M_PI)) + (Zn * sin(latitude * 180.0 / M_PI))
           // - (a2 * sqrt(1 - (e2 * pow(sin(latitude * 180.0 / M_PI), 2.0))));

}

void Coordinate::geographicToTransverseMercator(bool isFromOrigin)
{
    double phi, lambda;
    double phi_0  = 0.0;
    double T, A, C, M, M_0, v;
    double lambda_0, a, ecc, ecc_pri, FN, k, FE;

    if (isFromOrigin){
        lambda_0 = initialSystem->getCenterMeridian() * M_PI / 180.0;
        a = initialDatum->getSemiAxisA();
        ecc = initialDatum->getExentricity();
        ecc_pri = initialDatum->getExentricityPrime();
        FN = initialSystem->getFalseNorth();
        k = initialSystem->getScaleFactor();
        FE = initialSystem->getFalseEast();
    } else {
        lambda_0 = destinationSystem->getCenterMeridian() * M_PI / 180.0;
        a = destinationDatum->getSemiAxisA();
        ecc = destinationDatum->getExentricity();
        ecc_pri = destinationDatum->getExentricityPrime();
        FN = destinationSystem->getFalseNorth();
        k = destinationSystem->getScaleFactor();
        FE = destinationSystem->getFalseEast();
    }

    M_0 = a * ((1.0 - (pow(ecc, 2.0)) / 4.0 - (3.0 / 64.0) * pow(ecc, 4.0) - (5.0 / 256.0) * pow(ecc, 6.0)) * phi_0 - ((3.0 / 8.0) * pow(ecc, 2.0) + (3.0 / 32.0) * pow(ecc, 4.0) + (45.0 / 1024.0) * pow(ecc, 6.0)) * sin(2.0 * phi_0) + ((15.0 / 256.0) * pow(ecc, 4.0) + (45.0 / 1024.0) * pow(ecc, 6.0)) * sin(4.0 * phi_0) - ((35.0 / 3072.0) * pow(ecc, 6.0)) * sin(6.0 * phi_0));

    phi = latitude * M_PI / 180.0;
    lambda = longitude * M_PI / 180.0;
    T = pow((tan(phi)), 2.0);
    C = (pow(ecc, 2.0) * pow((cos(phi)), 2.0)) / (1.0 - pow(ecc, 2.0));
    A = (lambda - lambda_0) * cos(phi);
    v = a / sqrt(1.0 - pow(ecc, 2.0) * pow((sin(phi)), 2.0));
    M = a * ((1.0 - (pow(ecc, 2.0)) / 4.0 - (3.0 / 64.0) * pow(ecc, 4.0) - (5.0 / 256.0) * pow(ecc, 6.0)) * phi - ((3.0 / 8.0) * pow(ecc, 2.0) + (3.0 / 32.0) * pow(ecc, 4.0) + (45.0 / 1024.0) * pow(ecc, 6.0)) * sin(2.0 * phi) + ((15.0 / 256.0) * pow(ecc, 4.0) + (45.0 / 1024.0) * pow(ecc, 6.0)) * sin(4.0 * phi) - ((35.0 / 3072.0) * pow(ecc, 6.0)) * sin(6.0 * phi));
    finalX = FE + k * v * (A + (1.0 - T + C) * pow(A, 3.0) / 6.0 + (5.0 - 18.0 * T + pow(T, 2.0) + 72.0 * C - 58.0 * pow(ecc_pri, 2.0)) * pow(A, 5.0) / 120.0);
    finalY = FN + k * (M - M_0 + v * tan(phi) * (pow(A, 2.0) / 2.0 + (5.0 - T + 9.0 * C + 4.0 * pow(C, 2.0)) * pow(A, 4.0) / 24.0 + (61.0 - 58.0 * T + pow(T, 2.0) + 600.0 * C - 330.0 * pow(ecc_pri, 2.0)) * pow(A, 6.0) / 720.0));
    finalZ = elevation;

}

void Coordinate::transverseMercatorToGeographic(bool isFromOrigin)
{

    double phi, lambda;
    double phi_0  = 0.0;
    double T_1, mu_1, e_1, phi_1, rho, D, C_1, M_1, M_0, v_1;
    double lambda_0, a, ecc, ecc_pri, FN, k, FE;

    if (isFromOrigin){
        lambda_0 = initialSystem->getCenterMeridian() * M_PI / 180.0;
        a = initialDatum->getSemiAxisA();
        ecc = initialDatum->getExentricity();
        ecc_pri = initialDatum->getExentricityPrime();
        FN = initialSystem->getFalseNorth();
        k = initialSystem->getScaleFactor();
        FE = initialSystem->getFalseEast();
    } else {
        lambda_0 = destinationSystem->getCenterMeridian() * M_PI / 180.0;
        a = destinationDatum->getSemiAxisA();
        ecc = destinationDatum->getExentricity();
        ecc_pri = destinationDatum->getExentricityPrime();
        FN = destinationSystem->getFalseNorth();
        k = destinationSystem->getScaleFactor();
        FE = destinationSystem->getFalseEast();
    }

    double Este = initialX;
    double Norte = initialY;

    M_0 = a * ((1.0 - (pow(ecc, 2.0)) / 4.0 - (3.0 / 64.0) * pow(ecc, 4.0) - (5.0 / 256.0) * pow(ecc, 6.0)) * phi_0 - ((3.0 / 8.0) * pow(ecc, 2.0) + (3.0 / 32.0) * pow(ecc, 4.0) + (45.0 / 1024.0) * pow(ecc, 6.0)) * sin(2.0 * phi_0) + ((15.0 / 256.0) * pow(ecc, 4.0) + (45.0 / 1024.0) * pow(ecc, 6.0)) * sin(4.0 * phi_0) - ((35.0 / 3072.0) * pow(ecc, 6.0)) * sin(6.0 * phi_0));
    e_1 = (1.0 - sqrt(1.0 - pow(ecc, 2.0))) / (1.0 + sqrt(1.0 - pow(ecc, 2.0)));

    M_1 = M_0 + (Norte - FN) / k;
    mu_1 = M_1 / (a * (1.0 - pow(ecc, 2.0) / 4.0 - (3.0 / 64.0) * pow(ecc, 4.0) - (5.0 / 256.0) * pow(ecc, 6.0)));
    phi_1 = mu_1 + ((3.0 / 2.0) * e_1 - (27.0 / 32.0) * pow(e_1, 3.0)) * sin(2.0 * mu_1) + ((21.0 / 16.0) * pow(e_1, 2.0) - (55.0 / 32.0) * pow(e_1, 4.0)) * sin(4.0 * mu_1) + (151.0 / 96.0) * pow(e_1, 3.0) * sin(6.0 * mu_1) + (1097.0 / 512.0) * pow(e_1, 4.0) * sin(8.0 * mu_1);
    T_1 = pow((tan(phi_1)), 2.0);
    C_1 = ecc_pri * pow((cos(phi_1)), 2.0);
    v_1 = a / sqrt(1.0 - pow(ecc, 2.0) * pow((sin(phi_1)), 2.0));
    rho = a * (1.0 - pow(ecc, 2.0)) / pow((1.0 - pow(ecc, 2.0) * pow((sin(phi_1)), 2.0)), 1.5);
    D = (Este - FE) / (v_1 * k);
    phi = phi_1 - (v_1 * tan(phi_1) / rho) * (pow(D, 2.0) / 2.0 - (5.0 + 3.0 * T_1 + 10.0 * C_1 - 4.0 * pow(C_1, 2.0) - 9.0 * ecc_pri) * pow(D, 4.0) / 24.0 + (61.0 + 90.0 * T_1 + 298.0 * C_1 + 45.0 * pow(T_1, 2.0) - 252.0 * ecc_pri - 3.0 * pow(C_1, 2.0)) * pow(D, 6.0) / 720.0);
    lambda = lambda_0 + (D - (1.0 + 2.0 * T_1 + C_1) * pow(D, 3.0) / 6.0 + (5.0 - 2.0 * C_1 + 28.0 * T_1 - 3.0 * pow(C_1, 2.0) + 8.0 * ecc_pri + 24.0 * pow(T_1, 2.0)) * pow(D, 5.0) / 120.0) / cos(phi_1);

    latitude = phi * 180.0 / M_PI;
    longitude = lambda * 180.0 / M_PI;
    elevation = initialZ;

}

Coordinate::Coordinate(QWidget *parent) : QWidget(parent)
{
    initialX = finalX = initialY = finalY = initialZ = finalZ = elevation = latitude = longitude = 0.0;
    initialDatum = nullptr;
    initialSystem = nullptr;
    destinationDatum = nullptr;
    destinationSystem = nullptr;
}

Coordinate::Coordinate(const double & x, const double & y, const double &z)
{
    initialX = x;
    initialY = y;
    initialZ = z;
}

double Coordinate::getInitialX()
{
    return initialX;
}

double Coordinate::getInitialY()
{
    return initialY;
}

double Coordinate::getInitialZ()
{
    return initialZ;
}

void Coordinate::convertCoordinate()
{
    if (!(initialSystem->isGeographic())){
      transverseMercatorToGeographic(true);
    }
    if (initialDatum->getName() != destinationDatum->getName()){
        changeDatum();
    }
    if (!(destinationSystem->isGeographic())){
        geographicToTransverseMercator(false);
        emit xCalculated(finalX);
        emit yCalculated(finalY);
        emit zCalculated(finalZ);
    } else {
        emit xCalculated(longitude);
        emit yCalculated(latitude);
        emit zCalculated(elevation);
    }

}



Coordinate::~Coordinate()
{
    delete initialDatum;
    delete destinationDatum;
    delete initialSystem;
    delete destinationSystem;
}





Datum *Coordinate::getInitialDatum()
{
    return initialDatum;
}

Datum *Coordinate::getDestinationDatum()
{
    return destinationDatum;
}

void Coordinate::setInitialSystem(const QString &name)
{
    initialSystem = new CoordinateSystem(name, initialDatum);
}

void Coordinate::setDestinationSystem(const QString &name)
{
    destinationSystem  = new CoordinateSystem(name, destinationDatum);
}

void Coordinate::setInitialZone(const double &region, const CoordinateSystem::HemisphereType &hem)
{
    if (initialSystem != nullptr) {
        this->initialSystem->setZone(region, hem);
    }
}

void Coordinate::setDestinationZone(const double &region, const CoordinateSystem::HemisphereType &hem)
{
    if (destinationSystem != nullptr) {
        this->destinationSystem->setZone(region, hem);
    }

}

void Coordinate::setInitialX(const double x)
{
    initialX = x;
}

void Coordinate::setInitialY(const double y)
{
    initialY = y;
}

void Coordinate::setInitialZ(const double z)
{
    initialZ = z;
}

void Coordinate::setInitialLatitude(const double lat)
{
    latitude = lat;
}

void Coordinate::setInitialLongitude(const double lng)
{
    longitude = lng;
}

void Coordinate::setInitialElevation(const double elev)
{
    elevation = elev;
}

void Coordinate::setGeographicFormat(const GeographicType &format)
{
    geographicFormat = format;
}

void Coordinate::setInitialDatum(const QString name)
{
    initialDatum = setDatum(name, 1.0);
    if (initialSystem != nullptr) {
        initialSystem->changeDatum(initialDatum);
    }

}

void Coordinate::setDestinationDatum(const QString name)
{
    destinationDatum = setDatum(name, -1.0);
    if (destinationSystem != nullptr) {
        destinationSystem->changeDatum(destinationDatum);
    }

}
