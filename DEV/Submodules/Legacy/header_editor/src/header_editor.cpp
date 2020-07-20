#include "header_editor.h"
#include "user_interface.h"

HeaderEditor::HeaderEditor(QWidget * parent)
{
  ui_ = new UserInterface(parent);
  auto filename = QFileDialog::getOpenFileName();
  ExtractHeader(filename.toStdString());
  ui_->show();
}

HeaderEditor::HeaderEditor(std::string header, QWidget * parent) : header_{header}
{
  ExtractHeader(header);
  ui_ = new UserInterface(parent);
}


auto HeaderEditor::ExtractHeader(std::string filename) -> void
{
  using std::getline;

  header_fields_.clear();

  std::string line{};
  std::ifstream file{filename};

  if (!file.is_open()) { return; }

  // Case where we have a Optimized Single Detector Format File
  getline(file, header_);
  if (header_.front() == '{')
  {
    getline(file, header_, '}');
    header_fields_= QString::fromStdString(header_).split('\n');
    // The last token will always be empty when we spli with delimiter '\n'
    header_fields_.pop_back();

    for (int i = 0; i < header_fields_.size(); ++i)
    {
      if (header_fields_.at(i).contains(" = "))
      {
        header_fields_.removeAt(i);
      }
    }
    ui_->UpdateTextEditor(QString::fromStdString(header_));
    ui_->UpdateFieldsEditor(header_fields_);
  }
}
