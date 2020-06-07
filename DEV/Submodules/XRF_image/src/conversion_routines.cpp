/*
 * conversion_routines.cpp
 *
 *  Created on: Jun 5, 2020
 *      Author: rt135
 */

#include "MAXRF/conversion_routines.h"

FileConverter::FileConverter(std::string filename) : error{}, data_file_{}
{
	error = filename.empty();
	if (error)
	{
		err_message_ = "[!] Invalid filename";
	}
	else
	{
		data_file_.open(filename);
		error = data_file_.is_open();
		if (error)
		{
      err_message_ = "[!] Can't open file";
		}
	}
}

auto FileConverter::ConvertToHypercube() -> std::string
{

  int version = 5;
	switch (version)
	{
	case 0:
    LegacyFormatToHypercube();
		break;
	case 1:
		MultiDetectorFormatToHypercube();
		break;
	case 2:
		OptimizedSingleDetectorFormatToHypercube();
		break;
	case 3:
		error = true;
    err_message_ = "[!] The selected file is already a hypercube";
    break;
	default:
		error = true;
    err_message_ = "[!] Unrecognized data format";
		break;

	}
	return err_message_;
}

auto FileConverter::LegacyFormatToHypercube() -> void
{

}

auto FileConverter::MultiDetectorFormatToHypercube() -> void
{

}

auto FileConverter::OptimizedSingleDetectorFormatToHypercube() -> void
{

}

