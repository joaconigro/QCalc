#ifndef COLUMNSELECTDIALOG_H
#define COLUMNSELECTDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class ColumnSelectDialog;
}

class ColumnSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColumnSelectDialog(const QStringList list, QWidget *parent = 0);
    ~ColumnSelectDialog();

signals:
    void updateXColumn(int value);
    void updateYColumn(int value);
    void updateZColumn(int value);
    void configureColumns(bool value);

private slots:
    void on_XComboBox_currentIndexChanged(int index);
    void on_YComboBox_currentIndexChanged(int index);
    void on_ZComboBox_currentIndexChanged(int index);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    QStringList columnList;
    Ui::ColumnSelectDialog *ui;

    void showEvent(QShowEvent* event);
    void autoSelectColumns();

};

#endif // COLUMNSELECTDIALOG_H
