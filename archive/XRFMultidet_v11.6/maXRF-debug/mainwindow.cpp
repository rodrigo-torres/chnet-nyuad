#include "../Header.h"
#include "mainwindow.h"
#include "../variables.h"
#include "../QT_variables.h"
#include <../Shm.h>
#include <unistd.h>
#include <QThread>

extern int shmid[];
extern int Resolution_mode;
extern double numpixelforaccel;
extern double posXforacceleration;
extern double accelerationtime;
extern int accelerationtimesleep;
extern double Px;


bool PixelCorrection=false;  bool OnLine=false;         bool MapIsOpened=false;  bool CameraOn=false;
bool okX=false;              bool okY=false;          bool energychanged=false;
bool FirstRun=true;          bool opened=false;       bool XConnected=false;     bool YConnected=false;   bool TimerActive=false;
bool XOnTarget=false;        bool YOnTarget=false;    bool XHasMoved=true;       bool Xmoving=false;      bool Ymoving=false;
bool XYscanning=false;       bool YXscanning=false;   bool YHasMoved=true;       bool okZ=false;          bool TimerZActive=false;
bool ZConnected=false;       bool ZOnTarget=false;    bool ZHasMoved=true;       bool Zmoving=false;
bool InitY=false;        bool InitZ=false;          bool AutofocusOn=false;

int i=0;                     int j=0;                 int n=1;                   int MergePos=0;          int mempos=0; //memory position histo
int Pixeldim=1;              int EventOffset=0;       int nz=1;                  int missing=1;           int measuring_time=300;
int OffsetX;                 int OffsetY;             int NshiftY=0;             int onlyOne=0;           int NshiftX=0;/////o 1???
int point;                   int Clock=0;             int Clock2=0;              int ClockMotore=0;       int ClockZ=0;
int casenumber=4;            int interval = 250;        int NscanX=0;              int NscanY=0;            int StoredPoint=0;
int eventionline=0;          int m=1;

double positionX=100;        double valueY;           double ZPosition=25.0;    double tempoPos=1000;     double V=1;
double positionY=100;        double valueX;           double ChMin=3;           double ChMax=16384;       double Vz=1;
double tempoPosZ=1000;       double valueZ;           double positionZ;         double Pz=250;            double x_image;
double y_image;              double x_image2;         double y_image2; 

float Yo;                    float vy;                float Xo;                 float vx;                 float vz;
float temp;                  

char process[30];

QString NowZ="Z= ";      QString current_posZ;     QString NowX="X= ";       QString NowY="Y= ";

struct Pixel_BIG *Pointer; //puntatore da far puntare a PixelsMappa una volta creato

//////////// ADDED VARIABLES FOR DEVELOP VERSION (Bart_PE)

int selected_Xmotor = 3, selected_Ymotor = 3, selected_Zmotor = 1;

bool InitPhaseY=false; bool InitPhaseZ=false;
int nxInit=0, nyInit=0, nzInit=0; // used for motor initialisation

///////////////Variables for the composed map visualization (sum of three different elements)///////////
double ChMin1=0, ChMax1=0, ChMin2=0, ChMax2=0, ChMin3=0, ChMax3=0;


QString stylesheet3 = "QLineEdit {background-color: #2DC937; font-weight: bold; color: white;}";




MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    PixelX=595;
    PixelY=595;

    SHM_CREATOR();                 /// CREATING SHARED MEMORY SEGMENT
    createActions();
    builder_Menu();            	    /// CREATING MENU from Menu.cpp
    GUI_CREATOR();
    CONNECTIONS_CREATOR();

    imageLabel = new ImgLabel;

    QImage image1("IMG/TT_CHNet_res1.png");
    imageLabel->setPixmap(QPixmap::fromImage(image1));
    imageLabel->setBackgroundRole(QPalette::Base);

    scrollArea->setWidget(imageLabel);
    scrollArea->setAlignment(Qt::AlignCenter);

    timer = new QTimer(this);                                                // TIMER for program control
    connect(timer, SIGNAL(timeout()), this, SLOT(timerEvent()));

    timerAutofocus =new QTimer(this);                                        // TIMER for RECORDING Z DISTANCE FROM TARGET
    connect(timerAutofocus, SIGNAL(timeout()), this, SLOT(Focustimer()));


    readmultidetcalpar();
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


