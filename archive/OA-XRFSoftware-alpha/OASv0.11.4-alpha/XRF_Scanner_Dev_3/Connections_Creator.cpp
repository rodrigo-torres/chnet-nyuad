#include <mainwindow.h>
#include <../Header.h>
#include <../Shm.h>

void MainWindow::CONNECTIONS_CREATOR()
{

//////////////////////// CONNECTION TO SLOTS !!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////////////// TABWIDGET2 TAB1 ASSIGN X & Y PORTS


     connect(pushButton_tab_2_2X, SIGNAL(released()),
            this, SLOT(StartX()));

     connect(pushButton_tab_2_2Y, SIGNAL(released()),
            this, SLOT(StartY()));

     connect(pushButton_tab_2_2ABORT, SIGNAL(clicked()), this, SLOT(stop_motorXY()));

     connect(STOPMOVE_pushButton, SIGNAL(clicked()), this, SLOT(stop_motorXY()));

     connect(MOVE_TO_X_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(X_to(double)));

     connect(MOVE_TO_Y_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(Y_to(double)));

     connect(SET_SCAN_X_MIN_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(Xminimo(double)));

     connect(SET_SCAN_X_MAX_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(Xmassimo(double)));
   
     connect(SET_SCAN_Y_MIN_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(Yminimo(double)));

     connect(SET_SCAN_Y_MAX_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(Ymassimo(double)));

     connect(SCANXY_pushButton, SIGNAL(clicked()), this, SLOT(StartXYScan()));

     //connect(SCANYX_pushButton, SIGNAL(clicked()), this, SLOT(StartYXScan()));

     connect(SCAN_ABORT_pushButton, SIGNAL(clicked()), this, SLOT(Abort()));

     connect(XY_SPEED_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(Velocity(double)));
     
     connect(X_STEP_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(PassoX_Func(double)));

     connect(Y_STEP_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(PassoY_Func(double)));

////////////////////////////////////////////////////////////////////// Z MOTOR

     connect(INIT_Z_pushButton, SIGNAL(released()),
            this, SLOT(StartZ()));

     connect(MOVE_Z_To_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(Z_to(double)));

     connect(AUTOFOCUS_ON_pushButton, SIGNAL(clicked()), this, SLOT(Autofocus2()));

     connect(ENABLE_TRACKING_checkBox, SIGNAL(clicked()), this, SLOT(TrackingON()));


     // Device files configuration

     QSpinBox *buttons_device_files[4] = { };
     buttons_device_files[0] = spinBox_assignX;
     buttons_device_files[1] = spinBox_assignY;
     buttons_device_files[2] = spinBox_assignZ;
     buttons_device_files[3] = spinBox_assignACM;

     QSignalMapper *mapper_device_files = new QSignalMapper();
     for (int i = 0; i < 4; i++) {
         mapper_device_files->setMapping(buttons_device_files[i], i);
         connect(buttons_device_files[i], SIGNAL(valueChanged(int)), mapper_device_files, SLOT(map()));
     }   connect(mapper_device_files, SIGNAL(mapped(int)), this, SLOT(assign_ports(int)));

     connect(pushButton_assignX_port, SIGNAL(released()), this, SLOT(AssignX()));
     connect(pushButton_assignY_port, SIGNAL(released()), this, SLOT(AssignY()));
     connect(AssignZ_pushButton, SIGNAL(released()), this, SLOT(AssignZ()));
     
     connect(AssignACM_pushButton, SIGNAL(released()), this, SLOT(Init_KeyenceLaser()));


}




