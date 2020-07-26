#include "mainwindow.h"
#include <../Header.h>

extern int IniXready, IniYready, IniZready;
extern int serialX, serialY, serialZ, serialK;

int list_fds[] = { serialX, serialY, serialZ, serialK };
int motor_status_init[] = { IniXready, IniYready, IniZready };
int motor_status_conn[] = { XConnected, YConnected, ZConnected };

extern int send_command(int chan,const char *comando, const char *parametri, int port);
extern string read_answer(int port);

QString stylesheet2 = "QLineEdit {background-color: #E7B416; font-weight: bold; color: white;}";



char device_files_name[13]; // 12 chars and null terminated
void ports_configure(int device, int port, char* dir) {
    char num[2];
    const char *prefix = "/dev/tty";
    const char *type[] = { "USB", "ACM" };
    sprintf(num, "%d", port);

    strcpy(dir, prefix);
    strcat(dir, type[device]);
    strcat(dir, num);
}

int device_files[4]= { 0, 1, 2, 0 }; // Ports for x-stage, y-stage, z-stage, arduino, in that order
void MainWindow::assign_ports(int id) {
    QSignalMapper *tmp = static_cast<QSignalMapper*>(this->sender());
    QSpinBox *sender = static_cast<QSpinBox*>(tmp->mapping(id));
    int port = sender->value();

    device_files[id] = port;
}


int termiosConf(char *directory, bool mode = false) {
    // The flag is implemented as an octal number, defined in termios.h,  manipulated by means of bitwise operators
    struct termios my_termios;

    cfsetispeed(&my_termios, B9600);
    // Output as is, no processing or mapping
    my_termios.c_oflag = 0000000;
    // Ignore local modem lines, no rts/cts flow control, no hang-up, no parity check
    // Character size 8 bits, baud-rate 9600, enable receiver
    my_termios.c_cflag = 0004275;
    // No input processing, no signals, cbreak mode (non-canonical), no miscellaneous
    my_termios.c_lflag = 0000000;
    // CR to NL mapping, XON flow control, UTF8 encoding, rest is disabled
    my_termios.c_iflag = 0042400;

    if (!mode) {
        my_termios.c_cc[VMIN] = 1;
        my_termios.c_cc[VTIME] = 0;
    }
    else {
        my_termios.c_cc[VMIN] = 8;
        my_termios.c_cc[VTIME]= 0;
    }

    int serial;
    serial = open(directory, O_RDWR);
    if (serial < 0) {
        char err_message[31];
        strncpy(err_message, "[!] Error opening ", 19);
        strncat(err_message, directory, 13);
        throw std::invalid_argument(err_message);
    }

    tcsetattr(serial, TCSANOW, &my_termios);
    return serial;
}

void port_read_ready(int serial) {
    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Watch FD given by serial to see when it has input. */
    FD_ZERO(&rfds);
    FD_SET(serial, &rfds);

    /* Wait up to three seconds. */
    tv.tv_sec = 3; tv.tv_usec = 0;

    send_command(1,"*IDN?",NULL,serial);
    retval = select(serial + 1, &rfds, NULL, NULL, &tv);

    if (retval == -1) {
        char *buff = strerror(errno);
        throw std::runtime_error(buff);
    }
    else if (retval)
        printf("Data is available now.\n");
    else {
        const char *buff = "No data within three seconds.\n";
        throw std::runtime_error(buff);
    }
}


void MainWindow::AssignX() {
    if (!XConnected) SetSerialXName(device_files[0]);
    else qDebug()<<"[!] Connection to X-axis stage already established";
}


void MainWindow::AssignY() {
    if (!YConnected) SetSerialYName(device_files[1]);
    else qDebug()<<"[!] Connection to Y-axis stage already established";
}

void MainWindow::AssignZ() {
    if (!ZConnected) SetSerialZName(device_files[2]);
    else qDebug()<<"[!] Connection to X-axis stage already established";
}


