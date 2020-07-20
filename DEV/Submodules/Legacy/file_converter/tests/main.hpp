#ifndef MAIN_HPP
#define MAIN_HPP

#include <chrono>
#include <iostream>

#include <QApplication>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>
#include <QProgressBar>
#include <QStatusBar>

#include "../src/file_converter.h"

class UserInterface : public QWidget
{
  Q_OBJECT
public:
  explicit UserInterface(QWidget * parent = nullptr)
  {
    Q_UNUSED(parent)
    CreateGUI();
  }

private:
  auto CreateGUI() -> void
  {
    auto label = new QLabel{"File to convert:"};
    line_edit = new QLineEdit;
    line_edit->setPlaceholderText("Select a file to convert");
    auto push_button = new QPushButton{"Browse"};

    auto layout1 = new QHBoxLayout;
    layout1->addWidget(label);
    layout1->addWidget(line_edit, 1); // Give priority stretch to this Widget
    layout1->addWidget(push_button);

    progress_bar = new QProgressBar;
    progress_bar->setRange(0, 100);

    status_bar = new QStatusBar;

    auto master_layout = new QVBoxLayout;
    master_layout->addLayout(layout1);
    master_layout->addWidget(progress_bar);
    master_layout->addWidget(status_bar);

    // Then we do the connections
    connect(push_button, &QPushButton::clicked, this, &UserInterface::Convert);

    setLayout(master_layout);
    show();
  }

  void Convert()
  {
    auto name = QFileDialog::getOpenFileName(this);
    maxrf::FileConverter converter{name.toStdString()};

    auto pos = name.lastIndexOf('/');
    name = name.right(name.size() - pos);
    line_edit->setText(name);


    connect(&converter, &maxrf::FileConverter::UpdateProgressBar,
            progress_bar, &QProgressBar::setValue);

    connect(&converter, &maxrf::FileConverter::UpdateStatusBar,
            this, [&](std::string message){
      status_bar->showMessage(QString::fromStdString(message));
    });

    using namespace std::chrono;
    auto t1 = high_resolution_clock::now();
    converter.ConvertToHypercube();
    auto t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t2 - t1);
    std::cout << "Conversion time: " << duration.count() << std::endl;

    progress_bar->setValue(0);
  }
  QLineEdit * line_edit;
  QProgressBar * progress_bar;
  QStatusBar * status_bar;
};



#endif // MAIN_HPP
