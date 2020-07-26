////// STAGE_SELECTION.CPP (v1 29/1/2016) - frao
//
// Functions for stages selection are called by dropdown menus (comboBox_XMotor, comboBox_YMotor, comboBox_ZMotor in GUI_Creator.cpp)
// Variables motor_selectedX, motor_selectedY, motor_selectedZ are set for further use in X_Init.ccp, Y_Init.cpp, Z_init.cpp
// 

#include "../Header.h"
#include "mainwindow.h"
#include <../Shm.h>


extern int motor_selectedX;
extern int motor_selectedY;
extern int motor_selectedZ;

/*
void MainWindow::X_Motor_selection(int Xmotortype)
{
      switch (Xmotortype)
       {
        case(0): {CurrentAction->setText("no motor"); motor_selectedX=Xmotortype; break;}
        case(1): {CurrentAction->setText("M404-2PD"); motor_selectedX=Xmotortype; break;}
        case(2): {CurrentAction->setText("M404-4PD"); motor_selectedX=Xmotortype; break;}
        case(3): {CurrentAction->setText("M404-8PD"); motor_selectedX=Xmotortype; break;}
        case(4): {CurrentAction->setText("VT80-100"); motor_selectedX=Xmotortype; break;}
        case(5): {CurrentAction->setText("VT80-150"); motor_selectedX=Xmotortype; break;}
        case(6): {CurrentAction->setText("VT80-200"); motor_selectedX=Xmotortype; break;}
       }
}

void MainWindow::Y_Motor_selection(int Ymotortype)
{
      switch (Ymotortype)
       {
        case(0): {CurrentAction->setText("no motor"); motor_selectedY=Ymotortype; break;}
        case(1): {CurrentAction->setText("M404-2PD"); motor_selectedY=Ymotortype; break;}
        case(2): {CurrentAction->setText("M404-4PD"); motor_selectedY=Ymotortype; break;}
        case(3): {CurrentAction->setText("M404-8PD"); motor_selectedY=Ymotortype; break;}
        case(4): {CurrentAction->setText("VT80-100"); motor_selectedY=Ymotortype; break;}
        case(5): {CurrentAction->setText("VT80-150"); motor_selectedY=Ymotortype; break;}
        case(6): {CurrentAction->setText("VT80-200"); motor_selectedY=Ymotortype; break;}
       }
}

void MainWindow::Z_Motor_selection(int Zmotortype)
{
      switch (Zmotortype)
       {
        case(0): {CurrentAction->setText("no motor"); motor_selectedZ=Zmotortype; break;}
        case(1): {CurrentAction->setText("M404-2PD"); motor_selectedZ=Zmotortype; break;}
        case(2): {CurrentAction->setText("M404-4PD"); motor_selectedZ=Zmotortype; break;}
        case(3): {CurrentAction->setText("M404-8PD"); motor_selectedZ=Zmotortype; break;}
        case(4): {CurrentAction->setText("VT80-100"); motor_selectedZ=Zmotortype; break;}
        case(5): {CurrentAction->setText("VT80-150"); motor_selectedZ=Zmotortype; break;}
        case(6): {CurrentAction->setText("VT80-200"); motor_selectedZ=Zmotortype; break;}
       }
}

*/
