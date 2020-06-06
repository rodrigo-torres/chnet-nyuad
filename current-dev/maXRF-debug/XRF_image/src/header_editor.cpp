#include "header_editor.h"

HeaderEditor::HeaderEditor(QWidget * parent) : parent_{parent} {

}

HeaderEditor::HeaderEditor(std::string header, QWidget * parent) : header_{header},
		parent_{parent}
{
	CreateDialog();
}

auto HeaderEditor::CreateDialog() -> int void
{
	UpdateFields();
	if (header_fields.isNull)
	{
		emit reject();
	}

	auto enable_edit = QCheckBox{"Edit"};
	auto show_raw = QCheckBox{"Show raw header"};

	auto form_layout = Q


	this->setTitle("XRF Image Header Editor");
	this->show();
}
