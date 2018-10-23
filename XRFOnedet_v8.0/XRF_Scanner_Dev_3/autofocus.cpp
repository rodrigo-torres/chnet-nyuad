#include "../Header.h"
#include "mainwindow.h"


extern double ZPosition;
extern QString KeyenceValue;
extern int serialK;
extern int invia_comando_Z(int chan,const char *comando, const char *parametri);

extern bool noKeyence_init;       extern bool AutofocusOn; 
extern string checkZ;             extern string checkK;      extern string Zpos;

double AutofocusBuffer0=0;        double AutofocusBuffer1=0; double AutofocusBuffer2=0;   double AutofocusBuffer3=0;
double AutofocusBuffer4=0;        double Autofocus_value=0;  double AutoFocusRefValue=0;  double Autofocus_average_value=0;
double LinearStageRefPosition=25; double NewPositionValue;

int AutofocusIndex=0;             int AutofocusStore=0;      int DistanceLevel;           int NewPosInt=1000;    
int SpeedGrade=0;

bool IsAtuoFocusRefSet=false;      bool ValueInRange=false;   bool ReadyForTracking=true; bool TrackingCanBeEnabled=false; 
bool RunTracking=false;


void MainWindow::Autofocus2()
  {
   if(AutofocusOn) 
    {
     AutofocusOn=false; 
     printf("Autofocuson=false\n");
	timerAutofocus->stop();
     //timerZ->stop();
    }
   else
    {
     AutofocusOn=true;
	printf("Autofocuson=true\n");
     //Focustimer();
    timerAutofocus->start(100);
    // timerZ->start(100);
    } 
  }



char *read_Kanswer()
{
  char c[100];
  int n=0;
  string rest;
  while((n=read(serialK, &c, sizeof(c)))>0)      
    {
      c[n]=0;
      rest=rest+c;      
      if(c[n-1]=='\n')
	break;        
    }
  return c;  
}


string read_Kanswer2()
{
  char c[12];
  int n=0;
  string restK;
  string pippoK;
  while((n=read(serialK, &c, sizeof(c)))>0)      
    {
      c[n]=0;
      restK=restK+c;      
      if(c[n-1]=='\n')
	break;        
    }
pippoK =restK;
  return pippoK;  
}


void MainWindow::readKeyence()  //Autofocus Button
  {
   checkK = read_Kanswer2();
   KeyenceValue="";
   KeyenceValue.append(checkK.data());
   KeyenceValue.truncate(9);

   Autofocus_value=KeyenceValue.toDouble(); Autofocus_value=-((115218-Autofocus_value)/405.); // dalla calibrazione 
// ATTENZIONE IL SEGNO - PERCHE' SIA MINORE DI ZERO QUANDO TROPPO VICINO E MAGGIORE DI ZERO QUANDO TROPPO LONTANO//
///// inizia una media su 5 valori se la distanza in valore assoluto e' minore di 15 mm ///

   //qDebug()<<Autofocus_value;

   if((Autofocus_value <= (15.0))&&(Autofocus_value > (-15.0))) 
    { 
     ValueInRange=true;
     switch (AutofocusIndex)
      {
       case 0:
         AutofocusBuffer0=Autofocus_value; AutofocusIndex++;
       break;

       case 1:
         AutofocusBuffer1=Autofocus_value; AutofocusIndex++;
       break;

/*   case 2:
   AutofocusBuffer2=Autofocus_value; AutofocusIndex++;
   break;

   case 3:
   AutofocusBuffer3=Autofocus_value; AutofocusIndex++;
   break;

   case 4:
   AutofocusBuffer4=Autofocus_value; ReadyForTracking=true; AutofocusIndex++;
   break;*/
   } // fine switch per riempire il buffer della media

   if(AutofocusIndex>=2) AutofocusIndex=0;
   } // fine controllo sui dati contenuti in +/- 15mm dallo zero del sensore

   else
   {qDebug()<< "Out of range... Autofocus_value: "<< Autofocus_value;ValueInRange=false;}

  //changing division value for averages. original value is 5 
   if(ReadyForTracking) // valor medio del sensore di distanza
    {
     Autofocus_average_value=(AutofocusBuffer0+AutofocusBuffer1)/2/*+AutofocusBuffer2+AutofocusBuffer3+AutofocusBuffer4)/5*/;
     QString valueAsString = QString::number(Autofocus_average_value);
     lineEdit_2_tab_4->setText(valueAsString); 
    }
   }



void MainWindow::AutoFocusRunning()
  {
   if(RunTracking)
    {
     if(ValueInRange)
      {   
       NewPositionValue=(ZPosition+Autofocus_average_value); // nuovo valore di posizione
       DistanceLevel=abs(qRound((Autofocus_average_value*1000))); //qDebug()<<"DistanceLevel: "<<DistanceLevel;

       if(DistanceLevel>=5000)                         {SpeedGrade=5;} //qDebug()<<"SpeedGrade+3";}
       if((DistanceLevel>=3000)&&(DistanceLevel<5000)) {SpeedGrade=4;} //qDebug()<<"SpeedGrade+3";}
       if((DistanceLevel>=1000)&&(DistanceLevel<3000)) {SpeedGrade=3;} //qDebug()<<"SpeedGrade+2";}
       if((DistanceLevel>=500)&&(DistanceLevel<1000))  {SpeedGrade=2;} //qDebug()<<"SpeedGrade+1";}
       if((DistanceLevel>=200)&&(DistanceLevel<500))   {SpeedGrade=1;} //qDebug()<<"SpeedGrade+1";}
       if(DistanceLevel<200)                           {SpeedGrade=0;} //qDebug()<<"SpeedGrade+0";}

       char v[10]; 
       switch (SpeedGrade)
         {
          case 0:
            {sprintf(v,"%f",0.04); invia_comando_Z(1,"VEL",v);
             break;}
          case 1:
            {sprintf(v,"%f",0.1); invia_comando_Z(1,"VEL",v);
            break;}
          case 2:
            {sprintf(v,"%f",0.5); invia_comando_Z(1,"VEL",v);
             break;}
          case 3:
            {sprintf(v,"%f",1.2); invia_comando_Z(1,"VEL",v);
             break;}
          case 4:
            {sprintf(v,"%f",3.0); invia_comando_Z(1,"VEL",v);
             break;}
          case 5:
            {sprintf(v,"%f",5.0); invia_comando_Z(1,"VEL",v);
             break;}
         }

       NewPosInt=qRound(NewPositionValue*1000);
       if(DistanceLevel>=50) 
         {
          AutofocusStore=NewPosInt; //qDebug()<<"change in um"<<AutofocusStore;
          char sx[100];
          sprintf(sx,"%f",NewPositionValue);
          invia_comando_Z(1,"MOV",sx);
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
        InizializzazioneKeyence();
      else
        readKeyence();
    }
  }





