#include "mainwindow.h"
#include "../Header.h"
#include <../Shm.h>
#include <time.h>

// These are global variables used in the time stamp of the scan function.
extern int interval;
extern bool TimerActive;

// These are global variables that act as indicators of the state of the motors.
extern bool okX,okY,okZ;
extern bool InitY,InitZ;
extern bool IniXready, IniYready, IniZready;
extern bool InitPhaseY,InitPhaseZ;
extern bool XConnected,Xmoving,YConnected,Ymoving,ZConnected, ZOnTarget;
extern bool XOnTarget, YOnTarget, ZOnTarget;

// These are global variables that specify the file descriptors for the USB ports and the type of motors used.
extern int serialX,serialY,serialZ;
extern int selected_Xmotor,selected_Ymotor,selected_Zmotor;

// These are global functions used to commmunicate to the motors through the file descriptors.
extern int tty_send(int channel, const char *command, const char *parameter, int port);
//extern string read_answer(int port);

void modSendCmd(int ch, const char* cmd, const char* val, int port) {
    tty_send(ch, cmd, val, port);
    Sleeper::msleep(100);
}

int loadparam_M404_2pd(int port) {
    qDebug()<<"... Loading initialization parameters for Physik Instrumente M-404.2PD\n";

    modSendCmd(1,"SPA"," 60 M-404.2PD",port);
    modSendCmd(1,"SPA"," 1 180",port);
    modSendCmd(1,"SPA"," 2 45",port);
    modSendCmd(1,"SPA"," 3 300",port);
    modSendCmd(1,"SPA"," 4 2000",port);
    modSendCmd(1,"SPA"," 14 4000",port);
    modSendCmd(1,"SPA"," 15 1",port); ////0.064285257
    modSendCmd(1,"SPA"," 19 0",port);
    modSendCmd(1,"SPA"," 20 1",port);        //si usano i limit switches dell'hardware (top e bottom)
    modSendCmd(1,"SPA"," 21 50.",port);       //Max spostamento in positivo da home (0)
    modSendCmd(1,"SPA"," 22 25.",port);   //Value at Reference position
    modSendCmd(1,"SPA"," 23 25.",port);  //Distanza tra Ref e Neg-lim
    modSendCmd(1,"SPA"," 47 25",port);    //Distanza tra Ref e Pos-lim
    modSendCmd(1,"SPA"," 24 0",port);          //0 i limit switches sono attivi alti...va come 49
    modSendCmd(1,"SPA"," 48 0.000000",port);  //Max spostamento in negativo da home (0)
    modSendCmd(1,"SPA"," 49 0",port);        //0 non si inverte il riferimento
    modSendCmd(1,"SPA"," 50 0",port);      //0 Abiintervallita lo stop ai limit switches dell'hardware
    modSendCmd(1,"SPA"," 8 0.5",port);    //8 10 e 11 per ultimi senno' li ricambia 0.035156
    modSendCmd(1,"SPA"," 10 50",port);   // velocita'
    modSendCmd(1,"SPA"," 11 30",port);  //Accelerazione
    modSendCmd(1,"SPA"," 117442049 MM",port);  //Unita'

    return 1;
}

