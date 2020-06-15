#include "MultiFit_Window.h"
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <TROOT.h>
#include <TCanvas.h>
#include <TVirtualX.h>
#include <TSystem.h>
#include <TFormula.h>
#include <TF1.h>
#include <TH1.h>
#include <TFrame.h>
#include <TTimer.h>
#include <TLine.h>
#include <TLegend.h>
#include <TMath.h>
#include <TSpectrum.h>
#include <TPolyMarker.h>
#include <TMarker.h>


extern bool ROIStarted; extern float ROI[2];
extern bool FitWindowOn; extern bool Find_for_Multi; extern bool Click_for_Multi;
extern bool Clicked_for_Multi; extern bool PTabled_for_Multi;
extern bool PTable_for_Multi;
extern float PeakPositionX[12];
extern float PeakPositionY[12];
extern int PeaksFound;
extern int Max_spectrum;
extern float Cal; extern float offset;
extern float dataSpectrum[16384], SpectrumX[16384];
extern float Limit_dx[12], Limit_sx[12];
extern int FitFunctionsCodes[12];
extern int Tot_gauss, Tot_lor;
extern bool ROI_Ok, BKG_Ok, PEAKS_Ok, FFUNC_Ok;

extern bool EnROI, ClickROI;
extern bool Peaks_cancel;
extern bool SubtractBl;
extern int bkg;
extern int PMode;

extern bool single_draw;

