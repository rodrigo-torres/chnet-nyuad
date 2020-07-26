/** MAXRF_DAQ: Manage XRF DAQ sessions with the CHNET built MAXRF Scanner
 *  Copyright (C) 2020 Rodrigo Torres and contributors
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

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <iostream>
#include <filesystem>
#include <memory>
#include <type_traits>

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDoubleValidator>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QStatusBar>
#include <QTabWidget>
#include <QStackedWidget>
#include <QTimer>
#include <QWidget>

#include <sys/stat.h>   // For file statistics
#include <sys/types.h>
#include <unistd.h>

#include "MAXRF/daq_types.h"
#include "MAXRF/ipc_methods.h"

namespace maxrf::daq {

class DAQBoardsWidget : public QWidget {
  Q_OBJECT

  std::function<void (DAQInitParameters &)> callback {};
  void MakeWidget();

public:
  explicit DAQBoardsWidget(QWidget * parent = nullptr) : QWidget {parent} {
    MakeWidget();
  }
  DAQBoardsWidget(DAQBoardsWidget const & other)  = delete;
  DAQBoardsWidget & operator= (DAQBoardsWidget const & other) = delete;

  void PopulateParameterFields(DAQInitParameters & params) {
    callback(params);
  }
};

class DAQPHAWidget : public QWidget {
  Q_OBJECT

  std::function<MCAChannelParameters ()> callback;
  void MakeWidget();
public:
  explicit DAQPHAWidget(QWidget * parent = nullptr) : QWidget {parent} {
    MakeWidget();
  }

  auto GetParamsStruct() {
   return callback();
  }

};

class DAQHeaderWidget : public QWidget {
  Q_OBJECT

  std::function<void (DAQInitParameters &)> callback {};
  void MakeWidget();

public:
  explicit DAQHeaderWidget(QWidget * parent = nullptr) : QWidget {parent} {
    MakeWidget();
  }

  auto PopulateParameterFields(DAQInitParameters & params) {
    callback(params);
  }
};

class DAQParamWidget : public QWidget
{
  Q_OBJECT

  std::function<void (DAQInitParameters &)> callback {};
  void MakeWidget();

public:
  explicit DAQParamWidget(QWidget * parent = nullptr) : QWidget {parent} {
    MakeWidget();
  }

  void PopulateParameterFields(DAQInitParameters & params) {
    callback(params);
  }
};



class Wrapper : public QWidget
{
  Q_OBJECT

  DAQBoardsWidget * mca_config_widget_ {nullptr};
  DAQHeaderWidget * header_config_widget_ {nullptr};
  DAQParamWidget  * session_config_widget_ {nullptr};

  maxrf::ipc::IPCSocket socket_ {"/run/maxrf/daq"};
  maxrf::ipc::SHMHandle shm;

public:

  explicit Wrapper(QWidget * parent = nullptr) : QWidget {parent} {

//    shm.Create();
    try {
      shm.Init();
      socket_.StartSocket(maxrf::ipc::SocketOptions::kServer);
    } catch (std::exception & e) {
      std::cout << e.what() << std::endl;
      std::exit(EXIT_FAILURE);
    }

    auto tab_widget = new QTabWidget {};
    mca_config_widget_ = new DAQBoardsWidget {this};
    header_config_widget_ = new DAQHeaderWidget{this};
    session_config_widget_ = new DAQParamWidget {};

    auto tab = new QTabWidget {};
    tab->addTab(mca_config_widget_, "MCA");
    tab->addTab(header_config_widget_, "Header");
    tab->addTab(session_config_widget_, "DAQ");
    tab->setCurrentIndex(0);

    auto button = new QPushButton {"Test Start"};

    auto progress_bar = new QProgressBar {};
    progress_bar->setRange(0, 100);
    progress_bar->setValue(0);

    auto status_bar = new  QStatusBar {};
    status_bar->addPermanentWidget(progress_bar);
    status_bar->addPermanentWidget(button);

    auto layout = new QVBoxLayout {};
    layout->addWidget(tab_widget);
    layout->addWidget(tab);
    layout->addWidget(status_bar);

    this->setLayout(layout);

    connect(button, &QPushButton::clicked, this, &Wrapper::StartDAQ);

    this->resize(this->sizeHint());
    this->show();
  }
  ~Wrapper() {}

  void StartDAQ();
};


} // namespace maxrf::daq

#endif // USER_INTERFACE_H
