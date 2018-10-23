#include <QtGui>
#include "mainwindow.h"
#include "../Header.h"      
#include "../variables.h"
#include "../Shm.h"


MainWindow::MainWindow()
{




/*   key_cmd = 6900;
   shmid_cmd = shmget (key_cmd, SHMSZ_CMD_STATUS, IPC_CREAT | 0666);
   shared_memory_cmd =(int *) shmat(shmid_cmd, NULL, 0);

   cout<< "Data Memory attached at :" << (int *)shared_memory_cmd<< endl << " with shmid: " << shmid_cmd << endl;
   GuiCreator();

   int pid=getpid();
   *(shared_memory_cmd+84)=pid;
*/

    timer = new QTimer(this);                                                // TIMER for program control
    connect(timer, SIGNAL(timeout()), this, SLOT(Check_USB_Status()));
    timer->start(200);

}


MainWindow::~MainWindow()
  {
  /*   *(shared_memory_cmd+74)=0; // set status 0 for mainwindow.cpp*/
  }

void MainWindow::Check_USB_Status()
  { 
   system("./prova &");

     QString text="difference";
      QFile file(text);
      if(file.exists())
      {
        file.open(QIODevice::ReadOnly);
        QString line;
         line = file.readLine();
         int numero=line.toInt();
         file.close();
         if (numero>0)                    
          {
           QMessageBox msgBox;
           msgBox.setText("USB configuration CHNAGED....!!!");
           msgBox.exec(); 
          }

      }
   Sleeper::msleep(50);
  /*   *(shared_memory_cmd+74)=0; // set status 0 for mainwindow.cpp*/
  }