MultiFit_Window::MultiFit_Window(QWidget *parent)
    : QWidget(parent)
{
    createROIGroupBox();
    createBaselineGroupBox();
    createPeaksGroupBox();
    createFitFunctionsGroupBox();

    BigStart = new QPushButton(this);
    BigStart->setText("Start Multi Peak fit");
    BigStart->setFixedHeight(70);
    BigStart->setFixedWidth(180);
    BigStart->setEnabled(false);
    connect(BigStart, SIGNAL(clicked()),this, SLOT(StartMultiFit()));

    BigReset = new QPushButton(this);
    BigReset->setText("Reset all");
    BigReset->setFixedHeight(70);
    BigReset->setFixedWidth(180);
    BigReset->setEnabled(true);
    connect(BigReset, SIGNAL(clicked()),this, SLOT(ResetAll()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(ROIGroupBox, 0, 0);
    layout->addWidget(BaselineGroupBox, 0, 1);
    layout->addWidget(PeaksGroupBox, 1, 0);
    layout->addWidget(FitFunctionsGroupBox, 1, 1);
    layout->addWidget(BigReset, 2, 0,Qt::AlignCenter);
    layout->addWidget(BigStart, 2, 1,Qt::AlignCenter);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    //layout->setRowMinimumHeight(0, E_lowSpinBox->sizeHint().height());
    //layout->setColumnMinimumWidth(0,E_lowSpinBox->sizeHint().width());

    setWindowTitle(tr("Multi Fit Widget"));
}


void MultiFit_Window::createROIGroupBox()
{
    ROIGroupBox = new QGroupBox(tr("ROI Definition"));

    selBox1 = new QGroupBox(); ///modi di definizione della roi 

    ROI1 = new QRadioButton(tr("By mouse click"));
    connect(ROI1,SIGNAL(toggled(bool)),this,SLOT(ROIMod()));
    ROI2 = new QRadioButton(tr("By energy interval"));
    ROI1->setChecked(true);
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(ROI1);
    hbox->addWidget(ROI2);
    hbox->addStretch(1);
    selBox1->setLayout(hbox);

    state1= new QPushButton(this);
    state1->setFixedHeight(25);
    state1->setFixedWidth(25);
    state1->setStyleSheet("background-color: red");
    state1->setCheckable(false);

    QLabel *Label1 = new QLabel(tr("E_low (keV)"));
    E_lowBox = new QLineEdit;
    E_lowBox->setEnabled(false);
    QLabel *Label2 = new QLabel(tr("E_high (keV)"));
    E_highBox = new QLineEdit;
    E_highBox->setEnabled(false);

    Startbutton1 = new QPushButton(this);
    Startbutton1->setText("Start");
    Startbutton1->setFixedHeight(50);
    Startbutton1->setFixedWidth(130);
    Startbutton1->setEnabled(true);

    connect(Startbutton1, SIGNAL(clicked()),this, SLOT(StartROIDef()));

    ROILineEdit= new QLineEdit(this);
    ROILineEdit->setEnabled(false);
    ROILineEdit->setReadOnly(true);
    ROILineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ROILineEdit->setText("Press Start button");

    ROILayout = new QGridLayout;
    ROILayout->addWidget(selBox1, 0, 0,1,4, Qt::AlignCenter);
    ROILayout->addWidget(state1, 0,4,Qt::AlignTop);
    ROILayout->addWidget(Startbutton1, 1, 0, Qt::AlignLeft);
    ROILayout->addWidget(ROILineEdit,1,1,1,3);
    ROILayout->addWidget(Label1, 2, 0, Qt::AlignRight);
    ROILayout->addWidget(E_lowBox, 2, 1, Qt::AlignLeft);
    ROILayout->addWidget(Label2, 2, 2, Qt::AlignLeft);
    ROILayout->addWidget(E_highBox, 2, 3, Qt::AlignLeft);
    ROIGroupBox->setLayout(ROILayout);

ROIGroupBox->setStyleSheet("QGroupBox{border:2px solid gray;border-radius:5px;margin-top: 1ex;} QGroupBox::title{subcontrol-origin: margin;subcontrol-position:top center;padding:0 3px;}");
}

void MultiFit_Window::createBaselineGroupBox()
{
    BaselineGroupBox = new QGroupBox(tr("Spectrum background"));
  
    selBox3 = new QGroupBox(); ///tipi di baseline 

    Bl1 = new QRadioButton(tr("No background"));
    Bl2 = new QRadioButton(tr("Constant background"));
    Bl3 = new QRadioButton(tr("Linear background"));
    connect(Bl1,SIGNAL(toggled(bool)),this,SLOT(BKGMod()));
    connect(Bl2,SIGNAL(toggled(bool)),this,SLOT(BKGMod()));
    connect(Bl3,SIGNAL(toggled(bool)),this,SLOT(BKGMod()));
    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(Bl1);
    vbox1->addWidget(Bl2);
    vbox1->addWidget(Bl3);
    vbox1->addStretch(1);
    selBox3->setLayout(vbox1);

    state2= new QPushButton();
    state2->setFixedHeight(25);
    state2->setFixedWidth(25);
    state2->setStyleSheet("background-color: red");
    state2->setCheckable(false);

    BlLabel = new QLabel(tr("Peaks background:"));

    BlCheckBox = new QCheckBox(tr("Subtract background"));
    BlCheckBox->setChecked(false);
    connect(BlCheckBox, SIGNAL(toggled(bool)),this, SLOT(BaselineSub()));

    QGridLayout *outerLayout = new QGridLayout;
    outerLayout->addWidget(BlLabel, 0, 0, Qt::AlignRight);
    outerLayout->addWidget(selBox3, 0, 1, Qt::AlignCenter);
    outerLayout->addWidget(state2,0,2,Qt::AlignTop);
    outerLayout->addWidget(BlCheckBox,1,0,1,3, Qt::AlignCenter);
    BaselineGroupBox->setLayout(outerLayout);
 
BaselineGroupBox->setStyleSheet("QGroupBox{border:2px solid gray;border-radius:5px;margin-top: 1ex;} QGroupBox::title{subcontrol-origin: margin;subcontrol-position:top center;padding:0 3px;}");
}

void MultiFit_Window::createPeaksGroupBox()
{
    PeaksGroupBox = new QGroupBox(tr("Find Peaks"));

    selBox2 = new QGroupBox(); ///modi di cercare i picchi 

    Peaks1 = new QRadioButton(tr("Auto"));
    Peaks1->setChecked(true);
    connect(Peaks1,SIGNAL(toggled(bool)),this,SLOT(PeaksMod()));
    Peaks2 = new QRadioButton(tr("Manually"));
    connect(Peaks2,SIGNAL(toggled(bool)),this,SLOT(PeaksMod()));
    Peaks3 = new QRadioButton(tr("With Periodic Table"));
    connect(Peaks3,SIGNAL(toggled(bool)),this,SLOT(PeaksMod()));
    QHBoxLayout *hbox1 = new QHBoxLayout;
    hbox1->addWidget(Peaks1);
    hbox1->addWidget(Peaks2);
    hbox1->addWidget(Peaks3);
    hbox1->addStretch(1);
    selBox2->setLayout(hbox1);

    state3= new QPushButton();
    state3->setFixedHeight(25);
    state3->setFixedWidth(25);
    state3->setStyleSheet("background-color: red");
    state3->setCheckable(false);

    PeaksTextEdit= new QTextEdit(this);
    PeaksTextEdit->setEnabled(false);
    PeaksTextEdit->setText("\nAuto mode selected:\n the routine will find the peaks present in the spectrum.\n Press START button");
    PeaksTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    Startbutton2 = new QPushButton(this);
    Startbutton2->setText("START");
    Startbutton2->setFixedHeight(40);
    Startbutton2->setFixedWidth(140);
    Startbutton2->setEnabled(true);
    connect(Startbutton2, SIGNAL(clicked()),this, SLOT(StartFindPeaks()));

    Stopbutton2 = new QPushButton(this);
    Stopbutton2->setText("STOP");
    Stopbutton2->setFixedHeight(40);
    Stopbutton2->setFixedWidth(140);
    Stopbutton2->setEnabled(false);
    connect(Stopbutton2, SIGNAL(clicked()), this, SLOT(StopPeaks()));

    Cancelbutton2 = new QPushButton(this);
    Cancelbutton2->setText("CANCEL");
    Cancelbutton2->setFixedHeight(40);
    Cancelbutton2->setFixedWidth(140);
    Cancelbutton2->setEnabled(false);
    connect(Cancelbutton2, SIGNAL(clicked()), this, SLOT(CancelPeaks()));

    QGridLayout *outerLayout1 = new QGridLayout;
    outerLayout1->addWidget(selBox2, 0, 0,Qt::AlignCenter);
    outerLayout1->addWidget(state3,0,1,Qt::AlignRight);
    outerLayout1->addWidget(PeaksTextEdit,1,0,1,3);
    outerLayout1->addWidget(Startbutton2, 2, 0,Qt::AlignCenter);
    outerLayout1->addWidget(Stopbutton2,2,1,Qt::AlignCenter);
    outerLayout1->addWidget(Cancelbutton2,2,2,Qt::AlignCenter);
    PeaksGroupBox->setLayout(outerLayout1);
PeaksGroupBox->setStyleSheet("QGroupBox{border:2px solid gray;border-radius:5px;margin-top: 1ex;} QGroupBox::title{subcontrol-origin: margin;subcontrol-position:top center;padding:0 3px;}");
}

void MultiFit_Window::createFitFunctionsGroupBox()
{
    FitFunctionsGroupBox = new QGroupBox(tr("Fit functions"));

    Label3 = new QLabel(tr("Peak number:"));
    Label4 = new QLabel(tr("Energy (keV):"));
    Label5 = new QLabel(tr("f(x)"));

    textEditP = new QTextEdit;
    textEditP->setReadOnly(true);
    //textEditP->setEnabled(false);
    textEditP->setLineWrapMode(QTextEdit::NoWrap);

    textEditE = new QTextEdit;
    textEditE->setReadOnly(true);
    //textEditE->setEnabled(false);
    textEditE->setLineWrapMode(QTextEdit::NoWrap);

    state4= new QPushButton();
    state4->setFixedHeight(25);
    state4->setFixedWidth(25);
    state4->setStyleSheet("background-color: red");
    state4->setCheckable(false);

    FCombo = new QComboBox();
    FCombo->addItem(tr("Gauss"));
    FCombo->addItem(tr("Lorentz"));
    connect(FCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFitFunction0(int)));
    FCombo1 = new QComboBox();
    FCombo1->addItem(tr("Gauss"));
    FCombo1->addItem(tr("Lorentz"));
    connect(FCombo1, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFitFunction1(int)));
    FCombo2 = new QComboBox();
    FCombo2->addItem(tr("Gauss"));
    FCombo2->addItem(tr("Lorentz"));
    connect(FCombo2, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFitFunction2(int)));
    FCombo3 = new QComboBox();
    FCombo3->addItem(tr("Gauss"));
    FCombo3->addItem(tr("Lorentz"));
    connect(FCombo3, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFitFunction3(int)));
    FCombo4 = new QComboBox();
    FCombo4->addItem(tr("Gauss"));
    FCombo4->addItem(tr("Lorentz"));
    connect(FCombo4, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFitFunction4(int)));
    FCombo5 = new QComboBox();
    FCombo5->addItem(tr("Gauss"));
    FCombo5->addItem(tr("Lorentz"));
    connect(FCombo5, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFitFunction5(int)));
    FCombo6 = new QComboBox();
    FCombo6->addItem(tr("Gauss"));
    FCombo6->addItem(tr("Lorentz"));
    connect(FCombo6, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFitFunction6(int)));
    FCombo7 = new QComboBox();
    FCombo7->addItem(tr("Gauss"));
    FCombo7->addItem(tr("Lorentz"));
    connect(FCombo7, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFitFunction7(int)));
    FCombo8 = new QComboBox();
    FCombo8->addItem(tr("Gauss"));
    FCombo8->addItem(tr("Lorentz"));
    connect(FCombo8, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFitFunction8(int)));
    FCombo9 = new QComboBox();
    FCombo9->addItem(tr("Gauss"));
    FCombo9->addItem(tr("Lorentz"));
    connect(FCombo9, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFitFunction9(int)));
    FCombo10 = new QComboBox();
    FCombo10->addItem(tr("Gauss"));
    FCombo10->addItem(tr("Lorentz"));
    connect(FCombo10, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFitFunction10(int)));
    FCombo11 = new QComboBox();
    FCombo11->addItem(tr("Gauss"));
    FCombo11->addItem(tr("Lorentz"));
    connect(FCombo11, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFitFunction11(int)));

    QGridLayout *outerLayout1 = new QGridLayout;
    outerLayout1->addWidget(Label3, 0, 0,Qt::AlignCenter);
    outerLayout1->addWidget(Label4, 0, 1,Qt::AlignCenter);
    outerLayout1->addWidget(Label5, 0, 2,Qt::AlignCenter);
    outerLayout1->addWidget(state4,0,4,Qt::AlignTop);
    outerLayout1->addWidget(textEditP, 1, 0,12,1);
    outerLayout1->addWidget(textEditE, 1, 1,12,1);
    outerLayout1->addWidget(FCombo, 1, 2);
    outerLayout1->addWidget(FCombo1, 2, 2);
    outerLayout1->addWidget(FCombo2, 3, 2);
    outerLayout1->addWidget(FCombo3, 4, 2);
    outerLayout1->addWidget(FCombo4, 5, 2);
    outerLayout1->addWidget(FCombo5, 6, 2);
    outerLayout1->addWidget(FCombo6, 7, 2);
    outerLayout1->addWidget(FCombo7, 8, 2);
    outerLayout1->addWidget(FCombo8, 9, 2);
    outerLayout1->addWidget(FCombo9, 10, 2);
    outerLayout1->addWidget(FCombo10, 11, 2);
    outerLayout1->addWidget(FCombo11, 12, 2);
  
    FitFunctionsGroupBox->setLayout(outerLayout1);
