//#include "mainwindow.h"
#include "../Header.h"
#include <../Shm.h>
#include <time.h>
#include <fstream>
#include "mainwindow.h"
#include "all_tty.h"

controller *controller_ptr = nullptr;
extern int* shared_memory_cmd;

extern QString stylesheet3;

extern int serialX, serialY, serialZ;

extern int interval;
int interval = 250;

// These are global functions used to commmunicate to the motors through the file descriptors.
extern int tty_send(int channel, const char *command, const char *parameter, int port);
void tty_read(int port, char *ans, unsigned long wait = 0);

void controller::start_timer(int interval) {
    mainwindow_ptr->timer->start(interval);
}

all_tty::all_tty(MainWindow *ptr) : _ptr(ptr) {
    /* Initializing the members of struct stage_t */
    for (int i = 0; i < 3; i++) {
        stage[i].df_minor_number = i;
        stage[i].stage_init_status = 0;
        stage[i].stage_fd = -1;
        stage[i].stage_id = i;
        stage[i].stage_on_target = false;
    }
}

void all_tty::abort() {
    //int fds[] = { serialX, serialY, serialZ };
    for (int i = 0; i < 3; i++) {
        if (!isatty(stage[i].stage_fd)) continue ;

        char ans[15] = { '\0' };
        tty_send(1, "HLT", nullptr, stage[i].stage_fd);
        tty_send(1, "ERR?", nullptr, stage[i].stage_fd);
        tty_read(stage[i].stage_fd, ans);

        qDebug()<<"[!] Linear stage at FD "<<stage[i].stage_fd<<"halted with error code: "<<ans;
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

void all_tty::stage_check_on_target(stage_t* stage) {
    const char prefix[] = "Stage ";
    const char *axes[]  = { "X: ", "Y: ", "Z: "};
    QLineEdit *monitors[] = { _ptr->CurrentActionX, _ptr->CurrentActionY, _ptr->CurrentActionZ };

    int id = stage->stage_id;
    bool *on_target = &(stage->stage_on_target);

    char ans[15] = { '\0' };
    tty_send(1, "ONT?", nullptr, stage->stage_fd); // Expected a response with format 1=%d with %d = 1,0

    try {
        tty_read(stage->stage_fd, ans, 10);
    } catch (const runtime_error& e) {
        qDebug()<<e.what();
    }

    if (ans[2] == '1' ) *on_target = true;
    else *on_target = false;

    char ans2[15] = { '\0' };
    tty_send(1,"POS?", nullptr, stage->stage_fd); // Expected a response with format 1={x->xxx}.xxxx

    try {
        tty_read(stage->stage_fd, ans2, 15);
    } catch (const runtime_error& e) {
        qDebug()<<e.what();
    }

//  Not requires as when the servo is on, Z position is already stored
//    if (id == 2) {
//        QString temp = &ans2[2];
//        ZPosition = temp.toDouble();
//    }

    char message[25] = { '\0' };
    strncpy(message, prefix, sizeof(prefix));
    strncat(message, axes[id], 4);
    strncat(message, &ans2[2], 10);
    strncat(message, " mm", sizeof(" mm"));
    monitors[id]->setText(message);
    monitors[id]->setStyleSheet(stylesheet3);
}

void all_tty::stage_init(int ind) {
    stage_t* stage_p = &stage[ind];

    /* Identifying linear stage */
    int fd = stage_p->stage_fd;
    int id = stage_p->stage_id;
    bool* on_target = &(stage_p->stage_on_target);

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
    stage_load_param(fd, model[id]);

    if (ret) {
        qDebug()<<"... Changing selected stage velocity";
        const char *vel_param[] = { "VEL 1 15", "VEL 1 10", "VEL 1 5"};
        tty_send(1, vel_param[id], nullptr, fd);

        qDebug()<<"... Enabling position servo";
        tty_send(1, "SVO 1 1", nullptr, fd);

        qDebug()<<"... Moving selected stage to its negative limit";
        tty_send(1, "FNL 1", nullptr, fd);

        do {
            Sleeper::msleep(100);
            if (*(shared_memory_cmd+200) ==  1) {
                abort();
                return;
            }
            stage_check_on_target(stage_p);
        } while (!(*on_target));

        qDebug()<<"... Defining reference position";
        tty_send(1, "DEF 1", nullptr, fd);

        qDebug()<<"... Going to reference position"
                  "\n... Please wait";

        double ref_pos[] = { 100., 100., 25. };

        char stemp[100];
        sprintf(stemp, "%f", ref_pos[id]);
        tty_send(1, "MOV", stemp, fd);

        do {
            Sleeper::msleep(100);
            if (*(shared_memory_cmd+200) ==  1) {
                abort();
                return;
            }
            stage_check_on_target(stage_p);
        } while (!(*on_target));

        id == 2 ? _ptr->tab_3->setEnabled(true) : _ptr->Enable_TabWidget_3_4_XY();

        if (_ptr->timer->isActive()) {
            _ptr->timer->blockSignals(false);
            emit this->stage_timer_start(interval);
        }

        stage_p->stage_init_status = 1;
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


void MainWindow::Init_Xmotor() {
    if (!isatty(serialX)) {
        qDebug()<<"[!] Communication with this port has not yet been initialized";
        return;
    }
    if (controller_ptr == nullptr) controller_ptr = new controller(this);
    emit controller_ptr->stage_init(0);
}

void MainWindow::Init_Ymotor() {
    if (!isatty(serialY)) {
        qDebug()<<"[!] Communication with this port has not yet been initialized";
        return;
    }  
    if (controller_ptr == nullptr) controller_ptr = new controller(this);
    emit controller_ptr->stage_init(1);
}

void MainWindow::Init_Zmotor(){
    if (!isatty(serialZ)) {
        qDebug()<<"[!] Communication with this port has not yet been initialized";
        return;
    }
    if (controller_ptr == nullptr) controller_ptr = new controller(this);
    emit controller_ptr->stage_init(2);
}
















