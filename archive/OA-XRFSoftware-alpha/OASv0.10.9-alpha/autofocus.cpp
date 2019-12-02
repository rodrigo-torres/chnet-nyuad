#include "../Header.h"
#include "mainwindow.h"

extern bool noKeyence_init, AutofocusOn;

extern double ZPosition;
extern int serialK,serialZ;
extern int send_command(int chan,const char *comando, const char *parametri, int port);

string checkK;
QString KeyenceValue;
bool ValueInRange=false, RunTracking=false;
double Autofocus_value=0, Autofocus_average_value=0, NewPositionValue;
int AutofocusIndex=0, AutofocusStore=0,  DistanceLevel, NewPosInt=1000;

bool condition;
bool condition2=false;

void align() {
    int n=0;
    char delimiter[2] = {};
    //char buf_Keyence[9] = {};

    while ( ( n = read(serialK, &delimiter, 2 ) ) > 0 ) {
            if ( delimiter[0] == '\n' && delimiter [1] == '\n')
            {
                delimiter[0] = 0;
                delimiter[1] = 0;
                break;
            }
    }
}

string read_ACMport() {

    /*
     *  Optimized for the output rate of the ttyACM port and the format of the data
     */

    int n=0, interval=500000;
    char buf_Keyence[11] = {};
    char ans_Keyence[10] = {};

    while (true) {
        n = read(serialK, &buf_Keyence, 11);

        if ( n == -1 || n == 0 ) { return "0"; }
        else if ( n != 11) { usleep(interval); align(); continue; }
        else {
            for (int i=0; i<9; i++) { ans_Keyence[i] = buf_Keyence[i]; }
            ans_Keyence[9]='\0';

            for (int i=0; i<11; i++) { buf_Keyence[i] = '\0'; }
            break;
        }
    }

    string restK(ans_Keyence);
    printf("%s'\n'",restK.c_str());
    return restK;
}

void MainWindow::Autofocus2() {
    if(AutofocusOn) {
        AutofocusOn=false;
        //printf("Autofocuson=false\n");
        timerAutofocus->stop();
        condition=true;
        ENABLE_TRACKING_checkBox->setEnabled(false);
    }
    else {
        AutofocusOn=true;
        //printf("Autofocuson=true\n");
        timerAutofocus->start(500);
        ENABLE_TRACKING_checkBox->setEnabled(true);
    }
}

void MainWindow::readKeyence() {

    //qDebug()<<"[!] Start readKeyence";

    if ( noKeyence_init ) { Init_KeyenceLaser(); }

    int counter_readKeyence = 0;
    while ( counter_readKeyence < 1) {
        //qDebug()<<counter_readKeyence;
        checkK = read_ACMport();
        KeyenceValue="";
        KeyenceValue.append(checkK.data());
        KeyenceValue.truncate(9);
        Autofocus_value=KeyenceValue.toDouble();

        if ( Autofocus_value == 0) {
            qDebug()<<"... Keyence busy";
        }

        else {
            Autofocus_value=-((115226-Autofocus_value)/408.345);
            Autofocus_average_value=Autofocus_value;
            counter_readKeyence++;
        }
    }
   if ( (Autofocus_average_value <= (15.0)) && ( Autofocus_average_value > (-15.0)) ) {
       //qDebug()<<Autofocus_average_value;
       ValueInRange=true;
       QString valueAsString = QString::number(Autofocus_average_value);
       lineEdit_2_tab_4->setText(valueAsString);
   }

   else {
       qDebug()<<"[!] Laser out of range, autofocus value: "<<Autofocus_average_value;
       ValueInRange=false;
       lineEdit_2_tab_4->setText("[!] Out of range");
   }
   return;
}

void MainWindow::AutoFocusRunning() {

    if(RunTracking) {
        if(ValueInRange) {
            NewPositionValue=(ZPosition+Autofocus_average_value);
            DistanceLevel=abs(qRound((Autofocus_average_value*1000)));

            char v[10];

            /* [Doc.]   The following modifies the velocity of the Z-motor accordint to the difference between the current Z-motor position,
             *          and the desired Z-motor position. Then issues a command to the Z-motor to move with the specified velocity to the calculated
             *          position.
             */
            if(DistanceLevel>=5000)                         { sprintf(v, "%f", 5.0); send_command(1, "VEL", v,serialZ); }
            if((DistanceLevel>=3000)&&(DistanceLevel<5000)) { sprintf(v, "%f", 3.0); send_command(1, "VEL", v,serialZ); }
            if((DistanceLevel>=1000)&&(DistanceLevel<3000)) { sprintf(v, "%f", 1.2); send_command(1, "VEL", v,serialZ); }
            if((DistanceLevel>=500)&&(DistanceLevel<1000))  { sprintf(v, "%f", 0.5); send_command(1, "VEL", v,serialZ); }
            if((DistanceLevel>=200)&&(DistanceLevel<500))   { sprintf(v, "%f", 0.1); send_command(1, "VEL", v,serialZ); }
            if (DistanceLevel < 200)	                       { sprintf(v, "%f", 0.04); send_command(1, "VEL", v,serialZ);}

            NewPosInt=qRound(NewPositionValue*1000);

            if(DistanceLevel>=50) {
                AutofocusStore=NewPosInt;
                char sx[100];
                sprintf(sx,"%f",NewPositionValue);
                send_command(1,"MOV",sx,serialZ);
            }
        }
    }
    condition2=false;
}

void MainWindow::TrackingON() {
    if(RunTracking) {
        RunTracking=false;
        //qDebug()<<"OFF";
        MOVE_Z_FORWARD_pushButton->setEnabled(true);
        MOVE_Z_BACKWARD_pushButton->setEnabled(true);
        MOVE_Z_To_pushButton->setEnabled(true);
        MOVE_Z_To_doubleSpinBox->setEnabled(true);
        MOVE_Z_MOTOR_label_1->setEnabled(true);
        MOVE_Z_MOTOR_label_2->setEnabled(false);
    }
    else {
        RunTracking=true;
        //qDebug()<<"ON";
        MOVE_Z_FORWARD_pushButton->setEnabled(false);
        MOVE_Z_BACKWARD_pushButton->setEnabled(false);
        MOVE_Z_To_pushButton->setEnabled(false);
        MOVE_Z_To_doubleSpinBox->setEnabled(false);
        MOVE_Z_MOTOR_label_1->setEnabled(false);
        MOVE_Z_MOTOR_label_2->setEnabled(false);
    }
}

void MainWindow::Focustimer() {

    int n=0;
    char delimiter[2] = {};
    char buf_Keyence[11] = {};

    // Prevents double-calls to the functions below
    if ( condition2 == true ) { return; }

    //Goes to end of Keyence file descriptor
    while ( condition ) {

            // Search for new line, and align
            while ( ( n = read(serialK, &delimiter, 2 ) ) > 0 )
            {
                    if ( delimiter[0] == '\n' && delimiter [1] == '\n')
                    {
                        delimiter[0] = 0;
                        delimiter[1] = 0;
                        break;
                    }
            }

            while ( (n = read(serialK, &buf_Keyence, 11)) == 11 ) {}

            condition = false;
    }
    condition2=true;
    readKeyence();
    AutoFocusRunning();
}





