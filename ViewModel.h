#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QWidget>
#include <QStringList>
#include "Coordinate.h"

class ViewModel : public QWidget
{
    Q_OBJECT
public:
    enum class GeographicFormat{GMS, Decimal};

private:
    QStringList availableSystem;
    QStringList availableDatum;
    QStringList availableUTMZone;
    QStringList availableGKZone;
    bool setInputAsGeographic;
    bool setOutputAsGeographic;
    double initialX;
    double initialY;
    double initialZ;
    QString initialDatum;
    QString finalDatum;
    QString initialSystem;
    QString finalSystem;
    QString initialZone;
    QString finalZone;
    GeographicFormat geographicFormat;

    void genereteAvailableUTMZones();
    void genereteAvailableGKZones();


    QString setOutputAsGSM(const double value, bool isLatitude);

public:
    explicit ViewModel(QWidget *parent = 0);
    QStringList availableSystems() const;
    QStringList availableDatums() const;
    Coordinate *convertPoint;
    static double validateGeographicGMSCoordinate(const QString &value);

signals:
    void updateFinalX(const QString xValue);
    void updateFinalY(const QString yValue);
    void updateFinalZ(const QString zValue);
    void updateInputZoneList(const QStringList zoneList);
    void updateOutputZoneList(const QStringList zoneList);
    void initialDatumChanged(const QString value);
    void finalDatumChanged(const QString value);
    void initialSystemChanged(const QString value);
    void finalSystemChanged(const QString value);
    void initialZoneChanged(const double region, const CoordinateSystem::HemisphereType hem);
    void finalZoneChanged(const double region, const CoordinateSystem::HemisphereType hem);



public slots:
    void startConverter();
    void initialXChanged(const QString xValue);
    void initialYChanged(const QString yValue);
    void initialZChanged(const QString zValue);
    void finalXChanged(const double xValue);
    void finalYChanged(const double yValue);
    void finalZChanged(const double zValue);
    void selectedInputSystemChanged(const QString name);
    void selectedOutputSystemChanged(const QString name);
    void selectedInputDatumChanged(const QString name);
    void selectedOutputDatumChanged(const QString name);
    void selectedInputZoneChanged(const int region);
    void selectedOutputZoneChanged(const int region);
    void setGeographicFormat(const GeographicFormat format);


};

#endif // VIEWMODEL_H
