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

#ifndef UTILITY_C_H_
#define UTILITY_C_H_

#include <stdbool.h>    // bool
#include <stdio.h>      // File i/o
#include <stdlib.h>     // malloc(), calloc(), free()
#include <string.h>     // strcmp(), strcat()

// POSIX libraries
#include <dirent.h>     // Opening and listing of directories
#include <grp.h>        // User group information and control
#include <pwd.h>        // User information access
#include <sys/stat.h>   // File information
#include <unistd.h>     // Various, e.g. getuid()

#ifdef __cplusplus
extern "C" {
#endif

// Rather than setting file access bits and owners ourselves,
// we check if the user is in the group of the file owner.
// If not, we add the user to said group
bool check_CAEN_USB_driver(void);
bool check_serial_port_access(const char* port);

#ifdef __cplusplus
}
#endif

#endif // UTILITY_C_H_
