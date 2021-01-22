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


#include <filesystem>

#include "file_formats.hpp"

namespace maxrf {

void FileWriter::SetUpBuffers(uint scan_line_width) {
  // The buffer holds the pixel data of one line for every data file
  buffer.resize(scan_line_width);
  // The buffer is arranged as such:
  // pos 0 -> buffer of pixel 0
  // pos 1 -> buffer of pixel 1
  // ...
  // pos scan_width - 1 -> buffer of pixel scan_width - 1
  // pos scan_width     -> end of buffer (not an actual pixel)

  // Every pixel buffer reserves at first 1 page of memory (tipically ~4K)
  for (auto & pixel_buff : buffer) {
    pixel_buff.resize(0x1U << 12, 0);
  }
  scan_line_width_ = scan_line_width;
}

template <class P, class T>
void DataCopy(P & ptr, T const & val)  {
  static_assert (std::is_pod_v<T>, "Copied data must be of POD type");
  static_assert (std::is_pointer_v<P>, "P must be of  pointer type");

  memcpy(ptr, &val, sizeof (T));
  ptr += sizeof(T);
}

template <class P, class T>
void BufferRead(P & ptr, T & val)  {
  static_assert (std::is_pod_v<T>, "Copied data must be of POD type");
  static_assert (std::is_pointer_v<P>, "P must be of  pointer type");

  memcpy(&val, ptr, sizeof (T));
  ptr += sizeof(T);
}


void FileWriter::WriteDataToFiles(PixelData & packet)
{
  constexpr uint16_t command_flag  { std::numeric_limits<uint16_t>::max() };
  constexpr uint16_t zero_val         { 0 };

  auto & pixel_buff = buffer.at(packet.coord_x);

  auto buffer_pos = pixel_buff.begin().base();
  auto buffer_end = pixel_buff.end().base();

  DataCopy(buffer_pos, packet.coord_x);
  DataCopy(buffer_pos, packet.coord_y);

  std::uint16_t bin {0};
  for (auto & counts : packet.histogram) {
    if (counts == 0) {
      ++bin;
      continue;
    }
    // Checks the size of the buffer is enough
    if (buffer_end - buffer_pos < 100) {
      auto offset = buffer_pos - pixel_buff.begin().base();
      pixel_buff.resize(pixel_buff.size() * 2);
      buffer_pos = pixel_buff.begin().base() + offset;
      buffer_end = pixel_buff.end().base();
    }

    if (bin != 0){
      if (bin > 2) {
        // A command flag followed by a value means write #value zeroes
        DataCopy(buffer_pos, command_flag);
        DataCopy(buffer_pos, bin);
      }
      else {
        while (bin--) {
          DataCopy(buffer_pos,  zero_val);
        }
      }
      bin = 0;
    }

    DataCopy(buffer_pos, counts);
    counts = 0;
  }
  // Two consecutive command flags mean the end of the pixel
  DataCopy(buffer_pos, command_flag);
  DataCopy(buffer_pos, command_flag);

  // Wrote the buffer

  // If there are exactly scan_width pixels in the buffer, flush the
  // data in the correct order to the data file
  if (++stats.pixels_in_buffer == scan_line_width_) {
    uint32_t bytes_read_4 {0};
    uint16_t bytes_read_2;

    file_ << std::hex << std::uppercase;
    for (auto & pixel_buffer : buffer) {

      buffer_pos = pixel_buffer.begin().base();
      BufferRead(buffer_pos, bytes_read_4);
      file_ << bytes_read_4 << ',';  // Write the x coordinate
      BufferRead(buffer_pos, bytes_read_4);
      file_ << bytes_read_4;         // Write the y coordinate

      bool enable {true};
      while (enable) {
        BufferRead(buffer_pos, bytes_read_2);
        switch (bytes_read_2) {
        case command_flag:
          //  Go back and read 4 bytes instead
          buffer_pos -= 2;
          BufferRead(buffer_pos, bytes_read_4);

          if (bytes_read_4 == std::numeric_limits<uint32_t>::max()) {
            // This is a pixel stop flag
            file_ << std::endl;
            enable = false;
          }
          else {
            // This a zero block flag, the last 2 bytes read tell us the
            // number of zeroes to write
            file_ << ',' << (bytes_read_4 >> 16) << 'R';
          }
          break;
        case 0:
          [[fallthrough]];
        default:
          // This a normal counts value with magnitude >= 0
          file_ << ',' << bytes_read_2;
          break;
        }
      }

      ++stats.pixels_written;
      --stats.pixels_in_buffer;
    }

  }
}


bool MaskedDumpFile::ConvertToHypercube() {
  constexpr int detectors_total = 2;
  constexpr int det0_mask = 20000000;
  constexpr int det1_mask = 30000000;
  constexpr int posx_mask = 50000000;
  constexpr int posy_mask = 60000000;

  PixelData det0_data {};
  PixelData det1_data {};
  det0_data.histogram.resize(16384, 0);
  det1_data.histogram.resize(16384, 0);

  auto filepath = std::filesystem::current_path();
  decltype (filepath) filename;


  int val {0};
  int width {0}, height {0}, start_x {0}, start_y {0}, step_x {0}, step_y {0};


  std::string line {};
  std::getline(file_, line);
  file_ >> val;
  start_x = val;
//    file->EditToken(HeaderTokens::kStartXCoordinate, std::to_string(val / 1000.));
  file_ >> val;
  width   = val - start_x;
//    file->EditToken(HeaderTokens::kEndXCoordinate, std::to_string(val / 1000.));
  file_ >> val;
  start_y = val;
//    file->EditToken(HeaderTokens::kStartYCoordinate, std::to_string(val / 1000.));
  file_ >> val;
  height  = val - start_y;
//    file->EditToken(HeaderTokens::kEndYCoordinate, std::to_string(val / 1000.));
  file_ >> val;
  step_x = val;
//    file->EditToken(HeaderTokens::kMotorStepX, std::to_string(val / 1000.));
  file_ >> val;
  step_y = val;
//    file->EditToken(HeaderTokens::kMotorStepY, std::to_string(val / 1000.));
  file_ >> val;
//    file->EditToken(HeaderTokens::kMotorVelocity, std::to_string(val / 1000.));
  width /= step_x;
  height /= step_y;
  height += 1;

  // Is possible the file doesn't use masks (single-detector mode)
  // We test if this is the case

  bool use_masks {true};

  auto pos = file_.tellg();
  file_ >> val; // coord x
  file_ >> val; // coord y
  file_ >> val; // first_datum

  use_masks = val < det0_mask ? false : true;
  file_.seekg(pos);

  // It is also possible that the width is inclusive of the end x coordinate
  // In that case the width will have to be + 1
  // We scan the file to ascertain the case
  bool watch_next_one {false};
  while (file_ >> val) {
    if ( posx_mask <= val && val < posy_mask) {
      val  -= posx_mask;
      val  = (val - start_x) / step_x;
      if (watch_next_one) {
        width = val == width ? width + 1 : width;
        break;
      }
      if (val == width - 1) {
        watch_next_one = true;
      }
    }
  }
  file_.seekg(pos);

  std::shared_ptr<HypercubeFile> file_det0;
  std::shared_ptr<HypercubeFile> file_det1;
  for (int _= 0; _ < detectors_total; ++_)
  {
    filename = filepath;
    filename /= "_det" + std::to_string(_) + ".hyperc";

    auto ptr = DataFileHander::GetFileForDAQ(filename.string());

    if (ptr->GetFormat() != DataFormat::kHypercube) {
      throw std::runtime_error("Couldn't open output files");
    }

    auto file = std::static_pointer_cast<HypercubeFile>(ptr);

    file->MakeDefaultHeader();

    file->EditToken(HeaderTokens::kImageWidth, std::to_string(width));
    file->EditToken(HeaderTokens::kImageHeight, std::to_string(height));
    file->EditToken(HeaderTokens::kImagePixels, std::to_string(width * height));
    file->WriteHeader();

    file->SetWriteMode(WriteMode::kDAQScan, width);
    if (_ == 0) {
      file_det0 = file;
    }
    else {
      file_det1 = file;
    }
  }

  double progress_step { 100. / (width * height)};
  double progress {0.};

  bool write_preceding {false};
  while (file_ >> val) {
    if (val >= posx_mask) {
      if (write_preceding) {
        file_det0->WritePixel(det0_data);
        file_det1->WritePixel(det1_data);
        progress += progress_step;
        std::cout << "Progress: " << progress << std::endl;
      }
      // New pixel
      val -= posx_mask;
      val  = (val - start_x) / step_x;
      det0_data.coord_x = static_cast<uint32_t>(val);
      det1_data.coord_x = static_cast<uint32_t>(val);

      file_ >> val;
      val -= posy_mask;
      val  = (val - start_y) / step_y;
      det0_data.coord_y = static_cast<uint32_t>(val);
      det1_data.coord_y = static_cast<uint32_t>(val);

      write_preceding   = true;
    }
    else if (!use_masks) {
      det0_data.histogram[val] += 1;
      continue;
    }
    else if (val >= det1_mask) {
      val -= det1_mask;
      det1_data.histogram[val] += 1;
    }
    else if (val >= det0_mask) {
      val -= det0_mask;
      det0_data.histogram[val] += 1;
    }
    else {
      std::cout << "Parsing error at offset: " << file_.tellg() << std::endl;
    }
  }
  // Write last pixel
  file_det0->WritePixel(det0_data);
  file_det1->WritePixel(det1_data);
  file_det0->WriteFooter();
  file_det1->WriteFooter();

  return true;
}


} // namespace maxrf
