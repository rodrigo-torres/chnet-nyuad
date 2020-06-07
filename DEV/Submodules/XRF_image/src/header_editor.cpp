#include "MAXRF/header_editor.h"

HeaderEditor::HeaderEditor(QWidget * parent) : parent_{parent} {

}

HeaderEditor::HeaderEditor(std::string header, QWidget * parent) : header_{header},
		parent_{parent}
{
	CreateDialog();
}

auto HeaderEditor::CreateDialog() ->  void
{
	UpdateFields();
  if (header_fields_.isEmpty())
	{
		emit reject();
	}

  auto enable_edit = new QCheckBox{"Edit"};
  auto show_raw = new QCheckBox{"Show raw header"};

  auto form_layout = new QFormLayout;


  this->setWindowTitle("XRF Image Header Editor");
	this->show();
}

auto HeaderEditor::UpdateFields() -> void
{

}
