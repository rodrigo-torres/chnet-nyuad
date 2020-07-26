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

#include "tty.h"

extern int *shared_memory_cmd, *shared_memory;
extern maxrf::ipc::SHMHandle shm;

tty_agent::tty_agent(QObject * parent) : QObject {parent} {
    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &tty_agent::timer_event);

    servo_timer = new QTimer(this);
    connect(servo_timer, &QTimer::timeout, this, &tty_agent::servo);
}

void tty_agent::relay_command(int com, QString qstr)
{
    std::string str = qstr.toStdString();
    switch (com)
    {
    case 1: {
        /* New tty interface */
        auto it = stoi(str.substr(0, 1));
        if (it < 3) {
          auto & motor = HandleOfMotorID(it);
          if (!motor) {
            try {
              motor = std::make_unique<StageMotor>(str);
              emit update_monitor(motor->get_message(), style_yellow, it);
            } catch (std::runtime_error & e) {
              qDebug() << "[!] Can't connect to motor: " << e.what();
            }
          }
          else {
            qDebug()<<"[!] Connection already established";
          }
        }
        else {
          if (!laser_) {
            try {
              laser_ = std::make_unique<keyence>(str);
              emit update_monitor(laser_->get_message(), style_yellow, it);
            } catch (std::runtime_error & e) {
              qDebug() << "[!] Can't connect to motor: " << e.what();
            }
          }
          else {
            qDebug()<<"[!] Connection already established";
          }
        }
        break;
    }
    case 2: {
      /* Stage init */
      auto id = stoi(str);
      auto & motor = HandleOfMotorID(id);
      if (motor  == nullptr) {
        qDebug()<<"[!] Motor hasn't been connected yet";
        return;
      }

      if (motor->IsInited() == false) {
        emit toggle_tab1(false);
        try
        {
          motor->SendCommand(MotorCommands::kInitSequence);
          update_timer->start(250);
          if (id == 2) {
            emit toggle_widgets(3);
          }
          else {
            emit toggle_widgets(id);
          }
        }
        catch (const char *msg)
        {
          qDebug()<<msg;
        }
        catch (...) {}
        emit toggle_tab1(true);
      }
      else {
        qDebug() << "Motor has already been initialized";
      }
      break;
    }
    default:
      break;
    }

}

std::unique_ptr<StageMotor> & tty_agent::HandleOfMotorID(int id) {
  static std::unique_ptr<StageMotor> invalid_id { nullptr };

  switch (id) {
  case 0:
    return stage_x_;
  case 1:
    return stage_y_;
  case 2:
    return stage_z_;
  default:
    return invalid_id;
  }
}


void tty_agent::abort() {
    stage_x_->SendCommand(MotorCommands::kEmergencyHalt);
    stage_y_->SendCommand(MotorCommands::kEmergencyHalt);
    stage_z_->SendCommand(MotorCommands::kEmergencyHalt);


    if (*(shared_memory_cmd+300))
        *(shared_memory_cmd+300) = 0;


    QMutex mutex;
    QMutexLocker locker(&mutex);
    *(shared_memory_cmd+200) = 0;
}

//void StageMotor::init()
//{
//    load_conf();
//    SendData("VEL 1 " + params[1]);
//    SendData("SVO 1 1");
//    SendData("FNL 1");
//    if (wait())
//        throw "[!] Initialization aborted";

//    SendData("MOV 1 " + params[2]);
//    if (wait())
//        throw "[!] Initialization aborted";

//    is_inited = true;
//}


void tty_agent::timer_event() {
    /* Check for abort flag */
    if (*(shared_memory_cmd+200) == 1) {
        abort();
        if (servo_active) servo_active = false;
        if (scanning) scanning = false;
        return;
    }

    /* Check motors on target */
    if (stage_x_ != nullptr) {
      if (stage_x_->IsInited()) {
        stage_x_->check_ont();
        stage_x_->SendCommand(MotorCommands::kGetPosition);
        emit update_monitor(stage_x_->get_message(), style_green, 0);
      }
    }

    if (stage_y_ != nullptr) {
      if (stage_y_->IsInited()) {
        stage_y_->check_ont();
        stage_y_->SendCommand(MotorCommands::kGetPosition);
        emit update_monitor(stage_y_->get_message(), style_green, 1);
      }
    }

    if (stage_z_ != nullptr && !laser_active) {
      if (stage_z_->IsInited()) {
        stage_z_->check_ont();
        stage_z_->SendCommand(MotorCommands::kGetPosition);
        emit update_monitor(stage_z_->get_message(), style_green, 2);
      }
    }


    /* Check scan */
    if (scanning) {
      scan_loop();
    }
    else if (shm.GetVariable(&maxrf::ipc::SHMStructure::daq_request_scan_from_motors)){
      shm.WriteVariable(&maxrf::ipc::SHMStructure::daq_scan_request_acknowledged, true);
      shm.WriteVariable(&maxrf::ipc::SHMStructure::daq_request_scan_from_motors, false);
      scan();
    }
//    else if () {

//    }
}

