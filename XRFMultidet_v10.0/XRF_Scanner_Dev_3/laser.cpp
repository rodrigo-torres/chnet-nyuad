#include "../Header.h"
#include "mainwindow.h"


extern int serialK;
bool LaserEnabled=false;

void MainWindow::Laser_interface()
  {
   QMessageBox msgBox;
   msgBox.setText("LASER INTERFACE not implemented.... work in progress");
   msgBox.exec();
   //system("./Detector_interface &"); 
   if(LaserEnabled) LaserEnabled=false;
   else LaserEnabled=true;
   Laser_switching();
  }

void MainWindow::Laser_switching()
  { 
   qDebug()<<"inside Laser_Switching";
   if(LaserEnabled)
     LaserOn();
   else
     LaserOff();
  }

void MainWindow::LaserOn()
  {
   int r;
   string cm="1";
   cm=cm+'\n';
   qDebug()<<"Laser ON";
   if(write(serialK, cm.data(), cm.size())) r=0;
   else 
     {
      r=1; 
      qDebug()<<"Errore in scrittura  "<< strerror(errno)<<'\n';  
      qDebug()<<r;
     }
   // return r;
}

void MainWindow::LaserOff()
  {
   int r;
   string cm="0";
   cm=cm+'\n';
   qDebug()<<"Laser OFF";
   if(write(serialK, cm.data(), cm.size()))  r=0;
   else 
     {
      r=1; 
      qDebug()<<"Errore in scrittura  "<< strerror(errno)<<'\n'; 
      qDebug()<<r;
     }
   // return r;
  }



