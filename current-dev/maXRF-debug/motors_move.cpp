#include "mainwindow.h"
#include "../Header.h"
#include <../Shm.h>

extern double X_goto, Y_goto, Z_goto, Px, Py, Pz;
extern int IniXready, IniYready;
extern int *shared_memory, *shared_memory_cmd;
extern bool XYscanning, XOnTarget, YOnTarget, ZOnTarget;

extern int serialX, serialY, serialZ;
extern string read_answer(int port);
extern int send_command(int chan,const char *comando, const char *parametri,int port);

void MainWindow::moveStage(double pos, int serial) {
    char stemp[100];
    sprintf(stemp, "%f", pos / 1000);
    send_command(1, "MOV", stemp, serial);

    if (serial == serialX)      XOnTarget = false;
    else if (serial == serialY) YOnTarget = false;
    else if (serial == serialZ) ZOnTarget = false;
    else printf("[!] Unknown device handler\n");
}

void MainWindow::slotMoveStage(int id) {
    if (id > 2) slotMoveFine(id);

    if (id == 0 && XOnTarget)       moveStage(X_goto, serialX);
    else if (id == 1 && YOnTarget)  moveStage(Y_goto, serialY);
    else if (id == 2 && ZOnTarget)  moveStage(Z_goto, serialZ);
    else if (!XOnTarget || !YOnTarget || !ZOnTarget) printf("[!] Stage not on target, please wait");
    else printf("[!] Unknown signal sender\n");
}

double getPosition(int serial) {
    send_command(1, "POS?", NULL, serial);
    string pos = read_answer(serial);
    QString temp = pos.data();

    temp.remove(0, 2);
    return (temp.toDouble() * 1000);
}

double MainWindow::moveStep(double step, int serial, bool *condition, bool dir) {
    if (*condition) {
        double value = getPosition(serial);
        if (dir) value -= step;
        else value += step;

        moveStage(value, serial);
        *condition = false;
    }
    return 0;
}

void MainWindow::slotMoveFine(int id) {
    if (id == 3)        moveStep(Px, serialX, &XOnTarget, true);
    else if (id == 4)   moveStep(Px, serialX, &XOnTarget, false);
    else if (id == 5)   moveStep(Py, serialY, &YOnTarget, true);
    else if (id == 6)   moveStep(Py, serialY, &YOnTarget, false);
    else if (id == 7)   moveStep(Pz, serialZ, &ZOnTarget, true);
    else if (id == 8)   moveStep(Pz, serialZ, &ZOnTarget, false);
}


void MainWindow::MoveDoubleClick() {
    if( *(shared_memory+19) == 1) {
        if (XOnTarget==true && YOnTarget==true && XYscanning==false && IniXready==1 && IniYready==1) {
            moveStage(*(shared_memory_cmd+64), serialX);
            moveStage(*(shared_memory_cmd+65), serialY);
            printf("[!] Moving motors to the position clicked\n");
            *(shared_memory+19) = 0;
        }
    }
}



















