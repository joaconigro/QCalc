#include "CoordinateSystem.h"


CoordinateSystem::CoordinateSystem(const QString &name, const Datum *datum, const double &region, const HemisphereType &hem)
{
    if (name=="Gauss Krüger"){
        this->name = Id::Gauss_Kruger;
        if (datum != nullptr){
            changeDatum(datum);
        }

        flat = true;
        scaleFactor = 1.0;


    } else if (name == "UTM"){
        this->name = Id::UTM;
        flat = true;
        scaleFactor = 0.9996;

    }else if (name == "Geográficas"){
        this->name = Id::Geograficas;
        flat = false;

    }else {
        this->name = Id::Indefinido;
    }

    setZone(region, hem);
}

double CoordinateSystem::getFalseEast()
{
    return falseEast;
}

double CoordinateSystem::getFalseNorth()
{
    return falseNorth;
}

double CoordinateSystem::getCenterMeridian()
{
    return centerMeridian;
}

double CoordinateSystem::getScaleFactor()
{
    return scaleFactor;
}

double CoordinateSystem::getZone()
{
    return zone;
}

bool CoordinateSystem::isGeographic()
{
    return !flat;
}

QString CoordinateSystem::getName()
{
    switch (name) {
    case Id::UTM:
        return "UTM";
        break;
    case Id::Gauss_Kruger:
        return "Gauss Krüger";
        break;
    case Id::Geograficas:
        return "Geográficas";
        break;
    default:
        return "Indefinido";
        break;
    }
}

void CoordinateSystem::changeDatum(const Datum *datum)
{
    if (name == Id::Gauss_Kruger){
        if (datum->getName() == "WGS84") {
            falseNorth = 10001965.7;
        }else if (datum->getName() == "Campo Inchauspe"){
            falseNorth = 10002288.3;
        } else{
            falseNorth= 0.0;
        }
       }

}

void CoordinateSystem::setZone(const double &region, const HemisphereType &hem)
{
    switch (name) {
    case Id::Gauss_Kruger:
        centerMeridian = -75.0 + region * 3.0;
        falseEast = region * 1000000.0 + 500000.0;
        zone = region;
        break;
    case Id::UTM:
        centerMeridian = region * 6.0 - 183.0;
        falseEast = 500000.0;
        zone = region;
        this->hemisphere = hem;
        switch (hem) {
        case HemisphereType::North:
            falseNorth = 0.0;
            break;
        default:
            falseNorth= 10000000.0;
            break;
        }
        break;
    default:
        centerMeridian = 0.0;
        falseEast = 0.0;
        break;
    }
}
