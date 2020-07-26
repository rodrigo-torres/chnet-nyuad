//////////////////////////////////////////////////// FRAO 31 DEC 2016 ////////////////////////
// CHNet - INFN 
//
// MainWindow (MW): the MW GUI is built using the following files: 
//
// 
//
// menu_connections_creator.cpp:	creates connection between menu items (in Ui) and methods
// GUI_Creator.cpp:			builds the GUI (shape and object/items)
// Connections_Creator.cpp:		creates connection between gui-objects/items (buttons...) and methods
// 
// GLOBAL FILE: Header.h contains all the needed #<include> files
//
// MAIN BLOCKS IN MAINWINDOW.CPP 
// 
// - FUTHER ACTIONS:
//                  createStatusBar();
//                  SetCurrentAction(QString text);
//                  hideImage();
// - MAIN COMMAND CYCLE:
//                  timerEvent();
// - MOTORS: COMMAND and ANSWERS HANDSHAKE
//                  int invia_comando_X(int chan,const char *comando, const char *parametri);
//                  int invia_comando_Y(int chan,const char *comando, const char *parametri);
//                  int invia_comando_Z(int chan,const char *comando, const char *parametri);
//                  char *read_Xanswer(), char *read_Yanswer(), char *read_Zanswer();
//                  string read_Xanswer2(), string read_Yanswer2(), string read_Zanswer2();
// - MOTORS: CHECK_ON_TARGET
//                  CheckXOnTarget(), CheckYOnTarget(), CheckZOnTarget();
//                  Enable_TabWidget_3_4_XY()
// - MOTOR SETTINGS
//                  Velocity(double), VelocityZ(double);
//                  PassoX_Func(double), PassoY_Func(double), PassoY_Func(double);
//                  Xminimo(double), Yminimo(double),Zminimo(double); 
//                  Xmassimo(double), Ymassimo(double), Zmassimo(double); 
//                  X_to(double), Y_to(double), Z_to(double);
//                  StartX(), StartY(), StartZ();
//                  Stop(), StopZ();
// - SCAN SETTINGS
//                  StartXYScan(), StartXYScan(); // (bool)
// - DAQ SETTINGS AND DIGITISER PROGRAM CONTROL
//                  USB_DAQ();
//                  OPTICAL_DAQ();
//                  SelMeasTime();
//                  CheckSegFault();
// - MAP - PIXEL AND SPECTRUM MANAGEMENT
//                  open_MAP();
//                  SelectChannels();
//                  CutB(); 
//                  MapCorrection(); 
//                  Pixels();
//                  PixelCrct();
//                  Treshold();
// - MAP LOAD IN MEMORY
//                  LoadNewFile_SHM();
// - FILES MANAGEMENT
//                  LoadTxt();
//                  SaveTxt();
//                  MergeTxt();
// - WINDOWS DESTRUCTOR
//                  Abort(), AbortZ();
//                  Exit();
//                  ~MainWindow();
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

#include "../Header.h"
#include "mainwindow.h"
#include "../variables.h"
#include "../QT_variables.h"
#include <../Shm.h>


extern int Resolution_mode;
extern double numpixelforaccel;
extern double posXforacceleration;
extern double accelerationtime;
extern int accelerationtimesleep;
extern double Px;


bool PixelCorrection=false;  bool CutBorders=false;   bool OnLine=false;         bool MapIsOpened=false;  bool CameraOn=false;
bool okX=false;              bool okY=false;          bool energychanged=false;
bool FirstRun=true;          bool opened=false;       bool XConnected=false;     bool YConnected=false;   bool TimerActive=false;
bool XOnTarget=false;        bool YOnTarget=false;    bool XHasMoved=true;       bool Xmoving=false;      bool Ymoving=false;
bool XYscanning=false;       bool YXscanning=false;   bool YHasMoved=true;       bool okZ=false;          bool TimerZActive=false;
bool ZConnected=false;       bool ZOnTarget=false;    bool ZHasMoved=true;       bool Zmoving=false;      bool noKeyence_init=true;
bool InitX=false;            bool InitY=false;        bool InitZ=false;          bool AutofocusOn=false;

int i=0;                     int j=0;                 int n=1;                   int MergePos=0;          int mempos=0; //memory position histo
int Pixeldim=1;              int EventOffset=0;       int nz=1;                  int missing=1;           int measuring_time=300;
int OffsetX;                 int OffsetY;             int NshiftY=0;             int onlyOne=0;           int NshiftX=0;/////o 1???
int point;                   int Clock=0;             int Clock2=0;              int ClockMotore=0;       int ClockZ=0;
int casenumber=4;            int interval=100;        int NscanX=0;              int NscanY=0;            int StoredPoint=0;  
int eventionline=0;          int m=1;

double positionX=100;        double valueY;           double ZPosition=25.0;    double tempoPos=1000;     double V=1;
double positionY=100;        double valueX;           double ChMin=3;           double ChMax=16384;       double Vz=1;
double tempoPosZ=1000;       double valueZ;           double positionZ;         double Pz=250;            double x_image;
double y_image;              double x_image2;         double y_image2; 

float Yo;                    float vy;                float Xo;                 float vx;                 float vz;
float temp;                  
  
long int iLine=0;

char process[30];

QString KeyenceValue="";     QString NowZ="Z= ";      QString current_posZ;     QString NowX="X= ";       QString NowY="Y= ";

string checkX;               string checkY;           string checkZ;            string checkK;            string Zpos;
string posZ;

struct Pixel_BIG *Pointer; //puntatore da far puntare a PixelsMappa una volta creato

