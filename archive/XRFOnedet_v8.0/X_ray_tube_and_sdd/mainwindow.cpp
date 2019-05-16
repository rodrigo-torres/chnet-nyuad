#include <QtGui>
#include "mainwindow.h"
#include "../Header.h"      // file included from main program folder... Header.h contains all the variables for shm (TBD better)
//#include "../variables.h"
//#include "../Shm.h"

int Sacm=0;                   int serialACM;                 int portACM;          int timestop=8;
int XRAY_voltage_set_to=4;    int XRAY_current_set_to=10;    int xrf_wait=0;
bool arduino_inited=false;    bool XrfPower=false;           bool sddPower=false;  bool busy=false;    bool Set_XRAY_V=false;    bool Set_XRAY_C=false;
bool command=false;           bool XRAY_OFF=false;
char HV_Set[6];               char HV_Set_C[6];

QString TTY_ACM="/dev/";


MainWindow::MainWindow()
{

    GuiCreator();


//    timer = new QTimer(this);                                                // TIMER for program control
//    connect(timer, SIGNAL(timeout()), this, SLOT(timerEvent()));

    timer_XRF = new QTimer(this);                                                // TIMER for program control
    connect(timer_XRF, SIGNAL(timeout()), this, SLOT(timerXRF()));

    timer_Reading = new QTimer(this);                                                // TIMER for program control
    connect(timer_Reading, SIGNAL(timeout()), this, SLOT(MainLoop()));


}


MainWindow::~MainWindow()
{

}


void MainWindow::Exit()
{
qApp->quit();
}





/////////// TTYACM -> link to arduino

void MainWindow::NameACM(int numberacm)     //////// CALLED BY --> spinBox_assignACM (in mainwindow.cpp)
{Sacm=numberacm; qDebug()<<Sacm;}

void MainWindow::Arduino_serial_init()
{
  QString commentoACM=""; QString commentoACM_long=""; 
  bool ACM_selection_is_good=false;
  const char *MYTTY_ACM;
  TTY_ACM="/dev/";

  switch (Sacm)
   {
    case(1): {MYTTY_ACM="/dev/ttyACM0"; commentoACM.append("ttyACM0 connected"); ACM_selection_is_good=true; break;}

    case(2): {MYTTY_ACM="/dev/ttyACM1"; commentoACM.append("ttyACM1 connected"); ACM_selection_is_good=true; break;}

    case(3): {MYTTY_ACM="/dev/ttyACM2"; commentoACM.append("ttyACM2 connected"); ACM_selection_is_good=true; break;}
    }


  errno=0; //RispPortZ=2; i_Z2=0;

  qDebug()<< MYTTY_ACM <<'\n';

 if(ACM_selection_is_good)
  {
  if (!arduino_inited)
   {
    errno=0;
    serialACM=open(MYTTY_ACM,O_RDWR);  
    if(serialACM<0)
     {
      qDebug()<<"ERROR opening ACM port"<< MYTTY_ACM<< strerror(errno);
      return;
//      exit(-1);
     }
  struct termios my_termios;
  struct termios new_termios;

  tcgetattr( serialACM, &my_termios );
  int h=cfsetospeed(&my_termios,B57600);                                        ///////////////////////// SPEED /////////////////////////////////
  my_termios.c_oflag &= (unsigned short)(~(ONLCR | OPOST));
  my_termios.c_cflag |= CLOCAL;
  my_termios.c_lflag &= (unsigned short)(~(ICANON | ECHO | ISIG));  
  my_termios.c_cc[VMIN] = 1;
  tcsetattr( serialACM, TCSANOW, &my_termios );
  tcgetattr( serialACM, &new_termios );

  arduino_inited=true;

//tab_4->setEnabled(true);
   } 
 }
    timer_Reading->start(1000);
}

string read_ACManswer()                                                     // X MOTOR: READ ANSWER STRING
{
  char c[10];
  int n=0;
  string rest;
  string Xread;
  while((n=read(serialACM, &c, sizeof(c)))>0)      
    {
      c[n]=0;
      rest=rest+c;      
      if(c[n-1]=='\n')
	break;        
    }
  Xread =rest;
//  QString qstr = QString::fromStdString(Xread);
//      qDebug()<<qstr;
  return Xread;  
}





int invia_comando(const char *comando, const char *settings_type)   // X MOTOR: SEND COMMAND
  {
   int r;
   string cm=comando;
//   if(settings_type=="A")
//     {cm=settings_type+'\n';}
//   else
     {cm=settings_type+cm+'\n';}
      //QString qstr = QString::fromStdString(cm);
      //qDebug()<<qstr;
     
   if(write(serialACM, cm.data(), cm.size()))  r=0;
   else {r=1; qDebug()<<"Errore in scrittura  "<< strerror(errno)<<'\n';}
   return r;
  }


