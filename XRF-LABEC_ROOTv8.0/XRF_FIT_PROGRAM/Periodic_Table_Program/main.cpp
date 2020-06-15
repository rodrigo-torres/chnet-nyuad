#include <QApplication>

#include "Periodic_Table.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Periodic_Table tavola;
    tavola.show();
    return app.exec();
}