FitFunctionsGroupBox->setStyleSheet("QGroupBox{border:2px solid gray;border-radius:5px;margin-top: 1ex;} QGroupBox::title{subcontrol-origin: margin;subcontrol-position:top center;padding:0 3px;}");

}

void MultiFit_Window::ROIMod()
{
if(ROI1->isChecked()) {ClickROI=true; EnROI=false;}
else
if(ROI2->isChecked()) {ClickROI=false; EnROI=true;}
}

void MultiFit_Window::StartROIDef()
{
 ROILineEdit->setText("");

 if(ClickROI)
 {
  ROILineEdit->setText("Two clicks to define ROI");
  ROIStarted=true;
  }
 else
 if(EnROI)
 {
  /////Input energie per ROI///////
  QString mess1="Lower Energy";
  bool ok1;
      double chan1 = QInputDialog::getDouble(this, mess1, tr("ELow (keV):"), 0, 0, 60, 2, &ok1);
      if (ok1)
        {ROI[0]=chan1; QString s; s.setNum(chan1);E_lowBox->setText(s);}
  QString mess2="Upper Energy";
  bool ok2;
      double chan2 = QInputDialog::getDouble(this, mess2, tr("EHigh (keV):"), 60, 0, 60, 2, &ok2);
      if (ok2)
        {ROI[1]=chan2; QString s; s.setNum(chan2);E_highBox->setText(s);state1->setStyleSheet("background-color: green");
         ROILineEdit->setText("ROI ready"); ROI_Ok=true; if(BKG_Ok && PEAKS_Ok && FFUNC_Ok) BigStart->setEnabled(true);}

  }

}


