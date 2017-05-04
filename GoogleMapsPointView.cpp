#include "GoogleMapsPointView.h"
#include "ui_GoogleMapsPointView.h"
#include <QtWebEngine/QtWebEngine>

GoogleMapsPointView::GoogleMapsPointView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GoogleMapsPointView)
{
    ui->setupUi(this);
}

GoogleMapsPointView::~GoogleMapsPointView()
{
    delete ui;
}
