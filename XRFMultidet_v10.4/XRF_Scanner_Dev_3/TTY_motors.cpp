// Frao 31/1/2016
//
// TTY_motors.cpp manages the calls from spinBox_assignX, spinBox_assignY, pushButton_assignX_port, pushButton_assignY_port.
// no more timers for serial assignment. Sx, Sy and Sz are the storage variable for the serial number and are called from the spinboxes. 
// removed the shared memory call (ShmIniXStatusRead() and ShmReadyXStatusRead()). 
//
// PROGRAM FLOW: 
// 1 - collecting port numbers from spinboxes
// 2 - testing and trying connections for TTYs
// 3 - enabling depending widgets (move and scan)
//
// THREE IDENTICAL MODULES ARE PRESENT IN THIS FILE FOR THE MOTORS (X-Y-Z). THE LAST MODULE IS FOR ARDUINO (TTYACM)
//


#include "mainwindow.h"
#include <../Header.h>
//#include <../Shm.h>
//#include "../variables.h"

/////////variabili e funzioni definite altrove ma necessarie/////////////////////////


extern int portX, IniX, IniXready, serialX;
extern int portY, IniY, IniYready, serialY;
extern int portZ, IniZ, IniZready, serialZ;


extern int SerialiStatus;

extern int serialX,serialY,serialZ;
extern int send_command(int chan,const char *comando, const char *parametri, int port);
extern string read_answer(int port);
extern string read_Yanswer2();
extern string read_Zanswer2();

extern bool XConnected; extern bool YConnected; extern bool ZConnected;

//////////////////////////////////////////////////////////////////////

QString TTY_X="/dev/"; QString TTY_Y="/dev/"; QString TTY_Z="/dev/"; QString TTY_ACM="/dev/";

int RispPortX=2, RispPortY=2; int RispPortZ=2;

int Sx=1; int Sy=0; int Sz=2;

//bool go_x=true; bool go_y=true; bool go_z=true;

int i_X1=0, i_Y1=0, i_Z1=0; 
int i_X2=0, i_Y2=0, i_Z2=0; 

//////////////// SPINBOXES

void MainWindow::NameX(int numberx)     //////// CALLED FROM --> spinBox_assignX (in mainwindow.cpp)
{Sx=numberx;}
void MainWindow::NameY(int numbery)     //////// CALLED FROM --> spinBox_assignY (in mainwindow.cpp)
{Sy=numbery;}
void MainWindow::NameZ(int numberz)     //////// CALLED FROM --> spinBox_assignZ (in mainwindow.cpp)
{Sz=numberz; }


/////////////// PUSHBUTTONS

void MainWindow::AssignX()               //////// CALLED FROM --> pushButton_assignX_port (in mainwindow.cpp)
{
i_X1=0; i_X2=0;
if(!XConnected)
{QString NameTTY_X=SetSerialXName(Sx);} ///// CALL FOR THE X PORT CONNECTION
else
qDebug()<<"Connection X already established!!!";
}


void MainWindow::AssignY()               //////// CALLED FROM --> pushButton_assignY_port (in mainwindow.cpp)
{
i_Y1=0; i_Y2=0;
if(!YConnected)
{QString NameTTY_Y=SetSerialYName(Sy);} 
else
qDebug()<<"Connection Y already established!!!";
}



void MainWindow::AssignZ()
{
i_Z1=0; i_Z2=0;
if(!ZConnected) QString NameTTY_Z=SetSerialZName(Sz);
else qDebug()<<"Connection Z already established!!!";
return;
}




