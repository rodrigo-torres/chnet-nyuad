/*
 * buffer.cpp
 *
 *  Created on: Jan 20, 2020
 *      Author: Rodrigo Torres
 */

#include "src/namespaces.h"
#include "src/buffer.h"

namespace daq_writer {

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
// -------------------- STRUCT METHODS: ScanParams -------------------------- //
// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

ScanParameters::ScanParameters()
{
	common::SharedMemory<double> scan_shm;
	if (scan_shm.open(8000, 4096))
	{
		for (int i = 0; i < scan_params_no; i++) {
			if ( std::signbit(scan_shm.at(i)) ) {
				return;
			}
		}
		physical_x_start = scan_shm.at(0);
		physical_y_start = scan_shm.at(2);
		physical_pixel_x_dim = scan_shm.at(4);
		physical_pixel_y_dim = scan_shm.at(5);
		if (physical_pixel_x_dim == 0.0 || physical_pixel_y_dim == 0.0) {
			return;
		}

		x_dim = (scan_shm.at(1) - physical_x_start) / physical_pixel_x_dim;
		if ( std::signbit(x_dim) || x_dim == 0) {
			return;
		}
		y_dim = (scan_shm.at(3) - physical_y_start) / physical_pixel_y_dim + 1;
		if ( std::signbit(y_dim) || y_dim == 0) {
			return;
		}
		pixels = x_dim * y_dim;
		dwell_time = physical_pixel_x_dim / scan_shm.at(6);

		scan_shm.detach();
		is_valid = true;
	}
	// is_valid_ field is by default false and is not modified
}

std::string ScanParameters::getDataBlockHeader()
{
	std::string header {"P\t"};
	size_t x_coord = current_pix % x_dim;
	size_t y_coord = current_pix / x_dim;
	size_t pixel = current_pix;
	if ( y_coord % 2) {
		x_coord = (x_dim - 1) - x_coord;
		pixel = x_coord + y_coord * x_dim;
		odd_line = true;
	}
	else {
		odd_line = false;
	}
	header.append(std::to_string(pixel));
	header.push_back('\t');
	header.append(std::to_string(x_coord));
	header.push_back('\t');
	header.append(std::to_string(y_coord));
	header.push_back('\n');

	return header;
}

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

	multidet = std::make_unique<HistAlign>(1.234, 0.10);

