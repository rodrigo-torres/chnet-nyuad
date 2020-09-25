#ifndef XRFIMAGE_H
#define XRFIMAGE_H

#include <algorithm>  // For maximum element algorithm
#include <chrono>     // Timers for debugging purposes
#include <cmath>      // ceil functions
#include <fstream>
#include <filesystem>
#include <memory>     // Smart pointers
#include <string>
#include <iostream>
#include <vector>

#include <QDebug>
#include <QObject>
#include <QPoint>

#include "MAXRF/file_management.hpp"

using Histogram   = std::array<int32_t, 16384>;

///
/// \brief The Render struct is the final product of the Renderer class, a data
/// structure containing an 8-bit indexed image arranged on a 1D vector, with
/// each succesive line following the previous one in sequence
///
/// Each render occupies ~263 bytes in memory + 1 byte per pixel + filename
///
struct Render
{
  std::string filename;

  std::vector<std::byte>  indexed_data; ///< 8-bit matrix index data
  std::array<int, 256>    matrix_histogram;
  std::pair<int, int>     dimensions {0, 0};
  std::pair<int, int>     integral_limits   {0, 16384};
  std::pair<int, int>     intensity_cutoffs   {0, 255};
  int max_integral {0};
  int id {0};
};


class Renderer : public QObject
{
  Q_OBJECT

public:
  class Memento;


  explicit Renderer(QObject * parent = nullptr);
  ~Renderer() = default;


  ///
  /// \brief Open and load a data file with internal 'Hypercube' format with
  /// strong exception safety.
  /// \param filename
  ///
  void LoadHypercube(std::string filename);

  ///
  /// \brief Render an energy (un)filtered image from the current @ref RendererState
  /// \param filter
  /// \return
  ///
  auto RenderXRFImage() -> std::shared_ptr<Render const>;

  ///
  /// \brief Set the renderer state to one of its snapshot values
  /// \param index
  ///
  void RestoreState(Memento) noexcept;

  ///
  /// \brief SaveState
  /// \return
  ///
  Memento SaveState() noexcept;

  void SetIntegralLimits(int low, int high) {
    render_.integral_limits = {low, high};
  }

  void SetIntensityCutoffs(int low, int high) {
    render_.intensity_cutoffs = {low, high};
  }

  Histogram ComputeROISpectrum(std::array<QPoint, 4> corners) const;

  Histogram GetSumSpectrum() const;

signals:
  void UpdateProgressBar(int value) const;

private:
  // Initialization and other private methods
  void UpdatePixelIntegrals();




  ///
  /// \brief The State struct holds the internal state variables that are
  /// 'saved' whenever the Renderer class creates a Memento. Each state
  /// is tied to an opened MAXRF data file
  ///
  struct State {
    /// File with data
    std::shared_ptr<maxrf::HypercubeFile> file {nullptr};
    std::filesystem::path file_path;
    int state_id ;

    int width;
    int height;
    int pixels;
    Histogram sum_spectrum;

  } state_ {}; ///< The current state of the Renderer class

  Render  render_ {}; ///< The active render of the state
  std::vector<int32_t> buffer_ {};
};

class Renderer::Memento {
  friend class Renderer;

  // Private default and copy/copy-assignment constructors
  Memento(State state) : state_ {state} {}


  // We don't provide access points of no kind to the data to make sure that
  // the renderer class is the only one that can access the data
  State const state_;

public:
  Memento(Memento const & other) : state_ {other.state_} {}
  Memento & operator= (Memento const & other) = delete;

  ///
  /// \brief StateID returns the ID of the state stored by the Memento class.
  /// This is useful to determine whether a given \see Render was produced
  /// by the \see Renderer class with this state. If such is the case, the
  /// IDs of both the Renderer State and the Render should match;
  ///
  auto StateID() const noexcept {
    return state_.state_id;
  }

  ///
  /// \brief FilePath returns the filepath associated with the data file
  /// descriptor stored by the Memento class.
  ///
  auto FilePath() const noexcept {
    return state_.file_path;
  }
};

#endif // XRFIMAGE_H
