#include "../Header.h"
#include "mainwindow.h"
#include "../variables.h"
#include <unistd.h>
#include <QThread>
#include <tty.h>
#include <algorithm>

extern int shmid[8];
extern key_t key, key2, key3, key4, key5, key_cmd, key_rate;
extern int *shared_memory, *shared_memory2, *shared_memory3, *shared_memory4;
extern double *shared_memory5;
extern int *shared_memory_cmd, *shared_memory_rate;

tty_agent *tty_ptr;

bool MapIsOpened = false;
bool CameraOn = false;


int DAQ_TYPE = 1;
int measuring_time = 300; // for single-spectrum DAQ
int Pixeldim = 1; // for XRF map display

char process[30];


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    SHM_CREATOR();                 /// CREATING SHARED MEMORY SEGMENT
    createActions();
    builder_Menu();            	    /// CREATING MENU from Menu.cpp
    GUI_CREATOR();
    handle_connections();

    imageLabel = new ImgLabel;

    QImage image1("IMG/TT_CHNet_res1.png");
    imageLabel->setPixmap(QPixmap::fromImage(image1));
    imageLabel->setBackgroundRole(QPalette::Base);

    scrollArea->setWidget(imageLabel);
    scrollArea->setAlignment(Qt::AlignCenter);

    timer = new QTimer(this);                                                // TIMER for program control
    connect(timer, SIGNAL(timeout()), this, SLOT(tty_timer()));

    readmultidetcalpar();

    start_thread_tty();
}


