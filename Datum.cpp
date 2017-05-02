#include "Datum.h"
#include "QtMath"

//Metodos para la clase Datum
void Datum::setDatum(const double & factor)
{
    switch (name) {
    case Id::Campo_Inchauspe:
        //Parámetros para datum Campo Inchauspe --> WGS84. En caso de WGS84 --> Campo Inchauspe, se deben
        //multiplicar los parámetros por -1.
        dX = -149.1 * factor;
        dY = 138.1 * factor;
        dZ = 90.9 * factor;
        rX = -0.000000688435427175541 * factor;
        rY =  0.0000016677590630168 * factor;
        rZ = 0.0000014592891801397 * factor;
        scale = 0.000000388 * factor;
        break;
    default:
        //En caso de datum no definido o WGS84, los parámetros son 0.
        dX = dY = dZ = rX = rY = rZ = scale = 0.0;
        break;
    }

    setEllipsoid();
}

void Datum::setEllipsoid()
{
    //Configura los parámetros del elipsiode en base al
    //datum elegido
    switch (name) {
    case Id::WGS84:
        //Elipsoide WGS84
        semiAxisA = 6378137.0;
        semiAxisB =  6356752.31424518;
        exentricity = 0.081819190843;
        exentricityPrime = pow(0.082094437950, 2.0);
        break;
    case Id::Campo_Inchauspe:
        //Elipsoide Hayford
        semiAxisA = 6378388.0;
        semiAxisB =   6356911.94612795;
        exentricity = 0.081991889979;
        exentricityPrime = pow(0.082268889607, 2.0);
        break;
    default:
        //No hay elipsoide definido
        semiAxisA = 0.0;
        semiAxisB =   0.0;
        exentricity = 0.0;
        exentricityPrime = 0.0;
        break;
    }
}

Datum::Datum(const QString &name, double factor)
{

    if (name=="Campo Inchauspe"){
        this->name = Id::Campo_Inchauspe;
    } else if (name == "WGS84"){
        this->name = Id::WGS84;
    }else {
        this->name = Id::Indefinido;
    }

    setDatum(factor);

}

Datum::Datum(const int & code, double factor)
{
    name = (Id)code;

    setDatum(factor);
}

void Datum::extractData(double &dx, double &dy, double &dz, double &rx, double &ry, double &rz, double &s, double &a, double &b, double &a2, double &b2, double &e2, double &ep2, Datum * destination) const
{
    dx = destination->getDX();
    dy = destination->getDY();
    dz = destination->getDZ();
    rx = destination->getRX();
    ry = destination->getRY();
    rz = destination->getRZ();
    s = destination->getScale();
    a = semiAxisA;
    b = semiAxisB;
    a2 = destination->getSemiAxisA();
    b2 = destination->getSemiAxisB();
    e2 = destination->getExentricity();
    ep2 = destination->getExentricityPrime();
}

QString Datum::getName() const
{
    switch (name) {
    case Id::WGS84:
        return "WGS84";
        break;
    case Id::Campo_Inchauspe:
        return "Campo Inchauspe";
        break;
    default:
        return "Indefinido";
        break;
    }
}

bool Datum::operator !=(const Datum &right) const
{
    return !(this->name == right.name);
}

bool Datum::operator ==(const Datum &right) const
{
    return (this->name == right.name);
}

double Datum::getSemiAxisA()
{
    return semiAxisA;
}

double Datum::getSemiAxisB()
{
    return semiAxisB;
}

double Datum::getExentricity()
{
    return exentricity;
}

double Datum::getExentricityPrime()
{
    return exentricityPrime;
}

double Datum::getDX()
{
    return dX;
}

double Datum::getDY()
{
    return dY;
}

double Datum::getDZ()
{
    return dZ;
}

double Datum::getRX()
{
    return rX;
}

double Datum::getRY()
{
    return rY;
}

double Datum::getRZ()
{
    return rZ;
}

double Datum::getScale()
{
    return scale;
}

