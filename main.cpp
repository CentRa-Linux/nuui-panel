#include "mainpanel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainpanel w;
    w.show();
    return a.exec();
}
