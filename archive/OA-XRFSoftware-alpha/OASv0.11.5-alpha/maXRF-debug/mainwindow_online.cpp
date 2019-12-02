#include "mainwindow.h"
#include <QtGui>

using namespace std;


/* Variabili e funzioni definite altrove ma non necessarie
/////////variabili e funzioni definite altrove ma necessarie/////////////////////////
extern bool OnLine,MapIsOpened;
extern int OffsetX, OffsetY,PixelX,PixelY,Pixeldim,point,StoredPoint,IntR,IntG,IntB;
extern int *(shared_memory), *(shared_memory_cmd);
extern int i,j;
extern double X[20000],Y[20000];
extern double Integral[20000],MaxIntegral,MinIntegral;
extern QColor myColor;
///////////////////////////////////////////////////////////////////////////////////////
*/

extern int *(shared_memory_cmd);
extern int StoredPoint;

extern bool MapIsOpened;

void MainWindow::GoOnLine() {
    if (*(shared_memory_cmd+75) == 1) {
        *(shared_memory_cmd+75) = 0;
        StoredPoint=0;
        hideImage();
    }
    else {
        MapIsOpened=true;
        qDebug()<<"... Live-Map is active";
        *(shared_memory_cmd+75)=1;
        system("./app-modules/OnLineMap & ");
    }
}