//////////// ADDED VARIABLES FOR DEVELOP VERSION (Bart_PE)

int selected_Xmotor=3; int selected_Ymotor=3; int selected_Zmotor=1;
bool InitPhaseX=false; bool InitPhaseY=false; bool InitPhaseZ=false;
int nxInit=0, nyInit=0, nzInit=0; // used for motor initialisation

///////////////Variables for the composed map visualization (sum of three different elements)///////////
int ChMin1=0, ChMax1=0, ChMin2=0, ChMax2=0, ChMin3=0, ChMax3=0;
int ChMinBa=0, ChMaxBa=0, ChMinCa=0, ChMaxCa=0, ChMinK=0, ChMaxK=0, ChMinCo=0, ChMaxCo=0, ChMinAg=0, ChMaxAg=0, ChMinCr=0, ChMaxCr=0, ChMinCu=0, ChMaxCu=0, ChMinPbL=0, ChMaxPbL=0, ChMinAu=0, ChMaxAu=0, ChMinHg=0, ChMaxHg=0, ChMinSi=0, ChMaxSi=0, ChMinTi=0, ChMaxTi=0, ChMinSn=0, ChMaxSn=0, ChMinFe=0, ChMaxFe=0, ChMinZn=0, ChMaxZn=0, ChMinPbM=0, ChMaxPbM=0;

////Variables for multidetector expansion/////



///////////variables for tests///////////////
double millisec;
struct timeval tv;





MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    switch (Resolution_mode) {
    case 0:
        PixelX=795;
        PixelY=795;
        break;
    case 1:
        PixelX=595;
        PixelY=595;
        break;
    case 2:
        PixelX=395;
        PixelY=395;
        break;
    }

     SHM_CREATOR();                 /// CREATING SHARED MEMORY SEGMENT
     qDebug()<<"... Succesfully attached all memory segments";
     createActions();
     builder_Menu();            	    /// CREATING MENU from Menu.cpp
     GUI_CREATOR();
     CONNECTIONS_CREATOR();

     imageLabel = new ImgLabel;

     QImage image("IMG/TT_CHNet_res0.png");
     QImage image1("IMG/TT_CHNet_res1.png");
     QImage image2("IMG/TT_CHNet_res2.png");

     switch (Resolution_mode) {
     case 0:
         imageLabel->setPixmap(QPixmap::fromImage(image));
         //qDebug()<<"Map info: pixmap width scaled to 795x795";
         break;
     case 1 :
         imageLabel->setPixmap(QPixmap::fromImage(image1));
         //qDebug()<<"Map info: pixmap width scaled to 595x595";
         break;
       case 2 :
         imageLabel->setPixmap(QPixmap::fromImage(image2));
         //qDebug()<<"Map info: pixmap width scaled to 395x395";
         break;
      }

     imageLabel->setBackgroundRole(QPalette::Base);
     imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
     imageLabel->setScaledContents(true);

     scrollArea = new QScrollArea(centralWidget);
     scrollArea->setBackgroundRole(QPalette::Dark);
     scrollArea->setWidget(imageLabel);

     switch (Resolution_mode)
      {
       case(0):
               {scrollArea->setGeometry(QRect(24, 20, 800, 800));
                scrollArea->setMinimumSize(QSize(800, 800));
                scrollArea->setMaximumSize(QSize(800, 800));
                break;
               }
       case(1):
               {scrollArea->setGeometry(QRect(18, 15, 600, 600));
                scrollArea->setMinimumSize(QSize(600, 600));
                scrollArea->setMaximumSize(QSize(600, 600));
                break;
               }
       case(2): 
               {scrollArea->setGeometry(QRect(12, 10, 400, 400));
                scrollArea->setMinimumSize(QSize(400, 400));
                scrollArea->setMaximumSize(QSize(400, 400));
                break;
               }
       }


    timer = new QTimer(this);                                                // TIMER for program control
    connect(timer, SIGNAL(timeout()), this, SLOT(timerEvent()));

    timerAutofocus =new QTimer(this);                                        // TIMER for RECORDING Z DISTANCE FROM TARGET
    connect(timerAutofocus, SIGNAL(timeout()), this, SLOT(Focustimer()));

    ///Red channel intervals///
    char element[3];
    FILE *filech; //name of the file where the channel intervals are specified
    filech = fopen ("../channel_intervals", "r");
    fscanf(filech, "%s %d %d", element, &ChMinBa, &ChMaxBa);
    fscanf(filech, "%s %d %d", element, &ChMinCa, &ChMaxCa);
    fscanf(filech, "%s %d %d", element, &ChMinK, &ChMaxK);
    fscanf(filech, "%s %d %d", element, &ChMinCo, &ChMaxCo);
    fscanf(filech, "%s %d %d", element, &ChMinAg, &ChMaxAg);
    fscanf(filech, "%s %d %d", element, &ChMinCr, &ChMaxCr);
    fscanf(filech, "%s %d %d", element, &ChMinCu, &ChMaxCu);
    fscanf(filech, "%s %d %d", element, &ChMinPbL, &ChMaxPbL);
    fscanf(filech, "%s %d %d", element, &ChMinAu, &ChMaxAu);
    fscanf(filech, "%s %d %d", element, &ChMinHg, &ChMaxHg);
    fscanf(filech, "%s %d %d", element, &ChMinSi, &ChMaxSi);
    fscanf(filech, "%s %d %d", element, &ChMinTi, &ChMaxTi);
    fscanf(filech, "%s %d %d", element, &ChMinSn, &ChMaxSn);
    fscanf(filech, "%s %d %d", element, &ChMinFe, &ChMaxFe);
    fscanf(filech, "%s %d %d", element, &ChMinZn, &ChMaxZn);
    fscanf(filech, "%s %d %d", element, &ChMinPbM, &ChMaxPbM);
    fclose(filech);

    readmultidetcalpar();
    printf("%d %d %d\n",*(shared_memory_cmd+101), *(shared_memory_cmd+102), *(shared_memory_cmd+103));
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

