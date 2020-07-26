/** MAXRF_Main: Main point of access to control the MAXRF instrument
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

#include <QXmlStreamReader>
#include "tty.h"

extern int *shared_memory_cmd;

TTYHandle::TTYHandle(std::string s)
{
  using namespace std::literals;
    auto pos = s.find(" ");
    df_path = s.substr(pos + 1);

    /* Specifying the device file interface information (through the termios structure) */
    struct termios my_termios;
    ::cfsetispeed(&my_termios, B9600);
    my_termios.c_oflag = 0000000;
    // Output as is, no processing or mapping
    my_termios.c_cflag = 0004275;
    // Character size 8 bits, baud-rate 9600, enable receiver, no flow control or parity check
    my_termios.c_lflag = 0000000;
    // No input processing, no signals, cbreak mode (non-canonical), no echoing
    my_termios.c_iflag = 0040200;
    // UTF8 encoding, ignore CR on input, no flow control, no parity checks

    my_termios.c_cc[VMIN] = 0; // we read byte-by-byte, non-blocking
    my_termios.c_cc[VTIME]= 1;

    dev_fd = ::open(df_path.c_str(), O_RDWR);
    if (dev_fd == -1) {
      throw std::runtime_error("Couldn't open the character device file: "s
                               + strerror(errno));
    }

    ::tcsetattr(dev_fd, TCSANOW, &my_termios);
    ::tcflush(dev_fd, TCIOFLUSH);
}

TTYHandle::~TTYHandle() {
  ::close(dev_fd);
}

void TTYHandle::SendData(std::string command) {
    command = command + '\n'; // Very important, the controller expects a '\n' character
    long n = write(dev_fd, command.data(), command.size());
    tcdrain(dev_fd);

    if (n < 0)
    {
        qDebug()<<"[!] Writing to bus failed";
        qDebug()<<strerror(errno);
    }
    return;
}

std::string TTYHandle::ReadAvailableData() {
    char buff[100] = { '\0' };

    long n = 0;
    int size = 0;
    while ((n = read(dev_fd, &buff[size], 1)) > 0) {
        size += n;
        if (buff[size - 1] == '\n') break;
    }
    if (n < 0) throw std::runtime_error(strerror(errno));
    if (buff[size-1] == '\n') buff[size-1] = '\0';

    std::string ans(buff);
    return  ans;
}

StageMotor::StageMotor(std::string s) try : TTYHandle(s) {
  using namespace std::literals;
  // Identify the  motor
  SendData("*IDN?");
  std::cout << "[I] Device identified as:  " << ReadAvailableData().c_str() << std::endl;

  auto num = std::stoi(s.substr(0, s.find(" ")));
  switch (num) {
  case 0:
    stage_id_   = 'x';
    stage_name_ = "Stage X";
    break;
  case 1:
    stage_id_   = 'y';
    stage_name_ = "Stage Y";
    break;
  case 2:
    stage_id_   = 'z';
    stage_name_ = "Stage Z";
    break;
  default:
    stage_id_   = 'u';
    stage_name_ = "Unknown";
    break;
  }
  status_message_ = stage_name_ + ": "s + df_path;

  auto conf_file_path = std::string {PROJECT_LOCAL_FOLDER};
  conf_file_path.append("/etc/maxrf/user_conf.xml");

  std::ifstream file {conf_file_path};
  if (file.is_open()) {
    // Read the whole file onto a  string buffer
    std::ostringstream sstream;
    sstream << file.rdbuf();

    QXmlStreamReader xmldoc {sstream.str().c_str()};
    std::string search {"stage"s + stage_id_ + "_par"s};

    int counter {0};
    while (!xmldoc.atEnd()) {
      if (xmldoc.readNext() != QXmlStreamReader::StartElement) {
        continue;
      }
      auto attrib = xmldoc.attributes();
      if (attrib.value("name").toString().toStdString() ==
          search + std::to_string(counter)) {
        auto node = xmldoc.readElementText();
        if (!xmldoc.isEndElement())  {
          throw std::runtime_error("[!] Error parsing  motor configuration file");
        }
        params.push_back(node.toStdString());
        ++counter;
      }
    }
    if (xmldoc.hasError()) {
      throw std::runtime_error("[!] Error parsing  motor configuration file");
    }

  }
  else {
    throw std::runtime_error("[!] Could not open motor configuration file");
  }
} catch (std::runtime_error & e) {
  std::cout << "error occured: " << e.what() << std::endl;
} // implicit throw


std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

void StageMotor::load_conf() {
    if (!isatty(dev_fd))
    {
        throw "[!] This interface has not yet been configured";
    }

    auto conf_file_path = std::string {PROJECT_LOCAL_FOLDER};
    conf_file_path.append("/etc/maxrf/");
    conf_file_path.append(params[0]);


    std::ifstream file(conf_file_path);
    std::string line;
    if (file.is_open()) {
        do {
            getline(file, line);
        } while (line.compare("[check]") != 0 && !file.eof());
        getline(file, line);
        std::string check_pref = line;
        getline(file, line);

        int ret = 0;
        while (getline(file, line))
        {
            SendData(line);
            auto tokens = split(line, ' ');

            line = check_pref + ' ' + tokens.at(2);
            SendData(line);
            line = ReadAvailableData();
            auto ans = split(line, '=');

            ret |= ans.at(1).compare(0, tokens.at(3).length(), tokens.at(3));
        }
        if (ret != 0)
            qDebug()<<"[!] There were errors in setting the stage parameters";
        else
            qDebug()<<"[!] All stage parameters loaded succesfully";
    }
    else qDebug()<<"[!] Configuration file not found";
}


