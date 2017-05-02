#ifndef COORDINATESYSTEM_H
#define COORDINATESYSTEM_H

#include <QWidget>
#include <QString>
#include "Datum.h"

class CoordinateSystem : public QWidget
{
    Q_OBJECT

public:
    enum class Id{Indefinido, Geograficas, UTM, Gauss_Kruger};
    enum class HemisphereType{South, North};

private:
    bool flat;
    Id name;
    double falseEast;
    double falseNorth;
    double centerMeridian;
    double zone;
    double scaleFactor;
    HemisphereType hemisphere;



public:
    CoordinateSystem(const QString &name, const Datum *datum = nullptr, const double &region=1.0, const HemisphereType &hem = HemisphereType::South);

    double getFalseEast();
    double getFalseNorth();
    double getCenterMeridian();
    double getScaleFactor();
    double getZone();
    bool isGeographic();
    QString getName();
    void changeDatum(const Datum *datum);

    void setZone(const double &region, const HemisphereType &hem = HemisphereType::South);

};

#endif // COORDINATESYSTEM_H
