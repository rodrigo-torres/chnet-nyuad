#include "MAXRF/xrfimage.h"
#include "MAXRF/conversion_routines.h"
#include "pugi/pugixml.hpp"

#define MAXRF_DEBUG 1

Renderer::Renderer(QWidget * parent) : QObject{parent}
{
//  maximum_integrals_.fill(0);

//  integral_limits_.fill({0, 0});
//  integral_limits_.front() = std::make_pair(0, 16383);
}

Renderer::~Renderer() {}


bool Renderer::is_valid()
{
  return valid_;
}


auto Renderer::err_mesg() -> std::string
{
  return message_;
}

auto Renderer::roi_spectrum() -> Histogram &
{
  return roi_spectrum_;
}

auto Renderer::IsPixelOnImage(int x, int y) -> bool {
  return ( state_.width > x || state_.height > y) ? true : false;
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

void Renderer::ComputeROISpectrum(std::vector<int>&& pixels_selected)
{
  double progress = 0;
  auto & file = state_.file->DataFile();
  file >> std::hex >> std::noskipws;

  for (auto & i : pixels_selected) {
    state_.file->GoToPixel(i);
    state_.file->ParsePixel(roi_operator);

    progress = state_.progress_factor * i;
    emit UpdateProgressBar(static_cast<int>(progress));
  }
  file >> std::dec;

  roi_spectrum_.swap(roi_operator.spectrum);
  emit UpdateProgressBar(0);

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
  std::pair<int, int> limits;
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

void Renderer::UpdatePixelIntegrals(EnergyFilter filter)
{
#ifdef MAXRF_DEBUG
  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
#endif
  static bool kRenderUnfilteredImageOnce {true};

  double progress = 0;
//  auto & index = integral_operator.index ;
  size_t index;

  switch (filter)
  {
  case EnergyFilter::kUnfiltered :
    filtered_operator.result = 0;
    filtered_operator.limits.first = 0;
    filtered_operator.limits.second = 16384;
    index = 0;
    break;
  case EnergyFilter::kFilterRange1 :
    filtered_operator.result = 0;
    filtered_operator.limits.first = state_.integral_limits.at(0).first;
    filtered_operator.limits.second = state_.integral_limits.at(0).second;
    index = 1;
    break;
  case EnergyFilter::kFilterRange2 :
    filtered_operator.result = 0;
    filtered_operator.limits.first = state_.integral_limits.at(1).first;
    filtered_operator.limits.second = state_.integral_limits.at(1).second;
    index = 2;
    break;
  case EnergyFilter::kFilterRange3 :
    filtered_operator.result = 0;
    filtered_operator.limits.first = state_.integral_limits.at(2).first;
    filtered_operator.limits.second = state_.integral_limits.at(2).second;
    index = 3;
    break;
  }


  image_info_.resize(state_.pixels);

  auto & file = state_.file->DataFile();
  file >> std::hex >> std::noskipws;

  for (size_t i = 0; i < state_.pixels; ++i) {
    state_.file->GoToPixel(i);
    state_.file->ParsePixel(filtered_operator);

    if (filtered_operator.result > state_.maximum_integrals[index]) {
      state_.maximum_integrals[index] = filtered_operator.result;
    }
    image_info_[i] = filtered_operator.result;
    filtered_operator.result = 0;

    progress += state_.progress_factor;
    emit UpdateProgressBar(static_cast<int>(progress));
  }

  file >> std::dec;
  emit UpdateProgressBar(0);

#ifdef MAXRF_DEBUG
  auto t2 = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(t2 -t1);
  std::cout <<"Render time duration (ms): "<< duration.count() << std::endl;
#endif
}

auto Renderer::RenderQImage(EnergyFilter filter) -> std::unique_ptr<IndexedImage>
{
#ifdef MAXRF_DEBUG
  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
#endif

//  if (filter != EnergyFilter::kUnfiltered) {
//    UpdatePixelIntegrals(filter);
//  } switch (filter)
  int index;
  switch(filter)
  {
  case EnergyFilter::kUnfiltered :
    index = 0;
    break;
  case EnergyFilter::kFilterRange1 :
    index = 1;
    break;
  case EnergyFilter::kFilterRange2 :
    index = 2;
    break;
  case EnergyFilter::kFilterRange3 :
    index = 3;
    break;
  }
  UpdatePixelIntegrals(filter);

  auto image = std::make_unique<IndexedImage>();
//  image->matrix = QImage{state_.width, state_.height, QImage::Format_Indexed8};
//  image->matrix.setColorCount(256);
  std::fill(image->histogram.begin(), image->histogram.end(), 0);

//  if (state_.maximum_integrals[index] == 0) {
//    image->matrix.fill(Qt::black);
//    return image;
//  }

  double intensity = 0;
  double const coefficient = 255. / state_.maximum_integrals[index];

//  std::pair<int, int> coords;
//  int counter{0};
//  auto data_ptr = image->matrix.bits();
////  for (auto & pixel : data_->vector())
//  // The image is 64 bit aligned, and each pixel writes 8 bits, alignment comes at end?
//  auto alignment_bytes = image->matrix.bytesPerLine() - state_.width;
  auto padded_size = static_cast<size_t>(std::ceil(state_.pixels / 4.0));


  image->indexed_data.reserve(padded_size);
  auto data_ptr = reinterpret_cast<uint8_t *>(image->indexed_data.data());

  for (auto & integral : image_info_)
  {
 //   intensity = pixel.integrals[0] * coefficient;
    intensity = integral * coefficient;

    ++image->histogram.at(static_cast<size_t>(intensity));

    *data_ptr = static_cast<unsigned char>(intensity);
    ++data_ptr;
//    if (state_.width == ++counter) {
//      // Skip the null character termination for the line
//      //++data_ptr;
//      data_ptr += alignment_bytes;
//      counter = 0;
//    }
  }

#ifdef MAXRF_DEBUG
  auto t2 = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(t2 -t1);
  std::cout <<"Rendering time duration (ms): "<< duration.count() << std::endl;
#endif
  image->dimensions = QSize{state_.width, state_.height};
  return image;
}


void Renderer::LoadHypercube(std::string filename)
{
  message_  = "[!] This file is not an XML file with Hypercube data";
  valid_ = false;

  auto ptr = maxrf::DataFileHander::GetFile(filename);
  RendererState state;

  switch (ptr->GetFormat())
  {
  case maxrf::DataFormat::kInvalid :
    // The file is invalid
    return;
  case maxrf::DataFormat::kHypercube :
    // If the file is a Hypercube, it's safe to cast to derived class pointer
    state.file = std::static_pointer_cast<maxrf::HypercubeFile>(ptr);
    break;
  default:
    // For all other cases attempt a conversion
    return;
  }


  state.file->ExtractHeader();
  try {
    // Temporary object is moved by copy elision
    state.file->ComputeLookupTable();
  } catch (...) {
    return;
  }

  state.width  =
      std::stoi(state.file->GetTokenValue(maxrf::HeaderTokens::kImageWidth));
  state.height =
      std::stoi(state.file->GetTokenValue(maxrf::HeaderTokens::kImageHeight));
  state.pixels =
      std::stoul(state.file->GetTokenValue(maxrf::HeaderTokens::kImagePixels));
  state.progress_factor = (100. / state.pixels);


  // The state is valid so we assing it as the current state
  state_ = state;
  // And we add a snapshot to the list
  snapshots_.push_back(state);

  valid_ = true;
}
