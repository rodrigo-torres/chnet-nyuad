/**
 * @file
 * @author  Rodrigo Torres <rodrigo.torres@nyu.edu>
 * @version 1.1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The shared memory array class (shmarray) provides a bounds-safe, reentrant
 * wrapper for POSIX shared memory segments. It implements some of the methods
 * found in the std::array container for STL compatibility.
 */

#ifndef MAXRF_LIBRARIES_CONFIG_H
#define MAXRF_LIBRARIES_CONFIG_H

#include <QtCore/qglobal.h>

#if defined(MAXRF_LIBRARIES)
#  define MAXRF_LIBRARIES_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAXRF_LIBRARIES_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAXRF_LIBRARIES_CONFIG_H
