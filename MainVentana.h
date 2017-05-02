#ifndef MAINVENTANA_H
#define MAINVENTANA_H

#include <QMainWindow>
#include "ViewModel.h"
#include "OnePoint.h"
#include "TableView.h"
#include <QAction>
#include <QMenu>
#include <QActionGroup>

namespace Ui {
class MainVentana;
}

class MainVentana : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainVentana(QWidget *parent = 0);
    ~MainVentana();

signals:
    void geographicFormatChanged(const ViewModel::GeographicFormat format);
    void loadExcelFile();

private:
    Ui::MainVentana *ui;
    mainWindow *onePoint;
    TableView *manyPoints;


    QMenu *fileMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QMenu *geographicFormatMenu;
    QAction *openExcelFile;
    QAction *decimalFormatAction;
    QAction *gmsFormatAction;
    QActionGroup *geographicFormatGroup;

    void createMenus();
        void createActions();

private slots:
    void onGmsTriggered();
    void onDecimalTriggered();
    void onExcelFileOpened();
};

#endif // MAINVENTANA_H
