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

signals:
     void changeGeographicFormat(const ViewModel::GeographicFormat format);

public slots:
    void openExcelFile();
    void sheetListAccepted(const QString sheetName);
//    void updateFinalX(const QString xValue);
//    void updateFinalY(const QString yValue);
//    void updateFinalZ(const QString zValue);
    void inputSystemChanged(const QString name);
    void outputSystemChanged(const QString name);
    void updateInputZones(const QStringList list);
    void updateOutputZones(const QStringList list);
    void setGeographicFormat(const ViewModel::GeographicFormat format);
};

#endif // TABLEVIEW_H
