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
    void clearOnePoint();

private:
    Ui::MainVentana *ui;
    mainWindow *onePoint;
    TableView *manyPoints;

    //Menúes
    QMenu *fileMenu;
    QMenu *onePointMenu;
    QMenu *tableMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    QMenu *geographicFormatMenu;

    //Actions
    QAction *quitAction;
    QAction *viewGoogleMapsAction;
    QAction *clearOnePointAction;
    QAction *openExcelFileAction;
    QAction *viewOriginalDataAction;
    QAction *columnsAction;
    QAction *saveTableAction;
    QAction *decimalFormatAction;
    QAction *gmsFormatAction;
    QAction *viewHelpAction;
    QAction *aboutAction;
    QActionGroup *geographicFormatGroup;

    void createMenus();
    void createActions();

private slots:
    void onGmsTriggered();
    void onDecimalTriggered();
    void onExcelFileOpened();
    void viewHelp();
    void aboutQCalc();
    void tabChanged(int index);

public slots:
    void tableOpened(bool opened);
};

#endif // MAINVENTANA_H
