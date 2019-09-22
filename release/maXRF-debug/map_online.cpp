#include "mainwindow.h"
#include <QtGui>

extern int *shared_memory_cmd;
extern bool MapIsOpened;

void MainWindow::GoOnLine() {
    if (*(shared_memory_cmd+75) == 1) {
        *(shared_memory_cmd+75) = 0;
        hideImage();
    }
    else {
        MapIsOpened=true;
        qDebug()<<"... Live-Map is active";
        *(shared_memory_cmd+75)=1;
        system("./app-modules/OnLineMap & ");
    }
}