void MultiFit_Window::BKGMod()
{
if(Bl1->isChecked()) {bkg=0;}
else
if(Bl2->isChecked()) {bkg=1;}
else
if(Bl3->isChecked()) {bkg=2;}
state2->setStyleSheet("background-color: green");
BKG_Ok=true; 
if(ROI_Ok && PEAKS_Ok && FFUNC_Ok) BigStart->setEnabled(true);
}

void MultiFit_Window::BaselineSub()
{
if(SubtractBl) SubtractBl=false;
else SubtractBl=true;
}

void MultiFit_Window::PeaksMod()
{
if(Peaks1->isChecked()) 
  {
   PMode=1;     
   PeaksTextEdit->setText("\nAuto mode selected:\n the routine will find the peaks present in the spectrum.\n Press START button.");
  }
else if(Peaks2->isChecked()) 
  {
   PMode=2;
   PeaksTextEdit->setText("\nManual mode selected:\n click on the spectrum to indicate the peaks to be considered.\n Press START button to begin and STOP button at the end.\n Click CANCEL to restart.");
  }
else if(Peaks3->isChecked()) 
  {
   PMode=3;
   PeaksTextEdit->setText("\nPeriodic Table mode selected:\n click on the elements in the table to select the peaks on the spectrum. Only the peaks inside the ROI will be considered.\n Press START button to begin and STOP button at the end.\n Click CANCEL to restart.");
  }
}
void MultiFit_Window::StartFindPeaks()
{
 if(PMode==1)
 {
  Find_for_Multi=true;
  PeaksFound=0;
  emit FP();
  }
 else  if(PMode==2)
 {
  Click_for_Multi=true;
  PeaksFound=0;
  Startbutton2->setEnabled(false);
  Stopbutton2->setEnabled(true);
  Cancelbutton2->setEnabled(true);
 }
 else  if(PMode==3)
 {
  PeaksFound=0;
  Startbutton2->setEnabled(false);
  Stopbutton2->setEnabled(true);
  Cancelbutton2->setEnabled(true);
  PTable_for_Multi=true;
  emit PTable();
 }

}

