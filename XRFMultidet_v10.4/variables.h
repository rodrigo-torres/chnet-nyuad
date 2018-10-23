////////////////////////////////////////////////////////////
// 
// GLOBAL VARIABLES 
//
// SHM SEGMENTS are defined in Shm.h

#pragma once

int *shm_cmd;int *shmCommand_cmd;
int shmid_cmd; int shmidString_cmd; 
key_t key_cmd; key_t keyString_cmd;
int *shared_memory_cmd;

int *shm;int *shmCommand;
int shmid; int shmidString; 
key_t key; key_t keyString;
int *shared_memory;

int *shm2;int *shmCommand2;
int shmid2; int shmidString2; 
key_t key2; key_t keyString2;
int *shared_memory2;

int *shm3;int *shmCommand3;
int shmid3; int shmidString3; 
key_t key3; key_t keyString3;
int *shared_memory3;

int *shm4;int *shmCommand4;
int shmid4; int shmidString4; 
key_t key4; key_t keyString4;
int *shared_memory4;

int *shm_rate;int *shmCommand_rate;
int shmid_rate; int shmidString_rate; 
key_t key_rate; key_t keyString_rate;
int *shared_memory_rate;


int DAQ_TYPE=1;
bool NewFileLoaded=false;
///////////////////////////////// VARIABLES MOTORS 

int portX=0, IniX=0, IniXready=0, serialX;
int portY=1, IniY=0, IniYready=0, serialY;
int portZ=2, IniZ=0, IniZready=0, serialZ;
int portACM, serialK;

int SerialiStatus=0;

//////////////////////////////// VARIABLES SCANNER & MAPS


int PixelX=395; int PixelY=395; int pixel_Xstep=1000, pixel_Ystep=1000, PassoZ=1000; double Px=1000; double Py=1000;

double Integral[50000];
int Xmap[50000], Ymap[50000]; 
double X[50000],Y[50000];

int MaxX=0,MaxY=0,MinX=200,MinY=200;
int MaxX_ori, MinX_ori, MaxY_ori, MinY_ori;
int Xminvero,Yminvero, Xmaxvero, Ymaxvero;

double MaxIntegral=0, MinIntegral=10000;

double Xmin=100000., Ymin=100000., Xmax=200000., Ymax=200000., Zmin=25000., Zmax=50000.;
double Xmin1=100000., Ymax1=100000., Xmax1=100000., Ymin1=100000., Zmin1=25000., Zmax1=25000.;
double X_goto=100000.,Y_goto=100000., Z_goto=25000.;

int MotoreWindowStatus=0; int MotoreWindowStatusZ=0; 


//////////////////////////////// VARIABLES ONLINE MAPS
int deltaEventi=0;
int IntR, IntG, IntB;

int X1,Y1,X2,Y2; // in mainwindow_mouse.cpp


double numpixelforaccel;
double posXforacceleration;
double accelerationtime;
int accelerationtimesleep;








