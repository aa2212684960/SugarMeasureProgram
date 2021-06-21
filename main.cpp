#include "SugarMeasure.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SugarMeasure w;
    w.show();
    return a.exec();
}
