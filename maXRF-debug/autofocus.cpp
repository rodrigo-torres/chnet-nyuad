#include "../Header.h"
#include "mainwindow.h"

extern void tty_read(int port, char *ans, unsigned long wait = 0);
extern int tty_send(int chan,const char *comando, const char *parametri, int port);

extern bool AutofocusOn, RunTracking;
extern double ZPosition, Autofocus_value;
extern int serialK, serialZ, servo_thresh;

int servo_thresh = 200;
bool RunTracking = false;
double Autofocus_value = 0;
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
    string checkK;
    try {
        checkK = read_ACMport();
    } catch (const std::invalid_argument& e) {
        cerr<<e.what();
        tcflush(serialK, TCIFLUSH);
        tty_send(1,"HLT",NULL,serialZ);
        qDebug()<<"[!] Flushed the Keyence serial";
        return;
    }

    QString KeyenceValue = "";
    KeyenceValue.append(checkK.data());
    KeyenceValue.truncate(6);
    Autofocus_value = KeyenceValue.toDouble();
    Autofocus_value = (-1) * Autofocus_value;

    bool in_range = false;
    if ( (Autofocus_value <= (15.0)) && ( Autofocus_value > (-15.0)) ) {
        //qDebug()<<Autofocus_average_value;
        in_range=true;
        QString valueAsString = QString::number(Autofocus_value, 'f', 3);
        valueAsString.prepend("Target distance: ");
        valueAsString.append(" mm");
        lineEdit_2_tab_4->setText(valueAsString);
    }

    else {
        qDebug()<<"[!] Laser out of range, autofocus value: "<<Autofocus_value;
        lineEdit_2_tab_4->setText("[!] Out of range");
    }

    if (RunTracking && in_range) AutoFocusRunning();
    in_range = false;
}



void MainWindow::AutoFocusRunning() {
    int DistanceLevel = abs(qRound(Autofocus_value * 1000));
    double NewPositionValue = (ZPosition + Autofocus_value);
    double vel = static_cast<double>(DistanceLevel / 2000);

    char buff[50];
    if (DistanceLevel < servo_thresh) {
        tty_send(1, "HLT", nullptr, serialZ);
        tty_send(1, "ERR?", nullptr, serialZ);

        tty_read(serialZ, buff);
        int ret = strncmp(buff, "0", sizeof ("0"));
        if (ret != 0) qDebug()<<"[!] Stage Z motor stopped through servo with error: "<<buff;
        return;
    }
    else {
        char v[10];
        sprintf(v, "%f", vel);
        tty_send(1, "VEL", v, serialZ);

        sprintf(buff, "%f", NewPositionValue);
        tty_send(1, "MOV", buff, serialZ);
    }
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
    if (AutofocusOn) {
        tty_send(1, "POS?", nullptr, serialZ);

        char ans[30];
        tty_read(serialZ, ans);
        QString store(ans);
        ZPosition = store.toDouble();


        QString posLabelZ = "Stage Z: ";
        posLabelZ.append(store);
        posLabelZ.append(" mm");
        CurrentActionZ->setText(posLabelZ);
        CurrentActionZ->setStyleSheet(stylesheet3);
    }

    readKeyence();
}





