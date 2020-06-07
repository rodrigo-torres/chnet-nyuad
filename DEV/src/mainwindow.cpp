#include "MAXRF/mainwindow.h"
#include "MAXRF/tty.h"

extern int shmid[8];
extern key_t key, key2, key3, key4, key5, key_cmd, key_rate;
extern int *shared_memory, *shared_memory4;
extern double *shared_memory5;
extern int *shared_memory_cmd, *shared_memory_rate;

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

    readmultidetcalpar();
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
//                                MAINWINDOW: FURTHER ACTIONS
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::Changeparameters()
{
    bool ok1, ok2, ok3;
    
    int calpar1 = QInputDialog::getInt(this, tr("Multidetector calibration parameters"),tr("Parameter A"), 0, -1000000, 1000000, 0.000001, &ok1);

    int  calpar2 = QInputDialog::getInt(this, tr("Multidetector calibration parameters"),tr("Parameter B"), 0, -1000000, 1000000, 0.000001, &ok2);

    int  scalefactor = QInputDialog::getInt(this, tr("Multidetector calibration parameters"),tr("Scale factor"), 0, 1, 1000000000, 1, &ok3);

    FILE *filecalpar; //name of the file where the channel intervals are specified
    filecalpar = fopen ("../multidetector_calibrationparameters.txt", "w+");
    fprintf(filecalpar, "%d\n", calpar1);
    fprintf(filecalpar, "%d\n", calpar2);
    fprintf(filecalpar, "%d\n", scalefactor);
    fclose(filecalpar);

    *(shared_memory_cmd+101)=calpar1;
    *(shared_memory_cmd+102)=calpar2;
    *(shared_memory_cmd+103)=scalefactor;

}

