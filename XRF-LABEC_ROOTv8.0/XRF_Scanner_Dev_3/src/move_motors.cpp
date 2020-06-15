// Frao 31/1/2016
//
// move_motors.cpp manages the calls from pushbutton and spinboxes of tabwidget....  
// 
// 
// 
// THREE IDENTICAL MODULES ARE PRESENT IN THIS FILE TO MOVE THE MOTORS (X-Y-Z). 
//


#include "include/MAXRF/mainwindow.h"
#include "../Header.h"
#include <../Shm.h>

extern double X_goto, Y_goto, Z_goto, Px, Py, Pz;
extern bool XOnTarget,YOntarget, ZOnTarget;
extern int IniXready, IniYready, IniZready;
extern int *shared_memory, *shared_memory_cmd;
extern bool XYscanning, XOnTarget, YOnTarget, ZOnTarget;

extern int invia_comando_X(int chan,const char *comando, const char *parametri);
extern int invia_comando_Y(int chan,const char *comando, const char *parametri);
extern int invia_comando_Z(int chan,const char *comando, const char *parametri);
extern string read_Xanswer2();
extern string read_Yanswer2();
extern string read_Zanswer2();
extern int accelerationtime;


///////////////////////////////////// X MOTORS

void MainWindow::MoveX(double toX)  //// unit: micron
{
double destinationX=toX/1000;
char sx[100];
qDebug()<<"Spostamento a X= "<<destinationX<<'\n';
sprintf(sx,"%f",destinationX);
invia_comando_X(1,"MOV",sx);

XOnTarget=false; //// NEW

}

void MainWindow::MoveX_To()
{
double value=X_goto;
if(XOnTarget==true)
 {
  MoveX(value);
  XOnTarget=false; 
  }
}

void MainWindow::Move_right()
{
string posX;
QString current_posX;
double value;

if(XOnTarget==true)
{
invia_comando_X(1,"POS?",NULL);
posX = read_Xanswer2();
current_posX =posX.data();
current_posX.remove(0,2);
value=current_posX.toDouble();
value=(value - Px/1000)*1000;
MoveX(value);
XOnTarget=false;
}
}

void MainWindow::Move_left()
{
string posX;
QString current_posX;
double value;


if(XOnTarget==true)
{
invia_comando_X(1,"POS?",NULL);
posX = read_Xanswer2();
current_posX =posX.data();
current_posX.remove(0,2);
value=current_posX.toDouble();
value=(value + Px/1000)*1000;
MoveX(value);
XOnTarget=false;
}
}

           
///////////////////////////////////// Y MOTORS


void MainWindow::MoveY(double toY)
{
double destinationY=toY/1000;
char sy[100];
qDebug()<<"Spostamento a Y= "<<destinationY<<'\n';
sprintf(sy,"%f",destinationY);
invia_comando_Y(1,"MOV",sy);
YOnTarget=false; ///New
}

void MainWindow::MoveY_To()
{
 double value=Y_goto;
if(YOnTarget==true)
 {
  MoveY(value);	
  YOnTarget=false; 
  }
}


void MainWindow::Move_down()
{
string posY;
QString current_posY;
double valueY;

if(YOnTarget==true)
{
invia_comando_Y(1,"POS?",NULL);
posY = read_Yanswer2();
current_posY =posY.data();
current_posY.remove(0,2);
valueY=current_posY.toDouble();
valueY=(valueY - Py/1000)*1000;

MoveY(valueY);
YOnTarget=false;
}
}

void MainWindow::Move_up()
{
string posY;
QString current_posY;
double valueY;


if(YOnTarget==true)
{
invia_comando_Y(1,"POS?",NULL);
posY = read_Yanswer2();
current_posY =posY.data();
current_posY.remove(0,2);
valueY=current_posY.toDouble();
valueY=(valueY + Py/1000)*1000;
MoveY(valueY);
YOnTarget=false;
}
}


///////////////////////////////////// Z MOTORS




void MainWindow::MoveZ(double toZ)  
{
double destinationZ=toZ/1000;
char sxz[100];
qDebug()<<"Moving to Z= "<<destinationZ<<'\n';
sprintf(sxz,"%f",destinationZ);
invia_comando_Z(1,"MOV",sxz);
ZOnTarget=false; 
}

void MainWindow::MoveZ_To()
{
//char v[10]; sprintf(v,"%f",2.5); invia_comando_Z(1,"VEL",v); //inserito

 double value=Z_goto;
if(ZOnTarget==true)
 {
  MoveZ(value);
  ZOnTarget=false; 
  }
}

void MainWindow::Move_backward()
{
//char v[10]; sprintf(v,"%f",2.5); invia_comando_Z(1,"VEL",v); //inserito

string posZ;
QString current_posZ;
double value;

if(ZOnTarget==true)
{
invia_comando_Z(1,"POS?",NULL);
posZ = read_Zanswer2();
current_posZ =posZ.data();
current_posZ.remove(0,2);
value=current_posZ.toDouble();
value=(value - Pz/1000)*1000;
MoveZ(value); //qDebug()<<"test"<<value;
ZOnTarget=false;
}
}

void MainWindow::Move_forward()
{
//char v[10]; sprintf(v,"%f",2.5); invia_comando_Z(1,"VEL",v); //inserito

string posZ;
QString current_posZ;
double value;


if(ZOnTarget==true)
{
invia_comando_Z(1,"POS?",NULL);
posZ = read_Zanswer2();
current_posZ =posZ.data();
current_posZ.remove(0,2);
value=current_posZ.toDouble();
value=(value + Pz/1000)*1000;
MoveZ(value);//qDebug()<<"test2"<<value;
ZOnTarget=false;
}
}




////////////////////// REPOSITIONING XY MOTORS

void MainWindow::MoveDoubleClick()
{
  if( *(shared_memory+19)==1)
    {if (XOnTarget==true && YOnTarget==true && XYscanning==false && IniXready==1 && IniYready==1)
	{
	  MoveX(*(shared_memory_cmd+64));
	  MoveY(*(shared_memory_cmd+65));
	  qDebug()<<"Spostamento dei motori alla posizione cliccata";
	  *(shared_memory+19)=0;
	 }
    }
}



















