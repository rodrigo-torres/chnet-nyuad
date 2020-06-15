////////////////////////////////////////////////////////////
// 
// GLOBAL VARIABLES 
//
// SHM SEGMENTS are defined in Shm.h

#pragma once
#ifndef VARIABLES_H
#define VARIABLES_H

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
// port:      TTY_motors.cpp, mainwindow.cpp, SHM_creator.cpp
// Ini:       TTY_motors.cpp, mainwindow.cpp, move_motors.cpp, SHM_creator.cpp
// IniReady:  TTY_motors.cpp, mainwindow.cpp, move_motors.cpp, SHM_creator.cpp
// Serial:    TT_motors.cpp, mainwindow.cpp, XY_Z_Init.cpp (serialK not into the last)

int portX=0, IniX=0, IniXready=0, serialX;
int portY=1, IniY=0, IniYready=0, serialY;
int portZ=2, IniZ=0, IniZready=0, serialZ;
int portACM, serialK;

int SerialiStatus=0;

//////////////////////////////// VARIABLES SCANNER & MAPS
// Pixel(X,Y);mainwindow.cpp, mainwindow_showMap.cpp, mainwindow_DefinePixels.cpp, mainwindow_mouse.cpp, mainwindow_online.cpp 
// Passo(X,Y):mainwindow.cpp, mainwindow_showMap.cpp, mainwindow_DefinePixels.cpp, mainwindow_load_SHM.cpp, Connection_Creator.cpp 
// P(X,Y):    
// Integral;
// (X,Y)map:
// (X[],Y[]):    
// Xminvero:  mainwindow.cpp, mainwindow_showMap.cpp, mainwindow_DefinePixels.cpp, mainwindow_mouse.cpp 
// MaxX_ori:  mainwindow.cpp, mainwindow_showMap.cpp, mainwindow_DefinePixels.cpp 

//int Resolution_mode;

int PixelX=395; int PixelY=395; int PassoX=1000, PassoY=1000, PassoZ=1000; double Px=1000; double Py=1000;

double Integral[160000];
int Xmap[160000], Ymap[160000]; 
double X[160000],Y[160000];

int MaxX=0,MaxY=0,MinX=200,MinY=200;
int MaxX_ori, MinX_ori, MaxY_ori, MinY_ori;
int Xminvero,Yminvero, Xmaxvero, Ymaxvero;

double MaxIntegral=0, MinIntegral=10000;

double Xmin=100000., Ymin=100000., Xmax=200000., Ymax=200000., Zmin=25000., Zmax=50000.;
double Xmin1=100000., Ymax1=100000., Xmax1=100000., Ymin1=100000., Zmin1=25000., Zmax1=25000.;
double X_goto=100000.,Y_goto=100000., Z_goto=25000.;


//////////////////////////////// VARIABLES ONLINE MAPS
int deltaEventi=0;
int IntR, IntG, IntB;

int X1,Y1,X2,Y2; // in mainwindow_mouse.cpp


double numpixelforaccel;
double posXforacceleration;
double accelerationtime;
int accelerationtimesleep;

///////////////Variables for the composed map visualization (sum of three different elements)///////////

//int ChMin1=0, ChMax1=0, ChMin2=0, ChMax2=0, ChMin3=0, ChMax3=0;
//int ChMinBa, ChMaxBa, ChMinCa, ChMaxCa, ChMinK, ChMaxK, ChMinCo, ChMaxCo, ChMinAg, ChMaxAg, ChMinCr, ChMaxCr, ChMinCu, ChMaxCu, ChMinPbL, ChMaxPbL, ChMinAu, ChMaxAu, ChMinHg, ChMaxHg, ChMinSi, ChMaxSi, ChMinTi, ChMaxTi, ChMinSn, ChMaxSn, ChMinFe, ChMaxFe, ChMinZn, ChMaxZn, ChMinPbM, ChMaxPbM;

#endif





