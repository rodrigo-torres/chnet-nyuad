/*  INFN CHNet - NYUAD (XRF_SCANNER version 10.x)
 *
 *  Change log with respect to versions 9.x
 *
 *  1. Several rearrangements to the GUI for Resolution_mode=1
 *      i.      Port assignment and initilization for the linear stages are now  all under the same tab.
 *      ii.     Removed support for different motors, the mode of operation now is to set a default variable in the source code
 *              and to use the initilization parameters for the motor of interest.
 *      iii.    All autofocus and Arduino communication controls are now all under the same tab.
 *      iv.     Minor modifications to the status labels default values.
 *  2. Deprecated the ScreenDetector external program.
 *      i.      The relevant code for screen resolution detected is now embedded in main.cpp
 *  3. Source code changes to main.cpp
 *      i.      Commented out some dependencies that are already included in the headers
 *      ii.     Deprected the function timeout(int seconds) as it is not called anywhere
 *      iii.    Deprecated CAEN moduled addition to the kernel as it has no effect, the relevant libraries are automatically
 *              loaded into the kernel if the computer is physically connected to the digitizer
 *              and if the appropriate drivers are installed.
 *  4. Souce code changes to X_Init.cpp, Y_Init.cpp and Z_Init.cpp
 *      i.      Introduce new local functions to load M404-8PD and M404-2PD parameters, cutting down on source code.
 *      ii.     Introduce new global function to move X, Y, and Z motors to their reference positions, saving space.
 *      iii.    Rename some of the global and local variables to be more user friendly to English speakers.
 *      iv.      Removed junk code and condensed the remaining useful code into one file of name motors_init.cpp
 *  5. Several consistency modifications to the source code to account for the changes in 4.
 *  6. Renamed move_motors.cpp to motors_move.cpp for consistency
 *  7. Source code changes to mainwindow.cpp
 *      i.      No more X,Y,Z specific functions for read_answer_XXX() and send_command_XXX(), they are now global functions which admit a port as
 *              one of its arguments, i.e. read_answer_XXX() -> read_answer(serialXXX) and so on
 *      ii.     VelocityZ function has been deprecated as Z motor velocity is controled by the feedback loop and the motor initialization routine
 *      iii.    StepZ buttons have also been deprecated, as this functionality is implemented in the laser feedback source code
 *
 *  8. Implemented the XRayTable external program, now the button in the mainwindow should lunch a pop-up with the X-ray energies
 *
 *
 *
 *
 *
 */


//#include <QApplication> , already in mainwindow.h
//#include <QObject>
//#include <QTextStream>
//#include <QFile>
//#include <QString>

//#include <sys/types.h> , already in ../Header.h
//#include <sys/time.h>		/* for setitimer */
//#include <sys/ipc.h>
//#include <sys/shm.h>
//#include <stdio.h>
//#include <time.h>
//#include <unistd.h>		/* for pause */
//#include <signal.h>		/* for signal */



#include "mainwindow.h"
#include "../Header.h"
#include <QSizePolicy>
#include <QScreen>

//Inherited from external program ScreenDetector,

#include <QWidget>
#include <QThread>
#include <QDesktopWidget>



int Resolution_mode;    // 0 if          WIDTH >  2400px
                        // 1 if 1750px < WIDTH <= 2400
                        // 2 if          WIDTH <= 1750

/*int timeout ( int seconds ) , the function is not called anywhere in the program, and is thus legacy
{
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}

    return  1;
}
*/

int authentification()
{
    // Please do replace frao with your computer's username
    /*switch (module) , calling for insmod has no effect in Fedora, the appropriate modules are automatically loaded if the digitizer is detected and the appropriate drivers are installed.
       {
        case(0): {system("su -c 'sysctl -w kernel.shmmax=2000000000;sysctl -w kernel.shmall=720000000; chmod 777 /dev/ttyUSB*;  chmod 777 /dev/ttyACM*; chown frao:frao /dev/ttyACM*; chown frao:frao /dev/ttyUSB*; insmod Program_Data_Files/CAENUSBdrvB.ko.xz;'"); break;}
        case(1): {system("su -c 'sysctl -w kernel.shmmax=2000000000;sysctl -w kernel.shmall=720000000; chmod 777 /dev/ttyUSB*;  chmod 777 /dev/ttyACM*; chown frao:frao /dev/ttyACM*; chown frao:frao /dev/ttyUSB*;'"); break;}
       }
     */

    int error_auth = system("su -c 'sysctl -w kernel.shmmax=2000000000;"
                       "sysctl -w kernel.shmall=720000000;"
                       "chmod +rw /dev/ttyUSB*;"
                       "chmod +rw /dev/ttyACM*;"
                       "chown frao:frao /dev/ttyACM*;"
                       "chown frao:frao /dev/ttyUSB*;'");
    return error_auth;
}

int setresolution()
{
    qDebug()<< "... Detecting screen resolution";

    /* system("./ScreenDetector") , the functionality of this external program has been implemented in the code below.
     * If you run into issues, please comment out this line, and uncomment the relevant lines in the else clause below.
     */

    Sleeper::msleep(500);

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
    Sleeper::msleep(500);

    if(module==0)
    {
        qDebug()<<"[!] CAEN USB driver module not found\n"
                  "[!] You will not be able to obtain XRF spectra\n"
                  "[!] Check your USB connection to the digitizer. Error code: "<< module;
        Sleeper::msleep(500);
    }
    if(module==1)
    {
        qDebug()<<"... CAEN USB driver module found and loaded";
        Sleeper::msleep(500);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    checkcaenmodules();

    qDebug()<<"... Please input your root password";

    int counter_auth=0;
    int error_auth;
    while (counter_auth <= 2)
    {
        error_auth = authentification();
        if (error_auth != 0 && error_auth != 256 && counter_auth >= 2)
        {
            qDebug()<<"[!] Authentication failure.\n"
                      "[!] The program might not work as expected.";
            Sleeper::msleep(1000); return 1;}
        if (error_auth != 0 && error_auth != 256 && counter_auth != 2)
        {
            counter_auth++;
            qDebug()<<"[!] Wrong password. Try again.";}
        if (error_auth == 0 ) {break;}
    }

    QApplication a(argc, argv);

    Resolution_mode = setresolution();
    if (Resolution_mode == 3) {return 1;}

    MainWindow w;

    switch (Resolution_mode)
    {
    case(0): {w.resize( 2420, 1080 ); break;}
    case(1): {w.resize( 1320,  780 ); break;}
    case(2): {w.resize( 1210,  540 ); break;}
    }

    w.show();
    return a.exec();
}






