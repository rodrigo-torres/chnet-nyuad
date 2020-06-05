#ifndef SHM_WRAPPER_H
#define SHM_WRAPPER_H

#include <cstddef>
#include <stdexcept>

#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

namespace shm
{

enum Flags : int
{
  ACCESS = 0,
  CREATE = IPC_CREAT
};

enum Permissions : int
{
  USER_READ = S_IRUSR,
  USER_WRITE = S_IWUSR,
  USER_ALL = S_IRUSR | S_IWUSR,
  GROUP_READ = S_IRGRP,
  GROUP_WRITE = S_IWGRP,
  GROUP_ALL = S_IRGRP | S_IWGRP,
  OTHERS_READ = S_IROTH,
  OTHERS_WRITE = S_IWOTH,
  OTHERS_ALL = S_IROTH | S_IWOTH,
  ALL_ALL = USER_ALL | GROUP_ALL | OTHERS_ALL
};

typedef struct
{
  key_t key;
  size_t size;
  int shmflag;
} TypeDefInitSHM;


template <typename T>
class array
{
public:
  using value_type    = T;
  using pointer       = value_type *;
  using reference     = value_type &;
  using const_pointer = value_type const *;
  using const_reference = value_type const &;
  using size_type = std::size_t;

public:
  array();
  ~array();

  bool initialize(TypeDefInitSHM &) noexcept;

  // Deleted move and copy constructors
  array (array const &) = delete;
  array (array &&) = delete;
  // Deleted move and copy assignment operators
  array & operator= (array const &) = delete;
  array & operator= (array &&) = delete;

  //  Element acces -> Access is only permitted with boundary checks
  reference at (size_type pos);
  const_reference at (size_type pos) const;

  pointer data();

  //  Capacity;
  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type size_bytes() const noexcept;

private:
  int shmid_;
  size_type size_;
  size_type size_bytes_;
  pointer segment_;
};
template <typename T>
typename array<T>::pointer array<T>::data()
{
  return segment_;
}

template <typename T>
array<T>::array() : shmid_{-1}, size_{0}, size_bytes_{0}, segment_{nullptr}
{}

template<typename T>
array<T>::~array()
{
  if (segment_ != nullptr)
  {
    shmctl(shmid_, IPC_RMID, nullptr);
    shmdt(segment_);
  }
}


template<typename T>
bool array<T>::initialize(TypeDefInitSHM & init) noexcept
{
  shmid_ = shmget(init.key, init.size, init.shmflag);
  if (shmid_ == -1)
  {
    perror("[!] Can't access SHM: ");
    return false;
  }

  segment_ = static_cast<T*>(shmat(shmid_, nullptr, 0));
  // Get the page size
  auto page_size = sysconf(_SC_PAGESIZE);
  auto total_pages = init.size / page_size;
  total_pages += (init.size % page_size) ? 1 : 0;
  size_bytes_ = page_size * total_pages;
  size_ = size_bytes_ / sizeof(T);

  return true;
}

template <typename T>
typename array<T>::reference array<T>::at(size_t pos)
{
  if (!(pos < size()))
  {
    throw std::out_of_range("");
  }
  return segment_[pos];
}

template <typename T>
typename array<T>::const_reference array<T>::at(size_t pos) const
{
  if (!(pos < size()))
  {
    throw std::out_of_range("");
  }
  return segment_[pos];
}

template <typename T>
typename array<T>::size_type array<T>::size() const noexcept
{
  return size_;
}

template <typename T>
typename array<T>::size_type array<T>::size_bytes() const noexcept
{
  return size_bytes_;
}

template <typename T>
bool array<T>::empty() const noexcept
{
  return (size() == 0) ? true : false;
}

}


#endif // SHM_WRAPPER_H
