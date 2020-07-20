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

#include "ipc_methods.h"

template class maxrf::ipc::shmarray<int>;
template class maxrf::ipc::shmarray<double>;

namespace maxrf::ipc {

enum Flags : short
{
  ACCESS =  S_IRUSR | S_IWUSR,
  CREATE =  IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR
};

struct SegmentInitParams
{
  key_t key;
  int shmflag;
  size_t size;
};

template<typename T>
shmarray<T>::~shmarray()
{
  if (segment_ != nullptr)
  {
    shmctl(shmid_, IPC_RMID, nullptr);
    shmdt(segment_);
  }
}

/**
  * The only method to configure and initialize the shared memory segment
  *
  * A longer description is in the works
  *
  * @param segment is an instance of an enumeration type. Accepted values
  * are:
  *   SHARED_MEMORY_CMD
  *   SHARED_MEMORY_1
  *
  * @return a boolean value set to true if the operation was succesful, and set
  * to false otherwise.
  */
template<typename T>
auto shmarray<T>::initialize(Segments segment) noexcept -> bool
{
  SegmentInitParams init {};
  init.shmflag = Flags::CREATE;
  if (segment == Segments::kSHMCommand) {
    init.key  = 6900;
    init.size = 2048;
  }
  else if (segment == Segments::kSHMSpectra) {
    init.key  = 7000;
    init.size = 409600;
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


} // namespace maxrf::ipc

