/** MAXRF_Main: Main point of access to control the MAXRF instrument
 *  Copyright (C) 2020 Rodrigo Torres and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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


