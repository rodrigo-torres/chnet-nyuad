#include "MAXRF/mainwindow.h"
#include "MAXRF/tty.h"
#include "MAXRF/types.h"

extern int shmid[8];
extern int *shared_memory;
//extern double *shared_memory5;
extern int *shared_memory_cmd;

tty_agent *tty_ptr;


int DAQ_TYPE = 1;
int measuring_time = 300; // for single-spectrum DAQ
char process[30];


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
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


void MainWindow::StartThreadTTY() {

    tty_ptr = new tty_agent();
    tty_ptr->moveToThread(&test_thread);

    connect(&test_thread, &QThread::finished, tty_ptr, &QObject::deleteLater);
    connect(tty_ptr, &tty_agent::toggle_tab1, this, &MainWindow::toggle_tab1);
    connect(tty_ptr, &tty_agent::toggle_widgets, this, &MainWindow::toggle_widgets);
    connect(tty_ptr, &tty_agent::update_monitor, this, &MainWindow::update_monitor);

    connect(this, &MainWindow::set_target, tty_ptr, &tty_agent::set_target);
    connect(this, &MainWindow::keyence_reading, tty_ptr, &tty_agent::enable_servo);
    connect(this, &MainWindow::start_servo, tty_ptr, &tty_agent::start_servo);


    connect(tab4_start_scan, &QPushButton::clicked, tty_ptr, &tty_agent::scan);


    connect(this, &MainWindow::request_tty_action, tty_ptr, &tty_agent::relay_command);


    QSignalMapper *mapperMoveStages = new QSignalMapper();
    for (int i = 0; i < 9; i++) {
        mapperMoveStages->setMapping(buttonTab3[i], i);
        connect(buttonTab3[i], SIGNAL(released()), mapperMoveStages, SLOT(map()));
    }   connect(mapperMoveStages, SIGNAL(mapped(int)), tty_ptr, SLOT(move_stage(int)));



    test_thread.start();

};

/* Signals */

void MainWindow::enable_keyence_reading() {
    QCheckBox *tmp = static_cast<QCheckBox*>(this->sender());
    bool active = tmp->checkState();

    servo_checkbox->setEnabled(active);
    emit keyence_reading(active);
}

void MainWindow::enable_servo()
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


void MainWindow::toggle_tab1(bool state)
{
    tab1->setEnabled(state);

}

void MainWindow::toggle_widgets(int id) {
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

void MainWindow::update_monitor(QString message, QString style, int id){
    dev_monitor[id]->setText(message);
    dev_monitor[id]->setStyleSheet(style);
}

void MainWindow::tab3_set_target() {
    QDoubleSpinBox *tmp = static_cast<QDoubleSpinBox*>(this->sender());
    double number = tmp->value();

    ulong base = widgets::index_of<widgets::spinboxes>(widgets::spinboxes::targetx);
    for (int id = 0; id < 3; id++) {
        if (tmp == dspinboxes.at(base + id)) emit set_target(id, number);
    }
}


void MainWindow::set_abort_flag() { // raises a flag for abortion
    QMutex m;
    m.lock();
    *(shared_memory_cmd+200) = 1;
    m.unlock();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                MOTOR SETTINGS
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::SetScanParameters(double val) {
    /* Assign an index depending on the widget sending the signal */
    QDoubleSpinBox *temp = static_cast<QDoubleSpinBox*>(this->sender());

    int n = sizeof(scan_params) / sizeof(scan_params[0]);
    auto itr = std::find(scan_params, scan_params + n, temp);
    long index = distance(scan_params, itr);

    /* Pass the double value to shared memory at the assigned index */

//    *(shared_memory5+index) = val;
    switch (index) {
    case 0:
      scan_parameters.x_start_coordinate = val;
      break;
    case 1:
      scan_parameters.x_end_coordinate = val;
      break;
    case 2:
      scan_parameters.y_start_coordinate = val;
      break;
    case 3:
      scan_parameters.y_end_coordinate = val;
      break;
    case 4:
      scan_parameters.x_motor_step = val;
      break;
    case 5:
      scan_parameters.y_motor_step = val;
      break;
    case 6:
      scan_parameters.motor_velocity = val;
      break;
    default:
    break;
    }


}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                DAQ SETTINGS AND DIGITISER PROGRAM CONTROL
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::USB_DAQ(int val)                                  // DAQ VIA USB
{
    DAQ_TYPE = val;
    qDebug()<<"DAQ mode updated";
}

void MainWindow::OPTICAL_DAQ()                              // DAQ VIA OPTICAL LINK
{
    {DAQ_TYPE=0; qDebug()<<"OPTICAL link active..."<<"DAQ_TYPE: "<<DAQ_TYPE;}
}

void MainWindow::set_PMAcquisitionTime() {
    bool dlgok;
    measuring_time = QInputDialog::getInt(this, tr("Set Acquisition Time"), tr("Seconds:"),300, 0, 18000, 60, &dlgok);
    if (dlgok) {
        printf("... Point-mode acquisition time set to:\t%d seconds\n", measuring_time);
    }
}

void MainWindow::SelDigiCh0()
{
    typedef widgets::actions ac;
    w_actions.at(ac::daq_channel0)->setChecked(true);
    w_actions.at(ac::daq_channel1)->setChecked(false);
    w_actions.at(ac::daq_channel0and1)->setChecked(false);
    *(shared_memory_cmd+100)=0;
}

void MainWindow::SelDigiCh1()
{
    typedef widgets::actions ac;
    w_actions.at(ac::daq_channel0)->setChecked(false);
    w_actions.at(ac::daq_channel1)->setChecked(true);
    w_actions.at(ac::daq_channel0and1)->setChecked(false);
    *(shared_memory_cmd+100)=1;
}

void MainWindow::SelDigiCh0and1()
{
    typedef widgets::actions ac;
    w_actions.at(ac::daq_channel0)->setChecked(false);
    w_actions.at(ac::daq_channel1)->setChecked(false);
    w_actions.at(ac::daq_channel0and1)->setChecked(true);
    *(shared_memory_cmd+100)=2;
}





void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    event->accept();
}

MainWindow::~MainWindow() {

    int processIDs[7][2] = { { 0 }, { 0 } };

    printf("\n... Terminating data acquisition session\n");
    if (shared_memory_cmd[300]) shared_memory_cmd[300] = 0;

    for (int i = 0; i < 7; i++) {
        processIDs[i][0] = *(shared_memory_cmd+i+71);
        processIDs[i][1] = *(shared_memory_cmd+i+81);
    }

    test_thread.quit();
    test_thread.wait();

    printf("... Dettaching shared memory segments\n");
    for (int i = 0; i < 8; i++) if (shmid[i] != -1) shmctl(shmid[i], IPC_RMID, 0);
    shmdt(shared_memory);
//    shmdt(shared_memory5);
    shmdt(shared_memory_cmd);

    qDebug()<<"... Killing child processes";
    for (int i = 0; i < 7; i++) {
        if (processIDs[i][0] == 1) {
            qDebug()<<"... Killing process with ID: "<<processIDs[i][1];
            sprintf(process, "kill -s TERM %i &", processIDs[i][1]);
            system(process);
        }
    }



}

void MainWindow::Info1_1() {
    system("evince doc/Info_software_general.pdf &");
}

void MainWindow::Info1_2() {
    system("evince doc/Info_shared_memory.pdf &");
}

void MainWindow::Info2_1() {
    system("evince doc/Info_kernel_modules.pdf &");
}