	if (histogram_visualization_.open(7000, 409600)) {
		// TODO check for errors
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

void FileWriter::writeHeaderToFiles()
{
	std::stringstream ss_base;
	std::ifstream user_input {"conf/daq_session_header.txt"};
	// Check if open or signal error
	ss_base << user_input.rdbuf();

	ss_base << "TUBE_dwell_time = " << params_.getDwellTime() << "\n";
	// IMG parameters
	ss_base	<< "IMG_pixel_count = " << params_.getPixels() << "\n"
			<< "IMG_dimension_width = " << params_.getXDim() << "\n"
			<< "IMG_dimension_height = " << params_.getYDim() << "\n";
	// Motor parameters
	ss_base	<< "MOTOR_start_x_coord = " << params_.getPhysicalXStart() << "\n"
			<< "MOTOR_start_y_coord = " << params_.getPhysicalYStart() << "\n"
			<< "MOTOR_pixel_x_dimension = "
			<< params_.getPhysicalPixelXDim() << "\n"
			<< "MOTOR_pixel_y_dimension = "
			<< params_.getPhysicalPixelYDim() << "\n";

	int index = 0;
	for (auto& ofstr : files_)
	{
		ofstr << ss_base.str();
		&ofstr ==  files_.end() - 1 ?
			ofstr << "DET_ID = " << "SUM\n" :
			ofstr << "DET_ID = " << std::to_string(index) << "\n";
		ofstr << "DET_align_const = " << std::to_string(index) << "\n";
		ofstr << "DET_align_linear = " << std::to_string(index) << "\n";
		ofstr << "}\n";
		++index;
	}
}

void FileWriter::writeDataBlockToFiles()
{

	auto& buff	= (*data_).m_buffer;
	// DEBUG
	//auto time = clock();

	if ( data_->pixel_no != params_.getCurrentPix()) {
		// Signal synchronization error & possible shift in writing
	}

	auto pixel_header = params_.getDataBlockHeader();
	printf("Writing pixel no: %d\n", data_->pixel_no );

	// Process data for each detector histogram and produce a sum histogram
	multidet->processBuffer(buff);
	auto& processed_buff = multidet->getDataBuffers();

	int index = 0;
	for (auto& formatted_data : processed_buff)
	{
		auto opt = static_cast<size_t>(params_.oddLine());
		// Since the list is cleared at the end, this is either 0 or 1
		opt |= (line_buffer_.at(index).size() / (params_.getXDim() - 1)) << 1;

		switch (opt)
		{
		case 0:
			// Write pixel to file
			files_.at(index) << pixel_header << formatted_data;
			break;
		case 1:
			// Write pixel to line buffer
			line_buffer_.at(index).push_front(pixel_header);
			line_buffer_.at(index).front().append(std::move(formatted_data));
			break;
		case 3:
			// Write pixel and then line buffer to file
			files_.at(index) << pixel_header << formatted_data;
			for (auto& str : line_buffer_.at(index)) {
				files_.at(index) << std::move(str);
			}
			line_buffer_.at(index).clear();
			break;
		default:
			// Signal unwanted behavior, the previous list was not written
			break;
		}
		formatted_data.clear();
		++index;
	}
	params_.updateCurrentPix();

	{	// The data packet was zeroed in processing. We return to empty buffers
		std::lock_guard<std::mutex> lck(mutex_other_);
		queue_empty_.push(std::move(data_));
	}

	// DEBUG
	//time = clock() - time;
	//printf("Performance: %f\n", (float)(time) / CLOCKS_PER_SEC);\

	index = 0;
	auto total_histograms = multidet->getTotalHistograms();
	auto shm = histogram_visualization_.getPointer();
	for (auto i : total_histograms.front()) {
		shm[100 + index] = i;
		++index;
	}
	index = 0;
	for (auto i : total_histograms.at(1)) {
		shm[20000 + index] = i;
		++index;
	}
	index = 0;
	for (auto i : total_histograms.back()) {
		shm[40000 + index] = i;
		++index;
	}

	if (params_.getCurrentPix() == params_.getPixels()) {
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

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
// -------------------- CLASS METHODS: HistAlign ---------------------------- //
// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //


HistAlign::HistAlign(double _m, double _c) :
		params_(_m, _c), bin_overlap_(1),
		aligned_bin_limit_low_{0.0}, aligned_bin_limit_high_{1.0}
{
	// Seeds the random number generator
	try {
		std::random_device r;
		engine_.seed(r());
	} catch (std::exception& e) {
		printf("[!] No random device available, using default seed\n");
		engine_.seed();
	}
}


// -------------------------------------------------------------------------- //
//	\fn 	void multidet::distribute(std::vector<unsigned int>& _hist)
//	\brief	The function takes a histogram and re-bins the data into smaller
//			bins spanning a unit interval (i.e., width 1).
//	\par	The histogram to be re-binned, passed by reference.
//	\ret	None
// -------------------------------------------------------------------------- //
void HistAlign::processBuffer(MultiDetBuffer& buffer)
{
	//TODO check size and type of buffer with that is expected, exception
	int histogram_index_in_buffer {0};
	auto& index = histogram_index_in_buffer;

	// Loop over the bins of all detector histograms in buffer
	// Constructs a sum histogram for the pixel at the same time
	for (auto& histogram : buffer)
	{
		int unaligned_bin {0};
		data_buffers_.at(index).clear();
		for (auto& events: histogram)
		{
			if (events)
			{
				// TODO this structure could be improved to avoid dependencies
				constructProbabilityDistribution(unaligned_bin);
				if (weights_.size()) {
					addToSumHistogram(events);
				}

				//Write the detector's histogram to string buffer
				events |= (unaligned_bin << 14);
				total_histograms_.at(index).at(unaligned_bin)++;
				data_buffers_.at(index).append(std::to_string(events) + "\n");
				events = 0;
			}
			++unaligned_bin;
		}
		++index;
	}
	// Loop over the bins of the sum histogram
	int bin{0};
	for (auto& events : sum_histogram_)
	{
		if (events)
		{
			events |= (bin << 14);
			data_buffers_.back().append(std::to_string(events) + "\n");
			events = 0;
		}
		++bin;
	}
}

void HistAlign::constructProbabilityDistribution(int unaligned_bin)
{
	auto& low = aligned_bin_limit_low_;
	auto& high = aligned_bin_limit_high_;
// TODO select correct parameter alignments for each detector
// TODO exception when params_.first is 0, then n_overlap is 0
	low = static_cast<double>(unaligned_bin) * params_.first
			+ params_.second;
	high = low + params_.first;
	bin_overlap_ = ceil(high) - floor(low);

	weights_.clear();
	if (bin_overlap_ == 1) {
		return;
	}

// Constructs an array containing the probability weights of the distribution.
// The probability is directly proportional to the bin overlap.
// The array has the form { p_begin, p_mid, p_mid, ..., p_end }

	weights_.resize(bin_overlap_, 1);
	if (fmod(low, 1.0) != 0.0) {
		weights_.front() = ceil(low) - low;
	}
	if (fmod(high, 1.0) != 0.0) {
		weights_.back() = high - floor(high);
	}

}

void HistAlign::addToSumHistogram(int events)
{
	if (weights_.size() == 0) {
		printf("DEBUG: addToSumHistogram(int) has been called with no valid"
				"probability weights vector.\n"
				"DEBUG: refer to to the function's documentation\n");
		return;
	}
	// Normalizes the probability weight array
	std::discrete_distribution<int> offset(begin(weights_), end(weights_));

	int target_bin {0};
	while (events--)
	{
		target_bin = aligned_bin_limit_low_ + offset(engine_);
		if (target_bin > hist_size - 1) {
			total_histograms_.back().back()++;
			sum_histogram_.back()++;
		}
		else {
			total_histograms_.back().at(target_bin)++;
			sum_histogram_.at(target_bin)++;
		}
	}

	// DO NOT remove this expression.
	weights_.clear();
}


}	// namespace daq_writer
