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
#include <QFile>
#include <QDebug>
#include <../Shm.h>
#include <../variables.h>

using namespace std;

key_t key_cmd, key;
int *shared_memory_cmd, *shared_memory;

int* assignSHM(key_t key, size_t size) {
    int shmID = shmget(key, size, IPC_CREAT | 0666);
    if (shmID == -1) {
        printf("[!] Couldn't obtain a shared memory segment ID for key:\t%d\n", key);
        printf("%s\n", strerror(errno));
        return nullptr;
    }
    else return static_cast<int*>(shmat(shmID, nullptr, 0));
}

int main (int argc, char **argv) {
    key_t key_cmd = 6900, key = 7000;
    shared_memory_cmd = assignSHM(key_cmd, SHMSZ_CMD_STATUS);
    shared_memory = assignSHM(key, SHMSZ);

    *(shared_memory+24) = 0;

    QApplication a( argc, argv );
    MainWindow w;
    w.resize( 1125, 450 );
    w.show();

    return a.exec();
}
