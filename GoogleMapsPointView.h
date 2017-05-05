#ifndef GOOGLEMAPSPOINTVIEW_H
#define GOOGLEMAPSPOINTVIEW_H
#include <QWidget>
#include <QtWebEngine/QtWebEngine>
#include <QtWebEngineWidgets>
#include "ViewModel.h"

using namespace QtWebEngineCore;

namespace Ui {
class GoogleMapsPointView;
}

class GoogleMapsPointView : public QWidget
{
    Q_OBJECT

private:
    Ui::GoogleMapsPointView *ui;
    QWebEngineView *view;
    ViewModel *viewModel;
    QString lat;
    QString lng;

    void configureViewModel(const QString x, const QString y, const QString z, const QString coordinateSystem, const QString datum, int zone = 0);
    void loadMap(const QString coordinateString);

public:
    explicit GoogleMapsPointView(const QString x, const QString y, const QString z, const QString coordinateSystem, const QString datum, int zone = 0, QWidget *parent = 0);
    ~GoogleMapsPointView();

signals:
    void updateInitialX(const QString value);
    void updateInitialY(const QString value);
    void updateInitialZ(const QString value);
    void updateInitialSystem(const QString value);
    void updateFinalSystem(const QString value);
    void updateInitialDatum(const QString value);
    void updateFinalDatum(const QString value);
    void updateInitialZone(int value);
    void updateFinalZone(int value);
    void updateGeographicFormat(const ViewModel::GeographicFormat format);
    void convertPoint();

private slots:
    void latitudeCalculated(const QString value);
    void longitudeCalculated(const QString value);
};

#endif // GOOGLEMAPSPOINTVIEW_H
