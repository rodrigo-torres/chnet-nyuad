#include "mainwindow.h"
#include "../Header.h"
#include <QSizePolicy>
#include <QScreen>

// Inherited from external program ScreenDetector
#include <QWidget>
#include <QThread>
#include <QDesktopWidget>

int Resolution_mode;

int authentication() {
    // Please do replace rtorres with your computer's username

    /*switch (module) , calling for insmod has no effect in Fedora, the appropriate modules are automatically loaded if the digitizer is detected and the appropriate drivers are installed.
       {
        case(0): {system("su -c 'sysctl -w kernel.shmmax=2000000000;sysctl -w kernel.shmall=720000000; chmod 777 /dev/ttyUSB*;  chmod 777 /dev/ttyACM*; chown rtorres:rtorres /dev/ttyACM*; chown rtorres:rtorres /dev/ttyUSB*; insmod Program_Data_Files/CAENUSBdrvB.ko.xz;'"); break;}
        case(1): {system("su -c 'sysctl -w kernel.shmmax=2000000000;sysctl -w kernel.shmall=720000000; chmod 777 /dev/ttyUSB*;  chmod 777 /dev/ttyACM*; chown rtorres:rtorres /dev/ttyACM*; chown rtorres:rtorres /dev/ttyUSB*;'"); break;}
       }
     */

    /* The default values for shmmax and shmall are usually adequate, the program when using all available resources makes use of no more than 1.26 GB, well under the default limit for shmmax for Fedora
     * system("su -c 'sysctl -w kernel.shmmax=2000000000; sysctl -w kernel.shmall=720000000;'");
     */

    int error_ACM = system("chmod +rw /dev/ttyUSB* 2>/dev/null; chown rtorres:rtorres /dev/ttyUSB* 2>/dev/null;");
    int error_USB = system("chmod +rw /dev/ttyACM* 2>/dev/null; chown rtorres:rtorres /dev/ttyACM* 2>/dev/null;");

    // Please note that system() returns error codes multiplied by 256, so error code 1 -> 256, and 0 -> 0
    int error_auth = error_ACM*2 + error_USB;
    return error_auth;
}

int setresolution()
{
    qDebug()<< "... Detecting screen resolution";

    /* system("./ScreenDetector") , the functionality of this external program has been implemented in the code below.
     * If you run into issues, please comment out this line, and uncomment the relevant lines in the else clause below.
     */

    //Sleeper::msleep(500);

    QString Dimensions;
    QFile file("Resolution");
    QString line;

    if(file.exists())
      {
          file.open(QIODevice::ReadOnly);
          line = file.readLine();
          int numero=line.toInt();
          file.close();
          if (numero >  2400)                    {Resolution_mode=0; Dimensions="2420x1080";}
          if (numero <  2400 && numero >= 1200)  {Resolution_mode=1; Dimensions="1815x810";}
          if (numero <= 1200)                    {Resolution_mode=2; Dimensions="1210x540";}

          qDebug()<<"... We're setting your window dimensions at : "<<Dimensions<<" px";
          return Resolution_mode;

      }

    else
      {
          qDebug()<<"... No resolution configuration detected\n... Proceeding with initial setup";
          Sleeper::msleep(1000);

          const int width = QApplication::desktop()->width();
          const int height = QApplication::desktop()->height();

          file.open(QIODevice::WriteOnly);
          QTextStream out(&file);
          out<<width<<"\n"<<height<<"\n";
          file.close();

          if (file.exists())
          {
              file.open(QIODevice::ReadOnly);
              line = file.readLine();
              int numero=line.toInt();
              file.close();
              if (numero >  2400)                    {Resolution_mode=0; Dimensions="2420x1080";}
              if (numero <  2400 && numero >= 1200)  {Resolution_mode=1; Dimensions="1320x780";}
              if (numero <= 1200)                    {Resolution_mode=2; Dimensions="1210x540";}

              qDebug()<<"... We're setting your window dimensions at : "<<Dimensions<<" px";
              return Resolution_mode;
          }

          else
          {
              qDebug()<<"[!] Could not detect your screen resolution\n";
              return 3;
          }
      }

}

int checkcaenmodules()
{
    qDebug()<<"... Parsing through loaded kernel modules";
    system("rm -f modules");
    system("cp /proc/modules .");

    int module=0;

    QString content;
    QFile file2("modules");
    if(file2.exists())
    {
        file2.open(QIODevice::ReadOnly);
        while(!file2.atEnd())
        {
            content = file2.readLine();
            if (content.contains("CAENUSBdrvB", Qt::CaseSensitive)) {module=1;}
        }
    }

    file2.close();
    //Sleeper::msleep(500);

    if(module==0)
    {
        qDebug()<<"[!] CAEN USB driver module not found. You will not be able to obtain XRF spectra\n"
                  "[!] Check your USB connection to the digitizer";
        Sleeper::msleep(500);
    }
    if(module==1)
    {
        qDebug()<<"... CAEN USB driver module found and loaded";
        Sleeper::msleep(200);
    }
    return 0;
}


int main(int argc, char *argv[]) {

    checkcaenmodules();
    //qDebug()<<"... Please input your root password";

    int case_auth = authentication();
    switch (case_auth) {
    case 0:
        qDebug()<<"... All serial ports found";
        break;
    case 256:
        qDebug()<<"[!] Motor ports not found. Scans and tracking will not be available";
        Sleeper::msleep(1000);
        break;
    case 512:
        qDebug()<<"[!] DPP port not found. All data acquisition will not be available";
        Sleeper::msleep(1000);
        break;
    case 768:
        qDebug()<<"[!] No DPP or motor serial ports found. Please investigate";
        Sleeper::msleep(1000);
        break;
    default:
        qDebug()<<"[!] Unknown port communication error found. The program might not work as expected";
        Sleeper::msleep(1000);
        break;
    }

    QApplication a(argc, argv);

    Resolution_mode = setresolution();
    if (Resolution_mode == 3) { return 1; }

    MainWindow w;

    switch (Resolution_mode) {
    case(0): { w.resize( 2420, 1080 ); break; }
    case(1): { w.resize( 1320,  780 ); break; }
    case(2): { w.resize( 1210,  540 ); break; }
    }

    w.show();
    return a.exec();
}






