//////////////////////////////////////////////////// FRAO 2016 ////////////////////////
// CHNet - INFN 
//
//            
//            



#include "../Header.h"
#include "mainwindow.h"
#include "../variables.h"
#include "../QT_variables.h"
#include <../Shm.h>


extern int Resolution_mode;

int i=0;int j=0;
long int iLine=0;

struct Pixel_BIG *Pointer; //puntatore da far puntare a PixelsMappa una volta creato

char process[30];


QString NowX="X= ",NowY="Y= ";
string checkX,checkY;
int onlyOne=0;
double V=1;
QString cmd_to_send="";

int n=0; 

int Clock=0, Clock2=0, ClockMotore=0;
bool okX=false;
bool aborted=false;
bool ask=false;
bool XConnected=false;
double valueY,valueX;
int interval=100;
double tempoPos=1000;
bool XOnTarget=false, YOnTarget=false, XHasMoved=true,YHasMoved=true; 
bool Xmoving=false, Ymoving=false;
bool TimerActive=false;

double positionX=100;

float Yo,vy,Xo,vx;
bool InitX=false; bool InitPhase=false;
int motor_selected; //per inizializzazione
bool motor_selction_valid=false;

QList<int> list_int({1,2,3,4,5,9,14,15,19,20,24,26,49,50,51,52,54});
QList<int> list_double({8,10,11,12,21,22,23,47,48});
QList<int> list_nav({6,7,13,16,17,18,25,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,53,55,56,57,58,59});
QList<int> list_other({60});

//////////////////////////////////////////////////////////////////////////////////////////

int cmd_to_motor=0; int spa_channel; double spa_value;
extern int *shm_cmd, *shmCommand_cmd, shmid_cmd, shmidString_cmd, *shared_memory_cmd; 
extern key_t key_cmd; extern key_t keyString_cmd;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
      key_cmd = 6900;
      shmid_cmd = shmget (key_cmd, SHMSZ_CMD_STATUS, IPC_CREAT | 0666);
      shared_memory_cmd =(int *) shmat(shmid_cmd, NULL, 0);
      cout<< "Data Memory attached at :" << (int *)shared_memory_cmd<< endl << " with shmid: " << shmid_cmd << endl;
      int pid=getpid();
      *(shared_memory_cmd+86)=pid;

      switch (Resolution_mode)
       {
        case(0): {PixelX=795; PixelY=795; break;}
        case(1): {PixelX=595; PixelY=595; break;}
        case(2): {PixelX=395; PixelY=395; break;}
       }


      GUI_CREATOR();


      X_goto=25000; //init for VT80-100

/////////////////////// TIMER for program control


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerEvent()));

//////////////////////////// time for scan (to be rewritten)
    timerPos = new QTimer(this);

 }


//////////////////////COMMAND CYCLE ///////////////////

void MainWindow::timerEvent()
{
ClockMotore++;
if (InitX) CheckXOnTarget();
}

////////////////////////////////////////// FRIEND FUNCTIONS
void MainWindow::SetCurrentAction(QString text)
{CurrentAction->setText(text);}


 void MainWindow::createStatusBar()
 {
     statusBar()->showMessage(tr("Ready"));
 }


 MainWindow::~MainWindow()
 {
   *(shared_memory_cmd+76)=0; // set status 0 for mainwindow.cpp
 }



///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////MOTORE.cpp/////////////////////////////////////////////////////////////

int invia_comando_X(int chan,const char *comando, const char *parametri)
{
 char canale=chan+'0';
 int r;
 string cm=comando;
 if(parametri==NULL) 
      cm=cm+'\n';
 else
     cm=cm+' '+canale+' '+parametri+'\n';
 if(write(serialX, cm.data(), cm.size()))  r=0;
 else {r=1; qDebug()<<"Errore in scrittura  "<< strerror(errno)<<'\n';}
 return r;
}



char *read_Xanswer()
{
//    printf("In attesa di risposta...\n");
  char c_1[100];                             /////////// sostituito c con c_1
  int n=0;
  string rest;
  while((n=read(serialX, &c_1, sizeof(c_1)))>0)      
    {
      c_1[n]=0;
      rest=rest+c_1;      
      if(c_1[n-1]=='\n')
	break;        
    }

//    qDebug()<<"RISPOSTA: "<< rest.data()<<'\n';////////////////////////////////////////////
  return c_1;  
}



string read_Xanswer2()
{
  char c[100];
  int n=0;
  string rest;
  string pippo;
  while((n=read(serialX, &c, sizeof(c)))>0)      
    {
      c[n]=0;
      rest=rest+c;      
      if(c[n-1]=='\n')
	break;        
    }

pippo =rest;
  return pippo;  
}


