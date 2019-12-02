#include "../Header.h"
#include "mainwindow.h"

//UM -Need to see invia_comando_Z, to better understand the program
//UM -It is the Z motor that does autofocussing


// Next Step- Return false to arduino if value out of range. Distance is still measured, but the motor does not move. If inside range,distance is measured as well as motor is moving

extern double ZPosition;
extern QString KeyenceValue;
extern int serialK,serialZ;
extern int send_command(int chan,const char *comando, const char *parametri, int port);

extern bool noKeyence_init;       extern bool AutofocusOn;
extern string checkK;      

double AutofocusBuffer[5] = {0,0,0,0,0};

double Autofocus_value=0;  double Autofocus_average_value=0;



double NewPositionValue;

int AutofocusIndex=0;             int AutofocusStore=0;      int DistanceLevel;           int NewPosInt=1000;    

bool IsAtuoFocusRefSet=false;      bool ValueInRange=false;   bool ReadyForTracking=true; bool TrackingCanBeEnabled=false; 
bool RunTracking=false;

void MainWindow::Autofocus2()
  {
   if(AutofocusOn) 
    {
     AutofocusOn=false; 
     printf("Autofocuson=false\n");
     timerAutofocus->stop(); //UM- Where is timerAutofocus defined???
    }
   else
    {
    AutofocusOn=true;
    printf("Autofocuson=true\n");
    timerAutofocus->start(100); //UM -What units is 100 in? Does it mean the time for which autofocus will move?
    } 
  }



char *read_Kanswer() //UM- Where is this callled ??
{
  char c[100];
  int n=0;
  string rest;
  while((n=read(serialK, &c, sizeof(c)))>0)   //UM -Is serialK the serial where Keyence is connected??? 
    {
      c[n]=0;
      rest=rest+c;      
      if(c[n-1]=='\n')
	break;        
    }
  return c;  
}


string read_Kanswer2() //UM -removed pippoK
{
  char c[12];
  int n=0;
  string restK;
  while((n=read(serialK, &c, sizeof(c)))>0)     
    {
      c[n]=0;
      restK=restK+c;      
      if(c[n-1]=='\n')
	break;        
    }
  return restK;  
}


void MainWindow::readKeyence()
{

    checkK = read_Kanswer2();
    KeyenceValue="";
    KeyenceValue.append(checkK.data());
    KeyenceValue.truncate(9);

   Autofocus_value=KeyenceValue.toDouble(); Autofocus_value=-((115218-Autofocus_value)/405.); // UM -What is that callibration formula
// From callibration
// PAY ATTENTION TO THE SIGN - BECAUSE IT IS LESS OF ZERO WHEN TOO NEAR AND GREATER OF ZERO WHEN TOO FAR//
/////starts an average on 5 values ​​if the distance in absolute value is less than 15 mm ///

   if((Autofocus_value <= (15.0))&&(Autofocus_value > (-15.0))) 
    {  // UM- deleted the commented stuff
     ValueInRange=true;
     switch (AutofocusIndex)
      {
       case 0:
         AutofocusBuffer[0]=Autofocus_value; AutofocusIndex++;
		 break;
       case 1:
         AutofocusBuffer[1]=Autofocus_value; AutofocusIndex++;
       break;
   } // End switch to fill the buffer of the media

   if(AutofocusIndex>=2) AutofocusIndex=0;
   } // fine controllo sui dati contenuti in +/- 15mm dallo zero del sensore  UM-??????

   else
   {qDebug()<< "Out of range... Autofocus_value: "<< Autofocus_value;ValueInRange=false;}

  //changing division value for averages. original value is 5 
   if(ReadyForTracking) // average value of the distance sensor
    { // UM -Deleted the commented section
     Autofocus_average_value=(AutofocusBuffer[0]+AutofocusBuffer[1])/2;
     QString valueAsString = QString::number(Autofocus_average_value);
     lineEdit_2_tab_4->setText(valueAsString); 
    }
   return;
   }



void MainWindow::AutoFocusRunning() // UM -Combined the funciton of swich case structure into the if-else structure.
  {
   if(RunTracking)
    {
     if(ValueInRange)
      {   
       NewPositionValue=(ZPosition+Autofocus_average_value); // nuovo valore di posizione
       DistanceLevel=abs(qRound((Autofocus_average_value*1000))); // UM -What are we measuring the distance from?
	   
	   char v[10];

       if(DistanceLevel>=5000)                         { sprintf(v, "%f", 5.0); send_command(1, "VEL", v,serialZ); }
       if((DistanceLevel>=3000)&&(DistanceLevel<5000)) { sprintf(v, "%f", 3.0); send_command(1, "VEL", v,serialZ); }
       if((DistanceLevel>=1000)&&(DistanceLevel<3000)) { sprintf(v, "%f", 1.2); send_command(1, "VEL", v,serialZ); }
       if((DistanceLevel>=500)&&(DistanceLevel<1000))  { sprintf(v, "%f", 0.5); send_command(1, "VEL", v,serialZ); }
       if((DistanceLevel>=200)&&(DistanceLevel<500))   { sprintf(v, "%f", 0.1); send_command(1, "VEL", v,serialZ); }
       if (DistanceLevel < 200)	                       { sprintf(v, "%f", 0.04); send_command(1, "VEL", v,serialZ);}

       NewPosInt=qRound(NewPositionValue*1000);

       if(DistanceLevel>=50) 
         {
          AutofocusStore=NewPosInt; //qDebug()<<"change in um"<<AutofocusStore;
          char sx[100];
          sprintf(sx,"%f",NewPositionValue);
          send_command(1,"MOV",sx,serialZ);
          } 
       }
    }
  }


void MainWindow::TrackingON()
  {
   if(RunTracking) 
    {
      RunTracking=false;
      qDebug()<<"OFF";
      MOVE_Z_FORWARD_pushButton->setEnabled(true); 
      MOVE_Z_BACKWARD_pushButton->setEnabled(true); 
      MOVE_Z_To_pushButton->setEnabled(true);
      MOVE_Z_To_doubleSpinBox->setEnabled(true); 
      MOVE_Z_MOTOR_label_1->setEnabled(true); 
      MOVE_Z_MOTOR_label_2->setEnabled(false);
    }
   else 
    {
     RunTracking=true;
     qDebug()<<"ON"; 
     MOVE_Z_FORWARD_pushButton->setEnabled(false); 
     MOVE_Z_BACKWARD_pushButton->setEnabled(false); 
     MOVE_Z_To_pushButton->setEnabled(false);
     MOVE_Z_To_doubleSpinBox->setEnabled(false); 
     MOVE_Z_MOTOR_label_1->setEnabled(false); 
     MOVE_Z_MOTOR_label_2->setEnabled(false); 
    }
  }


void MainWindow::Focustimer()
  {
   if(AutofocusOn)
    {
      if(noKeyence_init)
        Init_KeyenceLaser();
      else
        readKeyence();
    }
  }





