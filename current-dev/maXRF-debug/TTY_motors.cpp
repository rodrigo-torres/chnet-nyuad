#include "mainwindow.h"
#include <../Header.h>

/*** PENDING: ALL device files should be closed at the program's execution end */
/*** PENDING: read_answer() should be corrected to display the response value */

extern int IniXready, IniYready, IniZready;
extern int serialX, serialY, serialZ, serialK;
extern bool XConnected, YConnected, ZConnected;

extern int send_command(int chan,const char *comando, const char *parametri, int port);
extern string read_answer(int port);
extern void tty_read(int port, char *ans);

QString stylesheet2 = "QLineEdit {background-color: #E7B416; font-weight: bold; color: white;}";

char device_files_name[13]; // 12 chars and null terminated
int device_files[4]= { 0, 1, 2, 0 }; // Ports for x-stage, y-stage, z-stage, arduino, in that order

void MainWindow::tty_set_path(int id) {
    QSignalMapper *tmp = static_cast<QSignalMapper*>(this->sender());
    QSpinBox *sender = static_cast<QSpinBox*>(tmp->mapping(id));
    int port = sender->value();

    device_files[id] = port;
}

int MainWindow::tty_interface_conf(int df_minor_no, int device_type, int device_number = 0) {
    /* Specifying the name of the (serial port) device file */
    char num[2];
    const char *df_path_prefix = "/dev/tty";
    const char *df_major_no[]  = { "USB", "ACM" };
    sprintf(num, "%d", df_minor_no);

    char df_path[13];
    strcpy(df_path, df_path_prefix);
    strcat(df_path, df_major_no[device_type]);
    strcat(df_path, num);

    strcpy(device_files_name, df_path);

    /* Specifying the device file interface information (through the termios structure) */
    struct termios my_termios;

    cfsetispeed(&my_termios, B9600);
    my_termios.c_oflag = 0000000;
    // Output as is, no processing or mapping
    my_termios.c_cflag = 0004275;
    // Character size 8 bits, baud-rate 9600, enable receiver, no flow control or parity check
    my_termios.c_lflag = 0000000;
    // No input processing, no signals, cbreak mode (non-canonical), no echoing
    my_termios.c_iflag = 0042400;
    // CR to NL mapping, XON flow control, UTF8 encoding, rest is disabled

    if (!device_type) {
        my_termios.c_cc[VMIN] = 0;
        my_termios.c_cc[VTIME] = 2;
    }
    else {
        my_termios.c_cc[VMIN] = 8;
        my_termios.c_cc[VTIME]= 0;
    }

    int serial;
    serial = open(df_path, O_RDWR);
    if (serial < 0) {
        char err_message[31];
        strncpy(err_message, "[!] Error opening ", 19);
        strncat(err_message, df_path, 13);
        throw std::runtime_error(err_message);
    }

    tcsetattr(serial, TCSANOW, &my_termios);

    /* Requesting the device to send an identifying string and cross-checking the interface is read ready */
    if (!device_type) {
        fd_set rfds;
        struct timeval tv;
        int retval;

        /* Watch (serial port) file descriptor to see when it has input. */
        FD_ZERO(&rfds);
        FD_SET(serial, &rfds);

        /* Wait up to three seconds. */
        tv.tv_sec = 3; tv.tv_usec = 0;

        tcflush(serial, TCIOFLUSH);
        send_command(1,"*IDN?",NULL,serial);

        /* The baud is B9600 and the message ~61 bytes which takes ~ 50ms to transmit */
        Sleeper::msleep(100);
        retval = select(serial + 1, &rfds, nullptr, nullptr, &tv);

        if (retval == -1) {
            char *buff = strerror(errno);
            throw std::runtime_error(buff);
        }
        else if (retval) {
            char ans[100] = { '\0' };
            tty_read(serial, ans);
            qDebug()<<"[!] Device identified as: "<<ans;
        }
        else {
            const char *buff = "No data within three seconds.\n";
            throw std::runtime_error(buff);
        }
    }

    /* Constructing serial monitor message */
    char comment[22];
    const char *comment_base[] = { "Stage X: ", "Stage Y: ", "Stage Z: ", "Arduino: " };
    QLineEdit *serial_monitors[] = { CurrentActionX, CurrentActionY, CurrentActionZ, CurrentActionACM };

    strcpy(comment, comment_base[device_number]);
    strcat(comment, device_files_name);
    QString qcomment = QString::fromUtf8(comment);

    /* Display serial monitor message */
    serial_monitors[device_number]->setText(qcomment);
    serial_monitors[device_number]->setStyleSheet(stylesheet2);

    return serial;
}

void MainWindow::tty_init(int id) {
    int* list_fds[] = { &serialX, &serialY, &serialZ, &serialK };
    int* list_motor_init[] = { &IniXready, &IniYready, &IniZready };
    bool* list_motor_conn[] = { &XConnected, &YConnected, &ZConnected };

    int dev_type;
    id < 3 ? dev_type = 0 : dev_type = 1;

    if (id < 3 && !(*list_motor_conn)) {
        qDebug()<<"[!] Connection to this linear stage already established";
        return;
    }

    try {
        *list_fds[id] = tty_interface_conf(device_files[id], dev_type, id);
    } catch (const std::runtime_error& e) {
        QString message = QString::fromUtf8(e.what());
        status->showMessage(message);
        return;
    }

    if (id < 3) {
        if (*list_motor_init[id]) status->showMessage("[!] This linear stage has already been initialized");
        else status->showMessage("[!] White while initializing this linear stage...");

        Enabling_Tabwidget();
        *list_motor_conn[id] = true;
    }
    else {
        AssignACM_pushButton->setEnabled(false);
        AUTOFOCUS_ON_pushButton->setEnabled(true);
    }
}


void MainWindow::Enabling_Tabwidget() {
    if (IniXready || IniYready) {
        tab2_3 ->setEnabled(true);
        tab2_4 ->setEnabled(true);
    }
    if (ZConnected) INIT_Z_pushButton->setEnabled(true);
}





