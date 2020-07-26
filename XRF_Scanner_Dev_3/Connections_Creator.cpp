#include <mainwindow.h>
#include <../Header.h>
#include <../Shm.h>

void MainWindow::CONNECTIONS_CREATOR()
{

//////////////////////// CONNECTION TO SLOTS !!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////////////// TABWIDGET2 TAB1 ASSIGN X & Y PORTS


     connect(pushButton_assignX_port, SIGNAL(released()), this, SLOT(AssignX()));

     connect(spinBox_assignX, SIGNAL(valueChanged(int)), 
          this, SLOT(NameX(int)));

     connect(pushButton_assignY_port, SIGNAL(released()),
            this, SLOT(AssignY()));

     connect(spinBox_assignY, SIGNAL(valueChanged(int)), 
          this, SLOT(NameY(int)));

     connect(pushButton_tab_2_2X, SIGNAL(released()),
            this, SLOT(StartX()));

     connect(pushButton_tab_2_2Y, SIGNAL(released()),
            this, SLOT(StartY()));

//     connect(pushButton_tab_2_2XY, SIGNAL(released()),   //////////////////////////////////////////// INIT X + Y
//            this, SLOT(StartBoth()));

     connect(pushButton_tab_2_2ABORT, SIGNAL(clicked()), this, SLOT(stop_motorXY()));

     connect(MOVEUP_pushButton, SIGNAL(released()),
            this, SLOT(Move_down()));

     connect(MOVELEFT_pushButton, SIGNAL(released()),   
            this, SLOT(Move_right()));
     ///inversione hardware

     connect(MOVERIGHT_pushButton, SIGNAL(released()),
            this, SLOT(Move_left()));
     ///inversione hardware

     connect(MOVEDOWN_pushButton, SIGNAL(released()),
            this, SLOT(Move_up()));

     connect(STOPMOVE_pushButton, SIGNAL(clicked()), this, SLOT(stop_motorXY()));

     connect(MOVE_TO_X_pushButton, SIGNAL(released()),
            this, SLOT(MoveX_To()));

     connect(MOVE_TO_Y_pushButton, SIGNAL(released()),
            this, SLOT(MoveY_To()));

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
     connect(spinBox_assignZ, SIGNAL(valueChanged(int)), 
          this, SLOT(NameZ(int)));
     
     connect(AssignZ_pushButton, SIGNAL(released()),
            this, SLOT(AssignZ()));

     connect(INIT_Z_pushButton, SIGNAL(released()),
            this, SLOT(StartZ()));

     connect(STOP_Z_INIT_pushButton, SIGNAL(clicked()), this, SLOT(StopZ())); ////////// FARE CHECK SU SLOT STOP (STOPZ??)

     connect(MOVE_Z_To_pushButton, SIGNAL(released()),
            this, SLOT(MoveZ_To()));

     connect(MOVE_Z_To_doubleSpinBox, SIGNAL(valueChanged(double)), 
          this, SLOT(Z_to(double)));

     connect(MOVE_Z_FORWARD_pushButton, SIGNAL(released()),
            this, SLOT(Move_forward())); 

     connect(MOVE_Z_BACKWARD_pushButton, SIGNAL(released()),
            this, SLOT(Move_backward()));

     connect(STOP_Z_MOVE_pushButton, SIGNAL(clicked()), this, SLOT(StopZ())); 

     connect(AUTOFOCUS_ON_pushButton, SIGNAL(clicked()), this, SLOT(Autofocus2()));

     connect(ENABLE_TRACKING_checkBox, SIGNAL(clicked()), this, SLOT(TrackingON()));

     //connect(Z_VELOCITY_doubleSpinBox, SIGNAL(valueChanged(double)),this, SLOT(VelocityZ(double)));
     
     //connect(Z_STEP_doubleSpinBox, SIGNAL(valueChanged(double)),this, SLOT(PassoZ_Func(double)));


/////////////////////////////////// ARDUINO //////////////////


     connect(spinBox_assignACM, SIGNAL(valueChanged(int)), 
          this, SLOT(NameACM(int)));
     
     connect(AssignACM_pushButton, SIGNAL(released()),
            this, SLOT(AssignACM()));


//////////////////////////////////////////////////////////

     //connect(SW_treshold_Button, SIGNAL(clicked()), this, SLOT(Treshold()));



   //  connect(quit, SIGNAL(clicked()), this, SLOT(Exit())); ///aggiungere check se è fermo qApp, SLOT(quit())

}




