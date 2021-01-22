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

#include "user_interface.h"
#include <thread>
static constexpr char term_command [] =
    "xterm -e './MAXRF_DAQ_Daemon' &";

namespace maxrf::daq {

void Wrapper::StartDAQ()
{
  using namespace maxrf::ipc;
  using namespace std::literals;

  DAQInitParameters params {};
  mca_config_widget_->PopulateParameterFields(params);
  header_config_widget_->PopulateParameterFields(params);
  session_config_widget_->PopulateParameterFields(params);


 // auto command = ""s + term_command + "'./MAXRF_DAQ_Daemon' &";

   if (system("./MAXRF_DAQ_Daemon &") == -1) {
    // Error starting the process. Log the error, inform UI
    shm.WriteVariable(&maxrf::ipc::SHMStructure::daq_daemon_enable, false);
    return;
  }

//  QMessageBox::information(this, "Start the Daemon now",
//                           "Unfortunately, you need to start the Daemon process manually. "
//                           "This window will become unresponsive until you do so.\n"
//                           "To start the Daemon call the command maxrf-daq-daemon from a terminal");

  int counter {0};
  do {
    while (shm.GetVariable(&SHMStructure::daq_daemon_active)  == false) {
      std::this_thread::sleep_for(std::chrono::milliseconds{500});
    }
    shm.WriteVariable(&SHMStructure::daq_daemon_enable, true);

    if (socket_.EstablishConnectionWithClient() == false) {
      socket_.ReleaseSocket();
      shm.WriteVariable(&maxrf::ipc::SHMStructure::daq_daemon_enable, false);
      std::cout << "No connection from client" << std::endl;
      return;
    }
    std::cout << "Connection Accepted" << std::endl;

  shm.WriteVariable(&SHMStructure::daq_session_params, params.mode_parameters);
  shm.WriteVariable(&SHMStructure::daq_request_scan_from_motors, true);

    for (auto & board : params.boards_config) {
      socket_.SendCharThroughSocket('B');
      socket_.SendThroughSocket(&board.conn);
      socket_.SendThroughSocket(&board.board_id);
      for (auto & channel : board.channels) {
        socket_.SendCharThroughSocket('C');
        socket_.SendThroughSocket(&channel.first);
        socket_.SendThroughSocket(&channel.second);
      }
    }

    for (auto & kv_pair : params.user_header_fields) {
      socket_.SendCharThroughSocket('K');
      socket_.SendThroughSocket(kv_pair.first.c_str());
      socket_.SendThroughSocket(kv_pair.second.c_str());
    }

    socket_.SendCharThroughSocket('E');
    socket_.SendThroughSocket(&params.mode_parameters);
    socket_.SendThroughSocket(params.output_path.c_str());
//    params.base_filename = "gui"s + std::to_string(counter);
    socket_.SendThroughSocket(params.base_filename.c_str());

    ++counter;
  } while (counter < 1);

  // Use a semaphore to indicate the process is running

}


void DAQBoardsWidget::MakeWidget() {
  auto layout = new QFormLayout {};

  auto connection = new QComboBox {};
  connection->addItems(QStringList {} << "USB" << "OptiLink" << "Serial");

  layout->addRow("Connection", connection);

  auto device_number = new QSpinBox {};
  device_number->setRange(0, std::numeric_limits<char>::max());
  auto dc_node_num = new QSpinBox();
  dc_node_num->setRange(0, std::numeric_limits<char>::max());
  auto vme_base_addr = new QSpinBox();
  vme_base_addr->setRange(0, std::numeric_limits<int>::max());

  layout->addRow("Device number", device_number);
  layout->addRow("Daisy chain node", dc_node_num);
  layout->addRow("VME base address", vme_base_addr);

  auto master_layout   = new QVBoxLayout;
  auto channels_config = new QTabWidget {};
  channels_config->addTab(new DAQPHAWidget, "Channel 0");

  master_layout->addLayout(layout);
  master_layout->addWidget(channels_config);

  this->setLayout(master_layout);


  // Compute Parameters structure
  callback = [=] (DAQInitParameters & config) {
//    MCABoardConfiguration params {};
    config.boards_config.push_back(MCABoardConfiguration {});
    auto & params = config.boards_config.front();

    switch (connection->currentIndex()) {
    case 0:
      params.conn.type = MCAConnectionType::kUSB;
      break;
    case 1:
      params.conn.type = MCAConnectionType::kOpticalLink;
      break;
    case 2:
      params.conn.type = MCAConnectionType::kSerial;
      break;
    default:
      break;
    }

    params.conn.device_number     = device_number->value();
    params.conn.daisy_chain_node  = dc_node_num->value();
    params.conn.vme_base_address  = vme_base_addr->value();

    params.channels.clear();

    // TODO user controls to add and remove channels

    for (uint8_t channel = 0; channel < 1; ++channel) {
      auto ptr = static_cast<DAQPHAWidget *>(channels_config->widget(0));
      params.channels.insert({channel, ptr->GetParamsStruct()});
    }
//    return params;
  };
}

void DAQPHAWidget::MakeWidget() {
  auto defaults = new QGroupBox {"Override PHA Parameters"};
  defaults->setCheckable(true);
  defaults->setChecked(false);


  auto cal_terms = new QGroupBox {"Energy Calibration Terms"};
  auto cal_layout = new QHBoxLayout {};
  cal_layout->addWidget(new QLabel {"Order 0"});
  cal_layout->addWidget(new QDoubleSpinBox {});
  cal_layout->addWidget(new QLabel {"Order 1"});
  cal_layout->addWidget(new QDoubleSpinBox {});
  cal_layout->addWidget(new QLabel {"Order 2"});
  cal_layout->addWidget(new QDoubleSpinBox {});

  cal_terms->setLayout(cal_layout);

  auto column1 = new QFormLayout {};
  auto column2 = new QFormLayout {};

  auto make_section_label = [](QString const title) {
    auto label = new QLabel {title};
    label->setStyleSheet("font-weight: bold");
    return label;
  };

  column1->addRow(make_section_label("Input"));
  column1->addRow("Input Range", new QComboBox {});
  column1->addRow("DC Offset", new QDoubleSpinBox {});
  column1->addRow("Decimation", new QSpinBox{});
  column1->addRow(make_section_label("Trigger"));
  column1->addRow("Rise", new QSpinBox{});
  column1->addRow("Threshold", new QSpinBox{});
  column1->addRow("Holdoff", new QSpinBox{});
  column1->addRow(make_section_label("Gain"));
  column1->addRow("Coarse", new QSpinBox{});
  column1->addRow("Fine", new QDoubleSpinBox {});

  column2->addRow(make_section_label("Trapezoid"));
  column2->addRow("Rise", new QSpinBox{});
  column2->addRow("Flat", new QSpinBox{});
  column2->addRow("Peak", new QSpinBox{});
  column2->addRow("Decay", new QSpinBox{});

  column2->addRow(make_section_label("Peak"));
  column2->addRow("Samples", new QSpinBox{});
  column2->addRow("Holdoff", new QSpinBox{});

  column2->addRow(make_section_label("Baseline"));
  column2->addRow("Samples", new QSpinBox{});
  column2->addRow("Holdoff", new QSpinBox{});

  auto columns = new QHBoxLayout {};
  columns->addLayout(column1);
  columns->addLayout(column2);

  defaults->setLayout(columns);

  auto master_layout = new QVBoxLayout{};
  master_layout->addWidget(cal_terms);
  master_layout->addWidget(defaults);

  this->setLayout(master_layout);

  callback = [] {
    return MCAChannelParameters{};
  };
}

void DAQParamWidget::MakeWidget() {
  auto output_box = new QGroupBox {"Files output"};
  auto output_layout = new QFormLayout {};

  auto select_out_dir = new QPushButton {"Output  Directory"};
  auto out_dir_edit   = new QLineEdit;
  out_dir_edit->setEnabled(false);

  auto filename_edit  = new QLineEdit {};
  auto name_validator = new QRegExpValidator{ QRegExp  {"\\w+"}};
  filename_edit->setValidator(name_validator);

  output_layout->addRow(select_out_dir, out_dir_edit);
  output_layout->addRow("Base Filename", filename_edit);

  output_box->setLayout(output_layout);

  connect(select_out_dir, &QPushButton::clicked, this, [=]{
    auto dir = QFileDialog::getExistingDirectory(this, "Choose Output Directory");

    std::error_code err {};
    if (std::filesystem::is_directory(dir.toStdString(), err) == false) {
      return;
    }

    auto ret = ::access(dir.toStdString().c_str(), F_OK);
    if (ret == -1) {
      std::cout << strerror(errno) << std::endl;
      return;
    }

    // The directory has read, write, and execute permissions for this user
    out_dir_edit->setText(dir);
  });


  auto point_box = new QGroupBox {"Single-Shot DAQ"};
  point_box->setCheckable(true);
  point_box->setChecked(false);

  //    auto duration_label = new QLabel {"Single-shot duration"};
  auto duration_field = new QSpinBox {};
  duration_field->setRange(0, 30);
  duration_field->setValue(5);
  duration_field->setSuffix(" minutes");

  auto point_layout = new QFormLayout {};
  point_layout->setAlignment(Qt::AlignRight);
  point_layout->addRow("Single-shot duration", duration_field);
  point_box->setLayout(point_layout);


  auto scan_box = new QGroupBox {"Scan DAQ"};
  scan_box->setCheckable(true);
  scan_box->setChecked(true);

//  QString input_mask {"0000.00;"};
  auto input_validator = new QDoubleValidator {0., 1000., 2, this};
  //   auto scan_fields = new QLineEdit[8] ();

  QLineEdit * scan_fields[8];
  std::for_each_n(scan_fields, 8, [&](QLineEdit * & edit){
    edit = new QLineEdit {};
 //   edit->setInputMask("0000.00;");
    edit->setValidator(input_validator);
  });

  scan_fields[0]->setText("100.00");
  scan_fields[1]->setText("100.00");
  scan_fields[2]->setText("110.00");
  scan_fields[3]->setText("103.00");
  scan_fields[4]->setText("1.00");
  scan_fields[5]->setText("1.00");
  scan_fields[6]->setText("5.00");
  scan_fields[7]->setText("0.20");

  auto scan_layout1 = new QFormLayout {};
  scan_layout1->addRow("Start X Coord.", scan_fields[0]);
  scan_layout1->addRow("Start Y Coord.", scan_fields[1]);
  scan_layout1->addRow("End X Coord.", scan_fields[2]);
  scan_layout1->addRow("End Y Coord.", scan_fields[3]);

  auto scan_layout2 = new QFormLayout {};
  scan_layout2->addRow("Motor X Step", scan_fields[4]);
  scan_layout2->addRow("Motor Y Step", scan_fields[5]);
  scan_layout2->addRow("Motor velocity", scan_fields[6]);
  scan_layout2->addRow("Pixel Dwell Time", scan_fields[7]);


  auto scan_layout_master = new QHBoxLayout {};
  scan_layout_master->addLayout(scan_layout1);
  scan_layout_master->addLayout(scan_layout2);

  scan_box->setLayout(scan_layout_master);

  auto daq_duration_label = new QLabel {"DAQ Duration"};
  auto daq_duration_field = new QLineEdit {"10100 seconds"};
  auto daq_confirm_button = new QPushButton {"Confirm"};


  auto footer_layout = new QHBoxLayout {};
  footer_layout->addWidget(daq_duration_label);
  footer_layout->addWidget(daq_duration_field);
  footer_layout->addWidget(daq_confirm_button);

  auto master_layout = new QVBoxLayout {};
  master_layout->addWidget(output_box);
  master_layout->addWidget(point_box);
  master_layout->addWidget(scan_box);
  master_layout->addLayout(footer_layout);

  this->setLayout(master_layout);


  // Connections
  // Single-shot and Scan DAQ mutual exclusion
  QObject::connect(point_box, &QGroupBox::toggled,
                   this, [=](bool on){ scan_box->setChecked(!on); });
  QObject::connect(scan_box, &QGroupBox::toggled,
                   this, [=](bool on){ point_box->setChecked(!on); });

  // Pixel dwell time calculation
  auto compute_dwell_time = [=]{
    if (scan_fields[6]->text().toDouble() == 0.) { // Check velocity is not 0
      return;
    }
    scan_fields[7]->setText(QString::number(scan_fields[4]->text().toDouble() /
                            scan_fields[6]->text().toDouble()));
  };

  QObject::connect(scan_fields[4], &QLineEdit::editingFinished,
      this, compute_dwell_time);
  QObject::connect(scan_fields[6], &QLineEdit::editingFinished,
      this, compute_dwell_time);


  // DAQ duration calculation
  auto compute_daq_duration = [=] () {
    if (point_box->isChecked()) {
      daq_duration_field->setText(QString::number(
                                    duration_field->value() * 60));
    }
    else {
      // TODO compare double values with zero through delta instead
      if (scan_fields[5]->text().toDouble() == 0. ||
          scan_fields[6]->text().toDouble() == 0.) {
        daq_duration_field->setText(QString::number(
                                      std::numeric_limits<int>::max()));
      }

      double lines {0.};
      double time_per_line {0.};
      time_per_line = (scan_fields[2]->text().toDouble() -
          scan_fields[0]->text().toDouble()) / scan_fields[6]->text().toDouble();

      lines = (scan_fields[3]->text().toDouble() -
          scan_fields[1]->text().toDouble()) / scan_fields[5]->text().toDouble();
      lines += 1;

      daq_duration_field->setText(QString::number(
                                    static_cast<int>(time_per_line * lines)));
    }
  };
  std::for_each_n(scan_fields, 7, [&](QLineEdit * & edit){
    QObject::connect(edit, &QLineEdit::editingFinished,
                     this, compute_daq_duration);
  });

  callback  = [=] (DAQInitParameters & config) {
    if (point_box->isChecked()) {
      config.mode_parameters.mode     = DAQMode::kDAQPoint;
      config.mode_parameters.timeout  = duration_field->value() * 60;
    }
    else if (scan_box->isChecked()) {
      config.mode_parameters.mode     = DAQMode::kDAQScan;
      config.mode_parameters.timeout  = scan_fields[7]->text().toDouble();
    }
    else {
      config.mode_parameters.mode     = DAQMode::kDAQInvalid;
      config.mode_parameters.timeout  = 0;
    }
    config.output_path = out_dir_edit->text().toStdString().c_str();
    config.base_filename = filename_edit->text().toStdString().c_str();

    auto & scan_params = config.mode_parameters;
    scan_params.x_start_coordinate =
        static_cast<int>(scan_fields[0]->text().toDouble() );
    scan_params.y_start_coordinate =
        static_cast<int>(scan_fields[1]->text().toDouble() );
    scan_params.x_end_coordinate =
        static_cast<int>(scan_fields[2]->text().toDouble() );
    scan_params.y_end_coordinate =
        static_cast<int>(scan_fields[3]->text().toDouble() );
    scan_params.x_motor_step =
        static_cast<int>(scan_fields[4]->text().toDouble() );
    scan_params.y_motor_step =
        static_cast<int>(scan_fields[5]->text().toDouble() );
    scan_params.motor_velocity =
        static_cast<int>(scan_fields[6]->text().toDouble() );
  };

}

void DAQHeaderWidget::MakeWidget() {
  // We specify this as parent for QRegExpValidator because QLineEdits won't
  // cleanup the validator
  auto valid_xml_cdata = new QRegExpValidator { QRegExp{"[^<>&?]*"}, this };
  auto form_layout     = new QFormLayout {};

  auto add_xml_cdata_editor = [=](QString const label) {
    auto xml_cdata_editor = new QLineEdit{};
    xml_cdata_editor->setValidator(valid_xml_cdata);
    form_layout->addRow(label, xml_cdata_editor);
  };

  auto add_section_label = [=](QString const title) {
    auto label = new QLabel {title};
    label->setStyleSheet("font-weight: bold");
    form_layout->addRow(label);
  };

  add_xml_cdata_editor("File URL");

  // Category
  add_section_label("Object");
  // We reuse the pointer as the ownership is acquired by calls to addRow
  add_xml_cdata_editor("Name");
  add_xml_cdata_editor("Identifier");
  add_xml_cdata_editor("Description");

  add_section_label("Device Information");

  auto add_double_input = [=](QString const label, QStringList const units) {
    auto hlayout = new QHBoxLayout {};
    auto double_input = new QDoubleSpinBox {};
    double_input->setRange(0, 1000.);
    double_input->setDecimals(2);

    auto units_selection = new QComboBox {};
    units_selection->addItems(units);

    hlayout->addWidget(double_input);
    hlayout->addWidget(units_selection);

    form_layout->addRow(label, hlayout);
  };

  auto add_bool_then_value = [=](QString const label, QString const val) {
    auto hlayout = new QHBoxLayout {};
    auto bool_select = new QComboBox {};
    bool_select->addItems(QStringList {} << "No" << "Yes");

    hlayout->addWidget(bool_select);
    hlayout->addWidget(new QLabel {val});

    auto double_input = new QDoubleSpinBox{};
    double_input->setRange(0, 2);
    double_input->setDecimals(3);

    hlayout->addWidget(double_input);

    form_layout->addRow(label, hlayout);
  };

  add_double_input("Tube Voltage", QStringList {} << "kV" << "MV");
  add_double_input("Tube Current", QStringList {} << "uA" << "mA");
  add_bool_then_value("Collimator", "Diameter");
  add_bool_then_value("Helium Flush", "Flow");

  add_section_label("Analysis Metadata");
  add_xml_cdata_editor("Operator");
  add_xml_cdata_editor("Sample Area File");
  add_xml_cdata_editor("Sampling Note");

  this->setLayout(form_layout);

  callback = [=] (DAQInitParameters & params) {
    Q_UNUSED(params)
  };

}

}  // namespace maxrf::daq
