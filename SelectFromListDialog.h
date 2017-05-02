#ifndef SELECTFROMLISTDIALOG_H
#define SELECTFROMLISTDIALOG_H
#include <QStringList>
#include <QDialog>

namespace Ui {
class SelectFromListDialog;
}

class SelectFromListDialog : public QDialog
{
    Q_OBJECT

private:
    QString selectedSheet;
    QStringList sheetsList;

public:
    explicit SelectFromListDialog(const QStringList list, QWidget *parent = 0);
    ~SelectFromListDialog();

private slots:
    void on_listWidget_itemSelectionChanged();

    void on_buttonBox_accepted();

signals:
    void selectedSheetChanged(const QString sheetName);

private:
    Ui::SelectFromListDialog *ui;
};

#endif // SELECTFROMLISTDIALOG_H