void MainWindow::readmultidetcalpar() //reads the file with the calibration parameters for the multidetector in sum mode
{
int calpar1=0;
int calpar2=0;
int scalefactor=1;

FILE *filecalpar; //name of the file where the channel intervals are specified
filecalpar = fopen ("../multidetector_calibrationparameters.txt", "r");
fscanf(filecalpar, "%d", &calpar1);
fscanf(filecalpar, "%d",&calpar2);
fscanf(filecalpar, "%d",&scalefactor);
fclose(filecalpar);

*(shared_memory_cmd+101)=calpar1;
*(shared_memory_cmd+102)=calpar2;
*(shared_memory_cmd+103)=scalefactor;

}


void MainWindow::createStatusBar()
  {
   statusBar()->showMessage(tr("Ready"));
  }


void MainWindow::SetCurrentAction(QString text)  // SET INFO ON BAR
  {
   CurrentAction->setText(text);
  }


void MainWindow::hideImage()  // MANAGE IMAGE IN SCROLL AREA
  {
   if(MapIsOpened==true)
    {
     QImage startimage("IMG/TT_CHNet_extended_395_395_3.png");
     imageLabel->setPixmap(QPixmap::fromImage(startimage));
     MapIsOpened=false;
   }
  }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                MAINWINDOW MAIN COMMAND CYCLE --> TIMER_EVENT
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::timerEvent() {

    //ClockMotore++;
    if(XYscanning)ScanXY();
    //if(YXscanning)ScanYX();
    if (InitX)CheckXOnTarget();
    if (InitY)CheckYOnTarget();
    if (InitZ)CheckZOnTarget();
    //if(AutofocusOn)AutoFocusRunning();
    MoveDoubleClick();
    CheckSegFault();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                MOTORS: COMMAND and ANSWERS HANDSHAKE
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int send_command(int channel,const char *command, const char *parameter, int port)
{
    int serial=port;
    char chan=channel+'0';
    string cm=command;

    if (parameter==NULL)
       cm=cm+'\n';
    else
       cm=cm+' '+chan+' '+parameter+'\n';
    if ( write(serial, cm.data(), cm.size()) )
       return 0;
    else {
       qDebug()<<"[!] Writing to bus failed"
                 "[!] Check connection and writing priviledges";
       qDebug()<<strerror(errno);
       return 1;
    }
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


/*
string read_Xanswer2()                                                     // X MOTOR: READ ANSWER STRING
{
  char c[100];
  int n=0;
  string rest;
  string Xread;
  while((n=read(serialX, &c, sizeof(c)))>0)      
    {
      c[n]=0;
      rest=rest+c;      
      if(c[n-1]=='\n')
	break;        
    }
  Xread =rest;
  return Xread;  
}
*/

string read_Yanswer2()                                                     // Y MOTOR: READ ANSWER STRING
{
  char cy[100];
  int ny=0;
  string resty;
  string Yread;
  while((ny=read(serialY, &cy, sizeof(cy)))>0)      
    {
      cy[ny]=0;
      resty=resty+cy;      
      if(cy[ny-1]=='\n')
	break;        
    }
  Yread =resty;
  return Yread;  
}

string read_Zanswer2()                                                     // Z MOTOR: READ ANSWER STRING
{
  char cz[100];
  int nz=0;
  string restz;
  string Zread;
  while((nz=read(serialZ, &cz, sizeof(cz)))>0)      
    {
      cz[nz]=0;
      restz=restz+cz;      
      if(cz[nz-1]=='\n')
	break;        
    }
  Zread =restz;
  return Zread;  
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                MOTORS: CHECK_ON_TARGET
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::CheckXOnTarget() {

    string a;
    send_command(1,"ONT?",NULL,serialX);
    a=read_answer(serialX);
    QString Qa=a.data();

    if ( Qa.contains("0", Qt::CaseInsensitive) == false ) {

        XOnTarget=true;
        Xmoving=false;
        if ( InitPhaseX && nxInit == 0 ) {
                nxInit=1;
                qDebug()<<"... X motor initialized\n";
                IniX=0;
                IniXready=1;
        }
    }
    else {
        XOnTarget=false;
        Xmoving=true;
    }
    
    send_command(1,"POS?",NULL,serialX);
    checkX = read_answer(serialX);
    NowX="X= ";
    NowX.append(checkX.data());
    NowX.remove(3,2);
    X_POSITION_lineEdit->setText(NowX);
}


void MainWindow::CheckYOnTarget()                                           // Y MOTOR: CHECK_ON_TARGET
  {
   string ay;
   send_command(1,"ONT?",NULL,serialY);
   ay=read_Yanswer2();
   QString Qay=ay.data();
   if(Qay.contains("0", Qt::CaseInsensitive)==false) // also 1=0 can be used 
     {
      YOnTarget=true; Ymoving=false;
      if(InitPhaseY)
        {
         if(nyInit==0)
           {
            nyInit=1;
            qDebug()<<"... Y motor initialized\n";
            IniY=0; IniYready=1; 
           }
        }
      }
   else
     {
      YOnTarget=false; 
      Ymoving=true;
     }
   send_command(1,"POS?",NULL,serialY);
   checkY = read_Yanswer2();
   NowY="Y= ";
   NowY.append(checkY.data());
   NowY.remove(3,2); 
   Y_POSITION_lineEdit->setText(NowY); 
  }


void MainWindow::CheckZOnTarget()                                           // Z MOTOR: CHECK_ON_TARGET
  {
   string az;
   send_command(1,"ONT?",NULL,serialZ);
   az=read_Zanswer2();
   QString Qaz=az.data();
   if(Qaz.contains("0", Qt::CaseInsensitive)==false) // also 1=0 can be used 
     {
      ZOnTarget=true; 
      Zmoving=false;
      if(InitPhaseZ)
        {
         if(nzInit==0)
          {
           nzInit=1;
           qDebug()<<"... Z motor initialized\n";
           IniZ=0; IniZready=1;
           }
        }
     }
   else 
     {
      ZOnTarget=false; 
      Zmoving=true;
     }
   send_command(1,"POS?",NULL,serialZ);
   checkZ = read_Zanswer2();
   NowZ="Z= ";
   NowZ.append(checkZ.data());
   NowZ.remove(3,2); 

   lineEdit_below_tab->setText(NowZ); 
   NowZ.remove(0,3);
   ZPosition=NowZ.toDouble();

  }

void MainWindow::Enable_TabWidget_3_4_XY()                                           // ENABLING XY MOVE WIDGET
  {
  if(InitX && InitY){tab2_3->setEnabled(true); tab2_4->setEnabled(true);}
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
   send_command(1,"VEL",v,serialX);
   send_command(1,"VEL",v,serialY);
  }

/* Function VelocityZ has been deprecated, as the Z motor velocity is entirely controlled by
   the initialization function, and by the laser feedback loop.
void MainWindow::VelocityZ(double numberZ)                       // MOTOR SETTINGS Z VELOCITY
  {
   char vz[10];
   sprintf(vz,"%f",numberZ);
   send_command(1,"VEL",vz,serialZ);
}
*/


void MainWindow::PassoX_Func(double number1)                     // MOTOR SETTINGS STEP
  {Px=number1*1000; *(shared_memory_cmd+60)=Px; pixel_Xstep=number1;} 
void MainWindow::PassoY_Func(double number5)
  {Py=number5*1000; *(shared_memory_cmd+61)=Py; pixel_Ystep=number5;} 
/*void MainWindow::PassoZ_Func(double number1)
  {Pz=number1*1000; *(shared_memory_cmd+62)=Pz;}
*/

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


void MainWindow::StartX()                                          // MOTOR START X
  {
   if(!okX)
     {
      okX=true; 
      Init_Xmotor();
      if(InitX && InitY)
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


void MainWindow::StartY()                                          // MOTOR START Y
  {
   if(!okY)
     {
      okY=true; 
      Init_Ymotor();
      if(InitX && InitY)
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
   send_command(1,"HLT",NULL,serialX);
   send_command(1,"ERR?",NULL,serialX);
   checkX = read_answer(serialX);

   send_command(1,"HLT",NULL,serialY);
   send_command(1,"ERR?",NULL,serialY);
   checkY = read_answer(serialY);
}

void MainWindow::StopZ()                                            // MOTOR STOP Z
  {
   send_command(1,"HLT",NULL,serialZ);
   send_command(1,"ERR?",NULL,serialZ);
   checkZ = read_Zanswer2();
  }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                SCAN SETTINGS
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



bool MainWindow::StartXYScan() {

    if(TimerActive==false) {
        timer->start(interval);
        TimerActive=true;
    }

    if(XYscanning==false) {
        positionY=Ymin1; positionX=Xmin1;
        Xmin=positionX; Ymin=positionY;
        Xmax=Xmax1;Ymax=Ymax1;

        // The acceleration of the motor is specified as 200 mm/s
        // The variable V is the desired motor velocity specified in the GUI

        XYscanning=true; NscanX=0; FirstRun=true;
        tempoPos=Px/V;
        accelerationtime=(V/200); //time interval for the acquisition startup, taken considering the time needed for acceleration
        posXforacceleration=(100*(accelerationtime*accelerationtime))*1000; //in um
        accelerationtimesleep=round(accelerationtime*1000)+23;

        printf("... Acquisition sleep to account for acceleration set at:%d ms\n", accelerationtimesleep);
    }

    return XYscanning;
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
    else
     if(*(shared_memory+24)==1)
     {
      double chan1 = QInputDialog::getDouble(this, tr("Lower Energy"), tr("ELow:"), 0, 0, 60, 1, &ok1);
      if (ok1)
        {qDebug()<<"New lower energy "<<chan1<<'\n'; ChMin=int(chan1*1000);energychanged=true;}
      double chan2 = QInputDialog::getDouble(this, tr("Upper Energy"), tr("EHigh:"), 60, 0, 60, 1, &ok2);
      if (ok2)
        {qDebug()<<"New upper energy "<<chan2<<'\n'; ChMax=int(chan2*1000);energychanged=true;}
    }
  }

void MainWindow::CutB()                                           // MAP: CUT BORDERS
  {
   if(CutBorders)CutBorders=false;
   else CutBorders=true;
  }

void MainWindow::MapCorrection()                                  // MAP: CORRECTION
  {
   bool ok3;
   int nX=QInputDialog::getInt(this, tr("Shift X"), tr("Positions to shift"), 0, -10, 10, 1, &ok3);
   if(ok3)
     {qDebug()<<"Shift of "<<nX<<"Positions\n"; pixel_Xstep=*(shared_memory_cmd+60); NshiftX=(nX*pixel_Xstep);}
   bool ok4;
   int nY=QInputDialog::getInt(this, tr("Shift Y"), tr("Positions to shift"), 0, -10, 10, 1, &ok4);
   if(ok4)
     {qDebug()<<"Shift of "<<nY<<"Positions\n"; pixel_Ystep=*(shared_memory_cmd+61); NshiftY=nY*pixel_Ystep;}
   LoadNewFile_SHM(); // inserted on 30/12/2016
}

void MainWindow::Pixels()                                         // PIXEL: SET DIMENSION
  {
   bool ok1;
   bool ok2=false;
   while(ok2==false)
      {
       int px = QInputDialog::getInt(this, tr("Pixel Size"),
                                     tr("pixels(only odd numbers!):"), 1, 1, 30, 2, &ok1);
       if((px%2)!=0) ok2=true;
       if (ok1 && ok2)
         {
          Pixeldim=px; 
          int MaxNumberX=qRound((float)PixelX/px);
          int MaxNumberY=qRound((float)PixelY/px);
          int MaxNumber=qRound((float)(PixelX/px)*(PixelY/px));
          qDebug()<<"New pixel dimension "<<px<<"---->"<<"Max number of points (X,Y)"<<MaxNumber<<'\n';
          qDebug()<<"Max number of points X"<<MaxNumberX<<'\n';
          qDebug()<<"Max number of points Y"<<MaxNumberY<<'\n';
         }
      }
  }

void MainWindow::PixelCrct()                                       // PIXEL: CORRECTION FOR MISSING PIXEL
  {
   if(PixelCorrection)
     {
      PixelCorrection=false;
      qDebug()<<"...pixel correction disabled..";
     }
   else 
     {
      PixelCorrection=true;
      qDebug()<<"...pixel correction on..";
     }
  }


void MainWindow::Treshold()                                         // SPECTRUM: LOW TRESHOLD SELECTION
 {
  int low_treshold;
  bool ok3;
  low_treshold = QInputDialog::getInt(this, tr("Low Treshold Level (ch)"),
		 tr("Low Ch:"),0, 0, 400, 1, &ok3);
  if(ok3)
    {
     qDebug()<<"Low Channel set to ="<<low_treshold<<'\n'; 
     *(shared_memory+40)=low_treshold;
    }
 }



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                MAP LOAD IN MEMORY
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::LoadNewFile_SHM()                               // LOAD FILE IN MEMORY
  {
   qDebug()<<"entering LoadNewFile_SHM";
   if(PixelCorrection)  {qDebug()<<"....enabling correction...";LoadNewFileWithCorrection_SHM(); displayImage_SHM();}    
   if(!PixelCorrection) {qDebug()<<"...no file correction active...";LoadNewFileWithNoCorrection_SHM();displayImage_SHM();}
  }

void MainWindow::LoadElementsMapSum() {

    elementsdlg = new QDialog;
    elementsdlg->resize(150,150);
    QGroupBox *elementsgroupBox = new QGroupBox( "Select Elements" );
    QLabel *element1label = new QLabel( "First Element (red)" );
    QComboBox *element1comboBox = new QComboBox;
        element1comboBox->insertItems(0, QStringList()
        << QApplication::translate("Element 1", "None", 0)
             << QApplication::translate("Element 1", "Ba", 0)
             << QApplication::translate("Element 1", "Ca", 0)
             << QApplication::translate("Element 1", "K", 0)
             << QApplication::translate("Element 1", "Co", 0)
        << QApplication::translate("Element 1", "Ag", 0)
        << QApplication::translate("Element 1", "Cr", 0)
        << QApplication::translate("Element 1", "Cu", 0)
        << QApplication::translate("Element 1", "PbL", 0)
        << QApplication::translate("Element 1", "Au", 0)
        << QApplication::translate("Element 1", "Hg", 0)
        << QApplication::translate("Element 1", "Si", 0)
        << QApplication::translate("Element 1", "Ti", 0)
        << QApplication::translate("Element 1", "Sn", 0)
        << QApplication::translate("Element 1", "Fe", 0)
        << QApplication::translate("Element 1", "Zn", 0)
        << QApplication::translate("Element 1", "PbM", 0)
            );
        connect(element1comboBox, SIGNAL(activated(int)),this, SLOT(SelectionElement1(int)));


QLabel *element2label = new QLabel( "Second Element (green)" );
QComboBox *element2comboBox = new QComboBox; 
    element2comboBox->insertItems(0, QStringList()
	<< QApplication::translate("Element 2", "None", 0)
         << QApplication::translate("Element 2", "Ba", 0)
         << QApplication::translate("Element 2", "Ca", 0)
         << QApplication::translate("Element 2", "K", 0)
         << QApplication::translate("Element 2", "Co", 0)
	<< QApplication::translate("Element 2", "Ag", 0)
	<< QApplication::translate("Element 2", "Cr", 0)
	<< QApplication::translate("Element 2", "Cu", 0)
	<< QApplication::translate("Element 2", "PbL", 0)
	<< QApplication::translate("Element 2", "Au", 0)
	<< QApplication::translate("Element 2", "Hg", 0)
	<< QApplication::translate("Element 2", "Si", 0)
	<< QApplication::translate("Element 2", "Ti", 0)
	<< QApplication::translate("Element 2", "Sn", 0)
	<< QApplication::translate("Element 2", "Fe", 0)
	<< QApplication::translate("Element 2", "Zn", 0)
	<< QApplication::translate("Element 2", "PbM", 0)
        );
    connect(element2comboBox, SIGNAL(activated(int)),this, SLOT(SelectionElement2(int)));

QLabel *element3label = new QLabel( "Third Element (blue)" );
QComboBox *element3comboBox = new QComboBox; 
    element3comboBox->insertItems(0, QStringList()
	 << QApplication::translate("Element 3", "None", 0)
         << QApplication::translate("Element 3", "Ba", 0)
         << QApplication::translate("Element 3", "Ca", 0)
         << QApplication::translate("Element 3", "K", 0)
         << QApplication::translate("Element 3", "Co", 0)
	<< QApplication::translate("Element 3", "Ag", 0)
	<< QApplication::translate("Element 3", "Cr", 0)
	<< QApplication::translate("Element 3", "Cu", 0)
	<< QApplication::translate("Element 3", "PbL", 0)
	<< QApplication::translate("Element 3", "Au", 0)
	<< QApplication::translate("Element 3", "Hg", 0)
	<< QApplication::translate("Element 3", "Si", 0)
	<< QApplication::translate("Element 3", "Ti", 0)
	<< QApplication::translate("Element 3", "Sn", 0)
	<< QApplication::translate("Element 3", "Fe", 0)
	<< QApplication::translate("Element 3", "Zn", 0)
	<< QApplication::translate("Element 3", "PbM", 0)
        );
    connect(element3comboBox, SIGNAL(activated(int)),this, SLOT(SelectionElement3(int)));

QDialogButtonBox *buttonBox = new QDialogButtonBox( Qt::Horizontal );
OKbutton = new QPushButton( "Ok" );

connect( OKbutton, SIGNAL( clicked() ), elementsdlg, SLOT( close() ));
connect( OKbutton, SIGNAL( clicked() ), this, SLOT( LoadSHM_SumMap() ));

buttonBox->addButton( OKbutton, QDialogButtonBox::AcceptRole );

CANCELbutton = new QPushButton( "Cancel" );

connect( CANCELbutton, SIGNAL(clicked() ), elementsdlg, SLOT( close() ));
buttonBox->addButton( CANCELbutton, QDialogButtonBox::AcceptRole );

QGridLayout *Layout = new QGridLayout( elementsgroupBox );
Layout->setSpacing(0);
Layout->addWidget( element1label,0,0 );
Layout->addWidget( element1comboBox,0,1 );
Layout->addWidget( element2label,1,0 );
Layout->addWidget( element2comboBox,1,1 );
Layout->addWidget( element3label,2,0 );
Layout->addWidget( element3comboBox,2,1 );


QVBoxLayout *elementsLayout = new QVBoxLayout( elementsdlg );
elementsLayout->addWidget( elementsgroupBox );
elementsLayout->addStretch();
elementsLayout->addWidget(buttonBox);

elementsdlg->show();

}


void MainWindow::SelectionElement1(int element1)
{
if(element1==0){ChMin1=0; ChMax1=0;}
else
if(element1==1){ChMin1=ChMinBa; ChMax1=ChMaxBa;}
else
if(element1==2){ChMin1=ChMinCa; ChMax1=ChMaxCa;}
else
if(element1==3){ChMin1=ChMinK; ChMax1=ChMaxK;}
else
if(element1==4){ChMin1=ChMinCo; ChMax1=ChMaxCo;}
else
if(element1==5){ChMin1=ChMinAg; ChMax1=ChMaxAg;}
else
if(element1==6){ChMin1=ChMinCr; ChMax1=ChMaxCr;}
else
if(element1==7){ChMin1=ChMinCu; ChMax1=ChMaxCu;}
else
if(element1==8){ChMin1=ChMinPbL; ChMax1=ChMaxPbL;}
else
if(element1==9){ChMin1=ChMinAu; ChMax1=ChMaxAu;}
else
if(element1==10){ChMin1=ChMinHg; ChMax1=ChMaxHg;}
else
if(element1==11){ChMin1=ChMinSi; ChMax1=ChMaxSi;}
else
if(element1==12){ChMin1=ChMinTi; ChMax1=ChMaxTi;}
else
if(element1==13){ChMin1=ChMinSn; ChMax1=ChMaxSn;}
else
if(element1==14){ChMin1=ChMinFe; ChMax1=ChMaxFe;}
else
if(element1==15){ChMin1=ChMinZn; ChMax1=ChMaxZn;}
else
if(element1==16){ChMin1=ChMinPbM; ChMax1=ChMaxPbM;}
}

void MainWindow::SelectionElement2(int element2)
{
if(element2==0){ChMin2=0; ChMax2=0;}
else
if(element2==1){ChMin2=ChMinBa; ChMax2=ChMaxBa;}
else
if(element2==2){ChMin2=ChMinCa; ChMax2=ChMaxCa;}
else
if(element2==3){ChMin2=ChMinK; ChMax2=ChMaxK;}
else
if(element2==4){ChMin2=ChMinCo; ChMax2=ChMaxCo;}
else
if(element2==5){ChMin2=ChMinAg; ChMax2=ChMaxAg;}
else
if(element2==6){ChMin2=ChMinCr; ChMax2=ChMaxCr;}
else
if(element2==7){ChMin2=ChMinCu; ChMax2=ChMaxCu;}
else
if(element2==8){ChMin2=ChMinPbL; ChMax2=ChMaxPbL;}
else
if(element2==9){ChMin2=ChMinAu; ChMax2=ChMaxAu;}
else
if(element2==10){ChMin2=ChMinHg; ChMax2=ChMaxHg;}
else
if(element2==11){ChMin2=ChMinSi; ChMax2=ChMaxSi;}
else
if(element2==12){ChMin2=ChMinTi; ChMax2=ChMaxTi;}
else
if(element2==13){ChMin2=ChMinSn; ChMax2=ChMaxSn;}
else
if(element2==14){ChMin2=ChMinFe; ChMax2=ChMaxFe;}
else
if(element2==15){ChMin2=ChMinZn; ChMax2=ChMaxZn;}
else
if(element2==16){ChMin2=ChMinPbM; ChMax2=ChMaxPbM;}
}

void MainWindow::SelectionElement3(int element3)
{
if(element3==0){ChMin3=0; ChMax3=0;}
else
if(element3==1){ChMin3=ChMinBa; ChMax3=ChMaxBa;}
else
if(element3==2){ChMin3=ChMinCa; ChMax3=ChMaxCa;}
else
if(element3==3){ChMin3=ChMinK; ChMax3=ChMaxK;}
else
if(element3==4){ChMin3=ChMinCo; ChMax3=ChMaxCo;}
else
if(element3==5){ChMin3=ChMinAg; ChMax3=ChMaxAg;}
else
if(element3==6){ChMin3=ChMinCr; ChMax3=ChMaxCr;}
else
if(element3==7){ChMin3=ChMinCu; ChMax3=ChMaxCu;}
else
if(element3==8){ChMin3=ChMinPbL; ChMax3=ChMaxPbL;}
else
if(element3==9){ChMin3=ChMinAu; ChMax3=ChMaxAu;}
else
if(element3==10){ChMin3=ChMinHg; ChMax3=ChMaxHg;}
else
if(element3==11){ChMin3=ChMinSi; ChMax3=ChMaxSi;}
else
if(element3==12){ChMin3=ChMinTi; ChMax3=ChMaxTi;}
else
if(element3==13){ChMin3=ChMinSn; ChMax3=ChMaxSn;}
else
if(element3==14){ChMin3=ChMinFe; ChMax3=ChMaxFe;}
else
if(element3==15){ChMin3=ChMinZn; ChMax3=ChMaxZn;}
else
if(element3==16){ChMin3=ChMinPbM; ChMax3=ChMaxPbM;}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                FILES MANAGEMENT
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::LoadTxt()  //carica Position.txt in memoria
  {
   int i=0;
   int numero=0;	
   QString text = QFileDialog::getOpenFileName(this,
                             tr("Open File"), QDir::currentPath());
   if (!text.isEmpty())
    {
     QFile file(text);
     if(file.exists())
      {
       qDebug()<<"....loading file...\n";
       file.open(QIODevice::ReadOnly);
       QString line;
       line = file.readLine();
	  if(line.startsWith("v"))
	  {
		line = file.readLine();
		numero=line.toInt();
                *(shared_memory_cmd+50)=numero;//Xmin
		printf("Xmin:%d\n", numero);
		line = file.readLine();
		numero=line.toInt();
                *(shared_memory_cmd+51)=numero;//Xmax
		printf("Xmax:%d\n", numero);
		line = file.readLine();
		numero=line.toInt();
                *(shared_memory_cmd+52)=numero;//Ymin
		printf("Ymin:%d\n", numero);
		line = file.readLine();
		numero=line.toInt();
                *(shared_memory_cmd+53)=numero;//Ymax
		printf("Ymax:%d\n", numero);
		line = file.readLine();
		numero=line.toInt();
		*(shared_memory_cmd+60)=numero;//Xstep
		printf("Xstep:%d\n", numero);
		line = file.readLine();
		numero=line.toInt();
		*(shared_memory_cmd+61)=numero;//Ystep
		printf("Ystep:%d\n", numero);
		line = file.readLine();
		numero=line.toInt();
		*(shared_memory_cmd+67)=numero;//scan velociy
		printf("Velocity:%d\n", numero);
	  }
	else
	{
		 bool ok1;
		  pixel_Xstep = QInputDialog::getInt(this, tr("Step X"),
                                 tr("Step X (um):"), 500, 1, 10000, 1, &ok1);
 		 *(shared_memory_cmd+60)=pixel_Xstep;

 		 pixel_Ystep = QInputDialog::getInt(this, tr("Step Y"),
                                 tr("Step Y (um):"), 500, 1, 10000, 1, &ok1);
		  *(shared_memory_cmd+61)=pixel_Ystep;

		numero=line.toInt();
          	i++;
          	*(shared_memory2+10+i)=numero;  //dalla posiz 11 in poi della memoria2 (scrive l'analogo di Position.txt)
		
	}

	
       while( !file.atEnd() ) 
         {
          line = file.readLine();
          numero=line.toInt();
          i++;
          *(shared_memory2+10+i)=numero;  //dalla posiz 11 in poi della memoria2 (scrive l'analogo di Position.txt)
	  if(i==1)printf("First position:%d\n", numero);
	  
         }
       file.close();
       *(shared_memory2+4)=i;
       qDebug()<<" .... file loaded.... !\n";
      
      }
    else qDebug()<<".... file not found....!!!\n";
   }
	LoadNewFile_SHM();
  }




void MainWindow::SaveTxt()                                                 //scrive Position.txt leggendo i dati in memoria
  {
   qDebug()<<"che fai?\n";
   QString percorso = QFileDialog::getSaveFileName(this,tr("Save as"), QDir::currentPath());
   QFile file2(percorso); 
   qDebug()<<"Inizio creazione di" <<percorso<<"\n";
   file2.open(QIODevice::ReadWrite);
   QTextStream out2(&file2);
   int Ntot=*(shared_memory2+4);    //// Numero dati in memoria 	
	// qDebug()<<"Ntot="<<Ntot<<"\n";
      out2<<"ver.001"<<'\n';      
      out2<<*(shared_memory_cmd+50)<<'\n';//writes Xmin
      out2<<*(shared_memory_cmd+51)<<'\n';//writes Xmax
      out2<<*(shared_memory_cmd+52)<<'\n';//writes Ymin
      out2<<*(shared_memory_cmd+53)<<'\n';//writes Ymax
      out2<<*(shared_memory_cmd+60)<<'\n';//writes Xstep
      out2<<*(shared_memory_cmd+61)<<'\n';//writes Ystep
      out2<<*(shared_memory_cmd+67)<<'\n';//writes the scan velocity
   for(int i=1;i<=Ntot;i++)
     {
      out2<<*(shared_memory2+10+i)<<'\n';
	//qDebug()<<"data:"<<*(shared_memory2+10+i)<<"\n";
      }
   file2.close();
   qDebug()<<" ....file saved in: "<<percorso;
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
    if ( *(shared_memory_cmd+70) == 1) {
        *(shared_memory_cmd+70)=0;
    }

    if ( XYscanning==true ) {
        send_command(1,"HLT",NULL,serialX);
        send_command(1,"ERR?",NULL,serialX);
        checkX = read_answer(serialX);

        send_command(1,"HLT",NULL,serialY);
        send_command(1,"ERR?",NULL,serialY);
        checkY = read_answer(serialY);

        onlyOne=0;
        XYscanning=false;
        SaveTxt();
    }
}



void MainWindow::AbortZ()
  {
   send_command(1,"HLT",NULL,serialZ);
   send_command(1,"ERR?",NULL,serialZ);
   checkZ = read_Zanswer2();
  }


void MainWindow::Exit()
  {
   if(Xmoving==false && Ymoving==false && XYscanning==false)
    {
     qApp->quit();
    }
   else
    {
     Abort();
     qApp->quit();
    }
   if(Zmoving==false)
    {
     qApp->quit();
    }
   else
    {
     AbortZ();
     qApp->quit();
    }
  }


MainWindow::~MainWindow() {

    qDebug()<<"... Unallocating shared memory segments";
    SHM_delete();

    qDebug()<<"... Killing child processes\n";
    Sleeper::msleep(500);

    if ( *(shared_memory_cmd+70) ) {
       *(shared_memory_cmd+70)=0;
    }

    else { qDebug()<< "... DPP-ADC already off"; }
    //Sleeper::msleep(200);

    if ( *(shared_memory_cmd+71) ) {

       int pidSpectrum= *(shared_memory_cmd+81);
       sprintf(process, "kill -s TERM %i &", pidSpectrum);
       system(process);
       qDebug()<< "... Killing spectrum viewer...";
    }
    else { qDebug()<< "... Spectrum viewer already off"; }
    //Sleeper::msleep(100);

    if ( *(shared_memory_cmd+72) )  {
       int piddigitiser=*(shared_memory_cmd+82);
       sprintf(process, "kill -s TERM %i &", piddigitiser);
       system(process);
       qDebug()<< "... Killing DPP interface";
    }
    else { qDebug()<< "... DPP interface already off"; }
    //Sleeper::msleep(100);

    if ( *(shared_memory_cmd+73) ) {
       int pidrate=*(shared_memory_cmd+83);
       sprintf(process, "kill -s TERM %i &", pidrate);
       system(process);
       qDebug()<< "... Killing rate meter";
    }
    else { qDebug()<< "... Rate meter already off"; }
    //Sleeper::msleep(100);

    if ( *(shared_memory_cmd+74) ) {
       int pidxraytable=*(shared_memory_cmd+84);
       sprintf(process, "kill -s TERM %i &", pidxraytable);
       system(process);
       qDebug()<< "... Killing X-ray reference table";
    }
    else { qDebug()<< "... X-ray reference table already off"; }
    //Sleeper::msleep(100);

    if ( *(shared_memory_cmd+75) ) {
       int pidmaponline=*(shared_memory_cmd+85);
       sprintf(process, "kill -s TERM %i &", pidmaponline);
       system(process);
       qDebug()<< "... Killing live map display...";
    }
    else { qDebug()<< "... Live map display already off"; }
    //Sleeper::msleep(100);

    if ( *(shared_memory_cmd+76) ) {
       int pid_motor_test=*(shared_memory_cmd+86);
       sprintf(process, "kill -s TERM %i &", pid_motor_test);
       system(process);
       qDebug()<< "... Killing PI motor test tool";
    }
    else { qDebug()<< "... PI motor test tool already off"; }
    //Sleeper::msleep(100);

    if ( *(shared_memory_cmd+77) ) {
       int pid_motor_parameter_table=*(shared_memory_cmd+87);
       sprintf(process, "kill -s TERM %i &", pid_motor_parameter_table);
       system(process);qDebug()<< "... Killing PI motor parameter table";
    }
    else { qDebug()<< "... PI parameter table already off"; }
    //Sleeper::msleep(100);

    shmctl(shmid_cmd, IPC_RMID, 0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                HOWTO
//                                
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 
void MainWindow::Info1_1()
  {
   system("evince manual/Info_software_general.pdf &");
  }

void MainWindow::Info1_2()
  {
   system("evince manual/Info_shared_memory.pdf &");
  }

void MainWindow::Info2_1()
  {
   system("evince manual/Info_kernel_modules.pdf &");
  }