int loadparam_M404_8pd(int port) {
    qDebug()<<"... Loading initialization parameters for Physik Instrumente M-404.8PD\n";

    modSendCmd(1,"SPA"," 60 M-404.8PD",port);
    modSendCmd(1,"SPA"," 1 180",port); //primo numero canale SPA secondo numero valore
    modSendCmd(1,"SPA"," 2 45",port);
    modSendCmd(1,"SPA"," 3 300",port);
    modSendCmd(1,"SPA"," 4 2000",port);
    modSendCmd(1,"SPA"," 14 4000",port);
    modSendCmd(1,"SPA"," 15 1",port); ////0.064285257
    modSendCmd(1,"SPA"," 19 0",port);
    modSendCmd(1,"SPA"," 20 1",port);        //si usano i limit switches dell'hardware (top e bottom)
    modSendCmd(1,"SPA"," 21 200.",port);       //Max spostamento in positivo da home (0)
    modSendCmd(1,"SPA"," 22 100.",port);   //Value at Reference position
    modSendCmd(1,"SPA"," 23 100.",port);  //Distanza tra Ref e Neg-lim
    modSendCmd(1,"SPA"," 47 100",port);    //Distanza tra Ref e Pos-lim
    modSendCmd(1,"SPA"," 24 0",port);          //0 i limit switches sono attivi alti...va come 49
    modSendCmd(1,"SPA"," 48 0.000000",port);  //Max spostamento in negativo da home (0)
    modSendCmd(1,"SPA"," 49 0",port);        //0 non si inverte il riferimento
    modSendCmd(1,"SPA"," 50 0",port);      //0 Abilita lo stop ai limit switches dell'hardware
    modSendCmd(1,"SPA"," 8 0.5",port);    //8 10 e 11 per ultimi senno' li ricambia 0.035156
    modSendCmd(1,"SPA"," 10 50",port);   // velocita'
    modSendCmd(1,"SPA"," 11 200",port);  //Accelerazione
    modSendCmd(1,"SPA"," 117442049 MM",port);  //Unita'

    return 1;
}


void MainWindow::moveToRef(float refpos, int serial) {
    int fds[] = { serialX, serialY, serialZ };
    int id = -1;
    for (int i = 0; i < static_cast<int>(sizeof(fds)); i++) {
        if (fds[i] == serial) id = i;
    }


    char stemp[100];
    sprintf(stemp, "%f", refpos);
    tty_send(1, "MOV", stemp, serial);

    stage_check_on_target(serial, id);
}


void MainWindow::stage_init(int serial) {
    /* Identifying linear stage */
    int fds[] = { serialX, serialY, serialZ };
    int id = -1;
    for (int i = 0; i < static_cast<int>(sizeof(fds)); i++) {
        if (fds[i] == serial) id = i;
    }

    bool *on_target[] = { &XOnTarget, &YOnTarget, &ZOnTarget};

    /* Disabling all stage initialization pushbuttons to prevent conflicts */
    pushButton_tab_2_2X->setEnabled(false);
    pushButton_tab_2_2Y->setEnabled(false);
    INIT_Z_pushButton->setEnabled(false);


    char message[25];
    const char *model[] = { "M404.2PD", "M404.8PD" };
    strncpy(message, "Initializing: ", sizeof(message));
    if (id == 2) strcat(message, model[0]);
    else strcat(message, model[1]);

    status->showMessage(message, 60);

    bool ret;
    id == 2 ? ret = loadparam_M404_2pd(serial) : ret = loadparam_M404_8pd(serial);

    if (ret) {
        qDebug()<<"... Changing selected stage velocity";
        tty_send(1, "VEL 1 15", nullptr, serial);

        qDebug()<<"... Enabling position servo";
        tty_send(1, "SVO 1 1", nullptr, serial);

        qDebug()<<"... Moving selected stage to its negative limit";
        tty_send(1, "FNL 1", nullptr, serial);

        do {
            Sleeper::msleep(100);
            stage_check_on_target(serial, id);
        } while (!(*on_target[id]));

        qDebug()<<"... Defining reference position";
        tty_send(1, "DEF 1", nullptr, serial);

        qDebug()<<"... Going to reference position"
                  "\n... Please wait";

        double ref_pos[] = { 100., 100., 25. };

        char stemp[100];
        sprintf(stemp, "%f", ref_pos[id]);
        tty_send(1, "MOV", stemp, serial);

        do {
            Sleeper::msleep(100);
            stage_check_on_target(serial, 0);
        } while (!(*on_target[id]));

        id == 2 ? tab_3->setEnabled(true) : Enable_TabWidget_3_4_XY();

        if (TimerActive == false) {
            timer->start(interval);
            TimerActive=true;
        }

        bool *inited[] = { &IniXready, &IniYready, &IniZready};
        *inited[id] = true;
    }

    else {
        qDebug()<<"Invalid motor selection";
    }
    /* Renabling all stage initialization pushbuttons */
    pushButton_tab_2_2X->setEnabled(true);
    pushButton_tab_2_2Y->setEnabled(true);
    INIT_Z_pushButton->setEnabled(true);

}

