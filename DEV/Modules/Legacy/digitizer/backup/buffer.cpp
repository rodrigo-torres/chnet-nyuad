/*
 * buffer.cpp
 *
 *  Created on: Jan 20, 2020
 *      Author: Rodrigo Torres
 */

#include "src/buffer.h"

namespace maxrf::daq {


// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
// -------------------- CLASS METHODS: FileWriter --------------------------- //
// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

FileWriter::FileWriter()
{
	// TODO check proper opening of scan file and alert in case of error
	int detector_no {0};
	for (auto& fstr : files_)
	{
		std::string file_name("scan_temp_det");
		file_name.append(std::to_string(detector_no) + ".raw");
		fstr.open(file_name);
		++detector_no;
	}
	writeHeaderToFiles();

	// TODO check syntax of thread ownership/reference
	thread = std::thread(&FileWriter::eventLoop, this);

	int i = 5;
	while (i-- > 0) {
		queue_empty_.push(std::make_unique<DataPacket>(0));
	}


}




void FileWriter::pushToQueue(Job&& data)
{
	std::unique_lock<std::mutex> lck(mutex_);
	queue_.push(std::move(data));

	// Lock before notifying condition variable to avoid spurious wake-ups
	lck.unlock();
	data_ready_ = true;
	cond_var_.notify_one();
}

void FileWriter::waitPopFromQueue()
{
	std::unique_lock<std::mutex> lck(mutex_);
	while (!data_ready_) {
		cond_var_.wait(lck);
	}

	data_ = std::move(queue_.front());
	queue_.pop();
	if (queue_.empty()) {
		data_ready_ = false;
	}
}


bool FileWriter::tryPopFromQueue()
{
	std::unique_lock<std::mutex> lck(mutex_);
	if (queue_.empty()) {
		return false;
	}

	data_ = std::move(queue_.front());
	queue_.pop();
	return true;
}

std::unique_ptr<DataPacket> FileWriter::getEmptyBufferForPixel(uint no)
{

	std::lock_guard<std::mutex> lck(mutex_other_);
	if (queue_empty_.empty()) {
		printf("DEBUG: Ran out of empty DataPackets!");
	}

	Job empty = std::move(queue_empty_.front());
	queue_empty_.pop();
	empty->setPixel(no);
	return empty;
}

void FileWriter::writeHeaderToFiles() {
  for (auto & hyperf : hfiles_) {
    hyperf.MakeDefaultHeader();
  }
  // Change the eventual fields we might need here

}

void FileWriter::writeDataBlockToFiles()
{

  auto & buff	= (*data_).m_buffer;

// TODO
//	if ( data_->pixel_no != params_.getCurrentPix()) {
//		// Signal synchronization error & possible shift in writing
//	}

	printf("Writing pixel no: %d\n", data_->pixel_no );

  auto x_coord = data_->pixel_no % scan_.width;
  auto y_coord = data_->pixel_no / scan_.width;
  auto pixel = data_->pixel_no;
  auto & pixel_buffer = line_buffer_.at(x_coord);

  // Process data for each detector histogram and produce a sum histogram
  pixel_buffer.clear();
  pixel_buffer << x_coord << ',' << y_coord;

  int expected_bin {0}, bin {0};
  for (auto & counts : buff.at(0)) {
    if (counts == 0)  {
      ++bin;
      continue;
    } else if (expected_bin != bin) {
      expected_bin = bin;
      pixel_buffer << ',' << expected_bin - 1 << 'R';
    } else {
      pixel_buffer << ',' << counts;
    }
  }
  pixel_buffer << '\n';

	{	// The data packet was zeroed in processing. We return to empty buffers
		std::lock_guard<std::mutex> lck(mutex_other_);
		queue_empty_.push(std::move(data_));
	}

  if ( x_coord == scan_.width - 1) {
    // Write line to file
  }

  if ( pixel == scan_.pixels - 1) {
		printf("Filewriter end of scan\n");
		scan_end_ = true;
	}
}


void FileWriter::eventLoop()
{
	// Event loop will only naturally break when the total number of pixels
	// have been written to file. As specified by the scan parameters.
	do {
		if (tryPopFromQueue()) {
			writeDataBlockToFiles();
		}
	} while (daq_active_ && !scan_end_);
}


FileWriter::~FileWriter()
{
	// The main thread will call the destructor so it's possible to join child threads
	daq_active_ = false;
	thread.join();

	for (auto& fstr : files_) {
		fstr.close();
	}
}




}	// namespace daq_writer
