#include "IoTivityDialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IoTivityDialog w;
    w.show();

    return a.exec();
}