void MainWindow::ReadAnalogValue()
{
   invia_comando("100", "A");//qDebug()<<"CMD A sent";
   string checkZ = read_ACManswer();
   QString NowZ;
   NowZ.append(checkZ.data()); //qDebug()<<"Voltage read: "<<NowZ;
//   NowZ.remove(0,10);
   double HV_value=(NowZ.toDouble())*50/1023;
   QString valueAsString = QString::number(HV_value);
   valueAsString.truncate(4);
   linedit_Xray_V->setText(valueAsString);
   //qDebug()<<valueAsString;
   invia_comando("100", "B");//qDebug()<<"CMD B sent";
   string checkC = read_ACManswer();
   QString NowC;
   NowC.append(checkC.data());//qDebug()<<"Current read: "<<NowC;
//   NowC.remove(0,10);
   double C_value=(NowC.toDouble())*125/1023;
   QString valueC = QString::number(C_value);
   valueC.truncate(4);
   linedit_Xray_I->setText(valueC);  
}




void MainWindow::MainLoop()
{
   if (XRAY_OFF)
   {invia_comando(HV_Set, "V");usleep(200000);invia_comando(HV_Set_C, "C");XRAY_OFF=false;}
   if(Set_XRAY_V && XrfPower)
   {invia_comando(HV_Set, "V");Set_XRAY_V=false;}
   if(Set_XRAY_C && XrfPower)
   {invia_comando(HV_Set_C, "C");Set_XRAY_C=false;} 

   ReadAnalogValue();
}


void MainWindow::Sdd_power()
{if (!sddPower)
{qDebug()<<"SDD Power ON"; linedit_SDD_HV_status->setText("ON"); sddPower=true;}
else
{qDebug()<<"SDD Power OFF"; linedit_SDD_HV_status->setText("OFF"); sddPower=false;}
}


///// ARDUINO DAQ WORKS WITH 255 BITS: 0=0.0V, 255=5.0V
///// MOXTEK INPUT DRIVING LOW VOLTAGE:   0V=0kV, 4.0V=40.0kV   (USABLE BIT 0-204)
///// MOXTEK INPUT DRIVING LOW CURRENT:   0V=0uA, 4.0V=100.0uA  

void MainWindow::Xray_voltage(double xray_v)
{
 {
  XRAY_voltage_set_to=qRound((xray_v/40)*255);
  qDebug()<<"XRAY voltage set to: "<< xray_v<<" -> Arduino DAC_V set to: "<< XRAY_voltage_set_to;
  char HV_tmp[6];
//  char HV_Set[6];
//  sprintf(HV_Set,"%i",XRAY_voltage_set_to);
  sprintf(HV_tmp,"%i",XRAY_voltage_set_to);
  for (int i=0; i<=6; i++) HV_Set[i]=HV_tmp[i];
  Set_XRAY_V=true;
//  invia_comando(HV_Set, "V"); 
 }
}


void MainWindow::Xray_current(double xray_c)
{
XRAY_current_set_to=qRound((xray_c/100)*255);
char ctemp[6];
sprintf(ctemp,"%i",XRAY_current_set_to);
for (int i=0; i<=6; i++) HV_Set_C[i]=ctemp[i];
Set_XRAY_C=true;
}


void MainWindow::Xray_power()
{if (!XrfPower)
{qDebug()<<"XRAY Power ON"; linedit_XRAY_HV_status->setText("ON"); XrfPower=true; Set_XRAY_V=true; Set_XRAY_C=true;}
else
{qDebug()<<"XRAY Power OFF"; timerXRF();
for (int i=0; i<=6; i++) {HV_Set_C[i]=0; HV_Set[i]=0;} XRAY_OFF=true; XrfPower=false;
tab3->setEnabled(false); timer_XRF->start(1000);
}
}


void MainWindow::timerXRF()
{
xrf_wait=xrf_wait+1;
QString text; text="WAIT "; int timeleft=timestop-xrf_wait; 
QString valueAsString = QString::number(timeleft);
text.append(valueAsString); linedit_XRAY_HV_status->setText(text);
if(xrf_wait>=timestop) 
  {tab3->setEnabled(true); linedit_XRAY_HV_status->setText("OFF");
  xrf_wait=0; timer_XRF->stop();
  }
}


/////// WRITING & READDING TO/FROM ARDUINO

//void MainWindow::ArduinoWrite_to()










/*
int invia_comando(int chan,const char *comando, const char *parametri)   // X MOTOR: SEND COMMAND
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
*/



/*

int invia_comando(const char *comando, const char *settings_type)   // X MOTOR: SEND COMMAND
  {

   timer_Reading->stop();
   int r;
   string cm=comando;
   if(settings_type==NULL) 
     cm=cm+'\n';
   else
     {cm=settings_type+cm+'\n';
      QString qstr = QString::fromStdString(cm);
      qDebug()<<qstr;
     }
   if(write(serialACM, cm.data(), cm.size()))  r=0;
   else {r=1; qDebug()<<"Errore in scrittura  "<< strerror(errno)<<'\n';}
   return r;
   timer_Reading->start(1000);
  }

*/





