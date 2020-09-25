/** MAXRF_DAQ: Manage XRF DAQ sessions with the CHNET built MAXRF Scanner
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

#ifndef IPC_METHODS_H
#define IPC_METHODS_H

#include <array>
#include <csignal>
#include <cstring>    //memcpy and the lik
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <queue>

#include <condition_variable>	// Concurrent queue notify
#include <mutex>              // Mutual exclusion
#include <queue>              // Job FIFO queue

// For POSIX SHM
#include <fcntl.h>           // For O_* constants
#include <sys/mman.h>
#include <sys/stat.h>        // For mode constants

// FOR System V SHM
// TODO don't use System V SHM and POSIX SHM together. Deprecate System V use
#include <sys/ipc.h>
#include <sys/shm.h>

// FOR UNIX Domain sockets
#include <sys/socket.h>
#include <sys/un.h>          // UNIX socket address struct
// For POSIX sempahores
#include <semaphore.h>
// FOR FD management
#include <sys/types.h>
#include <unistd.h>


#include <QtCore/qglobal.h>

#if defined(MAXRF_LIBRARIES)
#  define MAXRF_LIBRARIES_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAXRF_LIBRARIES_SHARED_EXPORT Q_DECL_IMPORT
#endif

#include "ipc_shm_mapping.h"

namespace maxrf::ipc {

class SHMHandle;

namespace // implementation detail
{
    // build R (*)(Args...) from R (Args...)
    // compile error if signature is not a valid function signature
    template <typename, typename>
    struct build_free_function;

    template <typename F, typename R, typename ... Args>
    struct build_free_function<F, R (Args...)>
    { using type = R (*)(Args...); };

    // build R (C::*)(Args...) from R (Args...)
    //       R (C::*)(Args...) const from R (Args...) const
    //       R (C::*)(Args...) volatile from R (Args...) volatile
    // compile error if signature is not a valid member function signature
    template <typename, typename>
    struct build_class_function;

    template <typename C, typename R, typename ... Args>
    struct build_class_function<C, R (Args...)>
    { using type = R (C::*)(Args...); };

    template <typename C, typename R, typename ... Args>
    struct build_class_function<C, R (Args...) const>
    { using type = R (C::*)(Args...) const; };

    template <typename C, typename R, typename ... Args>
    struct build_class_function<C, R (Args...) volatile>
    { using type = R (C::*)(Args...) volatile; };

    // determine whether a class C has an operator() with signature S
    template <typename C, typename S>
    struct is_functor_with_signature
    {
        typedef char (& yes)[1];
        typedef char (& no)[2];

        // helper struct to determine that C::operator() does indeed have
        // the desired signature; &C::operator() is only of type
        // R (C::*)(Args...) if this is true
        template <typename T, T> struct check;

        // T is needed to enable SFINAE
        template <typename T> static yes deduce(check<
            typename build_class_function<C, S>::type, &T::operator()> *);
        // fallback if check helper could not be built
        template <typename> static no deduce(...);

        static bool constexpr value = sizeof(deduce<C>(0)) == sizeof(yes);
    };

    // determine whether a free function pointer F has signature S
    template <typename F, typename S>
    struct is_function_with_signature
    {
        // check whether F and the function pointer of S are of the same
        // type
        static bool constexpr value = std::is_same<
            F, typename build_free_function<F, S>::type
        >::value;
    };

    // C is a class, delegate to is_functor_with_signature
    template <typename C, typename S, bool>
    struct is_callable_impl
        : std::integral_constant<
            bool, is_functor_with_signature<C, S>::value
          >
    {};

    // F is not a class, delegate to is_function_with_signature
    template <typename F, typename S>
    struct is_callable_impl<F, S, false>
        : std::integral_constant<
            bool, is_function_with_signature<F, S>::value
          >
    {};
}

// Determine whether type Callable is callable with signature Signature.
// Compliant with functors, i.e. classes that declare operator(); and free
// function pointers: R (*)(Args...), but not R (Args...)!
template <typename Callable, typename Signature>
struct is_callable
    : is_callable_impl<
        Callable, Signature,
        std::is_class<Callable>::value
      >
{};

enum class SocketOptions
{
  kServer = 0x0,
  kClient,
};

enum class Segments : int
{
  kSHMCommand = 0,  ///! For control and status flags
  kSHMSpectra,      ///! For passing XRF spectra between processes
};

///
/// \brief The Queue class is a wrapper around an STL queue which uses
/// a mutex and a condition variable to provide a thread-safe FIFO queue for
/// synchronizing jobs across threads
///
template <typename T>
class Queue
{
 public:
  using value_type  = T;

  ///
  /// \brief empty checks if there are items in the queue
  /// \return a boolean value, true if queue is empty, false otherwise
  ///
  bool empty() {
    std::unique_lock<std::mutex> mlock(mutex_);
    return queue_.empty();
  }

  ///
  /// \brief pop attempts to get the first item available on the queue (i.e. the
  /// oldest) and blocks if the queue is empty
  /// \return the oldest element in the queue of type value_type
  ///
  value_type pop()
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    auto item = queue_.front();
    queue_.pop();
    return item;
  }

  ///
  /// \brief pop an overload of the method above
  /// \param item
  ///
  void pop(value_type & item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    item = queue_.front();
    queue_.pop();
  }

  ///
  /// \brief try_pop same as \see Queue::pop but if the call to pop from queue
  /// would block, the function returns false. Otherwise the element is popped
  /// immediately
  /// \param item of type value_type, the element is retreived by copy
  /// \return a boolean value, true if an element was popped, false otherwise
  ///
  bool try_pop(value_type & item)
  {
    using namespace std::chrono_literals;

    std::unique_lock<std::mutex> mlock(mutex_);
    if (cond_.wait_for(mlock, 100ms, [&] {return !queue_.empty();})) {
      item = queue_.front();
      queue_.pop();
      return true;
    }
    else {
      return false;
    }
  }



  void push(value_type const & item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(item);
    mlock.unlock();
    cond_.notify_one();
  }

  void push(value_type && item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(std::move(item));
    mlock.unlock();
    cond_.notify_one();
  }

 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

///
/// \brief The SocketServer class uses Unix domain sockets to guarantee only
/// one instance of the program runs on a machine at any given time.
///
class IPCSocket
{
public:
  IPCSocket() = default;
  IPCSocket(std::string const & _path) : socket_path{_path} {
    address_.sun_family = AF_UNIX;
    strncpy(address_.sun_path, _path.c_str(), sizeof (address_.sun_path) - 1);
  }
  ~IPCSocket() {
    ReleaseSocket();
  }

  auto GetFD() {
    return connection_fd_;
  }

  void StartSocket(SocketOptions opt) {
    switch (opt) {
    case SocketOptions::kServer:
      CreateServer();
      is_server = true;
      break;
    case SocketOptions::kClient:
      CreateClient();
      break;
    }
    if (socket_fd == -1) {
      throw std::runtime_error("Error configuring socket");
    }
  }

  void StartSocket(pid_t pid, int fd) {
    char fd_path[64];  // actual maximal length: 37 for 64bit systems
    snprintf(fd_path, sizeof(fd_path), "/proc/%d/fd/%d", pid, fd);
    connection_fd_ = open(fd_path, O_RDWR);
    if (connection_fd_ == -1) {
      std::cout << strerror(errno) << std::endl;
    }
  }

  inline void ReleaseSocket() {
    if (connection_fd_ != -1) {
      shutdown(connection_fd_, SHUT_RDWR);
    }
    if (socket_fd != -1) {
      shutdown(socket_fd, SHUT_RDWR);
    }
    if (is_server) {
      unlink(address_.sun_path);
    }
  }

  bool EstablishConnectionWithClient() {
    bool connected {false};

    if (is_server) {
      // Drop previous connection
      if (connection_fd_ != -1) {
        shutdown(connection_fd_, SHUT_RDWR);
      }

      // Block until there's a connection
      connection_fd_ = accept(socket_fd, nullptr, nullptr);
      if (connection_fd_ == -1) {
        std::cout << strerror(errno) << std::endl;
      }
      else {
        connected = true;
      }
    }
    return connected;
  }

  ///
  /// \brief AttemptConnectionToServer
  /// \param timeout is the specified timeout for the operation in seconds
  /// \return
  ///
  bool AttemptConnectionToServer(double _seconds) {
    using namespace std::chrono;
    auto timeout = duration<double> {_seconds};
    auto micros  = duration_cast<microseconds>(timeout - floor<seconds>(timeout));

    auto ret = connect(socket_fd,
                       reinterpret_cast<sockaddr *>(&address_),
                       sizeof (address_));
    if (ret == -1 && errno == EINPROGRESS) {

      fd_set wfd;
      FD_ZERO(&wfd);
      FD_SET(socket_fd, &wfd);

      struct timeval timeoutv;
      timeoutv.tv_sec   = static_cast<long>(timeout.count());
      timeoutv.tv_usec  = static_cast<long>(micros.count());

      ret = select(socket_fd + 1, nullptr, &wfd, nullptr, &timeoutv);
      if (ret > 0) {
        // Connection succesful
        connection_fd_ = socket_fd;
        return true;
      }
      else if (ret == 0) {
        std::cout << "Attempt to connect timed out" << std::endl;
        return false;
      }
      else {
        return false;
      }
    }
    else {
      connection_fd_ = socket_fd;
      return true;
    }
  }

  template <class T>
  void ReceiveFromSocket(T * buff) {
    recv(connection_fd_, buff, sizeof (T), 0);
  }

  void ReceiveFromSocket(char * buff) {
    do {
      recv(connection_fd_, buff, 1, 0); // Read character by character
    } while (*(buff++) != '\0');
  }

  void ReceiveFromSocket(char & buff) {
    recv(connection_fd_, &buff, 1, 0);
  }

  template<class T>
  void SendThroughSocket(T const * data) {
    // TODO enable only for arithmetic, enum types and trivial classes
    send(connection_fd_, data, sizeof(T), 0);
  }


  void SendThroughSocket(char const * data) {
    // TODO enable only for arithmetic, enum types and trivial classes
    for (uint off = 0; off < strlen(data) + 1; ++off) {
      send(connection_fd_, data + off, 1, 0);
    }
  }

  void SendCharThroughSocket(char c) {
    char buff = c;
    send(connection_fd_, &buff, 1, 0);
  }


private:
  void CreateServer() {
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1) {
      std::cout << "Couldn't create process socket: " << strerror(errno)
                << std::endl;
      return;
    }

    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, socket_path.c_str(), 108 - 1); // UNIX_PATH_MAX is 108

    int rc = bind(socket_fd, reinterpret_cast<struct sockaddr *>(&address),
                  sizeof (address));
    if (rc == -1) {
      std::cout << "Couldn't bind socket: " << strerror(errno) << std::endl;
      close(socket_fd);
      socket_fd = -1;
      return;
    }

    rc = listen(socket_fd, 1);
    if (rc == -1) {
      std::cout << "Couldn't bind socket: " << strerror(errno) << std::endl;
      close(socket_fd);
      socket_fd = -1;
      return;
    }
  }

  void CreateClient() {
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1) {
      std::cout << "Couldn't create process socket: " << strerror(errno)
                << std::endl;
      return;
    }

    auto ret = connect(socket_fd,
                       reinterpret_cast<sockaddr *>(&address_),
                       sizeof (address_));

    if (ret == -1) {
      std::cout << "Couldn't connect to server: " << strerror(errno)
                << std::endl;
      return;
    }
    connection_fd_ = socket_fd;

//    // Put the socket in non-blocking mode
//    int flags = fcntl(socket_fd, F_GETFL, 0);
//    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
  }


  std::string const socket_path {};
  struct sockaddr_un address_ {};
  int socket_fd {-1};
  int connection_fd_ {-1};
  bool is_server {false};
};

class ResourceCleaner
{
public:
  ResourceCleaner(void (*_callback)(int)) : clean_up_callback {_callback}
  {
    if (clean_up_callback == nullptr) {
      std::cout << "Invalid clean-up callback pointer" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    std::array<int, 5> sigs { SIGINT, SIGTERM, SIGABRT, SIGSEGV, SIGFPE };
    for (auto sig : sigs) {
      if (std::signal(sig, clean_up_callback) == SIG_ERR) {
        std::cout << "Can't guarantee proper cleanup in case of failure!"
                  << std::endl;
        std::exit(EXIT_FAILURE);
      }
    }
  }
private:
  void (*clean_up_callback)(int) {nullptr};
};

class POSIXSemaphore
{
  bool is_unnamed_  {false};
  bool destroy_on_exit_ {false};
  sem_t * sem_ptr_ {nullptr};
  std::string sem_name {};

  friend SHMHandle;

protected:
  void MarkForDestruction() {
    destroy_on_exit_ = true;
  }

public:
  ~POSIXSemaphore() {
    if (sem_ptr_ && !is_unnamed_) {
      ::sem_close(sem_ptr_);
      if (destroy_on_exit_) {
        ::sem_unlink(sem_name.c_str());
      }
    }
  }

  ///
  /// \brief Init overload initializes a named semaphore
  /// \param sem_name
  ///
  void Init(std::string sem_name) {
    using namespace std::literals;

    auto ret =
        ::sem_open(sem_name.c_str(),  // Name of the semaphore
                   O_CREAT | O_EXCL,  // Create the semaphore
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP,   // RW permissions
                   0  // Initial value of the semaphore
                   );
    // The semaphore exists so we try opening instead
    if (ret == SEM_FAILED && errno == EEXIST) {
      ret = ::sem_open(sem_name.c_str(), 0);
      if (ret == SEM_FAILED) {
        // Couldn't open the semaphore
        throw std::runtime_error("Could not open semaphore: "s +
                                 ::strerror(errno));
      }
    }
    else if (ret == SEM_FAILED) {
      // Couldn't open the semaphore
      throw std::runtime_error("Could not open semaphore: "s +
                               ::strerror(errno));
    }

    // If we reach here, we have succesfully opened the semaphore
    sem_ptr_ = ret;
  }

  ///
  /// \brief Init overload for C-strings to guarantee proper overload resolution
  /// \param sem_name
  ///
  void Init(char const * sem_name) {
    Init(std::string {sem_name});
  }

  ///
  /// \brief Init overload initilaizes an unnamed semaphore
  /// \param unnamed_sem
  ///
  void Init(sem_t * unnamed_sem) {
    using namespace std::literals;

    if (::sem_init(unnamed_sem, 0, 1) == -1) {
      // Couldn't open the semaphore
      throw std::runtime_error("Could not open semaphore: "s +
                               ::strerror(errno));
    }
    sem_ptr_ = unnamed_sem;
    is_unnamed_ = true;
  }

  void operator= (sem_t * unnamed_sem) {
    sem_ptr_    = unnamed_sem;
    is_unnamed_ = true;
  }

  inline void Post() {
    ::sem_post(sem_ptr_);
  }

  inline bool TryWait() {
    return ::sem_trywait(sem_ptr_) == 0 ? true : false;
  }

//  inline bool TimedWait() {

//  }

  inline void Wait() {
    ::sem_wait(sem_ptr_);
  }
};


enum class SHMOperations : char
{
  kSum    = 0,
  kMinus,
  kMultiplies,
  kDivides,
  kModulus,
  kNegate
};

class SHMHandle
{
  POSIXSemaphore  shm_sem_ {};
  SHMStructure    * shm_ptr_ {nullptr};

public:
  ~SHMHandle() {
    if (shm_ptr_) {
      WriteVariable(&SHMStructure::mappings, SHMOperations::kMinus, 1);
      if (GetVariable(&SHMStructure::mappings) == 0) {
        ::sem_destroy(&shm_ptr_->segment_semaphore);
        ::munmap(shm_ptr_, sizeof (SHMStructure));
        ::shm_unlink("/maxrf_control");
      }
      else {
        ::munmap(shm_ptr_, sizeof (SHMStructure));
      }

    }
  }

  void Init() {
    using namespace std::literals;


    // Attempt to create the segment
    auto fd = ::shm_open("/maxrf_control", O_RDWR | O_CREAT | O_EXCL,
                         S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
    if (fd == -1) {
      switch (errno) {
      case EEXIST: {
        // The SHM object exists already. So we attempt to access it
        auto fd = ::shm_open("/maxrf_control", O_RDWR, 0);
        if (fd == -1) {
          // Couldn't open the segment
          throw std::runtime_error("Could not open shared memory object: "s +
                                   ::strerror(errno));
        }

        // The segment is open, we only have to map the memory addresses
        auto ptr = static_cast<SHMStructure *>(
              ::mmap(nullptr,                // Let the kernel choose the address
                     sizeof (SHMStructure),  // Size of the segment to map
                     PROT_READ | PROT_WRITE, // Allow read and write of pages
                     MAP_SHARED,             // Changes are seen by all processes
                     fd,                     // FD of the shared memory object
                     0                       // Offset of mapping
                     ));

        if (ptr == MAP_FAILED) {
          // Couldn't open the segment
          ::close(fd);
          throw std::runtime_error("Could not map shared memory object: "s +
                                   ::strerror(errno));
        }

        // Mapping is valid, we put the class in a valid sate
        ::close(fd);
        shm_ptr_ = ptr;
        shm_sem_ = &shm_ptr_->segment_semaphore;
        WriteVariable(&SHMStructure::mappings, SHMOperations::kSum, 1);
        return;
      }
      default:
        throw std::runtime_error("Could not open shared memory object: "s +
                                 ::strerror(errno));
      }
    }

    // We have create the segment, we truncate to the right size and attach
    if (::ftruncate(fd, sizeof (SHMStructure)) == -1) {
      // Truncating failed. We cleanup resources and throw
      ::close(fd);
      ::shm_unlink("/maxrf_control");
      throw std::runtime_error("Could not truncate shared memory object: "s +
                               ::strerror(errno));
    }

    auto ptr = static_cast<SHMStructure *>(::mmap(nullptr,
                                                  sizeof (SHMStructure),
                                                  PROT_READ | PROT_WRITE,
                                                  MAP_SHARED, fd, 0));
    if (ptr == MAP_FAILED) {
      // Couldn't open the segment
      ::close(fd);
      ::shm_unlink("/maxrf_control");
      throw std::runtime_error("Could not map shared memory object: "s +
                               ::strerror(errno));
    }

    // We populate the address space with the default values
    SHMStructure default_values {};
    memcpy(ptr, &default_values, sizeof (SHMStructure));

    try {
      shm_sem_.Init(&ptr->segment_semaphore);
    } catch (std::runtime_error &) {
      ::close(fd);
      ::munmap(ptr, sizeof (SHMStructure));
      ::shm_unlink("/maxrf_control");
      throw ;
    }

    // Valid state
    shm_ptr_ = ptr;
    WriteVariable(&SHMStructure::mappings, SHMOperations::kSum, 1);
  }


  template <class T>
  T GetVariable(T SHMStructure::* shm_structure_member) {
    T temp {};
    GetVariable(shm_structure_member, temp);
    return temp;
  }

  template <class T>
  void GetVariable(T SHMStructure::* shm_structure_member, T & in) {
    typename decltype (shm_ptr_->seq_number)::value_type read_seq_number { };
    while (true) {
      if ((read_seq_number  = shm_ptr_->seq_number.load()) % 2 == 1) {
        // Sequence number is odd so a writer acquired the lock
        continue;
      }
      in = (*shm_ptr_).*shm_structure_member;
      if (read_seq_number != shm_ptr_->seq_number.load()) {
        // A writer updated the value while we were reading, we try again
        continue;
      }
      break;
    }
  }

  template <class T>
  inline void CheckAccessRights(T SHMStructure::* shm_member) {
    static SHMStructure obj{};

    static_assert (!std::is_same_v<T, sem_t>,
                   "Don't use the WriteVariable and GetVariable methods to "
                   "modify or open semaphores. Use the OpenSemaphore and "
                   "InitSemaphore instead!");

//    static_assert (&(obj.*shm_member) != &obj.segment_semaphore,
//                   "The segment semaphore is a restricted access variable");


  }

  template <class T>
  void WriteVariable(T SHMStructure::* shm_structure_member, T const & val) {
    shm_sem_.Wait();
    shm_ptr_->seq_number.fetch_add(1);
    (*shm_ptr_).*shm_structure_member = val;
    shm_ptr_->seq_number.fetch_add(1);
    shm_sem_.Post();
  }

  ///
  /// \brief WriteVariable overload reads a variable, performs an operation
  /// and writes back the result of that operation in a single lock
  /// \param shm_member
  /// \param operation
  template <class T, class Functor>
  void WriteVariable(T SHMStructure::* shm_member, Functor op, T op_arg) {
    static_assert (is_callable<Functor, T (T const &, T const &) const>::value,
                   "The operation must have signature: "
                   " T (T const &, T const &) const");
    shm_sem_.Wait();
    shm_ptr_->seq_number.fetch_add(1);
    (*shm_ptr_).*shm_member = op((*shm_ptr_).*shm_member, op_arg);
    shm_ptr_->seq_number.fetch_add(1);
    shm_sem_.Post();
  }


  ///
  /// \brief WriteVariable overload reads a variable, performs an operation
  /// and writes back the result of that operation in a single lock
  /// \param shm_member
  /// \param operation
  template <class T>
  void WriteVariable(T SHMStructure::* shm_member, SHMOperations op, T op_arg) {
    static_assert (std::is_arithmetic_v<T>,
                   "This WriteVariable overload can only be used for SHM"
                   "variables that are of arithmetic type!");
    shm_sem_.Wait();
    shm_ptr_->seq_number.fetch_add(1);
    // Critical section
    switch (op) {
    case SHMOperations::kSum:
      (*shm_ptr_).*shm_member = (*shm_ptr_).*shm_member + op_arg;
      break;
    case SHMOperations::kMinus:
      (*shm_ptr_).*shm_member = (*shm_ptr_).*shm_member - op_arg;
      break;
    case SHMOperations::kNegate:
      (*shm_ptr_).*shm_member = -1 * (*shm_ptr_).*shm_member;
      break;
    case SHMOperations::kDivides:
      if (op_arg != 0) {
        (*shm_ptr_).*shm_member = (*shm_ptr_).*shm_member / op_arg;
      }
      break;
    case SHMOperations::kModulus:
      (*shm_ptr_).*shm_member = (*shm_ptr_).*shm_member % op_arg;
      break;
    case SHMOperations::kMultiplies:
      (*shm_ptr_).*shm_member = (*shm_ptr_).*shm_member * op_arg;
      break;
    }
    shm_ptr_->seq_number.fetch_add(1);
    shm_sem_.Post();
 //   (*shm_ptr_).*shm_member = op((*shm_ptr_).*shm_member, op_arg);
  }

};

template <typename T>
class MAXRF_LIBRARIES_SHARED_EXPORT shmarray
{
public:
  using value_type    = T;
  using pointer       = value_type *;
  using reference     = value_type &;
  using const_pointer = value_type const *;
  using const_reference = value_type const &;
  using size_type = std::size_t;

public:
  shmarray() = default;
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
  int shmid_  {-1};
  size_type size_ {0};
  size_type size_bytes_ {0};
  pointer segment_  {nullptr};
};

} // namespace maxrf::ipc

#endif // IPC_METHODS_H
