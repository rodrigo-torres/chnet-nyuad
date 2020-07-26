#include <mainwindow.h>
#include "../Header.h"
#include "DppGui.h"

extern int *shared_memory_cmd, *shared_memory4;
extern char *shared_memory5;

/*  Shared memory definitions for DPP:
                                  *(shared_memory4)=40;        // Trigger Threshold
                                  *(shared_memory4+1)=0;       // Input range digitalizzatore (0 -> 0.6V // 1 -> 1.4V
                                  *(shared_memory4+2)=800;     // Trapezoid Rise Time (ns)
                                  *(shared_memory4+3)=2500;    // Trapezoid Flat Top  (ns)
                                  *(shared_memory4+4)=12000;   // Decay Time Constant (ns) HACK-FPEP the one expected from fitting algorithm?
                                  *(shared_memory4+5)=1500;    // Peaking delay  (ns)
                                  *(shared_memory4+6)=2;       // Trigger Filter smoothing factor
                                  *(shared_memory4+7)=0;       // OFFSET
                                  *(shared_memory4+8)=3000;    // Trigger Hold Off
                                  *(shared_memory4+9)=256;     // BaseLine Mean - ?? 3 = bx10 = 64 samples (baseline mean del trapezio) (...?? 3)
                                  *(shared_memory4+10)=1;      // peak mean (numero punti su cui mediare per peaking del trapezio)
                                  *(shared_memory4+11)=3000;   // peak holdoff (min intervallo tra 2 trapezi...minimo=k+m)
                                  *(shared_memory4+12)=1;      // Trapezoid Gain
                                  *(shared_memory4+13)=100;    // NOT USED: Delay(b) Input Signal Rise time (ns)...sarebbe delay (b)
                                  *(shared_memory4+14)=10;     // NOT USED: Energy Normalization Factor

                                  *(shared_memory4+15)=0;      // Enable digitiser reading
*/

MainWindowDPP::MainWindowDPP() {
    creatorDppGui();
}

MainWindowDPP::~MainWindowDPP() { /* The destructor is useless when this class is accessed through a pointer*/}

void MainWindowDPP::closeEvent(QCloseEvent *event) {
    event->ignore();
    *(shared_memory_cmd+72)=0;
    //qDebug()<<"closeEvent() called, new value of *(shared_memory_cm+72) is :\t"<<*(shared_memory_cmd+72);
    event->accept();
}


void MainWindowDPP::Digi_treshold(int trshd)        ///// TRESHOLD
{*(shared_memory4)=trshd;}

void MainWindowDPP::Digi_range(int range) {
    *(shared_memory4+1) = range;
    qDebug()<<range;
}

void MainWindowDPP::Rise_Time(int risetime)         ///// RISE TIME
{*(shared_memory4+2)=risetime;}

void MainWindowDPP::Flat_Top(int flattop)           ///// FLAT TOP
{*(shared_memory4+3)=flattop;}

void MainWindowDPP::Fall_Time(int falltime)         ///// FALL TIME
{*(shared_memory4+4)=falltime;}

void MainWindowDPP::Peaking_Delay(int peaking)      ///// PEAKING TIME
{*(shared_memory4+5)=peaking;}

void MainWindowDPP::Smoothing_Factor(int smoothing) ///// SMOOTHING FACTOR
{*(shared_memory4+6)=smoothing;}

void MainWindowDPP::paramDCOffset(int offset) {
    double fract = (double) (65536 * offset) / 100;
    if ((fract - (int) fract) >= 0.5 && (((int) fract) % 2 == 0)) offset = ceil(fract);
    else offset = floor(fract);

//    The following is for the case when it becomes compulsory to specify the DC offset as an hex value
//    QString result = QString::number(offset,16).toUpper();
//    if (result.length() % 2) result.insert(0,"0");
//    result.insert(0,"0x");

//    char strResult[6] = { 0 };
//    memcpy(strResult,result.toStdString().c_str(),result.size());
//    for (int i = 0; i < result.size(); i++) *(shared_memory5+i) = strResult[i];
    *(shared_memory4+7) = offset;
}

void MainWindowDPP::HoldOff(int holdoff)            ///// TRIGGER HOLDOFF
{*(shared_memory4+8)=holdoff;}

void MainWindowDPP::Base_Line_Mean(int baseline)    ///// BASELINE MEAN
{*(shared_memory4+9)=baseline;}

void MainWindowDPP::Peak_Mean(int peakmean)         ///// PEAK MEAN
{*(shared_memory4+10)=peakmean;}

void MainWindowDPP::Peak_Holdoff(int peakholdoff)   ///// PEAK HOLDOFF
{*(shared_memory4+11)=peakholdoff;}

void MainWindowDPP::Trapezoid_Gain(int trapgain)    ///// TRAPEZOID GAIN
{*(shared_memory4+12)=trapgain;}

void MainWindowDPP::overrideDppParam(bool getData) {
    if (getData) {
        *(shared_memory4+15)=1;
        qDebug()<<"[!] Overriding default ADC parameters";
    }
    else {
        *(shared_memory4+15)=0;
        qDebug()<<"[!] Reverting to default ADC parameters";
    }
}
