#include "../Header.h"
#include "mainwindow.h"


extern double ZPosition;
 QString KeyenceValue;
extern int serialK,serialZ;
extern int send_command(int chan,const char *comando, const char *parametri, int port);
extern string read_answer(int port);

//extern bool noKeyence_init;
extern bool AutofocusOn, ZOnTarget;
 string checkK;

double Autofocus_value=0;  double Autofocus_average_value=0;

extern char process[30];

double NewPositionValue;

int AutofocusIndex=0;             int AutofocusStore=0;      int DistanceLevel;           int NewPosInt=1000;    

bool ValueInRange=false; bool RunTracking=false;

bool condition2=false;

extern QString stylesheet3;



void MainWindow::Autofocus2() {
    if (AutofocusOn) {
        AutofocusOn=false;
        timerAutofocus->stop();
        ENABLE_TRACKING_checkBox->setEnabled(false);
    }
    else {
        tcflush( serialK, TCIFLUSH );
        timerAutofocus->start(500);
        ENABLE_TRACKING_checkBox->setEnabled(true);
        AutofocusOn=true;
    }
}

static void printchar(unsigned char theChar) {

    switch (theChar) {

        case '\n':
            printf("\\n\n");
            break;
        case '\r':
            printf("\\r\n");
            break;
        case '\t':
            printf("\\t");
            break;
        default:
            if ((theChar < 0x20) || (theChar > 0x7f)) {
                printf("\\%03o", (unsigned char)theChar);
            } else {
                printf("%c", theChar);
            }
        break;
   }
}


string read_ACMport() { // The serial print of the Arduino is set to return 6 characters ( ±xx.xx )
    //int n=0;
    //char ans_Keyence[6] = {};
    char buf_Keyence[8] = {};
    //string restK;

    long n;
    char enableBit[2] = "1";
    write(serialK, enableBit, static_cast<size_t>(1));
    n = read(serialK, &buf_Keyence, 8); //The serial returns a \r\n at the end of each line

    if (buf_Keyence[6] != '\n' || buf_Keyence[7] != '\n') {
        printchar(buf_Keyence[6]);
        printchar(buf_Keyence[7]);
        throw std::invalid_argument("The data received is not in the format expected");
    }
    string restK(buf_Keyence, 0, 6);
    return restK;
    }

void MainWindow::readKeyence() {
    try {
        checkK = read_ACMport();
    } catch (const std::invalid_argument& e) {
        cerr<<e.what();
        tcflush(serialK, TCIFLUSH);
        send_command(1,"HLT",NULL,serialZ);
        qDebug()<<"[!] Flushed the Keyence serial";
        return;
    }

    KeyenceValue = "";
    KeyenceValue.append(checkK.data());
    KeyenceValue.truncate(6);
    Autofocus_value=KeyenceValue.toDouble();

    Autofocus_average_value=-Autofocus_value;

    //if (abs(floor(Autofocus_average_value)) > 7) stop_motorXY();

    if ( (Autofocus_average_value <= (15.0)) && ( Autofocus_average_value > (-15.0)) ) {
        //qDebug()<<Autofocus_average_value;
        ValueInRange=true;
        QString valueAsString = QString::number(Autofocus_average_value, 'f', 3);
        valueAsString.prepend("Target distance: ");
        valueAsString.append(" mm");
        lineEdit_2_tab_4->setText(valueAsString);
    }

    else {
        qDebug()<<"[!] Laser out of range, autofocus value: "<<Autofocus_average_value;
        ValueInRange=false;
        lineEdit_2_tab_4->setText("[!] Out of range");
    }

    if (RunTracking && ValueInRange) AutoFocusRunning();
}



void MainWindow::AutoFocusRunning() {
            NewPositionValue = (ZPosition + Autofocus_average_value);
            DistanceLevel = abs(qRound(Autofocus_average_value * 1000));

            char v[10];

            /* [Doc.]   The following modifies the velocity of the Z-motor accordint to the difference between the current Z-motor position,
             *          and the desired Z-motor position. Then issues a command to the Z-motor to move with the specified velocity to the calculated
             *          position.
             */
            if (DistanceLevel>=5000)                         { sprintf(v, "%f", 2.50); send_command(1, "VEL", v,serialZ); }
            if ((DistanceLevel>=3000)&&(DistanceLevel<5000)) { sprintf(v, "%f", 1.50); send_command(1, "VEL", v,serialZ); }
            if ((DistanceLevel>=1000)&&(DistanceLevel<3000)) { sprintf(v, "%f", 1.00); send_command(1, "VEL", v,serialZ); }
            if ((DistanceLevel>=500)&&(DistanceLevel<1000))  { sprintf(v, "%f", 0.50); send_command(1, "VEL", v,serialZ); }
            if ((DistanceLevel>=200)&&(DistanceLevel<500))   { sprintf(v, "%f", 0.25); send_command(1, "VEL", v,serialZ); }
            if (DistanceLevel < 200)	                     { send_command(1,"HLT",NULL,serialZ); return; }

            //NewPosInt=qRound(NewPositionValue*1000);
            //if (ZOnTarget) return;
            if (DistanceLevel > 500) {
                //AutofocusStore=NewPosInt;
                char sx[100];
                sprintf(sx,"%f",NewPositionValue);
                send_command(1,"MOV",sx,serialZ);
            }
            ValueInRange=false;
}


void MainWindow::TrackingON() {
    if(RunTracking) {
        RunTracking=false;
        qDebug()<<"OFF";
        MOVE_Z_FORWARD_pushButton->setEnabled(true);
        MOVE_Z_BACKWARD_pushButton->setEnabled(true);
        MOVE_Z_To_pushButton->setEnabled(true);
        MOVE_Z_To_doubleSpinBox->setEnabled(true);
        //MOVE_Z_MOTOR_label_1->setEnabled(true);
        //MOVE_Z_MOTOR_label_2->setEnabled(false);
    }
    else {
        RunTracking=true;
        qDebug()<<"ON";
        MOVE_Z_FORWARD_pushButton->setEnabled(false);
        MOVE_Z_BACKWARD_pushButton->setEnabled(false);
        MOVE_Z_To_pushButton->setEnabled(false);
        MOVE_Z_To_doubleSpinBox->setEnabled(false);
        //MOVE_Z_MOTOR_label_1->setEnabled(false);
        //MOVE_Z_MOTOR_label_2->setEnabled(false);

        tcflush( serialK, TCIFLUSH );
    }
}


void MainWindow::Focustimer() {
    //if (noKeyence_init) Init_KeyenceLaser();
//    if (AutofocusOn) {
//        send_command(1,"POS?",NULL,serialZ);
//        QString store = QString::fromStdString(read_answer(serialZ));
//        store.remove(0,2);
//        ZPosition = store.toDouble();


//        QString posLabelZ = "Stage Z: ";
//        posLabelZ.append(store);
//        posLabelZ.append(" mm");
//        CurrentActionZ->setText(posLabelZ);
//        CurrentActionZ->setStyleSheet(stylesheet3);
//    }

    readKeyence();
}





