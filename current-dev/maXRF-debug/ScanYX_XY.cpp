#include "mainwindow.h"
#include "../Header.h"
#include <../Shm.h>

extern bool TimerActive, FirstRun;
extern bool XYscanning, XOnTarget, YOnTarget, YHasMoved;
extern double positionX, positionY, V, Xmax, Xmin, Ymax, Ymin, tempoPos, Py;
extern double Xmin1, Xmax1, Ymin1, Ymax1;

extern int interval;     extern int NscanX;             extern int  onlyOne;
extern int DAQ_TYPE;


extern float Yo,vy,Xo,vx,temp;

extern int serialX, serialY;
extern int tty_send(int chan,const char *comando, const char *parametri, int port);
extern double posXforacceleration;     extern double accelerationtime;	extern int accelerationtimesleep;


extern int *shared_memory; extern int *shared_memory_cmd; extern int *shared_memory2;

bool MainWindow::StartXYScan() {

    if (!XYscanning) {
        positionY = Ymin1;
        positionX = Xmin1;
        Xmin = positionX;
        Xmax = Xmax1;
        Ymax = Ymax1;

        // The acceleration of the motor is specified as 200 mm/s
        // The variable V is the desired motor velocity specified in the GUI
        accelerationtime = (V / 200);
        posXforacceleration = 100 * 1000 * (accelerationtime * accelerationtime); //in um
        accelerationtimesleep = round(accelerationtime * 1000) + 23;
        //printf("... Acquisition sleep to account for acceleration set at:%d ms\n", accelerationtimesleep);

        if (*(shared_memory_cmd+70) == 0) { // Produces a confirmation prompt
            if (DAQ_TYPE == 1) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "WARNING", "[!] Start USB DAQ?", QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes) { system("./app-modules/ADCXRF_USB &"); }
            }
            if (DAQ_TYPE == 0) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "WARNING", "[!] Start Optical DAQ?", QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    system("./app-modules/ADCXRF_Optical_Link &");
                    *(shared_memory_cmd+70) = 1;
                    Sleeper::msleep(2000);
                }
            }
        }
        if (XOnTarget == true && YOnTarget == true) { // If motors are stationary, move to scan origin
            tty_send(1,"VEL 1 10",NULL,serialX);
            tty_send(1,"VEL 1 10",NULL,serialY);
            positionX = Xmin - posXforacceleration;

            moveStage(positionX, serialX);
            moveStage(positionY, serialY);
        }

        //Plan here is to add a synchronization signal between the ADCXRF_USB and the scan
        //Also, add an independent timer for the Check Z position
/*
        if (TimerActive) { // Starts an interval timer for the scan
            double time_seconds = (double)*(shared_memory_cmd+60) / (double)*(shared_memory_cmd+67);
            timer->start(static_cast<int>(time_seconds * 2000));
            TimerActive=true;
        }*/
        XYscanning = true;
    }

    return XYscanning;
}


void MainWindow::ScanXY() {
    if (!XOnTarget || !YOnTarget) return;
    if (positionY == Ymin1) { // Sets the velocity for the scan
        char v[10];
        sprintf(v,"%f",V);
        tty_send(1,"VEL",v,serialX);
        *(shared_memory_cmd+70) = 1;
    }

    if (YHasMoved == true) { // Starts a new line
        *(shared_memory2+3) = 1;
        if (positionX == Xmin - posXforacceleration) positionX = Xmax + posXforacceleration;
        else if (positionX == Xmax + posXforacceleration) positionX = Xmin - posXforacceleration;

        moveStage(positionX, serialX);
        YHasMoved = false;
        Sleeper::msleep(accelerationtimesleep);
    }

    else if (YHasMoved == false) {
        if (positionY < Ymax) { // Changes line
            positionY += Py;
            while (*(shared_memory2+8) != 1) Sleeper::msleep(100);
            moveStage(positionY, serialY);

            YHasMoved = true;
            *(shared_memory2+8) = 0;
        }
        else { // The scan is done
            XYscanning = false;
            YHasMoved  = true;
            if (*(shared_memory_cmd+70) == 1) {
                int counting = 0;
                while (*(shared_memory2+8) != 1 && counting < 5) { Sleeper::msleep(tempoPos); counting++; }
                *(shared_memory2+8) = 0;
                *(shared_memory_cmd+70) = 0;
                SaveTxt();
            }
        }
    }
}



















