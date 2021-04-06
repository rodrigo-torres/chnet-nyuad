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

#include <QMessageBox>


void tty_agent::timer_event() {
  using maxrf::ipc::SHMStructure;

  // Check for abort flag
  if (shared_memory_cmd[200]) {
    abort();
    if (servo_active) {
      servo_active = false;
    }
    if (scan_handle_) {
      scan_handle_->AbortScan();
      scan_handle_->TakeBackOwnership(stage_x_, stage_y_);
      scan_handle_.reset(nullptr);
    }
  }

  // Check ongoing scan
  if (scan_handle_) {
    // There's an active scan
    scan_handle_->UpdateEvent();

    if (scan_handle_->IsDone()) {
      scan_handle_->TakeBackOwnership(stage_x_, stage_y_);
      scan_handle_.reset(nullptr);
    }
  }
  else {
    // Update the position motors manually
    if (stage_x_) {
      emit update_monitor(stage_x_->TriggerPositionUpdate(), style_green, 0);
    }
    if (stage_y_) {
      emit update_monitor(stage_y_->TriggerPositionUpdate(), style_green, 1);
    }
  }

  // Check position of z stage if servo is not active
  if (laser_active) {
    // Reserved for future use
  }
  else if (stage_z_) {
    emit update_monitor(stage_z_->TriggerPositionUpdate(), style_green, 2);
  }

  if (shm.GetVariable(&SHMStructure::daq_request_scan_from_motors)) {
    // The signal requests the UI to produce a warning dialog asking the user
    // to either accept or cancel the DAQ request
    emit TriggerDAQPrompt();

    // Wait for a response
    while (daq_req_input_ready == false) {
      QThread::msleep(100); // Sleep for 0.1 seconds
    }
    daq_req_input_ready = false;

    if (daq_req_accepted) {
      // Transfer ownership of stages to ScanManager and begin scan
      scan_handle_ = std::make_unique<ScanManager>(std::move(stage_x_),
                                                   std::move(stage_y_),
                                                   this);
    }
    else {
      // Default action is to ignore the request
      shm.WriteVariable(&SHMStructure::daq_request_scan_from_motors, false);
    }
  }
}


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
        motor->SendCommand(MotorCommands::kStepDecrease);
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


void tty_agent::servo() {
  using namespace std::literals;
  stage_z_->SendCommand(MotorCommands::kGetPosition);
  auto pos = stage_z_->pos;

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


// BUG. The ScanManager by default moves the motor to position 100,100 independent
// of the type of DAQ session (i.e. it moves the motors to HOME position for a point DAQ).
// TODO Polymorphism of the ScanManager by adding one parameter to the constructor
// NOTE. A quick fix of this BUG has been implemented through an IF statement
ScanManager::ScanManager(MotorHandle x, MotorHandle y, tty_agent * handle) :
  stage_x_ {std::move(x)}, stage_y_ {std::move(y)} {

  connect(this, &ScanManager::UpdatePositionMonitors,
          handle, &tty_agent::update_monitor);

  params = shm.GetVariable(&SHMStructure::daq_session_params);

  sem_probe.Init("/daq_probe");
  sem_reply.Init("/daq_reply");


  // We acknowledge the scan request
  shm.WriteVariable(&SHMStructure::daq_request_scan_from_motors, false);
  shm.WriteVariable(&SHMStructure::daq_scan_request_acknowledged, true);
  
  // Gently stop the motors, check they are  fully immobile, then send
  // them to the scan start position.
  stage_x_->SendCommand(MotorCommands::kGentleStop);
  stage_y_->SendCommand(MotorCommands::kGentleStop);
  do {
    stage_x_->check_ont();
    stage_y_->check_ont();
  } while (!stage_x_->IsOnTarget() || !stage_y_->IsOnTarget());

  
  if (params.mode == DAQ::kDAQPoint) {
    // For a point DAQ, don't move the motors to HOME
    sem_reply.Wait(); // Wait for the daemon to be ready
    sem_probe.Post(); // Confirm the MAIN program is ready
    return;
  }

  // Adjust the scan limits to compensate for acceleration
  // Motor acceleration is  specifieda s 200mm/s^2
  auto accel_dist = 0.5 * (1 / 200.) * std::pow(params.motor_velocity, 2);
  params.x_start_coordinate -= accel_dist;
  params.x_end_coordinate   += accel_dist;

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
}

void ScanManager::UpdateEvent() {
  if (scan_done_) {
    return;
  }

  emit UpdatePositionMonitors(stage_x_->TriggerPositionUpdate(),
                              QString::fromStdString(kStyleGreen), 0);
  emit UpdatePositionMonitors(stage_y_->TriggerPositionUpdate(),
                              QString::fromStdString(kStyleGreen), 1);

//  stage_x_->TriggerPositionUpdate();  // Update the position monitors
//  stage_y_->TriggerPositionUpdate();

  // TODO implement some method in the scan UpdateEvent to check if the
  // motors are moving at all

  // The TriggerPositionUpdate method of the StageMotor class updates the
  // value of the internal on_target_ field
  
  // Fix for DAQPoint
  if (params.mode == DAQ::kDAQPoint) {
    if (sem_reply.TryWait()) { // Probe if the DAQ is done
      sem_probe.Post(); // Confirm the MAIN program is ready
      scan_done_ = true;    
    }
    return;
  }

  if (stage_x_->IsOnTarget()) {
    // Synchronize with the DAQ daemon and start the new line
    sem_reply.Wait();

    // Either the scan is done, or the next scan line is coming up
    if (fabs(params.y_end_coordinate - stage_y_->pos)
        > params.y_motor_step * 0.5) {
      // The scan is not done, we pass to the next line
      stage_y_->SendCommand(MotorCommands::kMoveToPosition,
                            stage_y_->pos + params.y_motor_step);

      // Wait for Y stage to be on target
      do {
        stage_y_->check_ont();
      } while (!stage_y_->IsOnTarget());

      stage_x_->SendCommand(MotorCommands::kMoveToPosition,
                            fabs(stage_x_->pos - params.x_start_coordinate)
                            < params.x_motor_step * 0.5 ?
                              params.x_end_coordinate :
                              params.x_start_coordinate);
      sem_probe.Post();
    }
    else {
      // The scan has finished.
      // 1. Reset the stage velocity to its default value (1mm/s)
      stage_x_->SendCommand(MotorCommands::kSetSpeed, 1.);
      stage_y_->SendCommand(MotorCommands::kSetSpeed, 1.);
      // 2. Signal the state
      scan_done_ = true;
      // TODO some methods to garbage collect the scan semaphores
    }
  }
  else {
    // Currently scanning a line. Do nothing
    // TODO perhaps update the UI while the scan line is ongoing to show
    // the application has not hanged up.
  }
}

void ScanManager::AbortScan() {

}

void ScanManager::TakeBackOwnership(MotorHandle &x, MotorHandle &y) {
  x = std::move(stage_x_);
  y = std::move(stage_y_);
}

bool ScanManager::IsDone() {
  return scan_done_;
}


