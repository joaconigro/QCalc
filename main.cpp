#include "MainVentana.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainVentana mv;
    QIcon icono(":/icon.ico");
    mv.setWindowIcon(icono);
    mv.show();

    return a.exec();
}