void MainWindow::CheckXOnTarget()
{
 string a;
 invia_comando_X(1,"ONT?",NULL); 

 a=read_Xanswer2();
 QString Qa=a.data();
 
//qDebug()<<"Qa: "<<Qa;

 if((Qa.contains("0", Qt::CaseInsensitive))==false) // also 1=0 can be used 
 {XOnTarget=true; Xmoving=false;
  if(InitPhase)
  {
   if(n==0){
           n=1; qDebug()<<"...Motor inited..."<<"\n";
           IniX=0; IniXready=1; 
           }
  }
 }

 else 
     {
      XOnTarget=false; Xmoving=true;}
      invia_comando_X(1,"POS?",NULL);           //scrive la posizione X sulla line edit
      checkX = read_Xanswer2();
      NowX="X= ";
      NowX.append(checkX.data());//qDebug()<<NowX;
      NowX.remove(3,2); 
      X_POSITION_lineEdit->setText(NowX);
     
 }



void MainWindow::Velocity(double number)
{
V=number;
tempoPos=Px/V;
qDebug()<<"velocità "<<V<<"mm/s"<<"Scrittura posizione ogni "<<tempoPos<<" ms";
char v[10];
sprintf(v,"%f",V);
invia_comando_X(1,"VEL",v);  
}


void MainWindow::StartX()
{
   if(XConnected)
    {if(!okX){okX=true; InizializzazioneX();} else qDebug() <<"...Init already running...";}
   else ANSWER_lineEdit->setText("... no active serial connection found...");
}  //ok è vero solo se l'inizializzazione è in corso


//////////////////////////////

void MainWindow::Stop()
{
if(InitX)
 {
 invia_comando_X(1,"HLT",NULL); 
 invia_comando_X(1,"ERR?",NULL);
 checkX = read_Xanswer2();
 }
}


void MainWindow::Exit()
{
*(shared_memory_cmd+76)=0; // set status 0 for mainwindow.cpp
qApp->quit();
}


void MainWindow::X_To(double valueX_to)
{X_goto=valueX_to*1000; qDebug()<<X_goto;}

void MainWindow::MoveX_To()
{

qDebug()<<"ce semo";
if(XOnTarget==true)
 { qDebug()<<"ce resemo";
  double destinationX=X_goto/1000;
  char sx[100];
  qDebug()<<"Moving to X= "<<destinationX<<'\n';
  sprintf(sx,"%f",destinationX);
  invia_comando_X(1,"MOV",sx);
  XOnTarget=false; 
  }
}

/*
void MainWindow::MoveX(double)  //// unit: micron
{
double destinationX=toX/1000;
char sx[100];
qDebug()<<"Spostamento a X= "<<destinationX<<'\n';
sprintf(sx,"%f",destinationX);
invia_comando_X(1,"MOV",sx);
}
*/


 void MainWindow::Show_PI_Parameters()
 {if(*(shared_memory_cmd+77)==0) system("./MotorParmetersTable & ");
 else qDebug()<< "Tables already opened...";}



 void MainWindow::SPA_Parameter(int input)
 {spa_channel=input;}


 void MainWindow::SPA_Parameter_value(double input)
 {spa_value=input;}



void MainWindow::Motor_selection(int motortype)
{
      switch (motortype)
       {
        case(0): {CurrentActionX->setText("no motor"); motor_selected=motortype; motor_selction_valid=false; break;}
        case(1): {CurrentActionX->setText("M404-2PD"); motor_selected=motortype; motor_selction_valid=true; break;}
        case(2): {CurrentActionX->setText("M404-4PD"); motor_selected=motortype; motor_selction_valid=true; break;}
        case(3): {CurrentActionX->setText("M404-8PD"); motor_selected=motortype; motor_selction_valid=true; break;}
        case(4): {CurrentActionX->setText("VT80-100"); motor_selected=motortype; motor_selction_valid=true; break;}
        case(5): {CurrentActionX->setText("VT80-150"); motor_selected=motortype; motor_selction_valid=true; break;}
        case(6): {CurrentActionX->setText("VT80-200"); motor_selected=motortype; motor_selction_valid=true; break;}
        case(7): {CurrentActionX->setText("VT75-600"); motor_selected=motortype; motor_selction_valid=true; break;}
       }
}