//void tty_agent::set_target(int id, double target) {
//  switch (id) {
//  case 0:
//    stage_x_->tar = target;
//    break;
//  case 1:
//    stage_y_->tar = target;
//    break;
//  case 2:
//    stage_z_->tar = target;
//    break;
//  default:
//    qDebug() << "Unknown case";
//    break;
//  }
//}

void tty_agent::move_stage(int id, double pos) {
    switch (id) {
    case 0:
      [[fallthrough]];
    case 1:
      [[fallthrough]];
    case 2: {
      auto & motor = HandleOfMotorID(id);
      if (motor == nullptr) {
        return;
      }
      if (motor->IsOnTarget()) {
        motor->SendCommand(MotorCommands::kMoveToPosition, pos);
      }
      else {
        qDebug()<<"[!] Stage not on target, please wait";
      }
      break;
    }
    case 3:
      [[fallthrough]];
    case 5:
      [[fallthrough]];
    case 7: {
      auto & motor = HandleOfMotorID((id - 3) / 2);
      if (motor != nullptr) {
        motor->SendCommand(MotorCommands::kStepIncrease);
      }
      break;
    }
    case 4:
      [[fallthrough]];
    case 6:
      [[fallthrough]];
    case 8: {
      auto & motor = HandleOfMotorID((id - 4) / 2);
      if (motor != nullptr) {
        motor->SendCommand(MotorCommands::kStepIncrease);
      }
      break;
    }
    default:
      qDebug() << "Unknown case";
      break;
    }
}


void tty_agent::enable_servo(bool val) {
    laser_active = val;
    if (val)
        servo_timer->start(servo_interval);
    else
        servo_timer->stop();
}

void tty_agent::start_servo(bool active) {
    servo_active = active;
}

void tty_agent::set_velocity(double vel) {
    s_vel = vel;
 //   scan_parameters.motor_velocity = vel;
 //   *(shared_memory5+6) = vel;

    stage_x_->SendCommand(MotorCommands::kSetSpeed, vel);
    stage_z_->SendCommand(MotorCommands::kSetSpeed, vel);
}

void tty_agent::servo() {
  using namespace std::literals;
  auto pos = stage_z_->SendCommand(MotorCommands::kGetPosition);
  emit update_monitor(stage_z_->get_message(), style_green, 2);

  double value = laser_->ReadDistanceOffset();

  if (value > -15.0 && value < 15.0) {
    auto message = "Keyence: "s + std::to_string(value) + "mm"s;
    emit update_monitor(QString::fromStdString(message), style_green, 3);

    if (servo_active) {
//      auto tar = stage_z_->pos + value;
//      double velocity = abs(value) / 2;

      if (servo_threshold > abs(value * 1000)) {
        stage_z_->SendCommand(MotorCommands::kGentleStop);
      }
      else {
        stage_z_->SendCommand(MotorCommands::kSetSpeed, pos + value);
        stage_z_->SendCommand(MotorCommands::kMoveToPosition, abs(value) / 2.);
      }
    }
  }
  else {
    stage_z_->SendCommand(MotorCommands::kGentleStop);
//    QString message = "Keyence: Out Of Range";
    emit update_monitor("Keyence: Out Of Range", style_yellow, 3);
  }
}

ScanManager::ScanManager(MotorHandle x, MotorHandle y) :
  stage_x_ {std::move(x)}, stage_y_ {std::move(y)} {

  // Let's check there's a request for a scan first
  if (!shm.GetVariable(&SHMStructure::daq_request_scan_from_motors)) {
    // There's no request
    return;
  }

  // There is such request
  params = shm.GetVariable(&SHMStructure::daq_session_params);

  shm.CheckAccessRights(&SHMStructure::segment_semaphore);
  // TODO Some sanity checks

  //    sem_probe = sem_open("/daq_probe", O_CREAT, 0644, 0);
  //    sem_reply = sem_open("/daq_reply", O_CREAT, 0644, 0);

  // Adjust the scan limits to compensate for acceleration
  // Motor acceleration is  specifieda s 200m/s^2
  auto accel_dist = 0.5 * 200 * std::pow(params.motor_velocity, 2);
  params.x_start_coordinate -= accel_dist;
  params.x_end_coordinate   += accel_dist;

  // Gently stop the motors, check they are  fully immobile, then send
  // them to the scan start position.
  stage_x_->SendCommand(MotorCommands::kGentleStop);
  stage_y_->SendCommand(MotorCommands::kGentleStop);
  do {
    stage_x_->check_ont();
    stage_y_->check_ont();
  } while (!stage_x_->IsOnTarget() || !stage_y_->IsOnTarget());

  stage_x_->SendCommand(MotorCommands::kSetSpeed, 10.);
  stage_y_->SendCommand(MotorCommands::kSetSpeed, 10.);
  stage_x_->SendCommand(MotorCommands::kMoveToPosition,
                        params.x_start_coordinate);
  stage_y_->SendCommand(MotorCommands::kMoveToPosition,
                        params.y_start_coordinate);

  // Wait again for both motors to reach the starting position

  do {
    stage_x_->check_ont();
    stage_y_->check_ont();
  } while (!stage_x_->IsOnTarget() || !stage_y_->IsOnTarget());

  // Synchronize with the DAQ program with the semaphores, then send
  // the motor to the first x limit position
  sem_reply.Wait();
  stage_x_->SendCommand(MotorCommands::kSetSpeed, params.motor_velocity);
  stage_x_->SendCommand(MotorCommands::kMoveToPosition,
                        params.x_end_coordinate);
  sem_probe.Post();
  //      update_timer->start(250);
}


