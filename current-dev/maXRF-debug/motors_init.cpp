#include "mainwindow.h"
#include "../Header.h"
#include <../Shm.h>
#include <time.h>
#include <fstream>
#include <all_tty.h>

controller *controller_ptr = nullptr;

extern int* shared_memory_cmd;

// Since qtimers cannot be started from a different thread, we're constricted to using QTHREAD
mutex m_lock;
// These are global variables used in the time stamp of the scan function.
int interval = 250;
bool TimerActive = false;

extern int IniXready, IniYready, IniZready;
extern bool stage_on_target[3];

// These are global variables that specify the file descriptors for the USB ports and the type of motors used.
extern int serialX,serialY,serialZ;

// These are global functions used to commmunicate to the motors through the file descriptors.
extern int tty_send(int channel, const char *command, const char *parameter, int port);
void tty_read(int port, char *ans, unsigned long wait = 0);

all_tty::all_tty(MainWindow *ptr) : _ptr(ptr) {
    int val  = -1;
    val = ptr->spinBox_assignX->value();
    qDebug()<<"pointer to class MainWindow succesfully passed, spinboxX value: "<<val;
}

void all_tty::abort() {
    int fds[] = { serialX, serialY, serialZ };
    for (int i = 0; i < 3; i++) {
        if (!isatty(fds[i])) continue ;

        char ans[15] = { '\0' };
        tty_send(1, "HLT", nullptr, fds[i]);
        tty_send(1, "ERR?", nullptr, fds[i]);
        tty_read(fds[i], ans);

        qDebug()<<"[!] Linear stage at FD "<<fds[i]<<"halted with error code: "<<ans;
    }   // Error code should be 10: "Controller stopped on command"
}

int all_tty::mod_tty_send(int fd, string line) {
    char buff[100];
    strncpy(buff, line.c_str(), sizeof (buff));

    char delim[] = " ";
    char *params[5] = { strtok(buff, delim), nullptr };

    int i = 0;
    while (params[i] != nullptr) {
        params[++i] = strtok(nullptr, delim);
        if (i == sizeof (params) - 1) break; // Preventing segmentation fault errors
    }

    int chan = *params[1] - '0'; char args[50];
    strncpy(args, params[2], sizeof (args));
    strcat(args, delim);
    strcat(args, params[3]);

    tty_send(chan, params[0], args, fd);
    tty_send(chan, "SPA?", params[2], fd);

    char ans[30];
    tty_read(fd, ans);

    char *ptr = strtok(ans, "=\n");
    ptr = strtok(nullptr, "=\n");

    int ret = strncmp(params[3], ptr, strlen(params[3]));
    if (ret != 0) qDebug()<<"[!] Parameter not set correctly, identifier"<<params[2];

    return abs(ret);
}

void all_tty::stage_load_param(int fd, const char *file_name) {
    QString file_path_q = QDir::currentPath();
    QByteArray temp = file_path_q.toUtf8();

    char file_path[100];
    strncpy(file_path, temp.data(), sizeof (file_path));
    strcat(file_path, "/conf");
    strcat(file_path, file_name);

    ifstream file;
    file.open(file_path);

    int ret = 0;
    if (file.is_open()) {
        string line;
        char buff[100];
        while (true) {
            getline(file, line, '\n');
            if (file.eof()) break;
            strncpy(buff, line.c_str(), sizeof (buff));
            if (buff[0] == '#') continue;
            else ret |= mod_tty_send(fd, line);
        }
        if (ret != 0) qDebug()<<"[!] There were errors in setting the stage parameters"
                                "[!] Please check your configuration file: "<<file_name;
        else qDebug()<<"[!] All stage parameters loaded succesfully";
    }
    else qDebug()<<"[!] Configuration file not found";
}

void all_tty::stage_init(int serial) {
    /* Identifying linear stage */
    int fds[] = { serialX, serialY, serialZ };
    int id = -1;
    for (int i = 0; i < 3; i++) {
        if (fds[i] == serial) id = i;
    }

    bool *on_target[] = { &stage_on_target[0], &stage_on_target[1], &stage_on_target[2]};

    /* Disabling all stage initialization pushbuttons to prevent conflicts */
    _ptr->pushButton_tab_2_2X->setEnabled(false);
    _ptr->pushButton_tab_2_2Y->setEnabled(false);
    _ptr->INIT_Z_pushButton->setEnabled(false);
    _ptr->timer->blockSignals(true);


    char message[100];
    const char *model[] = { "/M404_8PD.txt", "/M404_8PD.txt",  "/M404_2PD.txt" };
    strncpy(message, "Loading configuration file: ", sizeof(message));
    strcat(message, model[id]);

    //_ptr->status->showMessage(message, 60);

    bool ret = 1;
    stage_load_param(serial, model[id]);

    if (ret) {
        qDebug()<<"... Changing selected stage velocity";
        const char *vel_param[] = { "VEL 1 15", "VEL 1 10", "VEL 1 5"};
        tty_send(1, vel_param[id], nullptr, serial);

        qDebug()<<"... Enabling position servo";
        tty_send(1, "SVO 1 1", nullptr, serial);

        qDebug()<<"... Moving selected stage to its negative limit";
        tty_send(1, "FNL 1", nullptr, serial);

        do {
            Sleeper::msleep(100);
            if (*(shared_memory_cmd+200) ==  1) {
                abort();
                return;
            }
            _ptr->stage_check_on_target(serial, id);
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
            if (*(shared_memory_cmd+200) ==  1) {
                abort();
                return;
            }
            _ptr->stage_check_on_target(serial, id);
        } while (!(*on_target[id]));

        id == 2 ? _ptr->tab_3->setEnabled(true) : _ptr->Enable_TabWidget_3_4_XY();

        if (TimerActive == false) {
            _ptr->timer->blockSignals(false);
            emit this->stage_timer_start(interval);
            TimerActive=true;
        }

        int *inited[] = { &IniXready, &IniYready, &IniZready};
        *inited[id] = 1;
    }

    else {
        qDebug()<<"Invalid motor selection";
    }
    /* Renabling all stage initialization pushbuttons */
    _ptr->timer->blockSignals(false);
    _ptr->pushButton_tab_2_2X->setEnabled(true);
    _ptr->pushButton_tab_2_2Y->setEnabled(true);
    _ptr->INIT_Z_pushButton->setEnabled(true);
}

void MainWindow::stage_init(int serial) {
}

void MainWindow::Init_Xmotor() {
    if (!isatty(serialX)) {
        qDebug()<<"[!] Communication with this port has not yet been initialized";
        return;
    }
    if (controller_ptr == nullptr) controller_ptr = new controller(this);
    emit controller_ptr->stage_init(serialX);
    //all_tty tty_o(this);
    //tty_o.stage_init(serialX);
}

void MainWindow::Init_Ymotor() {
    if (!isatty(serialY)) {
        qDebug()<<"[!] Communication with this port has not yet been initialized";
        return;
    }  
    if (controller_ptr == nullptr) controller_ptr = new controller(this);
    emit controller_ptr->stage_init(serialY);
}

void MainWindow::Init_Zmotor(){
    if (!isatty(serialZ)) {
        qDebug()<<"[!] Communication with this port has not yet been initialized";
        return;
    }
    if (controller_ptr == nullptr) controller_ptr = new controller(this);
    emit controller_ptr->stage_init(serialZ);
}
















