#include "mainwindow.h"
#include "../Header.h"


extern bool CameraOn;
extern int measuring_time;   extern int DAQ_TYPE;
extern int *shared_memory_cmd, *shared_memory, *shared_memory2;



extern bool AutofocusOn;
extern int serialX, serialY, serialZ, serialK;
extern int send_command(int chan,const char *comando, const char *parametri, int port);

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
    send_command(1,"HLT",NULL,serialX);
    send_command(1,"HLT",NULL,serialY);
    send_command(1,"HLT",NULL,serialZ);
    timer->blockSignals(true);
    timerAutofocus->blockSignals(true);

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

    timer->blockSignals(false);
    timerAutofocus->blockSignals(false);
    tcflush(serialK, TCIFLUSH);
}

void MainWindow::ShowHistogram() {
    if ( *(shared_memory_cmd+71) == 0 ) {
        system("./XRF & ");
        *(shared_memory_cmd+71)=1; // XRF
    }
    else {
        qDebug()<<"[!] XRF histogram window already opened";
    }
}

void MainWindow::RateMeter() {
    if(*(shared_memory_cmd+73)==0)
    {
        system("./rate & ");
        *(shared_memory_cmd+73)=1; // Rate
    }
    else
        qDebug()<<"Ratemeter alreay opened!\n";
}

void MainWindow::XrayTable()                                        
{  
    if(*(shared_memory_cmd+74)==0)
    {
        system("./XRayTable & ");
        *(shared_memory_cmd+74)=1;
    }
    else qDebug()<<"[!] X-Ray table window already opened";
}


void MainWindow::openDPPInter() {
    if (*(shared_memory_cmd+72) == 0) {
        d_guiDPP =  new MainWindowDPP();
        d_guiDPP->setFixedSize(580,370);
        d_guiDPP->setWindowTitle("Digitizer Settings");
        d_guiDPP->show();

        *(shared_memory_cmd+72) = 1; // DPP interface active
    }
    else qDebug()<<"[!] DPP window already opened";
}

void MainWindow::Detector() {
}

void MainWindow::VLC_interface() {
    if (CameraOn) {system("pkill -9 vlc &"); CameraOn=false;}
    else {system("vlc & "); CameraOn=true;}
}

void MainWindow::Helium_interface() {
}

void MainWindow::caenoscilloscope() {
}


void MainWindow::TreD() {
}

void MainWindow::DueD() {
}




