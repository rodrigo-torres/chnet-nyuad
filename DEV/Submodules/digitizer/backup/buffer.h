/*
 * buffer.h
 *
 *  Created on: Jan 21, 2020
 *      Author: frao
 */
/*
 * What do we need to manage a file writer object in multi-thread program?
 *
 * 1. We need a class that works as an event loop and:
 * 		-  manages its own destruction
 * 		-  keeps track on a buffer and limits its access through a mutex
 * 		-  regulates the resizing and reallocation of the buffer if needed
 * 2. We need a class/structure that defines the object to be written
 * 3. We need a class that defines the rules for writing and performing data manip
 * 	on the data to be written. This class should somehow:
 * 		-  follow a read/write sequence. I.e. concurrent pixels are written into the buffer in order
 * 		-  regulate the destruction of the object they write
 * 		-  should not keep any internal state variables to avoid race conditions
 * 		-  operate exclusively on the data structure defined in point 2.
 *
 */

// Let's start with the data structure. It should at least contain 2 things.
// The ADC data for each channel, and the scan parameters.

#ifndef BUFFER_H_
#define BUFFER_H_

#include <array>
#include <chrono>				// Clock for debug purposes
#include <cmath>				// signbit()
#include <ctime>				// For performance debug
#include <condition_variable>	// Concurrent queue notify
#include <queue>				// Job FIFO queue
#include <fstream>				// Output files
#include <list>
#include <memory>				// Smart pointers
#include <mutex>				// Mutual exclusion
#include <random>				// Random numbers and probability distributions
#include <sstream>
#include <string>
#include <thread>				// Event-loop thread
#include <utility>
#include <vector>

#include "MAXRF/file_management.hpp"

namespace maxrf::daq {
// The data type used to store the bin counts of a detector's histogram.
// Modifying the alias could lead to undefined behavior
using event_t = unsigned int;
// The type of buffer containing all detector histograms.
// Just an alias for user convenience.
using MultiDetBuffer = std::vector<std::vector<event_t>>;

// Number of bins per histogram and total number of detectors respectively.
static const uint scan_params_no = 7;
static const uint hist_size = 0x4000;
static const uint hist_no = 0x0002;

struct DataPacket
{
	DataPacket(const DataPacket& packet2) :
		pixel_no(packet2.pixel_no + 1), m_buffer(packet2.m_buffer)
	{}
	DataPacket(uint no) :
		pixel_no(no), m_buffer(hist_no, std::vector<uint>(hist_size, 0))
	{}
	void setPixel(uint no)
	{
		pixel_no = no;
	}
	uint pixel_no;
	MultiDetBuffer m_buffer;
};


class FileWriter
{
	// DOC the FileWriter type is an amalgam of three classes
	// 1. Wrapper for an output file stream
	// 2. Definition of data processing and manipulation methods,
	//	including methods relevant to the multi-detector configuration
	// 3. An implementation of a concurrent queue

	// TODO document the alias below
	using Job = std::unique_ptr<DataPacket>;

public:
	FileWriter();
	~FileWriter();
	void pushToQueue(Job&&);
	Job getEmptyBufferForPixel(uint);


private:
	bool tryPopFromQueue();
	void waitPopFromQueue();
	void writeHeaderToFiles();
	void writeDataBlockToFiles();
	void eventLoop();
	// TODO a swap and push-back method
	// TODO mask enabling channels for being written

	bool daq_active_	{true};
	bool data_ready_	{false};
	bool scan_end_		{false};

	Job data_;
	std::condition_variable cond_var_;
	std::mutex mutex_;
	std::mutex mutex_other_;
	std::queue<Job> queue_;
	std::queue<Job> queue_empty_;


  struct ScanStats
  {
    int width;
    int height;
    int pixels;
  } scan_;

  std::array<maxrf::HypercubeFile, hist_no> hfiles_;

	std::array<std::ofstream, hist_no + 1> files_;
  std::vector<std::stringstream> line_buffer_;

	std::thread thread;
};

}	// namespace daq_writer

#endif /* BUFFER_H_ */
