// Frao 31/12/2016
//
// TTY_motors.cpp manages the calls from spinBox_assignX,  pushButton_assignX_port.


#include "mainwindow.h"
#include <../Header.h>
//#include <../Shm.h>
//#include "../variables.h"

/////////variabili e funzioni definite altrove ma necessarie/////////////////////////


extern int portX, IniX, IniXready, serialX;
extern int SerialiStatus;
extern int invia_comando_X(int chan,const char *comando, const char *parametri);
extern string read_Xanswer2();
extern bool XConnected; 

//////////////////////////////////////////////////////////////////////

QString TTY_X="/dev/"; QString TTY_Y="/dev/"; QString TTY_Z="/dev/"; QString TTY_ACM="/dev/";

int RispPortX=2, RispPortY=2; int RispPortZ=2;

int Sx=0; int Sy=1; int Sz=2;

//bool go_x=true; bool go_y=true; bool go_z=true;

int i_X1=0, i_Y1=0, i_Z1=0; 
int i_X2=0, i_Y2=0, i_Z2=0; 

//////////////// SPINBOXES

void MainWindow::NameX(int numberx)     //////// CALLED FROM --> spinBox_assignX (in mainwindow.cpp)
{Sx=numberx;}

/////////////// PUSHBUTTONS

void MainWindow::AssignX()               //////// CALLED FROM --> pushButton_assignX_port (in mainwindow.cpp)
{
i_X1=0; i_X2=0;
if(!XConnected)
{QString NameTTY_X=SetSerialXName(Sx);} ///// CALL FOR THE X PORT CONNECTION
else
qDebug()<<"Connection X already established!!!";
}


QString MainWindow::SetSerialXName(int number)
{
QString NameNumber; QString commentoX="X"; QString commentoX_long="";

const char *MYTTY_X;
     TTY_X="/dev/";

portX=number;

NameNumber.setNum(number);
TTY_X.append(NameNumber); 

 qDebug()<<TTY_X;

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
   int h=cfsetospeed(&my_termios,B9600);
   int h1=cfgetospeed(&my_termios);

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
   invia_comando_X(1,"*IDN?",NULL);
   Nfd=serialX+1;
   retval=select(Nfd, &rfds, NULL,NULL, &tv);
   string vediamo=read_Xanswer2();

   if (retval==0)   RispPortX=0;
   else RispPortX=1;
  }

///////////////////////////////////////////// RETURNING X-PORT CONNECTION RESULT AND WRITING COMMENTS

if(RispPortX==1 && i_X2<4){i_X2++; commentoX_long.append("TTY X: motor connected");if (i_X2==1) {XConnected=true;/*Enabling_Tabwidget();*/}}//Enabling_Tabwidget() enables the tab widgets for movements and scan
if(RispPortX==0 && i_X2<4){i_X2++; commentoX_long.append("TTY X: mototype cmdr NOT connected!");}
if(RispPortX==3 && i_X2<4){i_X2++; commentoX_long.append("TTY X: NO ACCESS to X CONTROLLER!!!");}
if(RispPortX==6 && i_X2<4){i_X2++; commentoX_long.append(" ");}


if(IniXready==1 && i_X1<4) {i_X1++; commentoX_long.append("TTY: X-motor initialised!");}

else {commentoX_long.append("TTY X-motor: wait while initialising...");}

CurrentActionX->setText(commentoX);
//CurrentActionX->setText(commentoX_long);

return TTY_X;
}







