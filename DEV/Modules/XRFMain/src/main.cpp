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

// Checking of access rights to COM ports, i.e. stages and MCA USB ports,
// happens at runtime (before attempting to access the port).
// Checking the CAEN USB driver is loaded in the kernel space also happens
// at runtime, before the start of a DAQ session.

// utility_c.h provides an API to perform the checks mentioned above

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  // Resizing and show directives are inside the MainWindow's constructor
  MAXRF_UI w;
  return a.exec();
}






