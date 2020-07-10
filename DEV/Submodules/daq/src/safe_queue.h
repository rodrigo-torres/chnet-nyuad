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


#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include <condition_variable>	// Concurrent queue notify
#include <mutex>				// Mutual exclusion
#include <queue>				// Job FIFO queue

template <typename T>
class Queue
{
 public:
  ///
  /// \brief empty checks if there are items in the queue
  /// \return a boolean value, true if queue is empty, false otherwise
  ///

  bool empty() {
    std::unique_lock<std::mutex> mlock(mutex_);
    return queue_.empty();
  }

  T pop()
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

  bool try_pop(T& item)
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

  void pop(T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    item = queue_.front();
    queue_.pop();
  }

  void push(const T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(item);
    mlock.unlock();
    cond_.notify_one();
  }

  void push(T&& item)
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


#endif // SAFE_QUEUE_H
