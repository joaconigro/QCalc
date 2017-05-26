#ifndef ABOUTQCALC_H
#define ABOUTQCALC_H

#include <QWidget>

namespace Ui {
class AboutQCalc;
}

class AboutQCalc : public QWidget
{
    Q_OBJECT

public:
    explicit AboutQCalc(QWidget *parent = 0);
    ~AboutQCalc();

private:
    Ui::AboutQCalc *ui;
};

#endif // ABOUTQCALC_H
