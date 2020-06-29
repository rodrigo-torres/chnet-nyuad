#include "MAXRF/image_display.h"
#include "palettes.h"

constexpr static int kLevelsOfGray = 0x100;

namespace maxrf {

ImgLabel::ImgLabel(QWidget * parent) :
  left_mouse_clicked_ {false}, coordinates_found_ {false},
  map_opened_ {false}, renderer_{}, parent_(parent)
{
  shared_memory_cmd.initialize(Segments::SHARED_MEMORY_CMD);
  shared_memory.initialize(Segments::SHARED_MEMORY_1);

//  CreatePalettes();

  displayed_histo_.resize(kLevelsOfGray);

  connect(&renderer_, &Renderer::UpdateProgressBar,  this,
           &ImgLabel::RelayProgressBarSignal);
}

ImgLabel::~ImgLabel() {}

void ImgLabel::AddImageToBuffer()
{
  QString filename = QFileDialog::getOpenFileName(
        nullptr, "Open XRF Image Data File", data_directory);

  if (filename.isEmpty())
  {
    QMessageBox::warning(this, "Warning!",
                         "You have chosen an empty filename.\n"
                         "No image data has been loaded onto memory.");
    return;
  }


  renderer_.LoadHypercube(filename.toStdString());
  if (renderer_.is_valid())
  {
    images_.emplace_back();
    image_index_ = images_.size() - 1;
    //RenderImage(EnergyFilter::kUnfiltered);

    auto pos = filename.lastIndexOf('/');
    emit AddToComboBox(filename.mid(pos));
    emit EnableImageControls();
  }
  else
  {
    QMessageBox::critical(this, "Error!",
                          QString::fromStdString(renderer_.err_mesg()));
    map_opened_ = false;
  }
}

void ImgLabel::ChangeImage(int index)
{
  if (index == 0) {
    // Show the logo
    QImage image(":/images/TT_CHNet_res2.png");
    setPixmap(QPixmap::fromImage(image));
    setBackgroundRole(QPalette::Base);
    resize(image.size());
  }
  else {
    // Change the index
    image_index_ = index - 1;
    //displayed_image_ = images_.at(image_index_)->matrix;
    auto const & color_table = palettes_.find(selected_palette);
    displayed_image_.setColorTable(color_table->second);

    renderer_.ChangeState(image_index_);
    ProcessImage();
  }
}

bool ImgLabel::is_map_opened() const
{
  return map_opened_;
}

QImage ImgLabel::qimage() const
{
  return displayed_image_;
  //return qimage_;
}

//void ImgLabel::set_pixel_dim(int dim)
//{
//  images_.at(image_index_)->pixel_replication = dim;
//}

void ImgLabel::ToggleEqualization (bool state)
{
  if (state) {
    images_.at(image_index_)->flags |= kEqualize;
    images_.at(image_index_)->flags &= ~kStretch;
  }
  else {
    images_.at(image_index_)->flags &= ~kEqualize;
  }
  ProcessImage();
}

void ImgLabel::ToggleStretching(bool state)
{
  if (state) {
    images_.at(image_index_)->flags |= kStretch;
    images_.at(image_index_)->flags &= ~kEqualize;
  }
  else {
    images_.at(image_index_)->flags &= ~kStretch;
  }
  ProcessImage();
}

void ImgLabel::set_brightness(int percentage)
{
  // Values are constrained between -50% and +50%
  images_.at(image_index_)->brightness = static_cast<int>(percentage * 2.56);
  if (percentage == 0) {
    images_.at(image_index_)->flags &= ~kBrighten;
  }
  else {
    images_.at(image_index_)->flags |= kBrighten;
  }
  ProcessImage();
}

//void ImgLabel::RenderImage(EnergyFilter filter)
//{
//  auto image = renderer_.RenderQImage(filter);

//  // Get a list of available palettes and ask the user to select one
//  QStringList available_palettes{};
//  for (auto & it : palettes_)
//  {
//    available_palettes.append(it.first);
//  }
//  bool valid {false};
//  QString item =
//      QInputDialog::getItem(this, "Color Palette", "Using color palette:",
//                            available_palettes, 0, false, &valid);

//  // Since we just rendered the image, there's no color palette active on the
//  //  image. So if the user cancels we assign the default one
//  // TODO this could be improved by using the last used value
//  selected_palette = (valid == true) ? item : available_palettes.at(0);

//  auto & palette = palettes_.find(selected_palette)->second;
//  //displayed_image_ = image->matrix;
//  displayed_image_.setColorTable(palette);
////  image->processed.setColorTable(palette);
//  images_.at(image_index_).swap(image);
//}

//void ImgLabel::CreatePalettes()
//{
//  QColor color;
//  ColorTable grayscale;
//  // A color-blind friendly palette
//  ColorTable viridis_palette;

//  using namespace maxrf::palettes;

//  // We use 8-bit color palettes
//  for (int i = 0; i < 0x100; ++i)
//  {
//    color.setRgb(i, i, i, 255);
//    grayscale.push_back(color.rgb());

//    color.setRgb(_viridis_data[i][0] * 255, _viridis_data[i][1] * 255,
//        _viridis_data[i][2] * 255, 255);
//    viridis_palette.push_back(color.rgb());
//  }

//  palettes_.insert(std::make_pair("Grayscale", grayscale));
//  palettes_.insert(std::make_pair("Viridis", viridis_palette));
//}

} // namespace maxrf
