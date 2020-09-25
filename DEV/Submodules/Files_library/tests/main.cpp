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

#include <filesystem>

#include "MAXRF/file_management.hpp"

//#define DEBUG

int main (int argc, char * argv[])
{
  using namespace maxrf;

#ifndef DEBUG
  if (argc < 2) {
    std::cout << "This program takes a filepath as its first argument.\n"
                 "No other arguments are accepted." << std::endl;
    return 1;
  }

  std::error_code err;
  std::filesystem::path filepath { argv[1] };
  if (std::filesystem::is_regular_file(filepath, err) == false) {
    std::cout << "The file you selected is not a regular file: "
              << err.message() << std::endl;
    return 1;
  }
#else
  std::filesystem::path filepath {"mao1_XY_camp9_25kV_100uA_Pompei.txt"};
#endif

  auto file = DataFileHander::GetFile(filepath.string());

  if (file->GetFormat() == DataFormat::kMultiDetectorMaskedDump) {
    file->ConvertToHypercube();
  }
  else {
    std:: cout << "You have selected a file with a format whose conversion"
                  " is not yet supported" << std::endl;
    return 1;
  }

  return 0;
}