void MultiFit_Window::StopPeaks()
{
state3->setStyleSheet("background-color: green");
PEAKS_Ok=true; 
FFUNC_Ok=true;
state4->setStyleSheet("background-color: green");
if(Click_for_Multi)
 {
  Click_for_Multi=false;
  Clicked_for_Multi=true;
 }
if(PTable_for_Multi)
 {
  PTable_for_Multi=false;
  PTabled_for_Multi=true;
 }
Cancelbutton2->setEnabled(false);
if(ROI_Ok && BKG_Ok && FFUNC_Ok) BigStart->setEnabled(true);
}

void MultiFit_Window::CancelPeaks()
{
PEAKS_Ok=false;
state3->setStyleSheet("background-color: red");
textEditP->setText("");
textEditE->setText("");
Peaks_cancel=true;
emit StopFindPeaks();
Startbutton2->setEnabled(true);
PeaksFound=0;
}

void MultiFit_Window::StartMultiFit()
{
int j;
Tot_gauss=0; Tot_lor=0;
//for(int k=1;k<5000;k++) printf("%f  %f\n",SpectrumX[k],dataSpectrum[k] );
for(int i=0;i<PeaksFound;i++) //ricerca valli a dx e sx di ogni picco
 {
  if(FitFunctionsCodes[i]==0) Tot_gauss++;
  else if (FitFunctionsCodes[i]==1) Tot_lor++;
 }
printf("totale gaussiane %d  totale lorentz %d\n", Tot_gauss, Tot_lor);
emit Fit_tutti();
//PeaksFound=0;
}

