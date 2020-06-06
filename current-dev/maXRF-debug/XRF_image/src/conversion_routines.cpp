/*
 * conversion_routines.cpp
 *
 *  Created on: Jun 5, 2020
 *      Author: rt135
 */

#include "conversion_routines.h"

auto FileConverter::FileConverter(std::string filename) : error{}, data_file_{}
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
			err_message = "[!] Can't open file";
		}
	}
}

auto FileConverter::ConvertToHypercube() -> std:string
{

	int version;
	switch (version)
	{
	case 0:
		LegacyFormatToHyperCube();
		break;
	case 1:
		MultiDetectorFormatToHypercube();
		break;
	case 2:
		OptimizedSingleDetectorFormatToHypercube();
		break;
	case 3:
		error = true;
		err_message = "[!] The selected file is already a hypercube"
	default:
		error = true;
		err_message = "[!] Unrecognized data format";
		break;

	}
	return err_message_;
}

auto FileConverter::OptimizedSingleDetectorFormatToHypercube() -> void
{

}

auto FileConverter::MultiDetectorFormatToHypercube() -> void
{

}