void MainWindow::start_thread_tty() {

    tty_ptr = new tty_agent();
    tty_ptr->moveToThread(&test_thread);

    connect(&test_thread, &QThread::finished, tty_ptr, &QObject::deleteLater);
    connect(tty_ptr, &tty_agent::toggle_tab1, this, &MainWindow::toggle_tab1);
    connect(tty_ptr, &tty_agent::toggle_widgets, this, &MainWindow::toggle_widgets);
    connect(tty_ptr, &tty_agent::update_monitor, this, &MainWindow::update_monitor);
    connect(tty_ptr, &tty_agent::save_file, this, &MainWindow::SaveTxt);

    connect(this, &MainWindow::set_target, tty_ptr, &tty_agent::set_target);
    connect(this, &MainWindow::keyence_reading, tty_ptr, &tty_agent::enable_servo);
    connect(this, &MainWindow::start_servo, tty_ptr, &tty_agent::start_servo);

    connect(this, &MainWindow::df_open, tty_ptr, &tty_agent::tty_init);

    connect(tab4_start_scan, &QPushButton::clicked, tty_ptr, &tty_agent::scan);



    QSignalMapper *mapper_init_stages = new QSignalMapper;
    for (int i = 0; i < 3; i++) {
        mapper_init_stages->setMapping(tab1_stage_init[i], i);
        connect(tab1_stage_init[i], SIGNAL(released()), mapper_init_stages, SLOT(map()));
    }   connect(mapper_init_stages, SIGNAL(mapped(int)), tty_ptr, SLOT(stage_init(int)));

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

void MainWindow::enable_servo() {
    QCheckBox *tmp = static_cast<QCheckBox*>(this->sender());
    bool active = tmp->checkState();

    buttonTab3[2]->setEnabled(active);
    buttonTab3[7]->setEnabled(active);
    buttonTab3[8]->setEnabled(active);
    spinboxTab3[2]->setEnabled(active);

    emit start_servo(active);
}


/* Public slots */


void MainWindow::toggle_tab1(bool state) {
    for (int i = 0; i < 3; i++) {
        tab1_df_number[i]->setEnabled(state);
        tab1_df_open[i]->setEnabled(state);
        tab1_stage_init[i]->setEnabled(state);
    }
}

void MainWindow::toggle_widgets(int id) {
    switch (id) {
    case 0: case 1:
        tab3->setEnabled(true);
        break;
    case 2:
        tab2->setEnabled(true);
        break;
    case 3:
        AUTOFOCUS_ON_pushButton->setEnabled(true);
    }
}

void MainWindow::update_monitor(QString message, QString style, int id){
    dev_monitor[id]->setText(message);
    dev_monitor[id]->setStyleSheet(style);
}

void MainWindow::tab3_set_target() {
    QDoubleSpinBox *tmp = static_cast<QDoubleSpinBox*>(this->sender());
    double number = tmp->value();

    for (int id = 0; id < 3; id++) {
        if (tmp == spinboxTab3[id]) emit set_target(id, number);
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

void MainWindow::hideImage() {
    if (MapIsOpened) {
        //QImage startimage("IMG/TT_CHNet_extended_395_395_3.png");
        //imageLabel->setPixmap(QPixmap::fromImage(startimage));
        MapIsOpened=false;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                MAINWINDOW MAIN COMMAND CYCLE --> TIMER_EVENT
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::tty_timer() {

    //if (XYscanning)ScanXY();
    CheckSegFault();
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


void MainWindow::USB_DAQ()                                  // DAQ VIA USB
{
    {DAQ_TYPE=1; qDebug()<<"USB link active..."<<"DAQ_TYPE: "<<DAQ_TYPE;}
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

    if(DigitizerChannel0->isChecked())
    {

        *(shared_memory_cmd+100)=0;
        DigitizerChannel1->setChecked(false); DigitizerChannel0and1->setChecked(false);
    }

}

void MainWindow::SelDigiCh1()
{

    if(DigitizerChannel1->isChecked())
    {
        *(shared_memory_cmd+100)=1;
        DigitizerChannel0->setChecked(false); DigitizerChannel0and1->setChecked(false);
    }

}

void MainWindow::SelDigiCh0and1()
{

    if(DigitizerChannel0and1->isChecked())
    {
        *(shared_memory_cmd+100)=2;
        DigitizerChannel0->setChecked(false); DigitizerChannel1->setChecked(false);
    }

}

void MainWindow::CheckSegFault()                           // DAQ: MEMORY CONTROL FOR SEGMENTATION FAULT
{
//    if(*(shared_memory2+6)==1)
//    {
//        if(*(shared_memory_cmd+70)==1)
//        {
//            int pidVme=*(shared_memory_cmd+80);
//            sprintf(process, "kill -s TERM %i &", pidVme);
//            system(process);
//            *(shared_memory_cmd+70)=0;
//            SaveTxt();
//            qDebug()<<"[!] Acquisition interrumpted because shared memory is full";
//        }

//        if(XYscanning==true)
//        {
//            XYscanning=false;
//            //abort
//            qDebug()<<"[!] Scan interrumpted because shared memory is full";
//        }
//        *(shared_memory2+6)=0;
//    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                MAP - PIXEL AND SPECTRUM MANAGEMENT
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::open_MAP()                                      // MAP: OPEN MAP
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty())
    {
        QImage image(fileName);
        if (image.isNull())
        {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }
        imageLabel->setPixmap(QPixmap::fromImage(image));
        scaleFactor = 1.0;
    }
}

void MainWindow::SelectChannels()                             // MAP: CHANNEL SELECTION
{
    bool ok1, ok2;
    if(*(shared_memory+24)==0)
    {
        double chan1 = QInputDialog::getInt(this, tr("Lower Channel"),tr("ChLow:"), 0, 0, 16384, 1, &ok1);
        if (ok1)
        {qDebug()<<"New lower channel "<<chan1<<'\n'; *(shared_memory5+100)=chan1;}
        double  chan2 = QInputDialog::getInt(this, tr("Upper Channel"),tr("ChHigh:"), 16384, 0, 16384, 1, &ok2);
        if (ok2)
        {qDebug()<<"New upper channel "<<chan2<<'\n'; *(shared_memory5+101)=chan2;}
    }
    else if (*(shared_memory+24) == 1) {
            double chan1 = QInputDialog::getDouble(this, tr("Lower Energy"), tr("ELow:"), 0, 0, 60, 2, &ok1);
            if (ok1)
            {qDebug()<<"New lower energy "<<chan1<<'\n'; *(shared_memory5+100)=chan1;}
            double chan2 = QInputDialog::getDouble(this, tr("Upper Energy"), tr("EHigh:"), 60, 0, 60, 2, &ok2);
            if (ok2)
            {qDebug()<<"New upper energy "<<chan2<<'\n'; *(shared_memory5+101)=chan2;}
        }
}




void MainWindow::Pixels() { // Change pixel dimension
    bool ok1;
    int px = QInputDialog::getInt(this, "Set Pixel Size", "Pixel side dimension:", 1, 1, 30, 1, &ok1);

    if (ok1) {
        Pixeldim = px;
        printf("[!] New pixel dimension: %d\n", px);
    }
    else printf("[!] Couldn't obtain new pixel dimensions\n");
}

void MainWindow::LoadNewFile_SHM() { // Loads a new file in memory
    LoadNewFileWithNoCorrection_SHM();
    displayImage_SHM();
}

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
    connect(OKbutton, SIGNAL(clicked()), this, SLOT(LoadSHM_SumMap()));
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                FILES MANAGEMENT
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <numeric>

class xrf_image {
    xrf_image() {
        loadDir = "/home/frao/Desktop/XRFData";
    }

public:
    void prototype_load_map();
    void create_pixel(int);

private:
    QFile file;
    uint map_length;
    uint map_height;
    uint pix_total;

    vector<int> x_coords;
    vector<int> y_coords;
    vector<int> integral;

    double parameters[7];
    QString loadDir;

};



template <typename T>
struct pixel_t {
    pixel_t() {

    }
    typedef typename std::vector<T>::iterator iterator;
    iterator x;
    iterator y;
    ushort pixel_no;

    vector<int> det_a;
    vector<int> det_b;
    vector<int>::iterator it_integral;
    vector<int>::iterator it_elem_integral;
};

static vector<pixel_t<int>> map_data;


void xrf_image::create_pixel(int p)
{
    typedef pixel_t<int> pixel;
    QString line;

    int number;
    int temp_a[16384] = {};
    int temp_b[16384] = {};

    int masks[4]  = { 0x8000000, 0x4000000, 0x2000000, 0x1000000 };

    // Creat the first pixel
    pixel *tmp = new pixel;

    line = file.readLine();
    number = line.toInt();
    number ^= masks[1];

    if (*x_coords.end() == number)

    x_coords.push_back(number);
    tmp->x = x_coords.begin() + p;

    line = file.readLine();
    number = line.toInt();
    number ^= masks[0];

    y_coords.push_back(number);
    tmp->y = y_coords.begin() + p;

    tmp->pixel_no = p;

    while (true)
    {
        line = file.readLine();
        number = line.toInt();

        if (number >= masks[1]) break;
        if (number >= masks[2])
        {
            number ^= masks[2];
            temp_b[number] += 1;
        }
        else
        {
            number ^= masks[3];
            temp_a[number] += 1;
        }
    }

    tmp->det_a.reserve(16384);
    tmp->det_b.reserve(16384);
    for (int i = 0; i < 16384; i ++)
    {
        if (temp_a[i] == 0) continue;
        temp_a[i] |= (i << 16);
        tmp->det_a.push_back(temp_a[i]);
    }
    for (int i = 0; i < 16384; i ++)
    {
        if (temp_b[i] == 0) continue;
        temp_b[i] |= (i << 16);
        tmp->det_a.push_back(temp_b[i]);
    }
    tmp->det_a.shrink_to_fit();
    tmp->det_b.shrink_to_fit();

    map_data.push_back(*tmp);
}

void xrf_image::prototype_load_map() {
    QString filepath = QFileDialog::getOpenFileName(nullptr, "Open file...", loadDir);

    if (!filepath.isEmpty()) {
        QFile file(filepath);
        if (file.exists()) {
            file.open(QIODevice::ReadOnly);
            QString line = file.readLine();


            if (line.startsWith("v")) { // Data with new header protocol
                int size  = (sizeof parameters) / (sizeof parameters[0]);
                for (int i = 0; i < size; i++) {
                    line = file.readLine();
                    parameters[i] = line.toInt();
                }

                map_length = (parameters[0] - parameters[1]) / parameters[4];
                map_height = 1 + (parameters[2] - parameters[3]) / parameters[5];
                pix_total  = map_length * map_height;

                typedef pixel_t<int> pixel;
                size_t size_m = sizeof (pixel);
                size_m *= pix_total;

                map_data.clear();
                map_data.reserve(size_m);
                x_coords.reserve(size_m);
                y_coords.reserve(size_m);
                integral.reserve(size_m);

                int i = 0;
                create_pixel(0); i++;
                while (!file.atEnd())
                {

                }








                // Rest of pixels

                while (!file.atEnd())
                {
                    pixel *tmp = new pixel;
                    tmp->pixel_no = i;





                }

            }
        }
    }


}

void MainWindow::LoadTxt()  { // Writes values of binary file into shared memory
    QString loadDir = "/home/frao/Desktop/XRFData";
    QString text = QFileDialog::getOpenFileName(this, "Open file..", loadDir);

    if (!text.isEmpty()) {
        QFile file(text);
        if (file.exists()) {
            file.open(QIODevice::ReadOnly);
            QString line = file.readLine();

            int i = 0;
            if (line.startsWith("v")) { // Data with new header protocol
                line = file.readLine();
                *(shared_memory_cmd+50) = line.toInt(); // X start
                line = file.readLine();
                *(shared_memory_cmd+51) = line.toInt(); // X end
                line = file.readLine();
                *(shared_memory_cmd+52) = line.toInt(); // Y start
                line = file.readLine();
                *(shared_memory_cmd+53) = line.toInt(); // Y end
                line = file.readLine();
                *(shared_memory_cmd+60) = line.toInt(); // X step
                line = file.readLine();
                *(shared_memory_cmd+61) = line.toInt(); // Y step
                line = file.readLine();
                *(shared_memory_cmd+67) = line.toInt(); // Velocity
            }
            else {
                *(shared_memory_cmd+60) = QInputDialog::getInt(this, "Set X pixel step", "X step (um):", 500, 1, 2000, 1, nullptr);
                *(shared_memory_cmd+61) = QInputDialog::getInt(this, "Set Y pixel step", "Y step (um):", 500, 1, 2000, 1, nullptr);

                *(shared_memory2+10+(++i)) = line.toInt();
            }

            while (!file.atEnd()) {
                line = file.readLine();
                *(shared_memory2+10+(++i)) = line.toInt();
                if (i == 1) printf("[!] First position: %d\n", *(shared_memory2+10+i));
            }
            file.close();
            *(shared_memory2+4) = i;
        }
        else printf("[!] File not found\n");
    }
    LoadNewFile_SHM();
}




void MainWindow::SaveTxt() {
    QString saveDir = "/home/frao/Desktop/XRFData";
    QString percorso = QFileDialog::getSaveFileName(this,tr("Save as"), saveDir);

    QFile file2(percorso);
    file2.open(QIODevice::ReadWrite);
    QTextStream out2(&file2);
    int Ntot = *(shared_memory2+4);
    out2<<"ver.001"<<'\n';
    out2<<*(shared_memory_cmd+50)<<'\n';//writes Xmin
    out2<<*(shared_memory_cmd+51)<<'\n';//writes Xmax
    out2<<*(shared_memory_cmd+52)<<'\n';//writes Ymin
    out2<<*(shared_memory_cmd+53)<<'\n';//writes Ymax
    out2<<*(shared_memory_cmd+60)<<'\n';//writes Xstep
    out2<<*(shared_memory_cmd+61)<<'\n';//writes Ystep
    out2<<*(shared_memory_cmd+67)<<'\n';//writes the scan velocity
    for (int i = 1; i <= Ntot; i++) {
        out2<<*(shared_memory2+10+i)<<'\n';
    }
    file2.close();


    string str = percorso.toStdString();
    char *cstr = &str[0u];
    printf("[!] File saved in: %s", cstr);
}




void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    event->accept();
}

MainWindow::~MainWindow() {

    int processIDs[7][2] = { { 0 }, { 0 } };

    printf("\n... Terminating data acquisition session\n");
    if (*(shared_memory_cmd+70)) *(shared_memory_cmd+70) = 0;

    for (int i = 0; i < 7; i++) {
        processIDs[i][0] = *(shared_memory_cmd+i+71);
        processIDs[i][1] = *(shared_memory_cmd+i+81);
    }

    printf("... Dettaching shared memory segments\n");
    for (int i = 0; i < 8; i++) if (shmid[i] != -1) shmctl(shmid[i], IPC_RMID, 0);
    shmdt(shared_memory);
    shmdt(shared_memory2);
    shmdt(shared_memory3);
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

    test_thread.quit();
    test_thread.wait();

}

void MainWindow::Info1_1() {
    system("evince manual/Info_software_general.pdf &");
}

void MainWindow::Info1_2() {
    system("evince manual/Info_shared_memory.pdf &");
}

void MainWindow::Info2_1() {
    system("evince manual/Info_kernel_modules.pdf &");
}

