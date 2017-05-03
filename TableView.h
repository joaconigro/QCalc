#ifndef TABLEVIEW_H
#define TABLEVIEW_H
#include "xlsxdocument.h"
#include <QWidget>
#include "ViewModel.h"

namespace Ui {
class TableView;
}

class TableView : public QWidget
{
    Q_OBJECT

public:
    explicit TableView(QWidget *parent = 0);
    ~TableView();

private:
    Ui::TableView *ui;
    QXlsx::Document *xlsx;
    ViewModel *viewModel;
    bool isGeographic;
    ViewModel::GeographicFormat geographicFormat;
    QStringList headers;
    int columnX;
    int columnY;
    int columnZ;
    bool columnsConfigured;
    int currentRow;

signals:
     void changeGeographicFormat(const ViewModel::GeographicFormat format);
     void convertPoint();
     void updateInitialX(const QString value);
     void updateInitialY(const QString value);
     void updateInitialZ(const QString value);


public slots:
    void openExcelFile();
    void sheetListAccepted(const QString sheetName);
    void updateFinalX(const QString xValue);
    void updateFinalY(const QString yValue);
    void updateFinalZ(const QString zValue);
    void inputSystemChanged(const QString name);
    void outputSystemChanged(const QString name);
    void updateInputZones(const QStringList list);
    void updateOutputZones(const QStringList list);
    void setGeographicFormat(const ViewModel::GeographicFormat format);
private slots:
    void on_columnsButton_clicked();
    void onXColumnChanged(int value);
    void onYColumnChanged(int value);
    void onZColumnChanged(int value);
    void onColumnsConfigured(bool value);
    void on_convertButton_clicked();
};

#endif // TABLEVIEW_H