QString MainWindow::SetSerialXName(int number)
{
QString NameNumber; QString commentoX="X"; QString commentoX_long="";

const char *MYTTY_X;
     TTY_X="/dev/";

portX=number;

NameNumber.setNum(number);
TTY_X.append(NameNumber); 

if(portX==0)  {MYTTY_X="/dev/ttyUSB0"; commentoX.append(" on ttyUSB0");}
else          
if(portX==1)  {MYTTY_X="/dev/ttyUSB1"; commentoX.append(" on ttyUSB1");}  
else
if(portX==2)  {MYTTY_X="/dev/ttyUSB2"; commentoX.append(" on ttyUSB2");}
else
if(portX==3)  {MYTTY_X="/dev/ttyUSB3"; commentoX.append(" on ttyUSB3");}

  errno=0; RispPortX=2; i_X2=0;

qDebug()<< MYTTY_X <<'\n';

  serialX=open(MYTTY_X,O_RDWR,0677);
  qDebug()<<"This is the value for serialX: "<<serialX;
 
  if(serialX<0)
    {
     qDebug()<< "ERROR opening"<< MYTTY_X<< strerror(errno)<<'\n';
      RispPortX=3;
     }

else   /////////////// START CONNECTION WITH FILE DESCRIPTOR
{
 fd_set rfds;
 struct timeval tv;
 int retval,Nfd;

  struct termios my_termios;
  struct termios new_termios;

  tcgetattr( serialX, &my_termios );
  cfsetospeed(&my_termios,B9600);

  my_termios.c_oflag &= (unsigned short)(~(ONLCR | OPOST));
  my_termios.c_cflag |= CLOCAL;
  my_termios.c_lflag &= (unsigned short)(~(ICANON | ECHO | ISIG));  
  my_termios.c_cc[VMIN] = 1;

  tcsetattr( serialX, TCSANOW, &my_termios );
  tcgetattr( serialX, &new_termios );

  FD_ZERO(&rfds);
  FD_SET(serialX, &rfds);
  tv.tv_sec=3;
  tv.tv_usec=0;
  send_command(1,"*IDN?",NULL,serialX);
  Nfd=serialX+1;
  retval=select(Nfd, &rfds, NULL,NULL, &tv);
  read_answer(serialX);


  if (retval==0)   RispPortX=0;
  else RispPortX=1;
 }

///////////////////////////////////////////// RETURNING X-PORT CONNECTION RESULT AND WRITING COMMENTS

if(RispPortX==1 && i_X2<4){i_X2++; commentoX_long.append("TTY X: motor connected");if (i_X2==1) {XConnected=true;Enabling_Tabwidget();}}//Enabling_Tabwidget() enables the tab widgets for movements and scan
if(RispPortX==0 && i_X2<4){i_X2++; commentoX_long.append("TTY X: motor NOT connected!");}
if(RispPortX==3 && i_X2<4){i_X2++; commentoX_long.append("TTY X: NO ACCESS to X CONTROLLER!!!");}
if(RispPortX==6 && i_X2<4){i_X2++; commentoX_long.append(" ");}


if(IniXready==1 && i_X1<4) {i_X1++; commentoX_long.append("TTY: X-motor initialised!");}

else {commentoX_long.append("TTY X-motor: wait while initialising...");}

CurrentActionX->setText(commentoX);
CurrentAction->setText(commentoX_long);


return TTY_X;
}

////////////////////////////////////////////////////////////// 2 - SAME FOR Y MOTOR

