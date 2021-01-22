/** MAXRF_Spectrum: Display XRF spectra
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

int *shared_memory_cmd, *shared_memory;

template class maxrf::ipc::shmarray<int>;

int main (int argc, char **argv) {
    maxrf::ipc::shmarray<int> shm0, shm1;
    shm0.initialize(maxrf::ipc::Segments::kSHMCommand);
    shm1.initialize(maxrf::ipc::Segments::kSHMSpectra);
    shared_memory_cmd = shm0.data();
    shared_memory = shm1.data();

    QApplication a( argc, argv );
    MainWindow w;
    w.resize( 1125, 450 );
    w.show();

    return a.exec();
}
