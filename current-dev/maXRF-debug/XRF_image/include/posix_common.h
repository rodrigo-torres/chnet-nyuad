#ifndef POSIX_COMMON_H
#define POSIX_COMMON_H

#include <cerrno>
#include <cstdio>
#include <cstring>

#include <sys/shm.h>
#include <sys/types.h>

namespace posix
{
template <typename T> T* assignSHM(key_t key, size_t size) {
    int shmID = shmget(key, size, IPC_CREAT | 0666);
    if (shmID == -1) {
        printf("[!] Couldn't obtain a shared memory segment ID for key:\t%d\n", key);
        printf("%s\n", strerror(errno));
        return nullptr;
    }
    else return static_cast<T*>(shmat(shmID, nullptr, 0));
}
}

#endif // POSIX_COMMON_H
