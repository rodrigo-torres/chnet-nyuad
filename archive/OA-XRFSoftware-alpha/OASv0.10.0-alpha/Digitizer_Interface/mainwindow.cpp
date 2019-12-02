#include <QtGui>
#include "mainwindow.h"
#include "../Header.h"      // file included from main program folder... Header.h contains all the variables for shm (TBD better)
#include "../variables.h"
#include "../Shm.h"

extern int *shm_cmd, *shmCommand_cmd, shmid_cmd, shmidString_cmd, *shared_memory_cmd; 
extern key_t key_cmd; extern key_t keyString_cmd;

extern int *shm, *shmCommand, shmid, shmidString, *shared_memory; 
extern key_t key; extern key_t keyString;

extern int *shm4, *shmCommand4, shmid4, shmidString4, *shared_memory4;
extern key_t key4; extern key_t keyString4;


MainWindow::MainWindow()
{

   key_cmd = 6900;
   shmid_cmd = shmget (key_cmd, SHMSZ_CMD_STATUS, IPC_CREAT | 0666);
   shared_memory_cmd =(int *) shmat(shmid_cmd, NULL, 0);

   cout<< "Data Memory attached at :" << (int *)shared_memory_cmd<< endl << " with shmid: " << shmid_cmd << endl;

   key = 7000;
   shmid = shmget (7000, SHMSZ, IPC_CREAT | 0666);
       if (shmid == -1) qDebug()<<"Errore  "<< strerror(errno)<<'\n';
   shared_memory =(int *) shmat(shmid, NULL, 0);
   cout << "Data Memory attached at :" << (int *)shared_memory<< endl << " with shmid: " << shmid << endl;

   key4 = 7400;
   shmid4 = shmget (7400, SHMSZDIGI, IPC_CREAT | 0666);
       if (shmid4 == -1) qDebug()<<"Errore  "<< strerror(errno)<<'\n';
   shared_memory4 =(int *) shmat(shmid4, NULL, 0);
   cout << "Data Memory attached at :" << (int *)shared_memory4<< endl << " with shmid4: " << shmid4 << endl;

/*  SHARED MEMORY DEF 



                                  *(shared_memory4)=40;        // Trigger Threshold                                   
                                  *(shared_memory4+1)=0;       // Input range digitalizzatore (0 -> 0.6V // 1 -> 1.4V                                  
                                  *(shared_memory4+2)=800;     // Trapezoid Rise Time (ns) 
                                  *(shared_memory4+3)=2500;    // Trapezoid Flat Top  (ns)
                                  *(shared_memory4+4)=12000;   // Decay Time Constant (ns) HACK-FPEP the one expected from fitting algorithm?
                                  *(shared_memory4+5)=1500;    // Peaking delay  (ns)
                                  *(shared_memory4+6)=2;       // Trigger Filter smoothing factor  
                                  *(shared_memory4+7)=0;       // OFFSET
                                  *(shared_memory4+8)=3000;    // Trigger Hold Off  
                                  *(shared_memory4+9)=256;     // BaseLine Mean - ?? 3 = bx10 = 64 samples (baseline mean del trapezio) (...?? 3)  
                                  *(shared_memory4+10)=1;      // peak mean (numero punti su cui mediare per peaking del trapezio)  
                                  *(shared_memory4+11)=3000;   // peak holdoff (min intervallo tra 2 trapezi...minimo=k+m)  
                                  *(shared_memory4+12)=1;      // Trapezoid Gain  
                                  *(shared_memory4+13)=100;    // NOT USED: Delay(b) Input Signal Rise time (ns)...sarebbe delay (b)
                                  *(shared_memory4+14)=10;     // NOT USED: Energy Normalization Factor

                                  *(shared_memory4+15)=0;      // Enable digitiser reading


*/



   GuiCreator();

   int pid=getpid();
   *(shared_memory_cmd+82)=pid;

}


MainWindow::~MainWindow()
{
   *(shared_memory_cmd+72)=0; //Digitiser_interface
}



void MainWindow::Digi_treshold(int trshd)        ///// TRESHOLD
{*(shared_memory4)=trshd;}

void MainWindow::Digi_range(int range)           ///// INPUT RANGE
{*(shared_memory4+1)=range;}

void MainWindow::Rise_Time(int risetime)         ///// RISE TIME
{*(shared_memory4+2)=risetime;}

void MainWindow::Flat_Top(int flattop)           ///// FLAT TOP
{*(shared_memory4+3)=flattop;}

void MainWindow::Fall_Time(int falltime)         ///// FALL TIME
{*(shared_memory4+4)=falltime;}

void MainWindow::Peaking_Delay(int peaking)      ///// PEAKING TIME
{*(shared_memory4+5)=peaking;}

void MainWindow::Smoothing_Factor(int smoothing) ///// SMOOTHING FACTOR
{*(shared_memory4+6)=smoothing;}

void MainWindow::DC_Offset(int offset)           ///// OFFSET
{*(shared_memory4+7)=offset;}

void MainWindow::HoldOff(int holdoff)            ///// TRIGGER HOLDOFF
{*(shared_memory4+8)=holdoff;}

void MainWindow::Base_Line_Mean(int baseline)    ///// BASELINE MEAN
{*(shared_memory4+9)=baseline;}

void MainWindow::Peak_Mean(int peakmean)         ///// PEAK MEAN
{*(shared_memory4+10)=peakmean;}

void MainWindow::Peak_Holdoff(int peakholdoff)   ///// PEAK HOLDOFF
{*(shared_memory4+11)=peakholdoff;}

void MainWindow::Trapezoid_Gain(int trapgain)    ///// TRAPEZOID GAIN
{*(shared_memory4+12)=trapgain;}

void MainWindow::Digitiser_data_download_enable_ON(bool getData)
{if (getData) {*(shared_memory4+15)=1; qDebug()<<"ADC reading: ON"; } else {*(shared_memory4+15)=0; qDebug()<<"ADC reading: OFF";}}




void MainWindow::Exit()
{
qApp->quit();
}