void MainWindow::SetSerialXName(int number) {
    ports_configure(0, number, device_files_name);
    try {
        serialX = termiosConf(device_files_name);
    } catch (const std::invalid_argument& e) {
        QString message = QString::fromUtf8(e.what());
        status->showMessage(message);
        qDebug()<<"\n[!] Linear stage X port not inited";
        return;
    }

    try {
        port_read_ready(serialX);
    } catch (const std::invalid_argument& e) {
        QString message = QString::fromUtf8(e.what());
        status->showMessage(message);
        return;
    }

    tcflush(serialX, TCIOFLUSH);
    if (IniXready) status->showMessage("[!] Linear stage X already inited");
    else status->showMessage("[!] Wait while initializing linear stage X...");

    Enabling_Tabwidget();
    XConnected = true;

    QString commentoX = "Stage X: ";
    commentoX.append(device_files_name);
    CurrentActionX->setText(commentoX);
    CurrentActionX->setStyleSheet(stylesheet2);
}

void MainWindow::SetSerialYName(int number) {
    ports_configure(0, number, device_files_name);
    try {
        serialY = termiosConf(device_files_name);
    } catch (const std::invalid_argument& e) {
        QString message = QString::fromUtf8(e.what());
        status->showMessage(message);
        qDebug()<<"\n[!] Linear stage Y port not inited";
        return;
    }

    try {
        port_read_ready(serialY);
    } catch (const std::invalid_argument& e) {
        QString message = QString::fromUtf8(e.what());
        status->showMessage(message);
        return;
    }

    tcflush(serialY, TCIOFLUSH);
    if (IniYready) status->showMessage("[!] Linear stage Y already inited");
    else status->showMessage("[!] Wait while initializing linear stage Y...");

    Enabling_Tabwidget();
    YConnected = true;

    QString commento = "Stage Y: ";
    commento.append(device_files_name);
    CurrentActionY->setText(commento);
    CurrentActionY->setStyleSheet(stylesheet2);
}

////////////////////////////////////////////////////////////// 3 - SAME FOR Z MOTOR

void MainWindow::SetSerialZName(int number) {
    ports_configure(0, number, device_files_name);
    try {
        serialZ = termiosConf(device_files_name);
    } catch (const std::invalid_argument& e) {
        QString message = QString::fromUtf8(e.what());
        status->showMessage(message);
        qDebug()<<"\n[!] Linear stage Z port not inited";
        return;
    }

    try {
        port_read_ready(serialZ);
    } catch (const std::invalid_argument& e) {
        QString message = QString::fromUtf8(e.what());
        status->showMessage(message);
        return;
    }

    tcflush(serialZ, TCIOFLUSH);
    if (IniZready) status->showMessage("[!] Linear stage Z already inited");
    else status->showMessage("[!] Wait while initializing linear stage Z...");

    Enabling_Tabwidget();
    ZConnected = true;

    QString commento = "Stage Z: ";
    commento.append(device_files_name);
    CurrentActionZ->setText(commento);
    CurrentActionZ->setStyleSheet(stylesheet2);

}


void MainWindow::Enabling_Tabwidget() {
    if (IniXready || IniYready) {
        tab2_3 ->setEnabled(true);
        tab2_4 ->setEnabled(true);
    }
    if (ZConnected) INIT_Z_pushButton->setEnabled(true);
}



void MainWindow::Init_KeyenceLaser() {
    ports_configure(1, device_files[3], device_files_name);
    try {
        serialK = termiosConf(device_files_name, true);
    } catch (const std::invalid_argument& e) {
        cerr<<e.what();
        qDebug()<<"\n[!] Keyence port not inited";
        return;
    }
    tcflush(serialK, TCIFLUSH);

    AssignACM_pushButton->setEnabled(false);
    AUTOFOCUS_ON_pushButton->setEnabled(true);

    QString monitor_keyence = "Arduino: ";
    monitor_keyence.append(device_files_name);

    CurrentActionACM->setText(monitor_keyence);
    CurrentActionACM->setStyleSheet(stylesheet2);
}









