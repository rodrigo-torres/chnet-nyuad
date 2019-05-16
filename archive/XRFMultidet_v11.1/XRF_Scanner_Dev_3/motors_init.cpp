#include "mainwindow.h"
#include "../Header.h"
#include <../Shm.h>
#include <time.h>

// These are global variables used in the time stamp of the scan function.
extern int interval;
extern bool TimerActive;

// These are global variables that act as indicators of the state of the motors.
extern bool okX,okY,okZ;
extern bool InitX,InitY,InitZ;
extern bool InitPhaseX,InitPhaseY,InitPhaseZ;
extern bool XConnected,Xmoving,YConnected,Ymoving,ZConnected,Zmoving;

// These are global variables that specify the file descriptors for the USB ports and the type of motors used.
extern int serialX,serialY,serialZ;
extern int selected_Xmotor,selected_Ymotor,selected_Zmotor;

// These are global functions used to commmunicate to the motors through the file descriptors.
extern int send_command(int channel, const char *command, const char *parameter, int port);
extern string read_answer(int port);

int loadparam_M404_2pd(int port)
{
    qDebug()<<"... Loading initialization parameters for Physik Instrumente M404-2PD\n"
            "... If your motor is different please change the source code in Z_Init.cpp";

    send_command(1,"SPA"," 60 M-404.2PD",port);
    Sleeper::msleep(500);
    send_command(1,"SPA"," 1 180",port);
    Sleeper::msleep(100);
    send_command(1,"SPA"," 2 45",port);
    Sleeper::msleep(100);
    send_command(1,"SPA"," 3 300",port);
    Sleeper::msleep(100);
    send_command(1,"SPA"," 4 2000",port);
    Sleeper::msleep(100);
    send_command(1,"SPA"," 14 4000",port);
    Sleeper::msleep(100);
    send_command(1,"SPA"," 15 1",port); ////0.064285257
    Sleeper::msleep(100);
    send_command(1,"SPA"," 19 0",port);
    Sleeper::msleep(100);
    send_command(1,"SPA"," 20 1",port);        //si usano i limit switches dell'hardware (top e bottom)
    Sleeper::msleep(100);
    send_command(1,"SPA"," 21 50.",port);       //Max spostamento in positivo da home (0)
    Sleeper::msleep(100);
    send_command(1,"SPA"," 22 25.",port);   //Value at Reference position
    Sleeper::msleep(100);
    send_command(1,"SPA"," 23 25.",port);  //Distanza tra Ref e Neg-lim
    Sleeper::msleep(100);
    send_command(1,"SPA"," 47 25",port);    //Distanza tra Ref e Pos-lim
    Sleeper::msleep(100);
    send_command(1,"SPA"," 24 0",port);          //0 i limit switches sono attivi alti...va come 49
    Sleeper::msleep(100);
    send_command(1,"SPA"," 48 0.000000",port);  //Max spostamento in negativo da home (0)
    Sleeper::msleep(100);
    send_command(1,"SPA"," 49 0",port);        //0 non si inverte il riferimento
    Sleeper::msleep(100);
    send_command(1,"SPA"," 50 0",port);      //0 Abiintervallita lo stop ai limit switches dell'hardware
    Sleeper::msleep(100);
    send_command(1,"SPA"," 8 0.5",port);    //8 10 e 11 per ultimi senno' li ricambia 0.035156
    Sleeper::msleep(100);
    send_command(1,"SPA"," 10 50",port);   // velocita'
    Sleeper::msleep(100);
    send_command(1,"SPA"," 11 30",port);  //Accelerazione
    Sleeper::msleep(100);
    send_command(1,"SPA"," 117442049 MM",port);  //Unita'
    Sleeper::msleep(100);

    send_command(1,"SPA? 1 1",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 2",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 3",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 4",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 8",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 10",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 11",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 14",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 15",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 19",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 20",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 21",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 22",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 23",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 24",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 48",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 49",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 50",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 60",NULL,port);
    read_answer(port);

    return 1;
}
int loadparam_M404_8pd(int port)
{
    qDebug()<<"... Loading initialization parameters for Physik Instrumente M404-8PD\n"
              "... If your motor is different please change the source code in Y_Init.cpp";

    send_command(1,"SPA"," 60 M-404.8PD",port);
    Sleeper::msleep(500);
    send_command(1,"SPA"," 1 180",port); //primo numero canale SPA secondo numero valore
    Sleeper::msleep(100);
    send_command(1,"SPA"," 2 45",port);
    Sleeper::msleep(100);
    send_command(1,"SPA"," 3 300",port);
    Sleeper::msleep(100);
    send_command(1,"SPA"," 4 2000",port);
    Sleeper::msleep(100);
    send_command(1,"SPA"," 14 4000",port);
    Sleeper::msleep(100);
    send_command(1,"SPA"," 15 1",port); ////0.064285257
    Sleeper::msleep(100);
    send_command(1,"SPA"," 19 0",port);
    Sleeper::msleep(100);
    send_command(1,"SPA"," 20 1",port);        //si usano i limit switches dell'hardware (top e bottom)
    Sleeper::msleep(100);
    send_command(1,"SPA"," 21 200.",port);       //Max spostamento in positivo da home (0)
    Sleeper::msleep(100);
    send_command(1,"SPA"," 22 100.",port);   //Value at Reference position
    Sleeper::msleep(100);
    send_command(1,"SPA"," 23 100.",port);  //Distanza tra Ref e Neg-lim
    Sleeper::msleep(100);
    send_command(1,"SPA"," 47 100",port);    //Distanza tra Ref e Pos-lim
    Sleeper::msleep(100);
    send_command(1,"SPA"," 24 0",port);          //0 i limit switches sono attivi alti...va come 49
    Sleeper::msleep(100);
    send_command(1,"SPA"," 48 0.000000",port);  //Max spostamento in negativo da home (0)
    Sleeper::msleep(100);
    send_command(1,"SPA"," 49 0",port);        //0 non si inverte il riferimento
    Sleeper::msleep(100);
    send_command(1,"SPA"," 50 0",port);      //0 Abilita lo stop ai limit switches dell'hardware
    Sleeper::msleep(100);
    send_command(1,"SPA"," 8 0.5",port);    //8 10 e 11 per ultimi senno' li ricambia 0.035156
    Sleeper::msleep(100);
    send_command(1,"SPA"," 10 50",port);   // velocita'
    Sleeper::msleep(100);
    send_command(1,"SPA"," 11 200",port);  //Accelerazione
    Sleeper::msleep(100);
    send_command(1,"SPA"," 117442049 MM",port);  //Unita'
    Sleeper::msleep(100);

    send_command(1,"SPA? 1 1",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 2",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 3",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 4",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 8",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 10",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 11",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 14",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 15",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 19",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 20",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 21",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 22",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 23",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 24",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 48",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 49",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 50",NULL,port);
    read_answer(port);
    send_command(1,"SPA? 1 60",NULL,port);
    read_answer(port);

    return 1;
}

void MainWindow::movetoref_Xmotor(float refpositionX)
{
    char sx[100];
    sprintf(sx,"%f",refpositionX);
    send_command(1,"MOV",sx,serialX);

    CheckXOnTarget();
    Sleeper::msleep(100);
    while(Xmoving)
    {
        CheckXOnTarget();
        Sleeper::msleep(100);
    }
    return;
}

void MainWindow::movetoref_Ymotor(float refpositionY)
{
    char sy[100];
    sprintf(sy,"%f",refpositionY);
    send_command(1,"MOV",sy,serialY);

    CheckYOnTarget();
    Sleeper::msleep(100);
    while(Ymoving)
    {
        CheckYOnTarget();
        Sleeper::msleep(100);
    }
    return;
}

void MainWindow::movetoref_Zmotor(float refpositionZ)
{
    char sz[100];
    sprintf(sz,"%f",refpositionZ);
    send_command(1,"MOV",sz,serialZ);

    CheckZOnTarget();
    Sleeper::msleep(100);
    while(Zmoving)
    {
        CheckZOnTarget();
        Sleeper::msleep(100);
    }
    return;
}

void MainWindow::Init_Xmotor()
{
    // Disabling Y and Z motor initialization push buttons to avoid conflicts.
    pushButton_tab_2_2Y->setEnabled(false);
    INIT_Z_pushButton->setEnabled(false);

    bool valid_Xmotor=false;
    switch (selected_Xmotor) {
    case 3:
        CurrentAction->setText("Initializing: M-404.8PD - 200mm");
        valid_Xmotor = loadparam_M404_8pd(serialX);
        break;
    default:
        CurrentAction->setText("Initializing: M-404.8PD - 200mm");
        valid_Xmotor = loadparam_M404_8pd(serialX);
        break;
    }

    qDebug()<<"... Initialization parameters succesfully loaded";
    Sleeper::msleep(500);

    // Moves the motor to the negative limit, then to its reference position.
    if(valid_Xmotor)
    {
        qDebug()<<"... Changing X velocity";
        send_command(1,"VEL 1 15",NULL,serialX);
        Sleeper::msleep(300);

        qDebug()<<"... Enabling position servo";
        send_command(1,"SVO 1 1",NULL,serialX);
        Sleeper::msleep(300);

        qDebug()<<"... Moving to X negative limit";
        send_command(1,"FNL 1",NULL,serialX);

        CheckXOnTarget();
        Sleeper::msleep(50);

        while(Xmoving)
        {
            CheckXOnTarget();
            Sleeper::msleep(100);
        }

        qDebug()<<"... Defining reference position";
        send_command(1,"DEF 1",NULL,serialX);
        Sleeper::msleep(300);

        qDebug()<<"... Going to reference position";
        qDebug()<<"... Please wait";

    switch (selected_Xmotor) {
    case 1:
        movetoref_Xmotor(25.0000);
        break;
    case 3:
        movetoref_Xmotor(100.0000);
        break;
    default:
        movetoref_Xmotor(100.0000);
        break;
    }

    switch (selected_Xmotor) {
    case 1:
        //"M404-2PD" 50mm
        MOVE_TO_X_doubleSpinBox->setMaximum(50);
        MOVE_TO_X_doubleSpinBox->setValue(25);
        SET_SCAN_X_MIN_doubleSpinBox->setMaximum(50);
        SET_SCAN_X_MIN_doubleSpinBox->setValue(25);
        SET_SCAN_X_MAX_doubleSpinBox->setMaximum(50);
        SET_SCAN_X_MAX_doubleSpinBox->setValue(25);
        break;
    case 3 :
        //"M404-8PD" 200mm code:
        MOVE_TO_X_doubleSpinBox->setMaximum(200);
        MOVE_TO_X_doubleSpinBox->setValue(100);
        SET_SCAN_X_MIN_doubleSpinBox->setMaximum(200);
        SET_SCAN_X_MIN_doubleSpinBox->setValue(100);
        SET_SCAN_X_MAX_doubleSpinBox->setMaximum(200);
        SET_SCAN_X_MAX_doubleSpinBox->setValue(100);
        break;
    default:
        //"M404-8PD" 200mm code:
        MOVE_TO_X_doubleSpinBox->setMaximum(200);
        MOVE_TO_X_doubleSpinBox->setValue(100);
        SET_SCAN_X_MIN_doubleSpinBox->setMaximum(200);
        SET_SCAN_X_MIN_doubleSpinBox->setValue(100);
        SET_SCAN_X_MAX_doubleSpinBox->setMaximum(200);
        SET_SCAN_X_MAX_doubleSpinBox->setValue(100);
        break;
    }

    Enable_TabWidget_3_4_XY();

    InitPhaseX=true;

    if(TimerActive==false)
    {
        timer->start(interval);
        TimerActive=true;
    }
    okX=false;
    InitX=true;
    send_command(1,"ERR?",NULL,serialX);
    read_answer(serialX);
    }

    else
    {
        qDebug()<<"... Motor selection not valid or driver missing";
        valid_Xmotor=false;
        okX=false;
    }

    pushButton_tab_2_2Y->setEnabled(true);
    INIT_Z_pushButton->setEnabled(true);

}

void MainWindow::Init_Ymotor()
{
    // Disabling X and Z motor initialization push buttons to avoid conflicts.
    pushButton_tab_2_2X->setEnabled(false);
    INIT_Z_pushButton->setEnabled(false);

    bool valid_Ymotor=false;
    switch (selected_Ymotor) {
    case 3:
        CurrentAction->setText("Initializing: M-404.8PD - 200mm");
        valid_Ymotor = loadparam_M404_8pd(serialY);
        break;
    default:
        CurrentAction->setText("Initializing: M-404.8PD - 200mm");
        valid_Ymotor = loadparam_M404_8pd(serialY);
        break;
    }

    if (valid_Ymotor)
    {
        qDebug()<<"... Changing Y velocity";
        send_command(1,"VEL 1 10",NULL,serialY);
        Sleeper::msleep(300);

        qDebug()<<"... Enabling position servo";
        send_command(1,"SVO 1 1",NULL,serialY);
        Sleeper::msleep(300);

        qDebug()<<"... Moving to Y negative limit";
        send_command(1,"FNL 1",NULL,serialY);

        CheckYOnTarget();
        Sleeper::msleep(50);

        while(Ymoving)
        {
            CheckYOnTarget();
            Sleeper::msleep(100);
        }

        qDebug()<<"... Defining reference position";
        send_command(1,"DEF 1",NULL,serialY);
        Sleeper::msleep(300);

        qDebug()<<"... Going to reference position";
        qDebug()<<"... Please wait";

        switch (selected_Ymotor) {
        case 1:
            movetoref_Ymotor(25.0000);
            break;
        case 2:
            movetoref_Ymotor(50.0000);
            break;
        case 3:
            movetoref_Ymotor(100.0000);
            break;
        default:
            movetoref_Ymotor(100.0000);
            break;
        }

        switch (selected_Ymotor) {
        case 1:
            //"M404-2PD" 50mm
            MOVE_TO_Y_doubleSpinBox->setMaximum(50);
            MOVE_TO_Y_doubleSpinBox->setValue(25);
            SET_SCAN_Y_MIN_doubleSpinBox->setMaximum(50);
            SET_SCAN_Y_MIN_doubleSpinBox->setValue(25);
            SET_SCAN_Y_MAX_doubleSpinBox->setMaximum(50);
            SET_SCAN_Y_MAX_doubleSpinBox->setValue(25);
            break;

        case 3 :
            //"M404-8PD" 200mm code:
            MOVE_TO_Y_doubleSpinBox->setMaximum(200);
            MOVE_TO_Y_doubleSpinBox->setValue(100);
            SET_SCAN_Y_MIN_doubleSpinBox->setMaximum(200);
            SET_SCAN_Y_MIN_doubleSpinBox->setValue(100);
            SET_SCAN_Y_MAX_doubleSpinBox->setMaximum(200);
            SET_SCAN_Y_MAX_doubleSpinBox->setValue(100);
            break;
        default:
            //"M404-8PD" 200mm code:
            MOVE_TO_Y_doubleSpinBox->setMaximum(200);
            MOVE_TO_Y_doubleSpinBox->setValue(100);
            SET_SCAN_Y_MIN_doubleSpinBox->setMaximum(200);
            SET_SCAN_Y_MIN_doubleSpinBox->setValue(100);
            SET_SCAN_Y_MAX_doubleSpinBox->setMaximum(200);
            SET_SCAN_Y_MAX_doubleSpinBox->setValue(100);
            break;
        }

        Enable_TabWidget_3_4_XY();

        InitPhaseY=true;

        if(TimerActive==false)
        {
            timer->start(interval);
            TimerActive=true;
        }
        okY=false;
        InitY=true;
        send_command(1,"ERR?",NULL,serialY);
        read_answer(serialY);
    }
    else
    {
        qDebug()<<"... Motor selection not valid or driver missing";
        valid_Ymotor=false;
        okY=false;
    }

    pushButton_tab_2_2X->setEnabled(true);
    INIT_Z_pushButton->setEnabled(true);
}

void MainWindow::Init_Zmotor()
{
    // Disabling X and Y initialization push buttons to avoid conflicts.
    pushButton_tab_2_2X->setEnabled(false);
    pushButton_tab_2_2Y->setEnabled(false);

    bool valid_Zmotor=false;
    switch (selected_Zmotor) {
    case 1:
        CurrentAction->setText("Initializing: M-404.2PD - 200mm");
        valid_Zmotor = loadparam_M404_2pd(serialZ);
        break;
    default:
        CurrentAction->setText("Initializing: M-404.2PD - 200mm");
        valid_Zmotor = loadparam_M404_2pd(serialZ);
        break;
    }

    if (valid_Zmotor)
    {
        qDebug()<<"... Changing Z velocity";
        send_command(1,"VEL 1 5",NULL,serialZ);
        Sleeper::msleep(300);

        qDebug()<<"... Enabling position servo";
        send_command(1,"SVO 1 1",NULL,serialZ);
        Sleeper::msleep(300);

        qDebug()<<"... Moving to Z negative limit";
        send_command(1,"FNL 1",NULL,serialZ);

        CheckZOnTarget();
        Sleeper::msleep(50);

        while(Zmoving)
        {
            CheckZOnTarget();
            Sleeper::msleep(100);
        }

        qDebug()<<"... Defining reference position";
        send_command(1,"DEF 1",NULL,serialZ);
        Sleeper::msleep(300);

        qDebug()<<"... Going to reference position";
        qDebug()<<"... Please wait";

        switch (selected_Zmotor) {
        case 1:
            movetoref_Zmotor(25.0000);
            break;
        case 2:
            movetoref_Zmotor(50.0000);
            break;
        case 3:
            movetoref_Zmotor(100.0000);
            break;
        default:
            movetoref_Zmotor(25.0000);
            break;
        }

        switch (selected_Zmotor) {
        case 1:
            //"M404-2PD" 50mm
            MOVE_Z_To_doubleSpinBox->setMaximum(50);
            MOVE_Z_To_doubleSpinBox->setValue(25);
            break;
        case 3:
            //"M404-8PD" 200mm code:
            MOVE_Z_To_doubleSpinBox->setMaximum(200);
            MOVE_Z_To_doubleSpinBox->setValue(100);
            break;
        default:
            //"VT80-100" 100mm code: 62309140
            MOVE_Z_To_doubleSpinBox->setMaximum(50);
            MOVE_Z_To_doubleSpinBox->setValue(25);
            break;
        }

        tab_3->setEnabled(true);
        InitZ=true;
        InitPhaseZ=true;
        if(TimerActive==false)
        {
            timer->start(interval);
            TimerActive=true;
        }
        okZ=false;
    }

    else
    {
        qDebug()<<"... Motor selection not valid or driver missing";
        valid_Zmotor=false;
        okZ=false;
    }

    pushButton_tab_2_2X->setEnabled(true); // re-enabling X and Y init button
    pushButton_tab_2_2Y->setEnabled(true);
    tab_2->setEnabled(true);
    INIT_Z_pushButton->setEnabled(false);
}
















