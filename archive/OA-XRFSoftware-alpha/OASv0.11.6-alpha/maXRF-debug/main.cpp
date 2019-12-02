#include "mainwindow.h"
#include "../Header.h"
#include <QSizePolicy>
#include <QScreen>

// Inherited from external program ScreenDetector
#include <QWidget>
#include <QThread>
#include <QDesktopWidget>

int authentication() {

    int error_USB = system("sudo chmod +rw /dev/ttyUSB* ; sudo chown frao:frao /dev/ttyUSB* 2>/dev/null;");
    int error_ACM = system("sudo chmod +rw /dev/ttyACM* ; sudo chown frao:frao /dev/ttyACM* 2>/dev/null;");

    // Please note that system() returns error codes multiplied by 256, so error code 1 -> 256, and 0 -> 0
    int error_auth = error_ACM*2 + error_USB;
    return error_auth;
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
    system("rm -f modules");
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
    int case_auth = authentication();

    if (case_auth == 0) qDebug()<<"... All serial ports found";
    else if (case_auth == 256) {
        qDebug()<<"[!] Motor ports not found. Scans and tracking will not be available";
        Sleeper::msleep(1000);
    }
    else if (case_auth == 512) {
        qDebug()<<"[!] DPP port not found. All data acquisition will not be available";
        Sleeper::msleep(1000);
    }
    else if (case_auth == 768) {
        qDebug()<<"[!] No DPP or motor serial ports found. Please investigate";
        Sleeper::msleep(1000);
    }
    else {
        qDebug()<<"[!] Unknown port communication error found. The program might not work as expected";
        Sleeper::msleep(1000);
    }


    QApplication a(argc, argv);
    MainWindow w;
    //w.resize(1320, 620);
    w.setFixedSize(1320,680);
    w.show();
    return a.exec();
}