void MainWindow::Init_Xmotor() {
    stage_init(serialX);
}

void MainWindow::Init_Ymotor()
{
    // Disabling X and Z motor initialization push buttons to avoid conflicts.
    pushButton_tab_2_2X->setEnabled(false);
    INIT_Z_pushButton->setEnabled(false);

    bool valid_Ymotor=false;
    switch (selected_Ymotor) {
    case 3:
        //CurrentAction->setText("Initializing: M-404.8PD - 200mm");
        status->showMessage("Initializing: M-404.8PD - 200mm", 60);
        valid_Ymotor = loadparam_M404_8pd(serialY);
        break;
    default:
        //CurrentAction->setText("Initializing: M-404.8PD - 200mm");
        status->showMessage("Initializing: M-404.8PD - 200mm", 60);
        valid_Ymotor = loadparam_M404_8pd(serialY);
        break;
    }

    if (valid_Ymotor)
    {
        qDebug()<<"... Changing Y velocity";
        tty_send(1,"VEL 1 10",NULL,serialY);
        Sleeper::msleep(300);

        qDebug()<<"... Enabling position servo";
        tty_send(1,"SVO 1 1",NULL,serialY);
        Sleeper::msleep(300);

        qDebug()<<"... Moving to Y negative limit";
        tty_send(1,"FNL 1",NULL,serialY);

        do {
            Sleeper::msleep(100);
            stage_check_on_target(serialY, 1);
        } while (!YOnTarget);

        qDebug()<<"... Defining reference position";
        tty_send(1,"DEF 1",NULL,serialY);
        Sleeper::msleep(300);

        qDebug()<<"... Going to reference position";
        qDebug()<<"... Please wait";


        moveToRef(100.0000, serialY);


        Enable_TabWidget_3_4_XY();

        InitPhaseY=true;

        if(TimerActive==false)
        {
            timer->start(interval);
            TimerActive=true;
        }
        okY=false;
        InitY=true;
        //tty_send(1,"ERR?",NULL,serialY);
        //read_answer(serialY);
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
        //CurrentAction->setText("Initializing: M-404.2PD - 200mm");
        status->showMessage("Initializing: M-404.2PD - 200mm", 60);
        valid_Zmotor = loadparam_M404_2pd(serialZ);
        break;
    default:
        //CurrentAction->setText("Initializing: M-404.2PD - 200mm");
        status->showMessage("Initializing: M-404.2PD - 200mm", 60);
        valid_Zmotor = loadparam_M404_2pd(serialZ);
        break;
    }

    if (valid_Zmotor)
    {
        qDebug()<<"... Changing Z velocity";
        tty_send(1,"VEL 1 5",NULL,serialZ);
        Sleeper::msleep(300);

        qDebug()<<"... Enabling position servo";
        tty_send(1,"SVO 1 1",NULL,serialZ);
        Sleeper::msleep(300);

        qDebug()<<"... Moving to Z negative limit";
        tty_send(1,"FNL 1",NULL,serialZ);


        do {
            Sleeper::msleep(500);
            stage_check_on_target(serialZ, 2);
        } while (!ZOnTarget);


        qDebug()<<"... Defining reference position";
        tty_send(1,"DEF 1",nullptr,serialZ);
        Sleeper::msleep(300);

        qDebug()<<"... Going to reference position";
        qDebug()<<"... Please wait";

        moveToRef(25.00, serialZ);

        tab_3->setEnabled(true);
        InitZ=true;
        //InitPhaseZ=true;
        if (!TimerActive) {
            timer->start(interval);
            TimerActive = true;
        }
        okZ=false;
    }

    else {
        okZ = false;
        valid_Zmotor = false;
        qDebug()<<"... Motor selection not valid or driver missing";
    }

    pushButton_tab_2_2X->setEnabled(true); // re-enabling X and Y init button
    pushButton_tab_2_2Y->setEnabled(true);
    //tab_2->setEnabled(true);
    INIT_Z_pushButton->setEnabled(false);

    printf("... Z-axis stage initialized\n");
}
















