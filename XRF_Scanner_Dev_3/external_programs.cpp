// ALL external Programs
//
// Index:
//

#include "mainwindow.h"
#include "../Header.h"

extern int *shared_memory;   extern int *shared_memory2;    extern int *shared_memory_cmd;
extern int measuring_time;   extern int DAQ_TYPE;
extern bool CameraOn;

char process_daq[30];



//////////////////////////////////////////////////////////////////// DAQ START and STOP


void MainWindow::StartVme() {

    if(*(shared_memory_cmd+70)==0) {

        if(DAQ_TYPE==1) {
            system("./ADCXRF_USB &");
        }

        if(DAQ_TYPE==0) {
            system("./ADCXRF_Optical_Link &");
        }

        *(shared_memory2+9)=1;      // Tells the external program ADCXRF it should run on point acquisition mode
        *(shared_memory_cmd+70)=1;

        printf("... Measuring time:\t%d seconds\n", measuring_time);

        QTimer::singleShot(measuring_time*1000, this, SLOT(Stop_Vme()));
    }
    else qDebug()<<"[!] Acquisition already running";
}

void MainWindow::Stop_Vme() {

    if(*(shared_memory_cmd+70)==1) {

        *(shared_memory_cmd+70)=0;
        *(shared_memory2+9)=0;

        // Somewhere here should be a clause telling  the QTimer in the function above to stop.
        qDebug()<<"... Saving spectrum into a .txt file";


        QString file_directory = QFileDialog::getSaveFileName(this,tr("Save as ..."), QDir::currentPath());

        QFile file2(file_directory);
        file2.remove();
        file2.open(QIODevice::ReadWrite);
        QTextStream out2(&file2);

        for(int i=1;i<=16384;i++) {
            out2<<*(shared_memory+100+i)<<"\t"<<*(shared_memory+20000+i)<<"\t"<<*(shared_memory+40000+i)<<"\t\n";
            *(shared_memory+100+i)=0;
            *(shared_memory+20000+i)=0;
            *(shared_memory+40000+i)=0;
        }

        file2.close();
        qDebug()<<"... Spectrum succesfully saved";
    }

    else qDebug()<<"[!] Point mode acquisition already off";
}


//////////////////////////////////////////////////////////////////// HISTOGRAM
void MainWindow::ShowHistogram() {
    if ( *(shared_memory_cmd+71) == 0 ) {
        system("./XRF & ");
        *(shared_memory_cmd+71)=1; // XRF
    }
    else {
        qDebug()<<"[!] XRF histogram window already opened";
    }
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
     system("./XRayTable & ");
     *(shared_memory_cmd+74)=1;
      }
  else qDebug()<<"[!] X-Ray table window already opened";
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




void MainWindow::TreD()
{;
  }

void MainWindow::DueD()
{;
  }




