#include "mainwindow.h"
#include "../Header.h"
#include <../Shm.h>

extern double X_goto, Y_goto, Z_goto, Px, Py, Pz;
extern int IniXready, IniYready;
extern int *shared_memory, *shared_memory_cmd;
extern bool XYscanning, stage_on_target[3];

extern int serialX, serialY, serialZ;
extern int tty_send(int chan,const char *comando, const char *parametri,int port);
extern void tty_read(int port, char *ans, unsigned long wait = 0);

void MainWindow::moveStage(double pos, int serial) {
    char stemp[100];
    sprintf(stemp, "%f", pos / 1000);
    tty_send(1, "MOV", stemp, serial);

    if (serial == serialX)      stage_on_target[0] = false;
    else if (serial == serialY) stage_on_target[1] = false;
    else if (serial == serialZ) stage_on_target[2] = false;
    else printf("[!] Unknown device handler\n");
}

void MainWindow::slotMoveStage(int id) {
    if (id > 2) slotMoveFine(id);

    if (id == 0 && stage_on_target[0])       moveStage(X_goto, serialX);
    else if (id == 1 && stage_on_target[1])  moveStage(Y_goto, serialY);
    else if (id == 2 && stage_on_target[2])  moveStage(Z_goto, serialZ);
    else if (!stage_on_target[0] || !stage_on_target[1] || !stage_on_target[2]) printf("[!] Stage not on target, please wait");
    else printf("[!] Unknown signal sender\n");
}

double getPosition(int serial) {
    tty_send(1, "POS?", nullptr, serial);

    char ans[15] = { '\0' };
    tty_read(serial, ans);

    QString temp = &ans[2];
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
    if (id == 3)        moveStep(Px, serialX, &stage_on_target[0], true);
    else if (id == 4)   moveStep(Px, serialX, &stage_on_target[0], false);
    else if (id == 5)   moveStep(Py, serialY, &stage_on_target[1], true);
    else if (id == 6)   moveStep(Py, serialY, &stage_on_target[1], false);
    else if (id == 7)   moveStep(Pz, serialZ, &stage_on_target[2], true);
    else if (id == 8)   moveStep(Pz, serialZ, &stage_on_target[2], false);
}


void MainWindow::MoveDoubleClick() {
    if( *(shared_memory+19) == 1) {
        if (stage_on_target[0]==true && stage_on_target[1]==true && XYscanning==false && IniXready==1 && IniYready==1) {
            moveStage(*(shared_memory_cmd+64), serialX);
            moveStage(*(shared_memory_cmd+65), serialY);
            printf("[!] Moving motors to the position clicked\n");
            *(shared_memory+19) = 0;
        }
    }
}



















