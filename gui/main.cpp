#include "mintty.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mintty w;
    w.show();

    return a.exec();
}