void tty_agent::scan() {
    // Disable conflicting widgets
    update_timer->stop();

    auto params = shm.GetVariable(&maxrf::ipc::SHMStructure::daq_session_params);



    x_min   = params.x_start_coordinate;
    x_max   = params.x_end_coordinate;
    x_step  = params.x_motor_step;
    y_min   = params.y_start_coordinate;
    y_max   = params.y_end_coordinate;
    y_step  = params.y_motor_step;
    s_vel   = params.motor_velocity;

//    sem_probe = sem_open("/daq_probe", O_CREAT, 0644, 0);
//    sem_reply = sem_open("/daq_reply", O_CREAT, 0644, 0);

    /* Calculating the compensation variables */

    double accel_time = s_vel / 200; // Motor acceleration specified as 200 mms-2
    double accel_dist = 0.5 * 100 * (accel_time * accel_time); // units are in um

    x_min -= accel_dist;
    x_max += accel_dist;

    /* Moving to beginning coordinates of the scan */

    do {
        stage_x_->check_ont();
        stage_y_->check_ont();
    } while (!stage_x_->IsOnTarget() || !stage_y_->IsOnTarget());

    stage_x_->SendCommand(MotorCommands::kSetSpeed, 10.);
    stage_y_->SendCommand(MotorCommands::kSetSpeed, 10.);
//    stage_x_->SetStageTarget(x_min);
//    stage_y_->SetStageTarget(y_min);
    stage_x_->SendCommand(MotorCommands::kMoveToPosition, x_min);
    stage_y_->SendCommand(MotorCommands::kMoveToPosition, y_min);
  //  stage_y_->move_totarget();

    /* Waiting for both motors on target */

    do {
        stage_x_->check_ont();
        stage_y_->check_ont();
    } while (!stage_x_->IsOnTarget() || !stage_y_->IsOnTarget());

    /* Setting scan velocity and scanning flag to true */
    scanning = true;
    stage_x_->SendCommand(MotorCommands::kSetSpeed, s_vel);

    // Request prompt dialogue from MainWindow
//    shared_memory_cmd[300] = 2;
//    system("./digitizer &");
    /* Synchronize */
    /* Sending X motor to 1st limit */
 //   sem_wait(sem_reply);
    stage_x_->SendCommand(MotorCommands::kMoveToPosition, x_max);
//    stage_x_->SetMotorSpeed(x_max);
//    stage_x_->move_totarget();
//    sem_post(sem_probe);
    update_timer->start(250);
}

void tty_agent::scan_loop() {
  if (next_line)
  {
    //sem_wait(sem_reply);
    auto tar = fabs(stage_x_->pos - x_min) < x_min * 0.01 ?
          x_max : x_min;
    stage_x_->SendCommand(MotorCommands::kMoveToPosition, tar);
    //sem_post(sem_probe);
    next_line = false;
  }
  else if (stage_x_->IsOnTarget() && !next_line)
  {
    double delta = fabs(stage_y_->pos - y_max);
    if (delta > y_max * 0.001)
    { // Pass to the next line
      auto tar = stage_y_->pos + y_step;
//      stage_y_->tar += y_step;
      stage_y_->SendCommand(MotorCommands::kMoveToPosition, tar);
      do {
        stage_y_->check_ont();
      } while (!stage_y_->IsOnTarget());
      next_line = true;
    }
    else
    { // The scan is done
      scanning = false;
      //            sem_wait(sem_reply);
      //            sem_close(sem_probe);
      //            sem_close(sem_reply);
      //            sem_unlink("/daq_probe");
      //            sem_unlink("/daq_reply");
    }
  }
}
