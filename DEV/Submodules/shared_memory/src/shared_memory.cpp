/*
 * shm_array.cpp
 *
 *  Created on: Jun 6, 2020
 *      Author: frao
 */

#include "MAXRF/shared_memory.h"

template class shmarray<int>;
template class shmarray<double>;

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
};


typedef struct
{
  key_t key;
  size_t size;
  int shmflag;
} SHM_InitTypeDef;

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
shmarray<T>::shmarray() : shmid_{-1}, size_{0}, size_bytes_{0}, segment_{nullptr}
{}

template<typename T>
shmarray<T>::~shmarray()
{
  if (segment_ != nullptr)
  {
    shmctl(shmid_, IPC_RMID, nullptr);
    shmdt(segment_);
  }
}


template<typename T>
auto shmarray<T>::initialize(Segments segment) noexcept -> bool
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
auto shmarray<T>::data() -> pointer
{
  return segment_;
}

template <typename T>
auto shmarray<T>::at(size_type pos) -> reference
{
  if (!(pos < size()))
  {
    throw std::out_of_range("");
  }
  return segment_[pos];
}

template <typename T>
auto shmarray<T>::at(size_type pos) const -> const_reference
{
  if (!(pos < size()))
  {
    throw std::out_of_range("");
  }
  return segment_[pos];
}

template <typename T>
auto shmarray<T>::size() const noexcept -> size_type
{
  return size_;
}

template <typename T>
auto shmarray<T>::size_bytes() const noexcept -> size_type
{
  return size_bytes_;
}

template <typename T>
auto shmarray<T>::empty() const noexcept -> bool
{
  return (size() == 0) ? true : false;
}
