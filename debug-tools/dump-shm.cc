#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <cerrno>



int *shared_memory, *shared_memory2;

int* assignSHM(key_t key, size_t size, int id) {
    int shmID = shmget(key, size, IPC_CREAT | 0666);
    if (shmID == -1) {
        printf("[!] Couldn't obtain a shared memory segment ID for key:\t%d\n", key);
        printf("%s\n", strerror(errno));
        return NULL;
    }
    else return (int*)(shmat(shmID, NULL, 0));
}

int main() {
	key_t key = 7000, key2 = 7200;
	size_t SHMSZ = 409600, SHMSZBIG = 1228800000;
	
	shared_memory = assignSHM(key, SHMSZ, 1);
	shared_memory2 = assignSHM(key2, SHMSZBIG, 2);

	FILE *out;		
	
	time_t rawtime;
	struct tm *timeinfo;
		
	time(&rawtime);
	timeinfo = localtime(&rawtime);	

	char suffix[3][4];
	sprintf(suffix[1], "%0.2d", timeinfo->tm_hour);
	sprintf(suffix[2], "%0.2d", timeinfo->tm_min);
	sprintf(suffix[3], "%0.2d", timeinfo->tm_sec);


	char cwd[1024];
	strcat(cwd, "debug-");
	strcat(cwd, suffix[1]);
	strcat(cwd, suffix[2]);
	strcat(cwd, suffix[3]);
	out = fopen(cwd, "w");
		
	for (int i = 0; i < 16384; i++) {
		fprintf(out, "%d %d %d \n", *(shared_memory+100+i), *(shared_memory+20000+i), *(shared_memory+40000+i));
	}
	fclose(out);

	FILE *out2;
	strcat(cwd, "-long");
	out2 = fopen(cwd, "w");

	for (int i = 0; i < *(shared_memory2+5); i++) fprintf(out2, "%d\n", *(shared_memory2+10+i));
	fclose(out2);


}
