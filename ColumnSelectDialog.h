#ifndef COLUMNSELECTDIALOG_H
#define COLUMNSELECTDIALOG_H

#include <QDialog>

namespace Ui {
class ColumnSelectDialog;
}

class ColumnSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColumnSelectDialog(QWidget *parent = 0);
    ~ColumnSelectDialog();

private:
    Ui::ColumnSelectDialog *ui;
};

#endif // COLUMNSELECTDIALOG_H
