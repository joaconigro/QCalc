#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "ViewModel.h"

namespace Ui {
class mainWindow;
}

class mainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();

private:
    Ui::mainWindow *ui;
    ViewModel *viewModel;
    bool isGeographic;
    ViewModel::GeographicFormat geographicFormat;

signals:
     void changeGeographicFormat(const ViewModel::GeographicFormat format);

public slots:
    void updateFinalX(const QString xValue);
    void updateFinalY(const QString yValue);
    void updateFinalZ(const QString zValue);
    void inputSystemChanged(const QString name);
    void outputSystemChanged(const QString name);
    void updateInputZones(const QStringList list);
    void updateOutputZones(const QStringList list);
    void setGeographicFormat(const ViewModel::GeographicFormat format);
};

#endif // MAINWINDOW_H
