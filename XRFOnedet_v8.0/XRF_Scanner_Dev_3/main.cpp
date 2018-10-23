////////////////////////////////////////////////////// FRAO 2016 - November /////////////////////////////////////
//
//         CHNet - INFN (XRF_SCANNER version 4+)
//
//     ChangeLog: main differences vs. version 3
//                                                                   
//     1 - Screen Resolution is detected and the GUI is built depending on the screen size  
//     2 - Embedded MapOnline has been removed. Now MapOnLine is available as external program 
//     3 - Rate Meter implemented
//     4 - ADCXRF (Caen driver) was modified for the new MapOnLine and RateMeter programs:
//     4.1 - Dedicated shared memory for Rate Meter: *shared memory+39  
//     5 - XRF9 (Spectrum Viewer) was modified and new functions are available:
//     5.1 - Variable resolution: 16k (default), 8k and 4k (only for ADC channels) 
//     5.2 - AutoScale can be activated via CheckBox
//     
//         CHNet - INFN (XRF_SCANNER version 5+)     
//
//     1 - Screen: low resolution can be forced when the program starts 
//     2 - File ui_MainWindow.h has been modified to resize the canvas depending on resolution. A copy of the file is ui_MainWindow.h.screen 
//     3 - 
//
//



#include <QApplication>
#include <QObject>
#include <QSizePolicy>
#include "mainwindow.h"
#include <sys/types.h>
#include <sys/time.h>		/* for setitimer */
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>
#include <QScreen>
#include <time.h>
#include <QTextStream>
#include <QString>
#include <QFile>
#include <unistd.h>		/* for pause */
#include <signal.h>		/* for signal */


int Resolution_mode; // 0 if          WIDTH >  2400px
                       // 1 if 1750px < WIDTH <= 2400 
                       // 2 if          WIDTH <= 1750

int timeout ( int seconds )
{
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}

    return  1;
}



int main(int argc, char *argv[])
{
//qDebug()<< "for optimised screen resolution: type 0 --- for low resolution: type 1";


/*    QTextStream s(stdin); 
    QString value = s.readLine();
    if( timeout(5) == 1 ){printf("Time Out\n");
                //return 0;
       } */


    qDebug()<< ".....Detecting screen resolution.....";

//    QTextStream s(stdin); 
//    QString value = s.readLine();
//    int toculo=value.toInt();
//    qDebug()<<toculo;

     QString ResMod;
     system("./ScreenDetector");
     Sleeper::msleep(100);

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

/*if (toculo==1) 
     {Resolution_mode=2; ResMod="low";

     QString text="ScreenResolution";
      QFile file(text);
      if(file.exists())
      {
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out << "1600\n"<<"1200\n"; 
      }
      file.close(); 
      qDebug()<<"Resolution Config file rewritten: 1600X1200 "<<ResMod;
     }

     qDebug()<<"ResolutionMode: "<<ResMod;
*/
     Sleeper::msleep(100);


////// LOOKING FOR CAEN USB and A3818 DRIVER

     system("rm -f modules");
     system("cp /proc/modules .");
     Sleeper::msleep(100);
     qDebug()<<"Looking for loaded modules....";
     Sleeper::msleep(100);
     int module=0;
//     const QString searchString("CAENUSBdrvB");

     QString content;
     QString text2="modules";
     QFile file2(text2);
      if(file2.exists())
      {
        file2.open(QIODevice::ReadOnly);
        while(!file2.atEnd())
        {content = file2.readLine();
          if (content.contains("CAENUSBdrvB", Qt::CaseSensitive)) {module=1;}  //(searchString, Qt::CaseSensitive)) 
        }
      }
      file2.close();

      if(module==0){qDebug()<<"CAEN USB module not found....  module_value: "<< module;}
      if(module==1){qDebug()<<"CAENUSBdrvB module already loaded....  module_value: "<< module;}
       
      Sleeper::msleep(100);

//     Q_INIT_RESOURCE(application);


      switch (module)
       {
        case(0): {system("su -c 'sysctl -w kernel.shmmax=320000000;sysctl -w kernel.shmall=720000000; chmod 777 /dev/ttyUSB*;  chmod 777 /dev/ttyACM*; chown frao:frao /dev/ttyACM*; chown frao:frao /dev/ttyUSB*; insmod Program_Data_Files/CAENUSBdrvB.ko.xz;'"); break;}
        case(1): {system("su -c 'sysctl -w kernel.shmmax=320000000;sysctl -w kernel.shmall=720000000; chmod 777 /dev/ttyUSB*;  chmod 777 /dev/ttyACM*; chown frao:frao /dev/ttyACM*; chown frao:frao /dev/ttyUSB*;'"); break;}
//        case(2): {A3818 case; break;}
       }
   

      QApplication a(argc, argv);
      MainWindow w;

      switch (Resolution_mode)
       {
        case(0): {w.resize( 2420, 1080 ); break;}
        case(1): {w.resize( 1815, 810  ); break;}
        case(2): {w.resize( 1210, 540  ); break;}
       }

       w.show();
    
return a.exec(); 
}






