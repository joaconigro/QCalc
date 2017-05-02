#ifndef DATUM_H
#define DATUM_H

#include <QWidget>
#include <QString>

//Clase para generar los datums
class Datum : public QWidget
{
    Q_OBJECT

public:
    enum class Id{Indefinido, WGS84, Campo_Inchauspe};

private:
    //Parámetros del datum
    double dX;
    double dY;
    double dZ;
    double rX;
    double rY;
    double rZ;
    double scale;
    Id name;

    //Parámetros del elipsoide
    double semiAxisA;
    double semiAxisB;
    double exentricity;
    double exentricityPrime;

    //Métodos privados
    void setDatum(const double &factor);
    void setEllipsoid();


public:
   Datum(const QString &name = "no_def", double factor = 1.0);
   Datum(const int & code = 0, double factor = 1.0);
   void extractData(double &dx, double &dy, double &dz, double &rx, double &ry, double &rz, double &s, double &a, double &b, double &a2, double &b2, double &e2, double &ep2, Datum *destination) const;
   QString getName() const;
   bool operator !=(const Datum &right) const;
   bool operator ==(const Datum &right) const;
   double getSemiAxisA();
   double getSemiAxisB();
   double getExentricity();
   double getExentricityPrime();
   double getDX();
   double getDY();
   double getDZ();
   double getRX();
   double getRY();
   double getRZ();
   double getScale();

};

#endif // DATUM_H



