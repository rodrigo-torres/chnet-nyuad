#include "mainwindow.h"
#include "tty.h"

//extern int shmid[8];
extern int *shared_memory;
extern int *shared_memory_cmd;

tty_agent *tty_ptr;

MAXRF_UI::MAXRF_UI(QWidget *parent) : QMainWindow(parent)
{
    CreateSHMResources();                 /// CREATING SHARED MEMORY SEGMENT
    create_menu_actions();
    CreateMenu();            	    /// CREATING MENU from Menu.cpp
    CreateGUI();
    CreateConnections();

    StartThreadTTY();

    this->resize(this->sizeHint());
    this->show();
}


void MAXRF_UI::StartThreadTTY() {

    tty_ptr = new tty_agent();
    tty_ptr->moveToThread(&motors_thread_);

    connect(&motors_thread_, &QThread::finished, tty_ptr, &QObject::deleteLater);
    connect(tty_ptr, &tty_agent::toggle_tab1, this, &MAXRF_UI::toggle_tab1);
    connect(tty_ptr, &tty_agent::toggle_widgets, this, &MAXRF_UI::toggle_widgets);
    connect(tty_ptr, &tty_agent::update_monitor, this, &MAXRF_UI::update_monitor);

//    connect(this, &MAXRF_UI::set_target, tty_ptr, &tty_agent::set_target);
    connect(this, &MAXRF_UI::keyence_reading, tty_ptr, &tty_agent::enable_servo);
    connect(this, &MAXRF_UI::start_servo, tty_ptr, &tty_agent::start_servo);

    connect(this, &MAXRF_UI::request_tty_action, tty_ptr, &tty_agent::relay_command);

    for (int i = 0; i < 9; i++) {
      connect(buttonTab3[i], &QPushButton::released, this, [=] {
        if (i < 3) {
          tty_ptr->move_stage(i, dspinboxes.at(i)->value());
        }
        else {
          tty_ptr->move_stage(i, 1.);
        }
      });
    }

    motors_thread_.start();
};

/* Signals */

void MAXRF_UI::enable_keyence_reading() {
    QCheckBox *tmp = static_cast<QCheckBox*>(this->sender());
    bool active = tmp->checkState();

    servo_checkbox->setEnabled(active);
    emit keyence_reading(active);
}

void MAXRF_UI::enable_servo()
{
    QCheckBox *tmp = static_cast<QCheckBox*>(this->sender());
    bool active = tmp->checkState();

    typedef widgets::spinboxes en_pb;

    //buttonTab3[2]->setEnabled(active);
    //buttonTab3[7]->setEnabled(active);
    //buttonTab3[8]->setEnabled(active);

    //dspinboxes.at(en_pb::targetz)->setEnabled(active);
    //spinboxTab3[2]->setEnabled(active);

    emit start_servo(active);
}


/* Public slots */


void MAXRF_UI::toggle_tab1(bool state)
{
    tab1->setEnabled(state);

}

void MAXRF_UI::toggle_widgets(int id) {
    switch (id) {
    case 0: case 1:
        tab2->setEnabled(true);
        break;
    case 2:
        break;
    case 3:
        laser_checkbox->setEnabled(true);
    }
}

void MAXRF_UI::update_monitor(QString message, QString style, int id){
    dev_monitor[id]->setText(message);
    dev_monitor[id]->setStyleSheet(style);
}

//void MAXRF_UI::tab3_set_target() {
//    QDoubleSpinBox *tmp = static_cast<QDoubleSpinBox*>(this->sender());
//    double number = tmp->value();

//    ulong base = widgets::index_of<widgets::spinboxes>(widgets::spinboxes::targetx);
//    for (int id = 0; id < 3; id++) {
//        if (tmp == dspinboxes.at(base + id)) emit set_target(id, number);
//    }
//}


void MAXRF_UI::set_abort_flag() { // raises a flag for abortion
    QMutex m;
    m.lock();
    *(shared_memory_cmd+200) = 1;
    m.unlock();
}


void MAXRF_UI::SelDigiCh0()
{
    typedef widgets::actions ac;
    w_actions.at(ac::daq_channel0)->setChecked(true);
    w_actions.at(ac::daq_channel1)->setChecked(false);
    w_actions.at(ac::daq_channel0and1)->setChecked(false);
    *(shared_memory_cmd+100)=0;
}

void MAXRF_UI::SelDigiCh1()
{
    typedef widgets::actions ac;
    w_actions.at(ac::daq_channel0)->setChecked(false);
    w_actions.at(ac::daq_channel1)->setChecked(true);
    w_actions.at(ac::daq_channel0and1)->setChecked(false);
    *(shared_memory_cmd+100)=1;
}

void MAXRF_UI::SelDigiCh0and1()
{
    typedef widgets::actions ac;
    w_actions.at(ac::daq_channel0)->setChecked(false);
    w_actions.at(ac::daq_channel1)->setChecked(false);
    w_actions.at(ac::daq_channel0and1)->setChecked(true);
    *(shared_memory_cmd+100)=2;
}





void MAXRF_UI::closeEvent(QCloseEvent *event) {
    event->ignore();
    event->accept();
}

MAXRF_UI::~MAXRF_UI() {

    int processIDs[7][2] = { { 0 }, { 0 } };

    printf("\n... Terminating data acquisition session\n");
    if (shared_memory_cmd[300]) shared_memory_cmd[300] = 0;

    for (int i = 0; i < 7; i++) {
        processIDs[i][0] = *(shared_memory_cmd+i+71);
        processIDs[i][1] = *(shared_memory_cmd+i+81);
    }

    motors_thread_.quit();
    motors_thread_.wait();

//    printf("... Dettaching shared memory segments\n");
//    for (int i = 0; i < 8; i++) if (shmid[i] != -1) shmctl(shmid[i], IPC_RMID, 0);
//    shmdt(shared_memory);
////    shmdt(shared_memory5);
//    shmdt(shared_memory_cmd);

    char process [30];
    qDebug()<<"... Killing child processes";
    for (int i = 0; i < 7; i++) {
        if (processIDs[i][0] == 1) {
            qDebug()<<"... Killing process with ID: "<<processIDs[i][1];
            sprintf(process, "kill -s TERM %i &", processIDs[i][1]);
            system(process);
        }
    }



}

void MAXRF_UI::Info1_1() {
    system("evince doc/Info_software_general.pdf &");
}

void MAXRF_UI::Info1_2() {
    system("evince doc/Info_shared_memory.pdf &");
}

void MAXRF_UI::Info2_1() {
    system("evince doc/Info_kernel_modules.pdf &");
}