void MainWindow::readmultidetcalpar() {
    int calpar1 = 0, calpar2 = 0, scalefactor=1;

    FILE *filecalpar;
    filecalpar = fopen ("../multidetector_calibrationparameters.txt", "r");
    if (filecalpar != nullptr) {
        fscanf(filecalpar, "%d", &calpar1);
        fscanf(filecalpar, "%d", &calpar2);
        fscanf(filecalpar, "%d", &scalefactor);
        fclose(filecalpar);
    }

    *(shared_memory_cmd+101) = calpar1;
    *(shared_memory_cmd+102) = calpar2;
    *(shared_memory_cmd+103) = scalefactor;

    if ((calpar1 != 0 || calpar2 != 0) && scalefactor != 0) printf("... Multidetector parameters found\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                MOTOR SETTINGS
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::scan_parameters(double val) {
    /* Assign an index depending on the widget sending the signal */
    QDoubleSpinBox *temp = static_cast<QDoubleSpinBox*>(this->sender());

    int n = sizeof(scan_params) / sizeof(scan_params[0]);
    auto itr = std::find(scan_params, scan_params + n, temp);
    long index = distance(scan_params, itr);

    /* Pass the double value to shared memory at the assigned index */

    *(shared_memory5+index) = val;

    //Px=val*1000 (and Py)
    //Xmin1=val*1000 (and Xmax1, Ymin1, Ymax1)
    //pixel_Xstep=val (and pixel_Ystep)
    //positionX=val (and positionY)
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

// Data is written directly to a file
// Segmentation fault errors are handled by the DAQ protocol
[[deprecated]] void MainWindow::CheckSegFault() {}


// ////////////////////////////////////////////////////////////////////////// //
// //////////////////// COMPOSED ELEMENTAL MAP //////////////////// //
// ////////////////////////////////////////////////////////////////////////// //



void MainWindow::LoadElementsMapSum() {
    for (int i = 0; i < 6; i++)
        *(shared_memory5+100+i) = 0;

    elementsdlg = new QDialog;
    elementsdlg->setFixedSize(400, 200);

    QLabel *labelElement0 = new QLabel("<b>Choose limits for elements shown in,<\b>");
    QLabel *labelElement1 = new QLabel("<b>red:<\b>");
    QLabel *labelElement2 = new QLabel("<b>green:<\b>");
    QLabel *labelElement3 = new QLabel("<b>blue:<\b>");

    QSpinBox *spinboxArray[6];
    QDoubleSpinBox *dspinboxArray[6];

    if (*(shared_memory+24)) {
        QSignalMapper *mapperElementSum = new QSignalMapper();
        for (int i = 0; i < 6; i++) {
            dspinboxArray[i] = new QDoubleSpinBox(this);
            dspinboxArray[i]->setMinimum(0.00);
            dspinboxArray[i]->setMaximum(60.00);
            dspinboxArray[i]->setSingleStep(1.0);
            if ((i % 2) == 0) dspinboxArray[i]->setPrefix("Min: ");
            else dspinboxArray[i]->setPrefix("Max: ");

            mapperElementSum->setMapping(dspinboxArray[i], i);
            connect(dspinboxArray[i], SIGNAL(valueChanged(double)), mapperElementSum, SLOT(map()));
        }
        connect(mapperElementSum, SIGNAL(mapped(int)), this, SLOT(writeCompMapLimits(int)));
    }

    else {
        QSignalMapper *mapperElementSum = new QSignalMapper();
        for (int i = 0; i < 6; i++) {
            spinboxArray[i] = new QSpinBox(this);
            spinboxArray[i]->setMinimum(0);
            spinboxArray[i]->setMaximum(16384);
            spinboxArray[i]->setSingleStep(1);
            if ((i % 2) == 0) spinboxArray[i]->setPrefix("Min: ");
            else spinboxArray[i]->setPrefix("Max: ");

            mapperElementSum->setMapping(spinboxArray[i], i);
            connect(spinboxArray[i], SIGNAL(valueChanged(int)), mapperElementSum, SLOT(map()));
        }
        connect(mapperElementSum, SIGNAL(mapped(int)), this, SLOT(writeCompMapLimits(int)));
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);

    OKbutton = new QPushButton("Ok");
    CANCELbutton = new QPushButton("Cancel");
    buttonBox->addButton(OKbutton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(CANCELbutton, QDialogButtonBox::AcceptRole);

    connect(OKbutton, SIGNAL(clicked()), elementsdlg, SLOT(close()));
    connect(CANCELbutton, SIGNAL(clicked()), elementsdlg, SLOT(close()));


    QGridLayout *Layout = new QGridLayout(elementsdlg);
    Layout->setSpacing(5);
    Layout->addWidget(labelElement0, 0, 0, 1, 3);
    Layout->addWidget(labelElement1, 1, 0);
    Layout->addWidget(labelElement2, 2, 0);
    Layout->addWidget(labelElement3, 3, 0);
    if (*(shared_memory+24)) {
        Layout->addWidget(dspinboxArray[0], 1, 1);
        Layout->addWidget(dspinboxArray[1], 1, 2);
        Layout->addWidget(dspinboxArray[2], 2, 1);
        Layout->addWidget(dspinboxArray[3], 2, 2);
        Layout->addWidget(dspinboxArray[4], 3, 1);
        Layout->addWidget(dspinboxArray[5], 3, 2);
    }
    else {
        Layout->addWidget(spinboxArray[0], 1, 1);
        Layout->addWidget(spinboxArray[1], 1, 2);
        Layout->addWidget(spinboxArray[2], 2, 1);
        Layout->addWidget(spinboxArray[3], 2, 2);
        Layout->addWidget(spinboxArray[4], 3, 1);
        Layout->addWidget(spinboxArray[5], 3, 2);
    }
    Layout->addWidget(buttonBox, 4, 0, 1, 3, Qt::AlignRight);

    elementsdlg->show();

}


void MainWindow::writeCompMapLimits(int id) {
    QSignalMapper *temp = static_cast<QSignalMapper*>(this->sender());

    double value;
    if (*(shared_memory+24)) {
        QDoubleSpinBox *obj = static_cast<QDoubleSpinBox*>(temp->mapping(id));
        value = obj->value();
    }
    else {
        QSpinBox *obj = static_cast<QSpinBox*>(temp->mapping(id));
        value = obj->value();
    }

    *(shared_memory5+102+id) = value;
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
    shmdt(shared_memory4);
    shmdt(shared_memory5);
    shmdt(shared_memory_rate);
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

