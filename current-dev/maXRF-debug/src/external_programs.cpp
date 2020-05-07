    #include "h/mainwindow.h"
#include "../Header.h"

extern tty_agent tty_ptr;

extern bool CameraOn;
extern int measuring_time;   extern int DAQ_TYPE;
extern int *shared_memory_cmd, *shared_memory;

void MainWindow::start_point_daq()
{
    switch(shared_memory_cmd[300])
    {
    case 0:
        shared_memory_cmd[300] = 1;
        shared_memory_cmd[301] = measuring_time;
        system("./app-modules/digitizer &");
        QTimer::singleShot(measuring_time * 1000, this, SLOT(stop_point_daq()));
        break;
    case 1:
    case 2:
        printf("[!] DAQ is already running\n");
        break;
    default:
        break;
    }
}

void MainWindow::stop_point_daq()
{
    switch(shared_memory_cmd[300])
    {
    case 0:
        printf("[!] Point mode DAQ already off");
        break;
    case 1:
    case 2:
    {
        shared_memory_cmd[300] = 0;
        QString file_directory = QFileDialog::getSaveFileName(this,tr("Save as ..."), QDir::currentPath());

        QFile file2(file_directory);
        file2.remove();
        file2.open(QIODevice::ReadWrite);
        QTextStream out2(&file2);

        for(int i=1;i<=16384;i++)
        {
            out2<<*(shared_memory+100+i)<<"\t"<<*(shared_memory+20000+i)<<"\t"<<*(shared_memory+40000+i)<<"\t\n";
            *(shared_memory+100+i)=0;
            *(shared_memory+20000+i)=0;
            *(shared_memory+40000+i)=0;
        }

        file2.close();
        qDebug()<<"... Spectra succesfully saved";
        break;
    }
    default:
        break;
    }
}

void MainWindow::ShowHistogram() {
    if ( *(shared_memory_cmd+71) == 0 ) {
        system("./app-modules/spectrum & ");
        *(shared_memory_cmd+71)=1; // XRF
    }
    else {
        qDebug()<<"[!] XRF histogram window already opened";
    }
}

void MainWindow::RateMeter() {
    if(*(shared_memory_cmd+73)==0)
    {
        system("./app-modules/rate & ");
        *(shared_memory_cmd+73)=1; // Rate
    }
    else
        qDebug()<<"Ratemeter alreay opened!\n";
}

void MainWindow::XrayTable()                                        
{  
    if(*(shared_memory_cmd+74)==0)
    {
        system("./app-modules/XRayTable & ");
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




