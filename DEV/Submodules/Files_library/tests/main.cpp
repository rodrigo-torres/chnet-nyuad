/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include <chrono>

#include "MAXRF/file_management.hpp"

int main (int argc, char * argv[])
{
  static std::string const filename =
      "/home/frao/Documents/Workspaces/MAXRF/Data/LAD/LAD_2014_024/LAD-2014-024-middle/original/LAD_2014_024_20191125_1703_30kV80uA_190x176y_middle_det0.hyperc";

  auto file = maxrf::DataFileHander::GetFile(filename);
  if (file->GetFormat() == maxrf::DataFormat::kInvalid) {
    return 1;
  }

  file->ExtractHeader();

  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();

  try {
    // Temporary object is moved by copy elision
    maxrf::LookupTable lut = file->ComputeLookupTable();
  } catch (...) {
    return 1;
  }

  auto t2 = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(t2 - t1);
  std::cout << duration.count() << std::endl;

  auto width = std::stoi(file->GetTokenValue(maxrf::HeaderTokens::kImageWidth));
  auto height= std::stoi(file->GetTokenValue(maxrf::HeaderTokens::kImageHeight));
  auto pixels= std::stoi(file->GetTokenValue(maxrf::HeaderTokens::kImagePixels));
  pixels = width * height;

  return 1;
}

