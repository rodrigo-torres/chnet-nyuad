/*
 * namespaces.h
 *
 *  Created on: Oct 24, 2019
 *      Author: Rodrigo Torres
 */

#ifndef SRC_NAMESPACES_H_
#define SRC_NAMESPACES_H_

#include <cerrno>
#include <cstring>
#include <cstdio>
#include <vector>

// Linux and POSIX libraries
#include <sys/ipc.h>	// Mode access definitions
#include <sys/shm.h>	// Linux native shared memory management methods
#include <sys/types.h>	// Types
#include <unistd.h>		// System configuration information

namespace common
{

// TODO for portability, native IPC should be avoided in favor of containers
// in the STL. At the very least, they key_t key should be generated with a
// a call to ftok()
// TODO the values of the key and size variables should be copied from a
// const variable in a header file common to ALL the project
// TODO should verify main creates the segment with modes IP_CREAT|IP_EXCL|0666


template <class T>
rt13
}	// namespace common


#endif /* SRC_NAMESPACES_H_ */