bool StageMotor::wait()
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    do {
        QThread::msleep(100);
        if (*(shared_memory_cmd+200) ==  1)
        {
            this->SendCommand(MotorCommands::kGentleStop);
            *(shared_memory_cmd+200) = 0;
            //err_flag = 1;
            //err_string = "[!] STOP requested";

            return true;
        }
        check_ont();
    } while (!on_target);
    return 0;
}

void StageMotor::check_ont() {
    SendData("ONT?");
    std::string ans;
    try {
        ans = ReadAvailableData();
    } catch (const std::runtime_error& e) {
        qDebug()<<e.what();
    }

    if (ans.compare(2, 1, "1") == 0) on_target = true;
    else on_target = false;
}

//void StageMotor::stop() {
//  SendData("HLT");
//  SendData("ERR?");

//  auto reply_code = ReadAvailableData();
//  if (reply_code.compare("10") != 0) {
//    qDebug() << "[!] " << stage_name_.c_str()
//             << " stopped. But  the error code was different from expected: "
//             << reply_code.c_str();

//  }
//}

double StageMotor::SendCommand(MotorCommands command, double argument) {
  using namespace std::literals;
  double ret {};

  switch (command) {
  case MotorCommands::kInitSequence :
    if (is_inited == false) {
      load_conf();
      SendData("VEL 1 " + params[1]);
      SendData("SVO 1 1");
      SendData("FNL 1");
      if (wait())
        throw "[!] Initialization aborted";

      SendData("MOV 1 " + params[2]);
      if (wait())
        throw "[!] Initialization aborted";

      is_inited = true;
    }
    break;
  case MotorCommands::kShutdownSequence :
    break;
  case MotorCommands::kIdentify :
    break;
  case MotorCommands::kGetSpeed :
    break;
  case MotorCommands::kSetSpeed :
    /// Check the speed is within the allowed limits (i.e not negative, not
    /// zero, and not an excessive value).
    if (std::signbit(argument) != true && argument < 15.) {
      this->SendData("VEL 1 "s + std::to_string(argument));
    }
    break;
  case MotorCommands::kGetPosition : {
    this->SendData("POS?");

    try {
      pos = atof(ReadAvailableData().substr(2).c_str());
      status_message_ = stage_name_ + ": " + std::to_string(pos) + " mm";
    } catch (const std::runtime_error& e) {
      this->SendCommand(MotorCommands::kGentleStop);
      qDebug()<< e.what();
    }
    break;
  }
  case MotorCommands::kMoveToPosition :
    this->SendData("MOV 1" + std::to_string(argument));
    on_target = false;
    break;
  case MotorCommands::kStepIncrease :
    if (on_target) {
      this->SendCommand(MotorCommands::kGetPosition);
      this->SendData("MOV 1"s + std::to_string(pos + 1));
    }
    break;
  case MotorCommands::kStepDecrease :
    if (on_target) {
      this->SendCommand(MotorCommands::kGetPosition);
      this->SendData("MOV 1"s + std::to_string(pos - 1));
    }
    break;
  case MotorCommands::kGentleStop : {
    SendData("HLT");
    SendData("ERR?");

    auto reply_code = ReadAvailableData();
    if (reply_code.compare("10") != 0) {
      qDebug() << "[!] " << stage_name_.c_str()
               << " halted. But  the error code was different from expected: "
               << reply_code.c_str();

    }
    break;
  }
  case MotorCommands::kEmergencyHalt : {
    SendData("STP");
    SendData("ERR?");

    auto reply_code = ReadAvailableData();
    if (reply_code.compare("10") != 0) {
      qDebug() << "[!] " << stage_name_.c_str()
               << " stopped. But  the error code was different from expected: "
               << reply_code.c_str();

    }
    break;
  }
  }
  return ret;
}


//void StageMotor::get_pos()
//{
//  SendData("POS?");

//  std::string ans;
//  try {
//    ans = ReadAvailableData();
//  } catch (const std::runtime_error& e) {
//    qDebug()<<e.what();
//  }

//  pos = atof((ans.substr(2)).c_str());
//  status_message_ = stage_name_ + ": " + ans.substr(2, 6) + " mm";
//}

//void StageMotor::move_totarget() {
//    std::string command = "MOV 1 " + std::to_string(tar);

//    SendData(command);
//    on_target = false;
//}

//void StageMotor::move_step(bool direction) {
//    if (on_target) {
//        get_pos();
//        direction == true ? tar = pos + 1  : tar = pos - 1;
//        move_totarget();
//    }
//    else return;
//}

//void StageMotor::SetMotorSpeed(double speed) {
//  using namespace std::literals;

//  /// Check the speed is within the allowed limits (i.e not negative, not
//  /// zero, and not an excessive value).
//  if (std::signbit(speed) == true) {
//    return;
//  }
//  if (speed > 15.) {
//    return;
//  }
//  this->SendData("VEL 1 "s + std::to_string(speed));
//}

/* Setter functions */
//void StageMotor::SetStageTarget(double val) {
//    tar = val;
//}


bool StageMotor::IsInited() const {
    return is_inited;
}

bool StageMotor::IsOnTarget() const {
    return on_target;
}





