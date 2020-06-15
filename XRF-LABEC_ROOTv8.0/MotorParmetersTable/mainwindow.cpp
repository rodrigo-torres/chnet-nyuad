#include <QtGui>
#include "mainwindow.h"
#include "../Header.h"      // file included from main program folder... Header.h contains all the variables for shm (TBD better)
#include "../variables.h"
#include "../Shm.h"

extern int *shm_cmd, *shmCommand_cmd, shmid_cmd, shmidString_cmd, *shared_memory_cmd; 
extern key_t key_cmd; extern key_t keyString_cmd;


MainWindow::MainWindow()
{

   key_cmd = 6900;
   shmid_cmd = shmget (key_cmd, SHMSZ_CMD_STATUS, IPC_CREAT | 0666);
   shared_memory_cmd =(int *) shmat(shmid_cmd, NULL, 0);

   cout<< "Data Memory attached at :" << (int *)shared_memory_cmd<< endl << " with shmid: " << shmid_cmd << endl;
   GuiCreator();

   int pid=getpid();
   *(shared_memory_cmd+87)=pid;
   *(shared_memory_cmd+77)=1;
}


MainWindow::~MainWindow()
{
   *(shared_memory_cmd+77)=0; // set status 0 for mainwindow.cpp
}



void MainWindow::Exit()
{
qApp->quit();
}
