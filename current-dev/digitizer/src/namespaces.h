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
class SharedMemory
{

public:
	SharedMemory() = default;
	[[nodiscard]] bool open(key_t keyval, size_t segsize) noexcept
	{
		// We try to access the segment as a client
		int shmid_ = shmget(keyval, segsize, 0);
		if (shmid_ == -1)
		{
			perror("[DAQ] Can't access SHM segment 5: ");
			return valid_ = false;
		}
		else
		{
			underlying_pointer = (T*)shmat(shmid_, nullptr, 0);

			// Compute the number of elements in a segment of size n bytes
			long pagesize = sysconf(_SC_PAGE_SIZE);
			long segsize = pagesize * ((segsize / pagesize) + 1);
			segsize  /= sizeof(T);

			underlying_container_.reserve(segsize);
			for (int i = 0; i < segsize; i++) {
				underlying_container_.push_back(underlying_pointer[i]);
			}

			//shmdt(shared_memory_);
			return valid_ = true;
		}
	}
	void detach() {
		shmdt(underlying_pointer);
	}

	T* getPointer() {
		return underlying_pointer;
	}

	bool isValid() {
		return valid_;
	}
	size_t size() {
		return underlying_container_.size();
	}
	T& operator[](size_t pos) {
		return underlying_container_[pos];
	}
	T& at(size_t pos) {
			return underlying_container_.at(pos);
	}
	const T& operator[](size_t pos) const {
		return underlying_container_[pos];
	}
	const T& at(size_t pos) const	{
		return underlying_container_.at(pos);
	}


private:
	bool valid_ {false};
	T* underlying_pointer;
	std::vector<T> underlying_container_;
};
}	// namespace common


#endif /* SRC_NAMESPACES_H_ */
