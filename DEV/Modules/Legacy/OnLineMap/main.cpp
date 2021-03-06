#include <QApplication>
#include <QObject>
#include <QSizePolicy>
#include <QScreen>

//#include "mainwindow.h"


#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <math.h>
#include <errno.h>
#include <sys/ioctl.h>
//using namespace std;

#include "mainwindow.h"
int Resolution_mode = 1; // 0 if          WIDTH >  2400px
                       // 1 if 1750px < WIDTH <= 2400 
                       // 2 if          WIDTH <= 1750



int main ( int argc, char **argv )
{
/////////////////////////////////////////SHM/////////////////////////////////////////////////////////////////

     QString ResMod;
     QString text="ScreenResolution";
      QFile file(text);
      if(file.exists())
      {
        file.open(QIODevice::ReadOnly);
        QString line;
         line = file.readLine();
         int numero=line.toInt();
         file.close();
         if (numero >  2400)                    {Resolution_mode=0; ResMod="high";}
         if (numero <  2400 && numero >= 1750)  {Resolution_mode=1; ResMod="medium";}
         if (numero <= 1750)                    {Resolution_mode=2; ResMod="low";}
      }
     else {qDebug()<<"ScreenResolution not defined....!!!\n"; exit ;}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QApplication a( argc, argv );
    MainWindow w;


    switch (Resolution_mode)
     {
      case(0):
             {
              w.resize( 860, 1110 );
              w.setMinimumSize( 860, 1110);
              w.setMaximumSize( 860, 1110);
              break;
              }
    case(1):
           {
            w.resize(645, 832 );
            w.setMinimumSize(645, 832);
            w.setMaximumSize(645, 832);
            break;
            }

    case(2):
           {
            w.resize( 430, 620 );
            w.setMinimumSize(430, 620);
            w.setMaximumSize(430, 620);
            break;
            }
      }



    w.show();

    return a.exec();
}


