void MainWindow::Motor_CMD(int cmd_type)
{
      switch (cmd_type)
       {
        case(0): {CurrentActionX->setText("no cmd"); cmd_to_motor=0; break;}
        case(1): {CurrentActionX->setText("CST? (... asking for stage type...)"); cmd_to_motor=1; break;}
        case(2): {CurrentActionX->setText("CSV? (... get current syntax version...)"); cmd_to_motor=2; break;}
        case(3): {CurrentActionX->setText("DFH (... define home position...)"); cmd_to_motor=3; break;}
        case(4): {CurrentActionX->setText("DFH? (... get home position...)"); cmd_to_motor=4; break;}
        case(5): {CurrentActionX->setText("ERR? (... asking for errors...)"); cmd_to_motor=5; break;}
        case(6): {CurrentActionX->setText("FNL (... move to negative limit...)"); cmd_to_motor=6; break;}
        case(7): {CurrentActionX->setText("FPL (... move to positive limit...)"); cmd_to_motor=7; break;}
        case(8): {CurrentActionX->setText("FRF (... move to reference limit...)"); cmd_to_motor=8; break;}
        case(9): {CurrentActionX->setText("FRF? (... get reference limit...)"); cmd_to_motor=9; break;}
        case(10): {CurrentActionX->setText("GOH (... go home...)"); cmd_to_motor=10; break;}
        case(11): {CurrentActionX->setText("*IDN? (... asking for controller identity...)"); cmd_to_motor=11; break;}
        case(12): {CurrentActionX->setText("LIM? (... indicate limit switches...)"); cmd_to_motor=12; break;}
        case(13): {CurrentActionX->setText("ONT? (... asking if stage is OnTarget...)"); cmd_to_motor=13; break;}
        case(14): {CurrentActionX->setText("RBT (... REBOOT SYSTEM...!!!...)"); cmd_to_motor=14; break;}
        case(15): {CurrentActionX->setText("SPA (... setting stage parameters...)"); cmd_to_motor=15; break;}
        case(16): {CurrentActionX->setText("SPA? (... asking for stage parameters...)"); cmd_to_motor=16; break;}
        case(17): {CurrentActionX->setText("VER? (... get versions of firmware and drivers...)"); cmd_to_motor=17; break;}
       }
}


 void MainWindow::Send_CMD()
{
char sx[100];
if (InitX)
     { switch (cmd_to_motor)
       {
        case(0): {CurrentActionX->setText("no cmd selected"); break;} // tested with read_Xanswer();
        case(1): {invia_comando_X(1,"CST?",NULL);  
                  checkX = read_Xanswer2();
                  CurrentActionX->setText(checkX.data());break;}
        case(2): {invia_comando_X(1,"CSV?",NULL);  
                  checkX = read_Xanswer2();
                  CurrentActionX->setText(checkX.data());break;}
        case(3): {//invia_comando_X(1,"CST?",NULL);  
                  //checkX = read_Xanswer2();
                  //CurrentActionX->setText(checkX.data());break;}
                  CurrentActionX->setText("not implemented...");break;}
        case(4): {invia_comando_X(1,"DFH?",NULL);  
                  checkX = read_Xanswer2();
                  CurrentActionX->setText(checkX.data());break;}
        case(5): {invia_comando_X(1,"ERR?",NULL);
                  checkX = read_Xanswer2();
                  CurrentActionX->setText(checkX.data());break;}
        case(6): {invia_comando_X(1,"FNL",NULL); break;}
        case(7): {invia_comando_X(1,"FPL",NULL); break;}
        case(8): {invia_comando_X(1,"FRF",NULL); break;}
        case(9): {invia_comando_X(1,"FRF?",NULL);
                  checkX = read_Xanswer2();
                  CurrentActionX->setText(checkX.data());break;}
        case(10): {invia_comando_X(1,"GOH",NULL); break;}
        case(11): {invia_comando_X(1,"*IDN?",NULL);
                  checkX = read_Xanswer2();
                  CurrentActionX->setText(checkX.data());break;}
        case(12): {invia_comando_X(1,"LIM?",NULL);
                  checkX = read_Xanswer2();
                  CurrentActionX->setText(checkX.data());break;}
        case(13): {invia_comando_X(1,"ONT?",NULL); 
                  checkX = read_Xanswer2();
                  CurrentActionX->setText(checkX.data());break;}
        case(14): {//invia_comando_X(1,"RBT",NULL); 
                  //checkX = read_Xanswer2();
                  //CurrentActionX->setText(checkX.data());break;}
                  CurrentActionX->setText("not implemented...");break;}
        case(15): {if(list_int.contains(spa_channel))
                    {sprintf(sx," %i, %i",spa_channel, spa_value);
                     invia_comando_X(1,"SPA", sx); break;}
                  if(list_double.contains(spa_channel))
                    {sprintf(sx," %i, %f",spa_channel, spa_value);
                     invia_comando_X(1,"SPA", sx); break;} 
                  if(list_nav.contains(spa_channel))
                    {CurrentActionX->setText("...no valid SPA parameters...)"); break;} 
                 }     
//invia_comando_X("SPA (... setting stage parameters...)"); cmd_to_motor=8; break;}
//        case(16): {invia_comando_X("SPA? (... asking for stage parameters...)"); cmd_to_motor=9; break;}
        case(17): {invia_comando_X(1,"VER?",NULL); 
                  checkX = read_Xanswer2();
                  CurrentActionX->setText(checkX.data());break;}


       }
     }


 }










