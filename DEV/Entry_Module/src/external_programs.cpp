#include "mainwindow.h"

extern tty_agent tty_ptr;

extern int *shared_memory_cmd, *shared_memory;


void MAXRF_UI::OpenDAQWindow() {
  system("gnome-terminal -q --window -- /bin/sh -c './MAXRF_DAQ'");
}

void MAXRF_UI::ShowHistogram() {
  if ( *(shared_memory_cmd+71) == 0 ) {
    system("./bin/spectrum & ");
    *(shared_memory_cmd+71)=1; // XRF
  }
  else {
    qDebug()<<"[!] XRF histogram window already opened";
  }
}

void MAXRF_UI::RateMeter() {
  if(*(shared_memory_cmd+73)==0)
  {
    system("./bin/rate & ");
    *(shared_memory_cmd+73)=1; // Rate
  }
  else
    qDebug()<<"Ratemeter alreay opened!\n";
}

void MAXRF_UI::XrayTable()
{  
  if(*(shared_memory_cmd+74)==0)
  {
    system("./bin/XRayTable & ");
    *(shared_memory_cmd+74)=1;
  }
  else qDebug()<<"[!] X-Ray table window already opened";
}


