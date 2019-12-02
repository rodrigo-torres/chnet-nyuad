#include <QtGui>
#include "mainwindow.h"
#include "../Header.h"      // file included from main program folder... Header.h contains all the variables for shm (TBD better)
#include "../variables.h"
#include "../Shm.h"

int shmid_cmd, *shared_memory_cmd; 
key_t key_cmd;

MainWindow::MainWindow() {

   key_cmd = 6900;
   shmid_cmd = shmget (key_cmd, SHMSZ_CMD_STATUS, IPC_CREAT | 0666);
   shared_memory_cmd =(int *) shmat(shmid_cmd, NULL, 0);

   cout<< "Data Memory attached at :" << (int *)shared_memory_cmd<< endl << " with shmid: " << shmid_cmd << endl;

   GuiCreator();

   int pid=getpid();
   *(shared_memory_cmd+84)=pid;

}


MainWindow::~MainWindow()
{
   *(shared_memory_cmd+74)=0; // set status 0 for mainwindow.cpp
}


void MainWindow::Digi_range(int range)           ///// INPUT RANGE
{//qDebug()<<range;

      switch (range)
       {
        case(3): {linedit_Element->setText("K: 0.052 keV"); break;}
        case(4): {linedit_Element->setText("K: 0.110 keV"); break;}
        case(5): {linedit_Element->setText("K: 0.185 keV"); break;}
        case(6): {linedit_Element->setText("K: 0.282 keV"); break;}
        case(7): {linedit_Element->setText("K: 0.392 keV"); break;}
        case(8): {linedit_Element->setText("K: 0.526 keV"); break;}
        case(9): {linedit_Element->setText("K: 0.677 keV"); break;}
        case(10): {linedit_Element->setText("K: 0.851 keV"); break;}
        case(11): {linedit_Element->setText("Ka: 1.04 keV"); linedit_Element1->setText("Kb: 1.07 keV"); break;}
        case(12): {linedit_Element->setText("Ka: 1.25 keV"); linedit_Element1->setText("Kb: 1.30 keV"); break;}
        case(13): {linedit_Element->setText("Ka: 1.49 keV"); linedit_Element1->setText("Kb: 1.55 keV"); break;}
        case(14): {linedit_Element->setText("Ka: 1.74 keV"); linedit_Element1->setText("Kb: 1.83 keV"); break;}
        case(15): {linedit_Element->setText("Ka: 2.02 keV"); linedit_Element1->setText("Kb: 2.14 keV"); break;}
        case(16): {linedit_Element->setText("Ka: 2.31 keV"); linedit_Element1->setText("Kb: 2.46 keV"); break;}
        case(17): {linedit_Element->setText("Ka: 2.62 keV"); linedit_Element1->setText("Kb: 2.82 keV"); break;}
        case(18): {linedit_Element->setText("Ka: 2.96 keV"); linedit_Element1->setText("Kb: 3.19 keV"); break;}
        case(19): {linedit_Element->setText("Ka: 3.31 keV"); linedit_Element1->setText("Kb: 3.59 keV"); break;}
        case(20): {linedit_Element->setText("Ka: 3.69 keV"); linedit_Element1->setText("Kb: 4.01 keV"); linedit_Element2->setText("La 0.34 keV"); break;}
        case(21): {linedit_Element->setText("Ka: 4.09 keV"); linedit_Element1->setText("Kb: 4.46 keV"); linedit_Element2->setText("La 0.40 keV"); break;}
        case(22): {linedit_Element->setText("Ka: 4.51 keV"); linedit_Element1->setText("Kb: 4.93 keV"); linedit_Element2->setText("La 0.45 keV");linedit_Element3->setText("Lb 0.46 keV"); break;}
        case(23): {linedit_Element->setText("Ka: 4.95 keV"); linedit_Element1->setText("Kb: 5.43 keV"); linedit_Element2->setText("La 0.51 keV");linedit_Element3->setText("Lb 0.52 keV"); break;}
        case(24): {linedit_Element->setText("Ka: 5.41 keV"); linedit_Element1->setText("Kb: 5.95 keV"); linedit_Element2->setText("La 0.57 keV");linedit_Element3->setText("Lb 0.58 keV"); break;}
        case(25): {linedit_Element->setText("Ka: 5.90 keV"); linedit_Element1->setText("Kb: 6.49 keV"); linedit_Element2->setText("La 0.64 keV");linedit_Element3->setText("Lb 0.65 keV"); break;}
        case(26): {linedit_Element->setText("Ka: 6.40 keV"); linedit_Element1->setText("Kb: 7.06 keV"); linedit_Element2->setText("La 0.70 keV");linedit_Element3->setText("Lb 0.72 keV"); break;}
        case(27): {linedit_Element->setText("Ka: 6.93 keV"); linedit_Element1->setText("Kb: 7.65 keV"); linedit_Element2->setText("La 0.78 keV");linedit_Element3->setText("Lb 0.79 keV"); break;}
        case(28): {linedit_Element->setText("Ka: 7.48 keV"); linedit_Element1->setText("Kb: 8.26 keV"); linedit_Element2->setText("La 0.85 keV");linedit_Element3->setText("Lb 0.87 keV"); break;}
        case(29): {linedit_Element->setText("Ka: 8.05 keV"); linedit_Element1->setText("Kb: 8.90 keV"); linedit_Element2->setText("La 0.93 keV");linedit_Element3->setText("Lb 0.95 keV"); break;}
        case(30): {linedit_Element->setText("Ka: 8.64 keV"); linedit_Element1->setText("Kb: 9.57 keV"); linedit_Element2->setText("La 1.01 keV");linedit_Element3->setText("Lb 1.03 keV"); break;}
        case(31): {linedit_Element->setText("Ka: 9.25 keV"); linedit_Element1->setText("Kb: 10.26 keV"); linedit_Element2->setText("La 1.10 keV");linedit_Element3->setText("Lb 1.12 keV"); break;}
        case(32): {linedit_Element->setText("Ka: 9.89 keV"); linedit_Element1->setText("Kb: 10.98 keV"); linedit_Element2->setText("La 1.19 keV");linedit_Element3->setText("Lb 1.21 keV"); break;}
        case(33): {linedit_Element->setText("Ka: 10.54 keV"); linedit_Element1->setText("Kb: 11.73 keV"); linedit_Element2->setText("La 1.28 keV");linedit_Element3->setText("Lb 1.32 keV"); break;}
        case(34): {linedit_Element->setText("Ka: 11.22 keV"); linedit_Element1->setText("Kb: 12.50 keV"); 
linedit_Element2->setText("La 1.38 keV");linedit_Element3->setText("Lb 1.42 keV"); break;}
        case(35): {linedit_Element->setText("Ka: 11.92 keV"); linedit_Element1->setText("Kb: 13.29 keV"); 
linedit_Element2->setText("La 1.48 keV");linedit_Element3->setText("Lb 1.53 keV"); break;}
        case(36): {linedit_Element->setText("Ka: 12.65 keV"); linedit_Element1->setText("Kb: 14.11 keV"); linedit_Element2->setText("La 1.59 keV");linedit_Element3->setText("Lb 1.64 keV"); break;}
        case(37): {linedit_Element->setText("Ka: 13.39 keV"); linedit_Element1->setText("Kb: 14.96 keV"); linedit_Element2->setText("La 1.69 keV");linedit_Element3->setText("Lb 1.75 keV"); break;}
        case(38): {linedit_Element->setText("Ka: 14.16 keV"); linedit_Element1->setText("Kb: 15.83 keV"); linedit_Element2->setText("La 1.81 keV");linedit_Element3->setText("Lb 1.87 keV"); break;}
        case(39): {linedit_Element->setText("Ka: 14.96 keV"); linedit_Element1->setText("Kb: 16.74 keV"); linedit_Element2->setText("La 1.92 keV");linedit_Element3->setText("Lb 2.00 keV"); break;}
        case(40): {linedit_Element->setText("Ka: 15.77 keV"); linedit_Element1->setText("Kb: 17.67 keV"); linedit_Element2->setText("La 2.04 keV");linedit_Element3->setText("Lb 2.12 keV"); break;}
        case(41): {linedit_Element->setText("Ka: 16.61 keV"); linedit_Element1->setText("Kb: 18.62 keV"); linedit_Element2->setText("La 2.17 keV");linedit_Element3->setText("Lb 2.26 keV"); break;}
        case(42): {linedit_Element->setText("Ka: 17.48 keV"); linedit_Element1->setText("Kb: 19.61 keV"); linedit_Element2->setText("La 2.29 keV");linedit_Element3->setText("Lb 2.40 keV"); break;}
        case(43): {linedit_Element->setText("Ka: 18.41 keV"); linedit_Element1->setText("Kb: 19.61 keV"); linedit_Element2->setText("La 2.42 keV");linedit_Element3->setText("Lb 2.54 keV"); break;}
        case(44): {linedit_Element->setText("Ka: 19.28 keV"); linedit_Element1->setText("Kb: 21.66 keV"); linedit_Element2->setText("La 2.56 keV");linedit_Element3->setText("Lb 2.68 keV"); break;}
        case(45): {linedit_Element->setText("Ka: 20.21 keV"); linedit_Element1->setText("Kb: 22.72 keV"); linedit_Element2->setText("La 2.70 keV");linedit_Element3->setText("Lb 2.83 keV"); break;}
        case(46): {linedit_Element->setText("Ka: 21.18 keV"); linedit_Element1->setText("Kb: 23.82 keV"); linedit_Element2->setText("La 2.84 keV");linedit_Element3->setText("Lb 2.99 keV"); break;}
        case(47): {linedit_Element->setText("Ka: 22.16 keV"); linedit_Element1->setText("Kb: 24.94 keV"); linedit_Element2->setText("La 2.98 keV");linedit_Element3->setText("Lb 3.15 keV"); break;}
        case(48): {linedit_Element->setText("Ka: 23.17 keV"); linedit_Element1->setText("Kb: 26.09 keV"); 
linedit_Element2->setText("La 3.13 keV");linedit_Element3->setText("Lb 3.32 keV"); break;}
        case(49): {linedit_Element->setText("Ka: 24.21 keV"); linedit_Element1->setText("Kb: 27.27 keV"); linedit_Element2->setText("La 3.29 keV");linedit_Element3->setText("Lb 3.49 keV"); break;}
        case(50): {linedit_Element->setText("Ka: 25.27 keV"); linedit_Element1->setText("Kb: 28.48 keV"); linedit_Element2->setText("La 3.44 keV");linedit_Element3->setText("Lb 3.66 keV"); break;}
        case(51): {linedit_Element->setText("Ka: 26.36 keV"); linedit_Element1->setText("Kb: 29.72 keV"); linedit_Element2->setText("La 3.61 keV");linedit_Element3->setText("Lb 3.84 keV"); break;}
        case(52): {linedit_Element->setText("Ka: 27.47 keV"); linedit_Element1->setText("Kb: 30.99 keV"); linedit_Element2->setText("La 3.77 keV");linedit_Element3->setText("Lb 4.03 keV"); break;}
        case(53): {linedit_Element->setText("Ka: 28.61 keV"); linedit_Element1->setText("Kb: 32.29 keV"); linedit_Element2->setText("La 3.94 keV");linedit_Element3->setText("Lb 4.22 keV"); break;}
        case(54): {linedit_Element->setText("Ka: 29.80 keV"); linedit_Element1->setText("Kb: 33.64 keV"); linedit_Element2->setText("La 4.11 keV");linedit_Element3->setText("Lb 4.42 keV"); break;}
        case(55): {linedit_Element->setText("Ka: 30.97 keV"); linedit_Element1->setText("Kb: 34.98 keV"); linedit_Element2->setText("La 4,29 keV");linedit_Element3->setText("Lb 4.62 keV"); break;}
        case(56): {linedit_Element->setText("Ka: 32.19 keV"); linedit_Element1->setText("Kb: 36.38 keV"); linedit_Element2->setText("La 4.47 keV");linedit_Element3->setText("Lb 4.83 keV"); break;}
        case(57): {linedit_Element->setText("Ka: 55.76 keV"); linedit_Element1->setText("Kb: 63.21 keV"); linedit_Element2->setText("La 7.90 keV");linedit_Element3->setText("Lb 9.02 keV"); linedit_Element4->setText("Ma: 1.46 keV"); linedit_Element5->setText("Mb 1.70 keV"); break;}
        case(58): {linedit_Element->setText("Ka: 57.52 keV"); linedit_Element1->setText("Kb: 65.21 keV"); linedit_Element2->setText("La 8.15 keV");linedit_Element3->setText("Lb 9.34 keV");linedit_Element4->setText("Ma: 1.70 keV"); linedit_Element5->setText("Mb 1.76 keV"); break;}
        case(59): {linedit_Element->setText("Ka: 59.31 keV"); linedit_Element1->setText("Kb: 67.23 keV"); linedit_Element2->setText("La 8.40 keV");linedit_Element3->setText("Lb 9.67 keV"); linedit_Element4->setText("Ma: 1.77 keV"); linedit_Element5->setText("Mb 1.80 keV"); break;}
        case(60): {linedit_Element->setText("Ka: 61.13 keV"); linedit_Element1->setText("Kb: 69.30 keV"); linedit_Element2->setText("La 8.65 keV");linedit_Element3->setText("Lb 10.01 keV"); linedit_Element4->setText("Ma: 1.84 keV"); linedit_Element5->setText("Mb 1.88 keV"); break;}
        case(61): {linedit_Element->setText("Ka: 62.99 keV"); linedit_Element1->setText("Kb: 71.40 keV"); linedit_Element2->setText("La 8.91 keV");linedit_Element3->setText("Lb 10.35 keV"); linedit_Element4->setText("Ma: 1.92 keV"); linedit_Element5->setText("Mb 1.99 keV"); break;}
        case(62): {linedit_Element->setText("Ka: 64.89 keV"); linedit_Element1->setText("Kb: 73.55 keV"); linedit_Element2->setText("La 9.19 keV");linedit_Element3->setText("Lb 10.71 keV"); linedit_Element4->setText("Ma: 1.98 keV"); linedit_Element5->setText("Mb 2.00 keV"); break;}
        case(63): {linedit_Element->setText("Ka: 66.82 keV"); linedit_Element1->setText("Kb: 75.74 keV"); linedit_Element2->setText("La 9.44 keV");linedit_Element3->setText("Lb 11.07 keV"); linedit_Element4->setText("Ma: 2.06 keV"); linedit_Element5->setText("Mb 2.13 keV"); break;}
        case(64): {linedit_Element->setText("Ka: 68.79 keV"); linedit_Element1->setText("Kb: 77.97 keV"); linedit_Element2->setText("La 9.71 keV");linedit_Element3->setText("Lb 11.44 keV"); linedit_Element4->setText("Ma: 2.13 keV"); linedit_Element5->setText("Mb 2.22 keV"); break;}
        case(65): {linedit_Element->setText("Ka: 70.82 keV"); linedit_Element1->setText("Kb: 80.26 keV"); linedit_Element2->setText("La 9.99 keV");linedit_Element3->setText("Lb 11.82 keV"); linedit_Element4->setText("Ma: 2.18 keV"); linedit_Element5->setText("Mb 2.29 keV"); break;}
        case(66): {linedit_Element->setText("Ka: 72.86 keV"); linedit_Element1->setText("Kb: 82.56 keV"); linedit_Element2->setText("La 10.27 keV");linedit_Element3->setText("Lb 12.21 keV"); linedit_Element4->setText("Ma: 2.27 keV"); linedit_Element5->setText("Mb 2.39 keV"); break;}
        case(67): {linedit_Element->setText("Ka: 74.96 keV"); linedit_Element1->setText("Kb: 84.92 keV"); linedit_Element2->setText("La 10.55 keV");linedit_Element3->setText("Lb 12.61 keV"); linedit_Element4->setText("Ma: 2.34 keV"); linedit_Element5->setText("Mb 2.48 keV"); break;}
        case(68): {linedit_Element->setText("Ka: 77.10 keV"); linedit_Element1->setText("Kb: 87.34 keV"); linedit_Element2->setText("La 10.84 keV");linedit_Element3->setText("Lb 13.02 keV"); linedit_Element4->setText("Ma: 2.43 keV"); linedit_Element5->setText("Mb 2.59 keV"); break;}
        case(69): {linedit_Element->setText("Ka: 79.30 keV"); linedit_Element1->setText("Kb: 89.81 keV"); linedit_Element2->setText("La 11.13 keV");linedit_Element3->setText("Lb 13.44 keV"); linedit_Element4->setText("Ma: 2.50 keV"); linedit_Element5->setText("Mb 2.68 keV"); break;}
        case(70): {linedit_Element->setText("Ka: 81.53 keV"); linedit_Element1->setText("Kb: 92.32 keV"); linedit_Element2->setText("La 11.42 keV");linedit_Element3->setText("Lb 13.87 keV"); linedit_Element4->setText("Ma: 2.58 keV"); linedit_Element5->setText("Mb 2.78 keV"); break;}
        case(71): {linedit_Element->setText("Ka: 83.80 keV"); linedit_Element1->setText("Kb: 94.88 keV"); linedit_Element2->setText("La 11.72 keV");linedit_Element3->setText("Lb 14.32 keV"); linedit_Element4->setText("Ma: 2.66 keV"); linedit_Element5->setText("Mb 2.88 keV"); break;}
        case(72): {linedit_Element->setText("Ka: 86.12 keV"); linedit_Element1->setText("Kb: 97.48 keV"); linedit_Element2->setText("La 12.03 keV");linedit_Element3->setText("Lb 14.77 keV"); linedit_Element4->setText("Ma: 2.74 keV"); linedit_Element5->setText("Mb 2.99 keV"); break;}
        case(73): {linedit_Element->setText("Ka: 88.46 keV"); linedit_Element1->setText("Kb: 100.14 keV"); linedit_Element2->setText("La 12.34 keV");linedit_Element3->setText("Lb 15.23 keV"); linedit_Element4->setText("Ma: 2.82 keV"); linedit_Element5->setText("Mb 3.09 keV"); break;}
   }

}


void MainWindow::Exit()
{
qApp->quit();
}
