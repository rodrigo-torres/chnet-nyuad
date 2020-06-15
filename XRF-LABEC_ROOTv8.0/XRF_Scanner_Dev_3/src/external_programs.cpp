// ALL external Programs
//
// Index:
//

#include "include/MAXRF/mainwindow.h"
#include "../Header.h"

extern int *shared_memory;   extern int *shared_memory2;    extern int *shared_memory_cmd;
extern int measuring_time;   extern int DAQ_TYPE;
extern bool CameraOn;	     extern bool DAQ_started;
extern void DAQ_lcd_Timer(); extern void Stop_Abort_DAQ();

char process_daq[30];



//////////////////////////////////////////////////////////////////// DAQ START and STOP


void MainWindow::StartVme()
{

//printf("meas_time %i\n", measuring_time);
if(*(shared_memory_cmd+70)==0) //INSERTED
     {
      if(DAQ_TYPE==1)
       system("./ADCXRF_USB &");
      if(DAQ_TYPE==0)
       system("./ADCXRF_Optical_Link &");
       *(shared_memory2+9)=1;//tells ADCXRF that it should run on point acquisition mode
       *(shared_memory_cmd+70)=1; //INSERTED
       QTimer::singleShot(measuring_time*1000, this, SLOT(Stop_Vme()));     ///////////misura con una durata definita
       DAQ_started=true; DAQ_lcd_Timer();
      }
 else qDebug()<<"Acquisizione già partita!!\n";
}

void MainWindow::Stop_Vme()
{
if(*(shared_memory_cmd+70)==1)
     {
       printf("stopping vme %i\n");
       *(shared_memory_cmd+70)=0;//INSERTED
       //int pidVme=*(shared_memory_cmd+80);	
	*(shared_memory2+9)=0;
       //sprintf(process_daq, "kill -s TERM %i &", pidVme);
       //system(process_daq);
      

       ////Salva in automatico Spettro.txt///
       for(int c=0;c<=16384;c++)
           {      
            *(shared_memory+100+c)=0;                              //Azzero le celle di memoria in cui viene scritto lo spettro
            }

       QString percorso = QFileDialog::getSaveFileName(this,tr("Save as"), QDir::currentPath());
       int eventi=*(shared_memory2+5);
       qDebug()<<"Inizio creazione di Spettro.txt...\n";

      // qDebug()<<"ev="<<eventi<<"\n";

       QFile file2(percorso);
       file2.remove();
       file2.open(QIODevice::ReadWrite);
       QTextStream out2(&file2);

       for(int u=0;u<eventi;u++)
          {
	    if (*(shared_memory2+11+u)<16384)
	      {
               int h=*(shared_memory2+11+u);
               *(shared_memory+100+h)= *(shared_memory+100+h)+1;
               //qDebug()<<"evento numero="<<u<<"energia="<<h<<"\n";
               }
	   }
              for(int i=1;i<=16384;i++)
              {      
               out2<<*(shared_memory+100+i)<<'\n';
              //if (*(shared_memory+100+i)!=0) {qDebug()<<"canale"<<i<<"conteggio"<<*(shared_memory+100+i)<<"\n";}
              *(shared_memory+100+i)=0;
              }

       file2.close();
       qDebug()<<"Spettro.txt pronto!\n"; 
      } 

 else qDebug()<<"Vme gia' spento!!\n";

 Stop_Abort_DAQ();
}


//////////////////////////////////////////////////////////////////// HISTOGRAM
void MainWindow::ShowHistogram()
{
if(*(shared_memory_cmd+71)==0)
 {
  system("./XRF & ");
  *(shared_memory_cmd+71)=1; // XRF
}
else
qDebug()<<"Finestra istogramma gia' aperta!\n";
}

///////////////FIT
void MainWindow::FitSpectrum()
{
/*
 QString text="FitProgramStatus";
 QFile file(text);
   if(file.exists())
      {
      file.open(QIODevice::ReadOnly);
      QString line;
      line = file.readLine();
      int numero=line.toInt();
      file.close();
      if (numero==0)                    
        system("./XRF_Fit & ");
      else 
        qDebug()<<"Programma di fit già aperto!\n";
      }
    else
      {
        QFile file("FitProgramStatus");
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out << "1" ;
        file.close();
        system("./XRF_Fit & ");
      }
*/
if(*(shared_memory_cmd+78)==0)
 {
  system("./XRF_Fit & ");
  *(shared_memory_cmd+78)=1; // XRF_Fit
 }
else
qDebug()<<"XRF_Fit alreay opened!\n";
}


//////////////////////////////////////////////////////////////////// RATEMETER
void MainWindow::RateMeter()
{
if(*(shared_memory_cmd+73)==0)
 {
  system("./rate & ");
  *(shared_memory_cmd+73)=1; // Rate
 }
else
qDebug()<<"Ratemeter alreay opened!\n";
}
//////////////////////////////////////////////////////////////////// XRAY TABLE
void MainWindow::XrayTable()                                        
{  
  if(*(shared_memory_cmd+74)==0)
      {
     system("./XrayTable & ");
     *(shared_memory_cmd+74)=1;
      }
  else qDebug()<<"XrayTable already started...!!!\n";
}
//////////////////////////////////////////////////////////////////// HELIUM INTERFACE
void MainWindow::Helium_interface()                                  
{
  QMessageBox msgBox;
  msgBox.setText("GAS INTERFACE not implemented.... work in progress");
  msgBox.exec();
}
//////////////////////////////////////////////////////////////////// CAEN OSCILLOSCOPE
void MainWindow::caenoscilloscope()                                   
{
}
//////////////////////////////////////////////////////////////////// DIGITISER INTERFACE
void MainWindow::Digitiser2()                                        
  {
   if(*(shared_memory_cmd+72)==0)
    {
     system("./Digitiser_interface &"); 
     *(shared_memory_cmd+72)=1; // Digitiser_interface
    } 
   else qDebug()<<"....digitiser already started...";
  }


/////////////////////////////////////////////////////////////////// PI MOTOR TEST AND TABLE
void MainWindow::PI_motor_test()
  {
  if (*(shared_memory_cmd+76)==0)
    {
     system("./Test_Stage &"); 
     *(shared_memory_cmd+76)=1; // Digitiser_interface
    } 
   else qDebug()<<"....PI motor test tool already started...";
  }

void MainWindow::PI_motor_table()
  {
  if (*(shared_memory_cmd+77)==0)
    {
     system("./MotorParametersTable &"); 
     *(shared_memory_cmd+77)=1; // Digitiser_interface
    } 
   else qDebug()<<"....PI motor perameters table already started...";
  }



////////////////////////////////////////////////////////////////////  DETECTOR INTERFACE
void MainWindow::Detector()          
{
system("gnome-terminal -x sh -c  'HV_SDD;  bash' &"); 
}
//////////////////////////////////////////////////////////////////// VLC INTERFACE 
void MainWindow::VLC_interface()                              
{
  if(CameraOn) {system("pkill -9 vlc &"); CameraOn=false;} 
  else {system("vlc & "); CameraOn=true;}
 }


///////////////////////////////// 3D 

void MainWindow::TreD()
{
{system("./3D_Map & "); ;}
  }

void MainWindow::DueD()
{;
system("./3D_Map_Dev & "); 
  }