QString MainWindow::SetSerialYName(int number)
{
QString NameNumber;  QString commentoY="Y"; QString commentoY_long="";
const char *MYTTY_Y;
     TTY_Y="/dev/";
    
portY=number;

NameNumber.setNum(number);
TTY_Y.append(NameNumber); 


if(portY==0)  {MYTTY_Y="/dev/ttyUSB0"; commentoY.append(" on ttyUSB0");}
else          
if(portY==1)  {MYTTY_Y="/dev/ttyUSB1"; commentoY.append(" on ttyUSB1");}
else
if(portY==2)  {MYTTY_Y="/dev/ttyUSB2"; commentoY.append(" on ttyUSB2");}
else
if(portY==3)  {MYTTY_Y="/dev/ttyUSB3"; commentoY.append(" on ttyUSB3");}

  errno=0; RispPortY=2; i_Y2=0;

qDebug()<< MYTTY_Y <<'\n';

  serialY=open(MYTTY_Y,O_RDWR,0677);
  qDebug()<<"This is the value for serialY: "<<serialY;

  if(serialY<0)
    {
     qDebug()<< "ERROR opening"<< MYTTY_Y<< strerror(errno)<<'\n';
      RispPortY=3;
     
    }

else   /////////////// START Y-CONNECTION WITH FILE DESCRIPTOR
 {
  fd_set rfds;
  struct timeval tv;
  int retval,Nfd;
  struct termios my_termios;
  struct termios new_termios;

  tcgetattr( serialY, &my_termios );
  cfsetospeed(&my_termios,B9600);
  //int h=cfsetospeed(&my_termios,B9600);
  //int h1=cfgetospeed(&my_termios);
  my_termios.c_oflag &= (unsigned short)(~(ONLCR | OPOST));
  my_termios.c_cflag |= CLOCAL;
  my_termios.c_lflag &= (unsigned short)(~(ICANON | ECHO | ISIG));  
  my_termios.c_cc[VMIN] = 1;
  tcsetattr( serialY, TCSANOW, &my_termios );
  tcgetattr( serialY, &new_termios );

  FD_ZERO(&rfds);
  FD_SET(serialY, &rfds);
  tv.tv_sec=3;
  tv.tv_usec=0;

  send_command(1,"*IDN?",NULL,serialY);
  Nfd=serialY+1;
  retval=select(Nfd, &rfds, NULL,NULL, &tv);
  read_Yanswer2();
  if (retval==0)   RispPortY=0;
  else RispPortY=1;
}

///////////////////////////////////////////// RETURNING Y-PORT CONNECTION RESULT AND WRITING COMMENTS

if(RispPortY==1 && i_Y2<4){i_Y2++; commentoY_long.append("TTY Y: motor connected");if (i_Y2==1) {YConnected=true; Enabling_Tabwidget();}} //Enabling_Tabwidget() enables the tab widgets for movements and scan
if(RispPortY==0 && i_Y2<4){i_Y2++; commentoY_long.append("TTY Y: motor NOT connected!");}
if(RispPortY==3 && i_Y2<4){i_Y2++; commentoY_long.append("TTY Y: NO ACCESS to Y CONTROLLER!!!");}
if(RispPortY==6 && i_Y2<4){i_Y2++; commentoY_long.append(" ");}

if(IniYready==1 && i_Y1<4) {i_Y1++; commentoY_long.append("TTY: Y-motor initialised!");}
else {commentoY_long.append("TTY Y-motor: wait while initialising...");}

CurrentActionY->setText(commentoY);
CurrentAction->setText(commentoY_long);

return TTY_Y;
}

////////////////////////////////////////////////////////////// 3 - SAME FOR Z MOTOR

QString MainWindow::SetSerialZName(int number)
{
QString commentoZ="Z"; QString commentoZ_long=""; QString NameNumber;
const char *MYTTY_Z;
     TTY_Z="/dev/";
    
portZ=number;

NameNumber.setNum(number);
TTY_Z.append(NameNumber); 

if(portZ==0)  {MYTTY_Z="/dev/ttyUSB0"; commentoZ.append(" on ttyUSB0");}
else          
if(portZ==1)  {MYTTY_Z="/dev/ttyUSB1"; commentoZ.append(" on ttyUSB1");}
else
if(portZ==2)  {MYTTY_Z="/dev/ttyUSB2"; commentoZ.append(" on ttyUSB2");}
else
if(portZ==3)  {MYTTY_Z="/dev/ttyUSB3"; commentoZ.append(" on ttyUSB3");}

  errno=0; RispPortZ=2; i_Z2=0;

qDebug()<< MYTTY_Z <<'\n';

  serialZ=open(MYTTY_Z,O_RDWR);
  qDebug()<<"This is the serialZ value: "<<serialZ;
  if(serialZ<0)
    {
     qDebug()<< "ERROR opening"<< MYTTY_Z<< strerror(errno)<<'\n';
      RispPortZ=3;
     }

else   /////////////// START Z-CONNECTION WITH FILE DESCRIPTOR
 {
  fd_set rfds;
  struct timeval tv;
  int retval,Nfd;
  struct termios my_termios;
  struct termios new_termios;
  tcgetattr( serialZ, &my_termios );
  //int h=cfsetospeed(&my_termios,B9600);
  cfsetospeed(&my_termios,B9600);
  //int h1=cfgetospeed(&my_termios);
  my_termios.c_oflag &= (unsigned short)(~(ONLCR | OPOST));
  my_termios.c_cflag |= CLOCAL;
  my_termios.c_lflag &= (unsigned short)(~(ICANON | ECHO | ISIG));  
  my_termios.c_cc[VMIN] = 1;
  tcsetattr( serialZ, TCSANOW, &my_termios );
  tcgetattr( serialZ, &new_termios );

  FD_ZERO(&rfds);
  FD_SET(serialZ, &rfds);
  tv.tv_sec=3;
  tv.tv_usec=0;
  send_command(1,"*IDN?",NULL,serialZ);
  Nfd=serialZ+1;
  retval=select(Nfd, &rfds, NULL,NULL, &tv);
  //string vediamo=read_Zanswer2();
  read_Zanswer2();

   if (retval==0)   RispPortZ=0;
   else RispPortZ=1;

   INIT_Z_pushButton->setEnabled(true); STOP_Z_INIT_pushButton->setEnabled(true);
   }

///////////////////////////////////////////// RETURNING Z-PORT CONNECTION RESULT AND WRITING COMMENTS

if(RispPortZ==1 && i_Z2<4){i_Z2++; commentoZ_long.append("TTY Z: motor connected");if (i_Z2==1)ZConnected=true; Enabling_Tabwidget();}
if(RispPortZ==0 && i_Z2<4){i_Z2++; commentoZ_long.append("TTY Z: motor NOT connected!");}
if(RispPortZ==3 && i_Z2<4){i_Z2++; commentoZ_long.append("TTY Z: NO ACCESS to Z CONTROLLER!!!");}
if(RispPortZ==6 && i_Z2<4){i_Z2++; commentoZ_long.append(" ");}

if(IniZready==1 && i_Z1<4) {i_Z1++; commentoZ_long.append("TTY: Z-motor initialised!");}
else {commentoZ_long.append(" TTY Z-motor: wait while initialising...");}

CurrentActionZ->setText(commentoZ);
CurrentAction->setText(commentoZ_long);

return TTY_Z;
}


