#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <cstddef>
#include <stdexcept>

#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#include <QtCore/qglobal.h>

#if defined(SHARED_MEMORY_LIBRARY)
#  define SHARED_MEMORYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHARED_MEMORYSHARED_EXPORT Q_DECL_IMPORT
#endif

enum Segments : int
{
  SHARED_MEMORY_CMD = 0,
  SHARED_MEMORY_RATE,
  SHARED_MEMORY_1,
  SHARED_MEMORY_4,
  SHARED_MEMORY_5,
  NO_SEGMENTS
};

template <typename T>
class SHARED_MEMORYSHARED_EXPORT shmarray
{
public:
  using value_type    = T;
  using pointer       = value_type *;
  using reference     = value_type &;
  using const_pointer = value_type const *;
  using const_reference = value_type const &;
  using size_type = std::size_t;

public:
  shmarray();
  ~shmarray();
  // Deleted move and copy constructors
  shmarray (shmarray const &) = delete;
  shmarray (shmarray &&) = delete;
  // Deleted move and copy assignment operators
  shmarray & operator= (shmarray const &) = delete;
  shmarray & operator= (shmarray &&) = delete;

  // Only way to initialize the array
  auto initialize(Segments segment) noexcept -> bool;

  // Element access -> Access is only permitted with boundary checks
  auto at (size_type pos) -> reference;
  auto at (size_type pos) const -> const_reference;

  // Underlying pointer
  auto data() -> pointer;

  //  Capacity;
  auto empty() const noexcept -> bool;
  auto size() const noexcept -> size_type;
  auto size_bytes() const noexcept -> size_type;
private:
  int shmid_;
  size_type size_;
  size_type size_bytes_;
  pointer segment_;
};

#endif // SHARED_MEMORY_H
