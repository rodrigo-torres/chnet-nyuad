#include <qapplication.h>
#include "mainwindow.h"

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <math.h>
#include <errno.h>
#include <sys/ioctl.h>
using namespace std;
#include <QFile>
#include <QDebug>
#include <../Shm.h>
#include <../variables.h>

extern int *shm_cmd, *shmCommand_cmd, shmid_cmd, shmidString_cmd, *shared_memory_cmd; 
extern key_t key_cmd; extern key_t keyString_cmd;

extern int *shm, *shmCommand, shmid, shmidString, *shared_memory; 
extern key_t key; extern key_t keyString;

extern int *shm2, *shmCommand2, shmid2, shmidString2, *shared_memory2; 
extern key_t key2; extern key_t keyString2;

extern int *shm3, *shmCommand3, shmid3, shmidString3, *shared_memory3;
extern key_t key3; extern key_t keyString3;

extern int *shm4, *shmCommand4, shmid4, shmidString4, *shared_memory4;
extern key_t key4; extern key_t keyString4;


int Resolution_mode=0;

int main ( int argc, char **argv ) {

    key_cmd = 6900;
    shmid_cmd = shmget (key_cmd, SHMSZ_CMD_STATUS, IPC_CREAT | 0666);
    shared_memory_cmd =(int *) shmat(shmid_cmd, NULL, 0);

    cout<<"Data Memory attached at:\t" << (int *)shared_memory_cmd <<"\n With shmid:\t\t"<< shmid_cmd <<endl;


    key = 7000;
    shmid = shmget (key, SHMSZ, IPC_CREAT | 0666);
    shared_memory =(int *) shmat(shmid, NULL, 0);

    cout<< "Data Memory attached at:\t"<< (int *)shared_memory <<"\n With shmid:\t\t"<< shmid << endl;
    *(shared_memory+24)=0;


    QApplication a( argc, argv );


    MainWindow w;

    w.resize( 1125, 675 );
    //w.resize( 750, 450 );

    w.show();

    return a.exec();
}