void MainWindow::Enabling_Tabwidget()
{
//if (XConnected || YConnected) {tab2_2 ->setEnabled(true);}
if (IniXready || IniYready) {tab2_3 ->setEnabled(true); tab2_4 ->setEnabled(true); }
if (ZConnected) {INIT_Z_pushButton->setEnabled(true); STOP_Z_INIT_pushButton->setEnabled(true);}
}






///////////////////////////////////// ARDUINO /////////////////////////////////////////////////////////
int Sacm;
extern int serialK, portACM;
extern bool noKeyence_init;

void MainWindow::NameACM(int numberacm)     //////// CALLED FROM --> spinBox_assignACM (in mainwindow.cpp)
{Sacm=numberacm;}

/////////////// PUSHBUTTONS

void MainWindow::AssignACM()               //////// CALLED FROM --> pushButton_assignACM_port (in mainwindow.cpp)
{
if(noKeyence_init)
Init_KeyenceLaser();
else
qDebug()<<"Connection ACM already established!!!";
}


void MainWindow::Init_KeyenceLaser() {

    QString commentoACM="Arduino";
    QString NameNumber;

const char *MYTTY_ACM;
     TTY_ACM="/dev/";
    
portACM=Sacm;

qDebug()<<"Arduino port is: "<< portACM <<'\n';

NameNumber.setNum(Sacm);
TTY_ACM.append(NameNumber); 
    
if(portACM==0)  {MYTTY_ACM="/dev/ttyACM0"; commentoACM.append(" on ttyACM0");}
else          
if(portACM==1)  {MYTTY_ACM="/dev/ttyACM1"; commentoACM.append(" on ttyACM1");}
else
if(portACM==2)  {MYTTY_ACM="/dev/ttyACM2"; commentoACM.append(" on ttyACM2");}
else
if(portACM==3)  {MYTTY_ACM="/dev/ttyACM3"; commentoACM.append(" on ttyACM3");}

  errno=0; //RispPortZ=2; i_Z2=0;

qDebug()<<"The communication bus is: "<<MYTTY_ACM<<'\n';


if (noKeyence_init)
{
  errno=0;


  serialK=open(MYTTY_ACM,O_RDWR);  
  if(serialK<0)
    {
      qDebug()<<"ERROR opening ACM port"<< MYTTY_ACM<< strerror(errno);
//      exit(-1);
    }
  else{
  struct termios my_termios;
  struct termios new_termios;

  tcgetattr( serialK, &my_termios );
  cfsetospeed(&my_termios,B9600);
  my_termios.c_oflag &= (unsigned short)(~(ONLCR | OPOST));
  my_termios.c_cflag |= CLOCAL;
  my_termios.c_lflag &= (unsigned short)(~(ICANON | ECHO | ISIG));  
  my_termios.c_cc[VMIN] = 1;
  tcsetattr( serialK, TCSANOW, &my_termios );
  tcgetattr( serialK, &new_termios );



noKeyence_init=false;
CurrentActionACM->setText(commentoACM);
AUTOFOCUS_ON_pushButton->setEnabled(true);
} 

}
}