void MainWindow::timerEvent() {

    if (InitZ && !AutofocusOn) stage_check_on_target(serialZ, 2);
    if (IniXready) stage_check_on_target(serialX, 0);
    if (InitY) stage_check_on_target(serialY, 1);
    if (XYscanning)ScanXY();
    //if(AutofocusOn)AutoFocusRunning();
    MoveDoubleClick();
    CheckSegFault();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                MOTORS: COMMAND and ANSWERS HANDSHAKE
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int tty_send(int channel,const char *command, const char *parameter, int port) {
    char chan=channel+'0';
    string cm = command;

    if (parameter == nullptr) cm = cm + '\n';
    else cm = cm + ' ' + chan + ' ' + parameter + '\n';

    long n = write(port, cm.data(), cm.size());
    tcdrain(port);

    if (n > 0) return 0;
    else {
        qDebug()<<"[!] Writing to bus failed"
                  "[!] Check connection and writing priviledges";
        qDebug()<<strerror(errno);
        return 1;
    }
}

void tty_read(int port, char *ans, unsigned long wait = 0) {
    char buff[100] = { '\0' };

    long n = 0;
    int size = 0;
    while ((n = read(port, &buff[size], 1)) > 0) {
        if (buff[size] == '\n') break;
        else size += n;
    }
    if (n < 0) throw std::runtime_error(strerror(errno));
    strncpy(ans, buff, static_cast<unsigned long>(size));
    if (ans[size-1] == '\n') ans[size-1] = '\0';
}

string read_answer(int port)                                                       // Z MOTOR: READ ANSWER CHAR
{
    string rest;
    char c[100];
    int n=0;

    while( ( n = read(port, &c, sizeof(c)) ) > 0 ) {
        c[n]=0;
        rest=rest+c;
        if ( c[n-1] == '\n' ) {
            break;
        }
    }
    return rest;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                MOTORS: CHECK_ON_TARGET
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::stage_check_on_target(int serial, int id) {
    const char prefix[] = "Stage ";
    const char *axes[]  = { "X: ", "Y: ", "Z: "};
    QLineEdit *monitors[] = { CurrentActionX, CurrentActionY, CurrentActionZ };
    bool *on_target[] = { &XOnTarget, &YOnTarget, &ZOnTarget };

    char ans[15] = { '\0' };
    tty_send(1, "ONT?", nullptr, serial); // Expected a response with format 1=%d with %d = 1,0

    try {
        tty_read(serial, ans, 10);
    } catch (const runtime_error& e) {
        qDebug()<<e.what();
    }

    if (ans[2] == '1' ) *on_target[id] = true;
    else *on_target[id] = false;

    char ans2[15] = { '\0' };
    tty_send(1,"POS?", nullptr, serial); // Expected a response with format 1={x->xxx}.xxxx

    try {
        tty_read(serial, ans2, 15);
    } catch (const runtime_error& e) {
        qDebug()<<e.what();
    }

    if (id == 2) {
        QString temp = &ans2[2];
        ZPosition = temp.toDouble();
        qDebug()<<ZPosition;
    }

    char message[25] = { '\0' };
    strncpy(message, prefix, sizeof(prefix));
    strncat(message, axes[id], 4);
    strncat(message, &ans2[2], 10);
    strncat(message, " mm", sizeof(" mm"));
    monitors[id]->setText(message);
    monitors[id]->setStyleSheet(stylesheet3);
}


void MainWindow::Enable_TabWidget_3_4_XY()                                           // ENABLING XY MOVE WIDGET
{
    if(IniXready && InitY){tab2_3->setEnabled(true); tab2_4->setEnabled(true);}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                MOTOR SETTINGS
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::Velocity(double number)                       // MOTOR SETTINGS XY VELOCITY
{
    V=number;
    int V_adc=V*1000;
    printf("V_adc:%d\n", V_adc);
    *(shared_memory_cmd+67)=V_adc;
    tempoPos=Px/V;
    qDebug()<<"velocità "<<V<<"mm/s"<<"Scrittura posizione ogni "<<tempoPos<<" ms";
    char v[10];
    sprintf(v,"%f",V);
    tty_send(1,"VEL",v,serialX);
    tty_send(1,"VEL",v,serialY);
}

void MainWindow::PassoX_Func(double number1)                     // MOTOR SETTINGS STEP
{Px=number1*1000; *(shared_memory_cmd+60)=Px; pixel_Xstep=number1;}
void MainWindow::PassoY_Func(double number5)
{Py=number5*1000; *(shared_memory_cmd+61)=Py; pixel_Ystep=number5;}


void MainWindow::Xminimo(double number2)                         // MOTOR SETTINGS MINIMUM POSITION
{Xmin1=number2*1000;  positionX=Xmin1; *(shared_memory_cmd+50)=Xmin1;}
void MainWindow::Yminimo(double number3) 
{Ymin1=number3*1000;  positionY=Ymin1; *(shared_memory_cmd+52)=Ymin1;}
void MainWindow::Zminimo(double number2) 
{Zmin1=number2*1000;  positionZ=Zmin1; *(shared_memory_cmd+54)=Zmin1;}


void MainWindow::Xmassimo(double number3)                         // MOTOR SETTINGS MAXIMUM POSITION
{Xmax1=number3*1000; *(shared_memory_cmd+51)=Xmax1;}
void MainWindow::Ymassimo(double number7) 
{Ymax1=number7*1000; *(shared_memory_cmd+53)=Ymax1;}
void MainWindow::Zmassimo(double number3) 
{Zmax1=number3*1000; *(shared_memory_cmd+55)=Zmax1;}


void MainWindow::X_to(double number9)                             // MOTOR SETTINGS GO_TO
{X_goto=number9*1000;}
void MainWindow::Y_to(double number10) 
{Y_goto=number10*1000;}
void MainWindow::Z_to(double number9) 
{Z_goto=number9*1000;}


void MainWindow::StartX() {
    Init_Xmotor();
}


void MainWindow::StartY()                                          // MOTOR START Y
{
    if(!okY)
    {
        okY=true;
        Init_Ymotor();
        if(IniXready && InitY)
        {
            tab2_3->setEnabled(true);
            tab2_4->setEnabled(true);
        }
    }
    else
    {
        qDebug() <<"...Already initing...";
    }
}


void MainWindow::StartZ()                                          // MOTOR START Z
{
    if(!okZ)
    {
        okZ=true;
        Init_Zmotor();
    }
    else
    {
        qDebug() <<"Init already done...";
    }
}


void MainWindow::stop_motorXY() {
    int fds[] = { serialX, serialY, serialZ };
    for (int i = 0; i < static_cast<int>(sizeof(fds)); i++) {
        int t = isatty(fds[i]);
        if (t == 0) continue;

        char ans[15] = { '\0' };
        tty_send(1, "HLT", nullptr, fds[i]);
        tty_send(1, "ERR?", nullptr, fds[i]);
        tty_read(fds[i], ans);

        qDebug()<<"[!] Linear stage at FD "<<fds[i]<<"halted with error code: "<<ans;
    }   // Error code should be 10: "Controller stopped on command"
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
    if(*(shared_memory2+6)==1)
    {
        if(*(shared_memory_cmd+70)==1)
        {
            int pidVme=*(shared_memory_cmd+80);
            sprintf(process, "kill -s TERM %i &", pidVme);
            system(process);
            *(shared_memory_cmd+70)=0;
            SaveTxt();
            qDebug()<<"[!] Acquisition interrumpted because shared memory is full";
        }

        if(XYscanning==true || YXscanning==true)
        {
            XYscanning=false;
            YXscanning=false;
            Abort();
            qDebug()<<"[!] Scan interrumpted because shared memory is full";
        }
        *(shared_memory2+6)=0;
    }
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
        {qDebug()<<"New lower channel "<<chan1<<'\n'; ChMin=chan1;}
        double  chan2 = QInputDialog::getInt(this, tr("Upper Channel"),tr("ChHigh:"), 16384, 0, 16384, 1, &ok2);
        if (ok2)
        {qDebug()<<"New upper channel "<<chan2<<'\n'; ChMax=chan2;}
    }
    else if (*(shared_memory+24) == 1) {
            double chan1 = QInputDialog::getDouble(this, tr("Lower Energy"), tr("ELow:"), 0, 0, 60, 2, &ok1);
            if (ok1)
            {qDebug()<<"New lower energy "<<chan1<<'\n'; ChMin=int(chan1*1000);energychanged=true;}
            double chan2 = QInputDialog::getDouble(this, tr("Upper Energy"), tr("EHigh:"), 60, 0, 60, 2, &ok2);
            if (ok2)
            {qDebug()<<"New upper energy "<<chan2<<'\n'; ChMax=int(chan2*1000);energychanged=true;}
        }
}




void MainWindow::Pixels() { // Change pixel dimension
    bool ok1;
    int px = QInputDialog::getInt(this, "Set Pixel Size", "Pixel side dimension:", 1, 1, 30, 1, &ok1);

    if (ok1) {
        Pixeldim = px;
        printf("[!] New pixel dimension: %d", px);
    }
    else printf("[!] Couldn't obtain new pixel dimensions\n");
}

void MainWindow::LoadNewFile_SHM() { // Loads a new file in memory
    LoadNewFileWithNoCorrection_SHM();
    displayImage_SHM();
}

void MainWindow::LoadElementsMapSum() {

    ChMin1 = 0; ChMin2 = 0; ChMin3 = 0;
    ChMax1 = 0; ChMax2 = 0; ChMax3 = 0;

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
    if (*(shared_memory+24)) {
        QDoubleSpinBox *spnbox = static_cast<QDoubleSpinBox*>(temp->mapping(id));
        double value = spnbox->value();

        if (id == 0) ChMin1 = value;
        else if (id == 1) ChMax1 = value;
        else if (id == 2) ChMin2 = value;
        else if (id == 3) ChMax2 = value;
        else if (id == 4) ChMin3 = value;
        else if (id == 5) ChMax3 = value;
        else printf("[!] Unknown sender to the composed map limits function");
    }
    else {
        QSpinBox *spnbox = static_cast<QSpinBox*>(temp->mapping(id));
        int value = spnbox->value();

        if (id == 0) ChMin1 = static_cast<double>(value);
        else if (id == 1) ChMax1 = static_cast<double>(value);
        else if (id == 2) ChMin2 = static_cast<double>(value);
        else if (id == 3) ChMax2 = static_cast<double>(value);
        else if (id == 4) ChMin3 = static_cast<double>(value);
        else if (id == 5) ChMax3 = static_cast<double>(value);
        else printf("[!] Unknown sender to the composed map limits function");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                FILES MANAGEMENT
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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




void MainWindow::SaveTxt() { //scrive Position.txt leggendo i dati in memoria

    tty_send(1,"HLT",NULL,serialX);
    tty_send(1,"HLT",NULL,serialY);
    tty_send(1,"HLT",NULL,serialZ);
    timer->blockSignals(true);
    timerAutofocus->blockSignals(true);

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

    timer->blockSignals(false);
    timerAutofocus->blockSignals(false);
    tcflush(serialK, TCIFLUSH);
}

void MainWindow::MergeTxt()  //carica File.txt in memoria
{
    MergePos=*(shared_memory+22);
    if(MergePos==0)
    {
        system("./QT_Position  & ");
        MergePos=1;
        *(shared_memory+22)=MergePos;
    }
    else qDebug()<<"...program already running...\n";
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                WINDOWS DESTRUCTOR
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::Abort() {
    if (*(shared_memory_cmd+70) == 1) *(shared_memory_cmd+70) = 0;

    if ( XYscanning==true ) {
        tty_send(1,"HLT",NULL,serialX);
        tty_send(1,"ERR?",NULL,serialX);

        char *cstr1 = &(read_answer(serialX))[0u];
        printf("[!] X-axis stage stopped with exit value: %s", cstr1);

        tty_send(1,"HLT",NULL,serialY);
        tty_send(1,"ERR?",NULL,serialY);

        char *cstr2 = &(read_answer(serialY))[0u];
        printf("[!] Y-axis stage stopped with exit value: %s", cstr2);

        XYscanning = false;
        YHasMoved  = true;
        SaveTxt();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();

    int processIDs[7][2] = { { 0 }, { 0 } };
    int processLaser = *(shared_memory_laser+40) ;

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
    shmdt(shared_memory_laser);
    shmdt(shared_memory_cmd);

    qDebug()<<"... Killing child processes";

    sprintf(process, "kill -s TERM %i &", processLaser);
    system(process);

    for (int i = 0; i < 7; i++) {
        if (processIDs[i][0] == 1) {
            qDebug()<<"... Killing process with ID: "<<processIDs[i][1];
            sprintf(process, "kill -s TERM %i &", processIDs[i][1]);
            system(process);
        }
    }

    event->accept();
}

MainWindow::~MainWindow() {

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

