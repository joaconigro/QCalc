#ifndef GOOGLEMAPSPOINTVIEW_H
#define GOOGLEMAPSPOINTVIEW_H

#include <QWidget>


namespace Ui {
class GoogleMapsPointView;
}

class GoogleMapsPointView : public QWidget
{
    Q_OBJECT

private:

public:
    explicit GoogleMapsPointView(QWidget *parent = 0);
    ~GoogleMapsPointView();

private:
    Ui::GoogleMapsPointView *ui;
};

#endif // GOOGLEMAPSPOINTVIEW_H
