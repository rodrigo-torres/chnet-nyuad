#include <QApplication>
#include <QObject>
#include <QSizePolicy>
#include "mainwindow.h"
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>
#include <QScreen>
#include <QFile>
#include <QString>



int main(int argc, char *argv[])
{


  QApplication a(argc,argv);


//SYSTEM INFO
/*
     system("rm -f cpuinfo");
     system("cp /proc/cpuinfo .");
     system("rm -f meminfo");
     system("cp /proc/meminfo .");
     system("rm -f version");
     system("cp /proc/version .");

     Sleeper::msleep(100);

     QString content;
     QString text2="cpuinfo";
     QFile file2(text2);
      if(file2.exists())
      {
        file2.open(QIODevice::ReadOnly);
        for(int i=0; i<4; i++)
        content = file2.readLine();
        content = file2.readLine();qDebug()<<content;
      }
      file2.close();

     text2="meminfo";
     QFile file3(text2);
      if(file2.exists())
      {
        file3.open(QIODevice::ReadOnly);
        content = file3.readLine();qDebug()<<content;
        content = file3.readLine();qDebug()<<content;
      }
      file3.close();

     text2="version";
     QFile file4(text2);
      if(file4.exists())
      {
        file4.open(QIODevice::ReadOnly);
        content = file4.readLine();qDebug()<<content;
      }
      file4.close();
*/

  const int width = QApplication::desktop()->width();
  const int height = QApplication::desktop()->height();
  qDebug()<<"...Detecting monitor resolution...";
  Sleeper::msleep(300);
  qDebug()<<"Monitor settings: width "<<width<<" - height "<<height;
  Sleeper::msleep(200);

  QFile file("ScreenResolution");
  file.open(QIODevice::WriteOnly);
  QTextStream out(&file);
  out << width << "\n"<<height<< "\n";
  file.close();
  Sleeper::msleep(200);

  qDebug()<<"writing settings...";
  Sleeper::msleep(200);
  qDebug()<<"...done...";
  Sleeper::msleep(200);
  qDebug()<<"Starting GUI...";
  Sleeper::msleep(100);

}






