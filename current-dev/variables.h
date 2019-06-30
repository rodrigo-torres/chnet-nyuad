#pragma once

extern int shmid[8];
extern key_t key, key2, key3, key4, key5, key_cmd, key_rate;
extern int *shared_memory, *shared_memory2, *shared_memory3, *shared_memory4;
extern double *shared_memory5;
extern int *shared_memory_cmd, *shared_memory_rate;


/********** Variables (to be) encapsulated in map methods **********/
int DAQ_TYPE=1;

int PixelX = 395, PixelY = 395;
int pixel_Xstep = 1000, pixel_Ystep = 1000;
int MaxX = 0, MaxY = 0, MinX = 200, MinY = 200;
int MaxX_ori, MinX_ori, MaxY_ori, MinY_ori;

double MaxIntegral = 0;

double Integral[50000];
int Xmap[50000], Ymap[50000];
double X[50000],Y[50000];
int MotoreWindowStatus=0;






