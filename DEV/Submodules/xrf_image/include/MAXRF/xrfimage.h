#ifndef XRFIMAGE_H
#define XRFIMAGE_H

#include <algorithm>  // For maximum element algorithm
#include <chrono>     // Timers for debugging purposes
#include <cmath>      // ceil functions
#include <fstream>
#include <memory>     // Smart pointers
#include <string>
#include <iostream>
#include <vector>

#include <QInputDialog>
#include <QProgressBar>
#include <QObject>
#include <QString>

#include <sys/stat.h> // For file information

#include "MAXRF/types.h"
#include "MAXRF/file_management.hpp"

constexpr static int kMaximumFilters = 3;

enum class EnergyFilter : char
{
  kUnfiltered = 0,
  kFilterRange1,
  kFilterRange2,
  kFilterRange3
};



struct RendererState
{
  using Pair      = std::pair<int, int>;
  using List      = std::array<int, kMaximumFilters + 1>;
  using PairList  = std::array<Pair, kMaximumFilters>;

  RendererState() {
    maximum_integrals.fill(0);
    integral_limits.fill(Pair {0, std::numeric_limits<int>::max()});
    cutoff_bins = Pair {0, std::numeric_limits<int>::max()};

  }

  /// File with data
  std::shared_ptr<maxrf::HypercubeFile> file;

  /// Dimensions information about the image
  int width;
  int height;
  size_t pixels;

  /// Integrals information for rendering
  List      maximum_integrals;
  PairList  integral_limits;
  Pair      cutoff_bins;

  /// Progress step for the QStatusBar
  double progress_factor;
};


class Renderer : public QObject
{
  Q_OBJECT

  using StateSnapshots = std::vector<RendererState>;

public:
  explicit Renderer(QWidget * parent = nullptr);
  ~Renderer();


  ///
  /// \brief Open and load a data file with internal 'Hypercube' format
  /// \param filename
  ///
  void LoadHypercube(std::string filename);

  ///
  /// \brief Render an energy (un)filtered image from the ]current @ref RendererState
  /// \param filter
  /// \return
  ///
  auto RenderQImage(EnergyFilter filter) -> std::unique_ptr<IndexedImage>;

  ///
  /// \brief Set the renderer state to one of its snapshot values
  /// \param index
  ///
  inline void ChangeState(int index) {
    state_ = snapshots_.at(static_cast<size_t>(index));
  }

  void SetIntegralLimits(EnergyFilter filter, int low, int high) {
    switch (filter)
    {
    case EnergyFilter::kFilterRange1 :
      state_.integral_limits.at(0).first = low;
      state_.integral_limits.at(0).second = high;
      break;
    case EnergyFilter::kFilterRange2 :
      state_.integral_limits.at(1).first = low;
      state_.integral_limits.at(1).second = high;
      break;
    case EnergyFilter::kFilterRange3 :
      state_.integral_limits.at(2).first = low;
      state_.integral_limits.at(2).second = high;
      break;
    case EnergyFilter::kUnfiltered :
      break;
    }
  }

  auto IsPixelOnImage(int x, int y) -> bool;
  void ComputeROISpectrum(std::vector<int>&& pixels_selected);


  // ACCESOR METHODS
  auto err_mesg() -> std::string;
  auto is_valid() -> bool;
  auto sum_spectrum() -> Histogram &;
  auto roi_spectrum() -> Histogram &;

signals:
  void UpdateProgressBar(int value);

private:
  // Initialization and other private methods
  void UpdatePixelIntegrals(EnergyFilter);


  RendererState   state_;
  StateSnapshots  snapshots_;


  // Parent communication
  bool valid_;
  std::string message_;

  Histogram roi_spectrum_;
  Histogram sum_spectrum_;

  std::vector<uint32_t> image_info_;
};

#endif // XRFIMAGE_H
