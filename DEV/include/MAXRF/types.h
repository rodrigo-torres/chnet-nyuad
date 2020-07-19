#ifndef TYPES_H
#define TYPES_H

struct DAQScanParameters {
  double x_start_coordinate {100.};
  double y_start_coordinate {100.};
  double x_end_coordinate   {100.};
  double y_end_coordinate   {100.};
  double x_motor_step   {1.0};
  double y_motor_step   {1.0};
  double motor_velocity {1.0};
};

extern DAQScanParameters scan_parameters;

#endif // TYPES_H
