#include "putty.h"
#include "dialogpopup.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Putty w;
    DialogPopup w;
    w.show();

    return a.exec();
}