void MultiFit_Window::SetFitFunction0(int k)
{
if(k==0) FitFunctionsCodes[0]=0;
else
if(k==1) FitFunctionsCodes[0]=1;
}
void MultiFit_Window::SetFitFunction1(int k)
{
if(k==0) FitFunctionsCodes[1]=0;
else
if(k==1) FitFunctionsCodes[1]=1;
}
void MultiFit_Window::SetFitFunction2(int k)
{
if(k==0) FitFunctionsCodes[2]=0;
else
if(k==1) FitFunctionsCodes[2]=1;
}
void MultiFit_Window::SetFitFunction3(int k)
{
if(k==0) FitFunctionsCodes[3]=0;
else
if(k==1) FitFunctionsCodes[3]=1;
}
void MultiFit_Window::SetFitFunction4(int k)
{
if(k==0) FitFunctionsCodes[4]=0;
else
if(k==1) FitFunctionsCodes[4]=1;
}
void MultiFit_Window::SetFitFunction5(int k)
{
if(k==0) FitFunctionsCodes[5]=0;
else
if(k==1) FitFunctionsCodes[5]=1;
}
void MultiFit_Window::SetFitFunction6(int k)
{
if(k==0) FitFunctionsCodes[6]=0;
else
if(k==1) FitFunctionsCodes[6]=1;
}
void MultiFit_Window::SetFitFunction7(int k)
{
if(k==0) FitFunctionsCodes[7]=0;
else
if(k==1) FitFunctionsCodes[7]=1;
}
void MultiFit_Window::SetFitFunction8(int k)
{
if(k==0) FitFunctionsCodes[8]=0;
else
if(k==1) FitFunctionsCodes[8]=1;
}
void MultiFit_Window::SetFitFunction9(int k)
{
if(k==0) FitFunctionsCodes[9]=0;
else
if(k==1) FitFunctionsCodes[9]=1;
}
void MultiFit_Window::SetFitFunction10(int k)
{
if(k==0) FitFunctionsCodes[10]=0;
else
if(k==1) FitFunctionsCodes[10]=1;
}
void MultiFit_Window::SetFitFunction11(int k)
{
if(k==0) FitFunctionsCodes[11]=0;
else
if(k==1) FitFunctionsCodes[11]=1;
}

void MultiFit_Window::ResetAll()
{
ROIStarted=false;
ROI_Ok=false; 
EnROI=false; ClickROI=true;
ROI[0]=200*Cal+offset; ROI[1]=5000*Cal+offset;
state1->setStyleSheet("background-color: red");
E_lowBox->setText("");
E_highBox->setText("");
ROILineEdit->setText("Press Start button");

BKG_Ok=false; SubtractBl=false;
bkg=-1;
BlCheckBox->setChecked(false);
Bl1->setAutoExclusive(false);
Bl1->setChecked(false);
Bl1->setAutoExclusive(true);
Bl2->setAutoExclusive(false);
Bl2->setChecked(false);
Bl2->setAutoExclusive(true);
Bl3->setAutoExclusive(false);
Bl3->setChecked(false);
Bl3->setAutoExclusive(true);
CancelPeaks();
PeaksFound=0;
PMode=1;
Peaks1->setChecked(true);
Peaks2->setChecked(false);
PeaksTextEdit->setText("\nAuto mode selected:\n the routine will find the peaks present in the spectrum.\n Press START button");
state2->setStyleSheet("background-color: red");
state3->setStyleSheet("background-color: red");
state4->setStyleSheet("background-color: red");
for(int g=0;g<12;g++)FitFunctionsCodes[g]=0;
Tot_gauss=0; Tot_lor=0;
PEAKS_Ok=false;
Startbutton2->setEnabled(true); Stopbutton2->setEnabled(false); Cancelbutton2->setEnabled(false);


FFUNC_Ok=false;
textEditE->setText("");
textEditP->setText("");
FCombo->setCurrentIndex(0);
FCombo1->setCurrentIndex(0);
FCombo2->setCurrentIndex(0);
FCombo3->setCurrentIndex(0);
FCombo4->setCurrentIndex(0);
FCombo5->setCurrentIndex(0);
FCombo6->setCurrentIndex(0);
FCombo7->setCurrentIndex(0);
FCombo8->setCurrentIndex(0);
FCombo9->setCurrentIndex(0);
FCombo10->setCurrentIndex(0);
FCombo11->setCurrentIndex(0);

BigStart->setEnabled(false);
emit ClearAll();
}


MultiFit_Window::~MultiFit_Window()
{
FitWindowOn=false;
}
