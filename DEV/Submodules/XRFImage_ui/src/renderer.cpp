/** MAXRF_IMAGE: Visualize and analyze XRF scanimages
 *  Copyright (C) 2020 Rodrigo Torres (rodrigo.torres@nyu.edu) and contributors
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
//#include "conversion_routines.h"

#include "renderer.h"
#include "pugi/pugixml.hpp"

void Renderer::RestoreState(Memento past) noexcept {
  state_ = past.state_;
}

Renderer::Memento Renderer::SaveState() noexcept {
  return Memento {state_};
}

Renderer::Renderer(QObject * parent) : QObject {parent} {}

//bool Renderer::is_valid() const {
//  return valid_;
//}


//std::string Renderer::err_mesg() const {
//  return message_;
//}

Histogram Renderer::GetSumSpectrum() const{
  return state_.sum_spectrum;
}


static struct ROIOperator
{
  ROIOperator() {
    spectrum.fill(0);
  }
  auto operator() (int bin, int value) -> bool  {
    spectrum[bin] += value;
    return false;
  }
  std::array<int, 16384> spectrum;
} roi_operator;

Histogram Renderer::ComputeROISpectrum(std::array<QPoint, 4> corners) const
{
  for (auto & point : corners) {
    if (point.x() >= state_.width || point.y() >= state_.height) {
      throw std::range_error("Coordinates are in scan range.");
    }
  }

  std::pair<int, int> x_maxima;
  std::pair<int, int> y_maxima;
  // Find one of the two points with the largest y coordinate
  auto it = std::minmax_element(corners.begin(), corners.end(),
                                 [](const auto & lhs, const auto & rhs)
                                     { return lhs.x() < rhs.x(); });
  x_maxima = {it.first->x(), it.second->x()};

  it = std::minmax_element(corners.begin(), corners.end(),
                                 [](const auto & lhs, const auto & rhs)
                                     { return lhs.y() < rhs.y(); });
  y_maxima = {it.first->y(), it.second->y()};

  double const progress_step = 100. / state_.pixels;
  double progress = 0;

  auto & file = state_.file->DataFile();
  file >> std::hex >> std::noskipws;

  roi_operator.spectrum.fill(0);
  for (auto j = y_maxima.first; j < y_maxima.second + 1; ++j) {
    for (auto i = x_maxima.first; i < x_maxima.second + 1; ++i) {
      state_.file->GoToPixel(i + j * state_.width);
      state_.file->ParsePixel(roi_operator);

      progress = progress_step * i;
      emit UpdateProgressBar(static_cast<int>(progress));
    }
  }
  file >> std::dec;
  emit UpdateProgressBar(0);

  return roi_operator.spectrum;
}

static struct IntegralFilteredParse
{
  IntegralFilteredParse() : result{0} {}
  auto operator() (int bin, int value) -> bool  {
    if (limits.first < bin && bin < limits.second) {
      result += value;
    }
    return false;
  }
  int result;
  std::pair<int, int> limits {0, 16384};
} filtered_operator;

static struct IntegralParse
{
  IntegralParse() : result{0} {}
  auto operator() (int bin, int value) -> bool  {
    result += value;
    return false;
  }
  int result;
} integral_operator;



void Renderer::UpdatePixelIntegrals()
{
  filtered_operator.limits = render_.integral_limits;
  render_.max_integral = 0;

  double const progress_step = 100. / state_.pixels;
  double progress = 0;

  buffer_.resize(state_.pixels, 0);
  auto & file = state_.file->DataFile();
  file >> std::hex >> std::noskipws;


  std::size_t pixel {0};
  for (auto & integral : buffer_) {
    state_.file->GoToPixel(pixel);

    state_.file->ParsePixel(filtered_operator);

    if (filtered_operator.result > render_.max_integral) {
      render_.max_integral = filtered_operator.result;
    }
    integral = filtered_operator.result;
    filtered_operator.result = 0;

    progress += progress_step;
    emit UpdateProgressBar(static_cast<int>(progress));
    ++pixel;
  }

  file >> std::dec;
  emit UpdateProgressBar(0);
}

auto Renderer::RenderXRFImage() -> std::shared_ptr<Render const>
{
  using namespace std::chrono;
  // The idea is that when this function is called, the state of the member
  // active_render_ variable is valid and will be the starting point for
  // the Render object we will return
  render_.filename = std::filesystem::path {state_.file_path}.filename();

  std::fill(render_.matrix_histogram.begin(),
            render_.matrix_histogram.end(), 0);

  render_.dimensions= {state_.width, state_.height};

  // The spectral bin_limits and the pixel_integra_cutoffs will have been
  // filled in by the class already

  // We update only the maximum_integral
  auto t1 = steady_clock::now();
  UpdatePixelIntegrals();
  auto t2 = steady_clock::now();
  qDebug() << "Rendering took (ms): "
           << duration_cast<milliseconds>(t2 - t1).count();

  // We resize the index data vector to the total number of pixels
  render_.indexed_data.resize(render_.dimensions.first *
                                     render_.dimensions.second,
                                     std::byte {0});

  if (render_.max_integral == 0) {
    // The image is empty and we return it as it is
    return std::make_shared<Render const>(render_);
  }


  // Otherwise we calculate the normalized integrals for each pixel
  unsigned char gray_level = 0;
  double const normalization = 255. / render_.max_integral;

  // TODO check the special case where the maximum_integral is zero
  auto data = render_.indexed_data.data();
  for (auto & pixel_data : buffer_)  {
    gray_level = static_cast<unsigned char>(pixel_data * normalization);
    ++render_.matrix_histogram.at(gray_level);

    *data = static_cast<std::byte>(gray_level);
    ++data;
  }

  return std::make_shared<Render const>(render_);
}


void Renderer::LoadHypercube(std::string filename)
{

  State state;
//  message_  = "[!] This file is not an XML file with Hypercube data";
//  valid_ = false;

  auto ptr = maxrf::DataFileHander::GetFile(filename);

  if (ptr->GetFormat() != maxrf::DataFormat::kHypercube) {
    throw ("The file is not in the .hyperc format. You may convert it first");
  }
  // If the file is a Hypercube, it's safe to cast to derived class pointer
  state.file = std::static_pointer_cast<maxrf::HypercubeFile>(ptr);


  state.file->ExtractHeader();
  try {
    // Temporary object is moved by copy elision
    state.file->ComputeLookupTable();
  } catch (std::exception const &) {
    // We throw to force the caller to log the exception
    throw;
  } catch (...) {
    throw std::runtime_error("Unknown exception occured in LUT computation.");
  }
  state.file_path = filename;

  state.width =
      std::stoi(state.file->GetTokenValue(maxrf::HeaderTokens::kImageWidth));
  state.height =
      std::stoi(state.file->GetTokenValue(maxrf::HeaderTokens::kImageHeight));
  state.pixels =
      std::stoi(state.file->GetTokenValue(maxrf::HeaderTokens::kImagePixels));

  // The state is valid so we assign it as the current state
  state_ = std::move(state);
//  valid_ = true;
}
