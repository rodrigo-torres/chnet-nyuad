#include "user_interface.h"

UserInterface::UserInterface(QWidget *parent) : QWidget(parent)
{
  CreateUI();
}

auto UserInterface::CreateUI() -> void
{

  auto add_field = new QPushButton{"Add"};
  add_field->setIcon(QIcon::fromTheme("list-add"));

  auto remove_field = new QPushButton{"Remove"};
  remove_field->setIcon(QIcon::fromTheme("list-remove"));

  auto show_raw = new QCheckBox{"Show Header"};
  auto enable_edit = new QCheckBox{"Edit"};

  auto controls_layout = new QHBoxLayout;
  controls_layout->addWidget(add_field);
  controls_layout->addWidget(remove_field);
  controls_layout->addWidget(show_raw);
  controls_layout->addWidget(enable_edit);

  // FORM DISPLAY WIDGET
  // When we first construct the widget it'll be empty
  auto widget_1 = new QWidget;
  form_= new QFormLayout;

  widget_1->setLayout(form_);
  widget_1->setEnabled(false);
  widget_1->setObjectName("Header Fields Form");

  QObject::connect(add_field, &QPushButton::clicked,
                   [=] () {
    QRegExp regexp{"^[a-zA-Z0-9_+-\\.]+$"};
    auto validator = new QRegExpValidator{regexp};

    auto field_descriptor = new QLineEdit;
    field_descriptor->setValidator(validator);
    field_descriptor->setPlaceholderText("Enter a field description");
    auto field_value = new QLineEdit;
    field_value->setValidator(validator);
    field_value->setPlaceholderText("Enter the field value");

    auto layout = dynamic_cast<QFormLayout *>(widget_1->layout());
    layout->addRow(field_descriptor, field_value);

    widget_1->setLayout(layout);
  });

  QObject::connect(remove_field, &QPushButton::clicked,
                   [=] () {
    auto layout = dynamic_cast<QFormLayout *>(widget_1->layout());
    auto rows = layout->rowCount();
    if (rows != 0)
    {
     // Remove the last row
     layout->removeRow(rows - 1);
    }
    widget_1->setLayout(layout);
  });

  // HEADER DISPLAY WIDGET
  auto widget_2 = new QWidget;

  text_editor_ = new QTextEdit{};
  text_editor_->setObjectName("Text Editor");
  auto confirm_button = new QPushButton{"Update Fields Form"};

  auto text_editor_layout = new QVBoxLayout;
  text_editor_layout->addWidget(confirm_button, Qt::AlignRight);
  text_editor_layout->addWidget(text_editor_);

  widget_2->setLayout(text_editor_layout);
  widget_2->setEnabled(false);

  // STACKED WIDGET

  auto stacked_widget = new QStackedWidget;
  stacked_widget->addWidget(widget_1);
  stacked_widget->addWidget(widget_2);

  QObject::connect(show_raw, &QCheckBox::toggled,
                   [=] (bool checked) {
    if (checked)
    {
      QString header{};
      auto layout = dynamic_cast<QFormLayout *>(widget_1->layout());
      for (int i = 0; i < layout->rowCount(); ++i)
      {
        auto item = layout->itemAt(i, QFormLayout::LabelRole);
        auto pointer = dynamic_cast<QLineEdit *>(item->widget());
        header.append(pointer->text());
        header.append(" = ");

        item = layout->itemAt(i, QFormLayout::FieldRole);
        pointer = dynamic_cast<QLineEdit *>(item->widget());
        header.append(pointer->text());
        header.push_back('\n');
      }
      text_editor_->setText(header);
    }

    stacked_widget->setCurrentIndex(checked);
  });


  auto save = new QPushButton{"Confirm and Save Header"};

  auto widget_layout = new QVBoxLayout;
  widget_layout->addLayout(controls_layout);
  widget_layout->addWidget(stacked_widget);
  widget_layout->addWidget(save);


  QObject::connect(enable_edit, &QCheckBox::toggled,
                   [=] (bool checked) {
    widget_1->setEnabled(checked);
    widget_2->setEnabled(checked);
    save->setEnabled(checked);
  });


  this->setLayout(widget_layout);
  this->setWindowTitle("XRF Image Header Editor");
}

auto UserInterface::UpdateTextEditor(QString const header) -> void
{
  auto text_editor = this->findChild<QTextEdit *>("Text Editor");
  text_editor->setText(header);
};

auto UserInterface::UpdateFieldsEditor(QStringList & fields) -> void
{

  auto fields_form = this->findChild<QWidget *>("Header Fields Form");
  auto layout = dynamic_cast<QFormLayout *>(fields_form->layout());
  delete layout;
  layout = new QFormLayout;

  QStringList tokens{};
  for (int i = 0; i < fields.count(); ++i)
  {
    tokens = fields.at(i).split(" = ");
    auto field_descriptor = new QLineEdit{tokens.at(0)};
    auto field_value = new QLineEdit {tokens.at(1)};

    layout->addRow(field_descriptor, field_value);
  }
  fields_form->setLayout(layout);
}
