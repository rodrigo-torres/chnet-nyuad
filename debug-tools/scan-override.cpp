#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
using namespace std;

#define SHMSZ_CMD_STATUS 2048 
#define SHMSZBIG 1228800000


int *shared_memory_cmd, *shared_memory2;
int shmID = -1;

int* assignSHM(key_t key, size_t size, int id) {
    shmID = shmget(key, size, IPC_CREAT | 0666);
    if (shmID == -1) {
        printf("[!] Couldn't obtain a shared memory segment ID for key:\t%d\n", key);
        printf("%s\n", strerror(errno));
        return NULL;
    }
    else return (int*)(shmat(shmID, NULL, 0));
}


int main(){

key_t key_cmd = 6900, key2 = 7200;

shared_memory_cmd = assignSHM(key_cmd, SHMSZ_CMD_STATUS, 0);
shared_memory2 = assignSHM(key2, SHMSZBIG, 2);

int pID = *(shared_memory_cmd+80); 
char process[30];

sprintf(process, "kill -s TERM %i &", pID);
system(process);

*(shared_memory2+8) = 1;

    if (shmID != -1) shmctl(shmID, IPC_RMID, 0);
    shmdt(shared_memory2);


return 0;   
}
