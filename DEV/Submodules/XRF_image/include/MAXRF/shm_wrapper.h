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

// TODO implement ftok to generate dynamic keys for the segments

typedef struct
{
  key_t key;
  size_t size;
  int shmflag;
} SHM_InitTypeDef;


enum Flags : int
{
  ACCESS = 0,
  CREATE = IPC_CREAT
};

enum Permissions : int
{
  USER_READ = S_IRUSR,
  USER_WRITE = S_IWUSR,
  USER_ALL = S_IRUSR | S_IWUSR
//  GROUP_READ = S_IRGRP,
//  GROUP_WRITE = S_IWGRP,
//  GROUP_ALL = S_IRGRP | S_IWGRP,
//  OTHERS_READ = S_IROTH,
//  OTHERS_WRITE = S_IWOTH,
//  OTHERS_ALL = S_IROTH | S_IWOTH,
//  ALL_ALL = USER_ALL | GROUP_ALL | OTHERS_ALL
};

enum Segments : int
{
  SHARED_MEMORY_CMD = 0,
  SHARED_MEMORY_RATE,
  SHARED_MEMORY_1,
  SHARED_MEMORY_4,
  SHARED_MEMORY_5,
  NO_SEGMENTS
};

static SHM_InitTypeDef const Segment_CMD_InitStruct =
  { 6900, 2048, Flags::CREATE | Permissions::USER_ALL };
static SHM_InitTypeDef const Segment_Rate_InitStruct =
  { 7500, 128, Flags::CREATE | Permissions::USER_ALL };
static SHM_InitTypeDef const Segment_1_InitStruct =
  { 7000, 409600, Flags::CREATE | Permissions::USER_ALL };
static SHM_InitTypeDef const Segment_4_InitStruct =
  { 7400, 2048, Flags::CREATE | Permissions::USER_ALL };
static SHM_InitTypeDef const Segment_5_InitStruct =
  { 8000, 4096, Flags::CREATE | Permissions::USER_ALL };


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

  auto initialize(Segments segment) noexcept -> bool;

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
auto array<T>::initialize(Segments segment) noexcept -> bool
{
  SHM_InitTypeDef init;
  switch (segment)
  {
  case SHARED_MEMORY_CMD:
    init = Segment_CMD_InitStruct;
    break;
  case SHARED_MEMORY_RATE:
    init = Segment_Rate_InitStruct;
    break;
  case SHARED_MEMORY_1:
    init = Segment_1_InitStruct;
    break;
  case SHARED_MEMORY_4:
    init = Segment_4_InitStruct;
    break;
  case SHARED_MEMORY_5:
    init = Segment_5_InitStruct;
    break;
  default:
    printf("[!] Invalid selection of SHM segment\n");
    return false;
    break;
  }

  shmid_ = shmget(init.key, init.size, init.shmflag);
  if (shmid_ == -1)
  {
    printf("[!] Error number: %d\n", errno);
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
