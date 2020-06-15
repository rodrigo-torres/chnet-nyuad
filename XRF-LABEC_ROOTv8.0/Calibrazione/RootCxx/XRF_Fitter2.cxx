// Compilare come:

// rootcint -f ex2aDict.cxx -c example2a.h ex2aLinkDef.h
// g++ `root-config --cflags --glibs` -o mainframe example2a.cxx ex2aDict.cxx

// g++ -DSTANDALONE -O3 -Wall $(root-config --cflags --glibs) shmemsildaq6fAms.cxx ex2aDict.cxx -o mainframe


#include <TApplication.h>
#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TRootEmbeddedCanvas.h>
#include "example2a.h"

////// New INCLUDE added
#include <stdio.h>
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TFile.h>
#include <TPad.h>

#include <stdlib.h> 
#include <sys/io.h> 
#include <TUnixSystem.h>
#include <TNamed.h>
#include <TSystem.h>

//added
#include <TTimer.h>
#include <TString.h>
#include <TGStatusBar.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TPaveLabel.h>
#include <TPaveText.h>
#include <TPavesText.h>
#include <TGComboBox.h>  /// 	TAttText
#include <TGString.h>

#include <TMath.h>

// Prova Shared Memory
//#include </home/frao/QtRoot/root/cint/cint/include/iostream.h>
//#include <iostream.h> c++ added
#include <fstream>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
//using namespace std; 
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#define SHMSZ  16
#define SHMSZString  256

int *shm; // *s;
int shmid; int shmidDouble;
key_t key; key_t keyString;

double *shared_memDouble;
int *shared_memory; 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// START STANDARD GUI ///////////////////////////////////////
char StatusBarText1[80];

char defDaq[200];
char DefDirV;

int i;
FILE *fpx;
char ch;
float y;
int x; int z; float CalFactor=1.0; int CalValue=1; bool TimeLinear=false;
float xf,zf;

int cmnd=0;
TH1F *h1x=NULL;

int xx1,xx2,xx3;
FILE *fzoom, *fpid1, *froot, *fileType, *fprt;

int fitStatus=0;
char ExportValue[200];
int min, max, amp; 
int XampH2=128; int XminH2,XmaxH2; int YampH2=128; int YminH2,YmaxH2; int H2DZUP=0;
int XminH2Cal,XmaxH2Cal,YampH2Cal,YminH2Cal;

int PrType=0; int cmndStore=0;
float channel[65536]; 
int PrevOn=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////// BOOLS FOR SELECTIONS ////////////////////////////////////////// BOOLS FOR SELECTIONS  /////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool LogOn=false; bool GridOn=false; //bool H2DSURF=false;
bool PviewOn=false;  bool Histo=false; bool Graph=false; bool PrintOn=false;

bool SimpleGauss=false; bool Linear=false;
int FitOnGauss=0;
bool GFit=false; bool H2D=false;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Calibration=false;

bool Fit_enabled_bool=false;

FILE *PlotTitle; FILE *XLabel; FILE *YLabel;   
TString htitle; TString xaxis; TString yaxis;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MainCommand=0; int CommandList=0; int GraphHistoSel=0;
int StartStopCmd=0;
Double_t GFitParam[8]; int iGFit=0;
/////////////////////////////////////////////////////////////////////// TextPave for fit ///////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TGString Lego, Lego1, Lego2, Surf, Select;
 int SurfSel=0; TGString SelectedSurf;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool DoDrawInProgress=false; int marker[3], i_marker_ev=0; bool line1_on=false, line2_on=false, line3_on=false;
int Fit_Xmin=0; int Fit_Xmax=0; int X_Peak_value=0; bool run_fit=false;

//////////////////////////////////////////////////////// MOUSE ON GPAD

/*
void GetMouseEvent()
{

   // get event information
   int event = gPad->GetEvent();
   int px    = gPad->GetEventX();
   int py    = gPad->GetEventY();

   // some magic to get the coordinates...
   double xd = gPad->AbsPixeltoX(px);
   double yd = gPad->AbsPixeltoY(py);
   float x = gPad->PadtoX(xd);
   float y = gPad->PadtoY(yd);


if (event==1) { // left mouse button click
               cout<< "X: "<<x<<" Y: "<<y;
              }

return;
}
*/





//////////////////////////// FIT FUNCTIONS ///////////////////////////////////////////


// Polinomial background function
Double_t background(Double_t *x, Double_t *par) {
   return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}


// Gauss  f(x)= p0*exp(-0.5*((x-p1)/p2)^2)

Double_t GaussPeak(Double_t *x, Double_t *par) {
  return (par[0]*TMath::Exp(-0.5*((x[0]-par[1])/par[2])*((x[0]-par[1])/par[2])));
}

Double_t fitFunction(Double_t *x, Double_t *par) {
  return background(x,par) + GaussPeak(x,&par[3]);
}


  MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {  //Tutte le modifiche fatte qui dentro devono passare dal ROOTCINT
  // Create a main frame
  fMain = new TGMainFrame(p,w,h);

//  fMain->SetIconPixmap("Pictures/Infn2.gif");
  Lego="Linear"; Lego1="Quadratic"; Lego2="Cubic"; Select="no BG Function"; 

// Create canvas widget
  fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,1200,460);
  fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX| kLHintsExpandY,
					      10,10,10,1)); //10,10,10,1





  fMain->Connect("CloseWindow()", "MyMainFrame", this, "CloseMainWindow()");

  TTimer *timer1 = new TTimer();
  timer1->Start(250, kFALSE); //era500
  timer1->Connect("Timeout()", "MyMainFrame",this, "TimeOut()");

// Create a horizontal frame widget with buttons
  TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,1200,300);


  TGCheckButton *EnTimeCal = new TGCheckButton(hframe,"&EnTimeCal");
  EnTimeCal->Connect("Clicked()","MyMainFrame",this,"EnTimeCal()");
  hframe->AddFrame(EnTimeCal, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGCheckButton *TimeLinear = new TGCheckButton(hframe,"&t^(-1/2)");
  TimeLinear->Connect("Clicked()","MyMainFrame",this,"TimeSqrt()");
  hframe->AddFrame(TimeLinear, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGComboBox *H2Surf = new TGComboBox(hframe,"SelectBGFunction");
  H2Surf->AddEntry(Select,0);
  H2Surf->AddEntry(Lego,1);
  H2Surf->AddEntry(Lego1,2); 
  H2Surf->AddEntry(Lego2,3); 
  H2Surf-> Resize(150,20);
  H2Surf->Connect("Selected(int)","MyMainFrame",this,"H2DSurf(int)");
  hframe->AddFrame(H2Surf, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGCheckButton *LogScale = new TGCheckButton(hframe,"&LogScale");
  LogScale->Connect("Clicked()","MyMainFrame",this,"LogScale()");
  hframe->AddFrame(LogScale, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGCheckButton *Grid = new TGCheckButton(hframe,"&Grid");
  Grid->Connect("Clicked()","MyMainFrame",this,"Grid()");
  hframe->AddFrame(Grid, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGCheckButton *EnablingFit = new TGCheckButton(hframe,"&EnableFit");
  EnablingFit->Connect("Clicked()","MyMainFrame",this,"Fit_enabled()");
  hframe->AddFrame(EnablingFit, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGTextButton *DoFit = new TGTextButton(hframe,"&Fit");
//  draw->SetFont(32);
  DoFit->Connect("Clicked()","MyMainFrame",this,"RunFit()");
  hframe->AddFrame(DoFit, new TGLayoutHints(kLHintsCenterX,5,5,3,3));



  TGTextButton *draw = new TGTextButton(hframe,"&Draw/ReDraw");
//  draw->SetFont(32);
  draw->Connect("Clicked()","MyMainFrame",this,"SingleDraw()");
  hframe->AddFrame(draw, new TGLayoutHints(kLHintsCenterX,5,5,3,3)); /////////////////////////////////////////////////////////////////////////////////////7

  TGCheckButton *PreviewOn = new TGCheckButton(hframe,"&Preview on Export");
  PreviewOn->Connect("Clicked()","MyMainFrame",this,"PreviewOn()");
  hframe->AddFrame(PreviewOn, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  //  TGTextButton *exit = new TGTextButton(hframe,"&Exit",
  //					"gApplication->Terminate(0)");

  TGTextButton *exit = new TGTextButton(hframe,"&Exit");
  exit->Connect("Clicked()","MyMainFrame",this,"endRoot()");
  hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,3));



  //Int_t parts[] = {45, 15, 10, 30}; fStatusBar->SetParts(parts, 4) SetStatusText(text3,3); 
  Int_t parts[]={24,68,8};

  /*  TGStatusBar * */StatusBar = new TGStatusBar(fMain,5,5,kVerticalFrame);         
  fMain->AddFrame(StatusBar, new TGLayoutHints(kLHintsExpandX,10,10,2,2));
// StatusBar->SetTextSize(20);
  

  StatusBar->SetParts(parts,3);
  StatusBar->SetText("XRF Fitter: RootGui StatusBar",0);
  //  StatusBar->Connect("MyMainFrame",this,"BarStatus()");

  // add hframe to main
  fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

  // Set a name to the main frame
  fMain->SetWindowName("XRF Fitter: RootGui (alpha)");
  // Map all subwindows of main frame
  fMain->MapSubwindows();
  // Initialize the layout algorithm
  fMain->Resize(fMain->GetDefaultSize());

  // Map main frame
  fMain->MapWindow();

}

void MyMainFrame::CloseMainWindow()  ////////////////  ACKNOWLEDGE FOR QtGUI on xButton close....
{ int rootDone=0;  *(shared_memory+1)=rootDone;  
  gApplication->Terminate(0);}

void MyMainFrame::endRoot() {
  int rootDone=0;
   *(shared_memory+1)=rootDone; 
   gApplication->Terminate(0);
}

MyMainFrame::~MyMainFrame() { 
  fMain->Cleanup();   // Clean up used widgets: frames, buttons, layouthints
  delete fMain;
 }

void example() {new MyMainFrame(gClient->GetRoot(),200,200);}

void MyMainFrame::InfoBar()
{ //printf("da qua....\n");
  if(Histo) StatusBar->SetText("Histo",1);
  if(Graph) StatusBar->SetText("Graph",1);
  Refresh();
}

void MyMainFrame::LogScale()
{if(!LogOn){LogOn=true;} else{LogOn=false;}DoDraw();}
void MyMainFrame::Grid()
{if(!GridOn){GridOn=true;} else{GridOn=false;}DoDraw();}

void MyMainFrame::EnTimeCal()
{if(!Calibration){Calibration=true; CalValue=*(shared_memory+42); CalFactor= CalValue*0.0001;xaxis="keV";} else{Calibration=false;CalFactor=1.0;xaxis="Channel";}}

void MyMainFrame::TimeSqrt()
{if(!TimeLinear){TimeLinear=true;} else {TimeLinear=false;}} 

void MyMainFrame::Refresh()
{  if(!DoDrawInProgress)  //if(!PrintOn)  // To let the program process printing data....// meglio mettere un if sul DODRAW
    { CmndList();  //printf("MCmnd %d\n", MainCommand); 
      if (MainCommand==1) {DoDraw();}}
}

void MyMainFrame::CmndList()
{
  MainCommand=*shared_memory;

  printf("here\n");

   // get event information
   int event = gPad->GetEvent();
   int px    = gPad->GetEventX();
   int py    = gPad->GetEventY();

   // some magic to get the coordinates...
   double xd = gPad->AbsPixeltoX(px);
   double yd = gPad->AbsPixeltoY(py);
   float x = gPad->PadtoX(xd);
   float y = gPad->PadtoY(yd);


if (event==1) { // left mouse button click

               marker[i_marker_ev]=int(x);
               //cout<< "X: "<<marker[i_marker_ev];
               i_marker_ev++;
               if (i_marker_ev > 2) i_marker_ev=0;

               if(line2_on) line3_on=true;
               if(line1_on) line2_on=true;               
               line1_on=true;
               DoDraw();
              }
}

void MyMainFrame::SingleDraw() ////////////// QUi ///////////////////////////////  e gira
{ if(!DoDrawInProgress){CmndList(); DoDraw();}}
//{ if(!PrintOn){CmndList(); DoDraw();}} // meglio mettere un if sul DODRAW

void MyMainFrame::Fit_enabled()
{
if (!Fit_enabled_bool)
     {Fit_enabled_bool=true;printf("fit enabled\n");}
else 
     {Fit_enabled_bool=false;printf("fit disabled\n");}
}

void MyMainFrame::TimeOut()   // Refresh Data Input File
{Refresh();}

void MyMainFrame::PreviewOn()
{
  if(PviewOn)
    {PviewOn=false; PrevOn=0; /*printf("%i\n",PrevOn); */}
  else
    {PviewOn=true; PrevOn=1; /*printf("%i\n",PrevOn);*/}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MyMainFrame::H2DSurf(int BG_Sel)
{
  SurfSel=BG_Sel;
  if(BG_Sel==0) {SurfSel=0;}
  else
    { SurfSel=1;
      switch(BG_Sel)
	{ case 1: {SelectedSurf="LEGO";break;}
	  case 2: {SelectedSurf="LEGO1";break;}
	  case 3: {SelectedSurf="LEGO2";break;}
	  case 4: {SelectedSurf="SURF";break;}
	}
    }
  printf("case %i\tComboSel %i\n",BG_Sel,SurfSel);
}



void MyMainFrame::RunFit()
{
 if(line1_on && line2_on && line3_on)
{
//   for (int i = 0; i < 3; ++i)
//      cout <<" ..Value "<<marker[i] ;   
   
   int maxtmp=marker[0];
   int mintmp=marker[0];
   int peak_value=marker[0];

    if (marker[1]>marker[0]) {maxtmp=marker[1];} else {mintmp=marker[1];}
    if (marker[2]>maxtmp)    {peak_value=maxtmp; maxtmp=marker[2];}
    if (marker[2]<mintmp)    {peak_value=mintmp; mintmp=marker[2];}

Fit_Xmin=mintmp; Fit_Xmax=maxtmp; X_Peak_value=peak_value;
run_fit=true;

//    cout <<" min: "<< mintmp << " ---- peak val "    << peak_value << " ---- max " <<  maxtmp;   
}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// START ROOTGUI DRAW   ////// START ROOTGUI DRAW   ////// START ROOTGUI DRAW   ////// START ROOTGUI DRAW   ////// START ROOTGUI DRAW   /////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MyMainFrame::DoDraw()
{ DoDrawInProgress=true; 
  gROOT->Reset();   // AZZERA TUTTE LE VARIABILI//////////////////////////////
  //  gStyle->SetOptStat("ne"); 




   int j; const int bin= 16384;
   Double_t dataSpectrum[bin];
   int ch =0;
   FILE *f0;
   f0=fopen("data_prova","r");
   while( ! feof(f0))
     {
      fscanf(f0,"%d\n",&j);
      dataSpectrum[ch]=j; ch++;
     }
   fclose(f0);



   TH1F *f1 = new TH1F("histo",
   "Fit",4092,0,4092);
   f1->Reset();
   f1->SetMarkerStyle(21);
   f1->SetMarkerSize(0.8);
   f1->SetStats(0);



   for(int i=0; i < bin;  i++) f1->SetBinContent(i+1,dataSpectrum[i]);

     ///// parameter serarch

    double angular=1, coefficient=1;
    if((Fit_Xmax-Fit_Xmin)!=0) 
       {angular=(dataSpectrum[Fit_Xmax]-dataSpectrum[Fit_Xmin])/(Fit_Xmax-Fit_Xmin);
        coefficient=((Fit_Xmax*dataSpectrum[Fit_Xmin])-(Fit_Xmin*dataSpectrum[Fit_Xmax]))/(Fit_Xmax-Fit_Xmin);
       }

     ////// parameter search gauss
     int Par_gaus1= int (dataSpectrum[X_Peak_value]);
     printf("Xgauss %i     Value %i    Sigma", X_Peak_value, Par_gaus1);




     TF1 *fitFcn = new TF1("fitFcn",fitFunction,Fit_Xmin,Fit_Xmax,6); //950,1150,6)
     TF1 *backFcn = new TF1("backFcn",background,Fit_Xmin,Fit_Xmax,3);
     TF1 *signalFcn = new TF1("signalFcn",GaussPeak,Fit_Xmin,Fit_Xmax,3);
     TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);

   if (run_fit) //////////////////////////////////////////////////////////////////////////////////////////////////////////////
   {

     // create a TF1 with the range from 0 to 3 and 6 parameters
//     TF1 *fitFcn = new TF1("fitFcn",fitFunction,Fit_Xmin,Fit_Xmax,6); //950,1150,6)
     fitFcn->SetNpx(500);
     fitFcn->SetLineWidth(4);
     fitFcn->SetLineColor(kMagenta);

//     fitFcn->SetParameters(coefficient,angular,0.01,Par_gaus1,X_Peak_value,20);

//     fitFcn->SetParameters(coefficient,angular,Par_gaus1,X_Peak_value,20);

     fitFcn->SetParameters(1,1,0.1,Par_gaus1,X_Peak_value,10);

     f1->Fit("fitFcn","R","ep");

   // improve the picture:
//     TF1 *backFcn = new TF1("backFcn",background,Fit_Xmin,Fit_Xmax,3);
     backFcn->SetLineColor(kRed);
//     TF1 *signalFcn = new TF1("signalFcn",GaussPeak,Fit_Xmin,Fit_Xmax,3);
     signalFcn->SetLineColor(kBlue);
     signalFcn->SetNpx(500);
     Double_t par[6];

   // writes the fit results into the par array
     fitFcn->GetParameters(par);
     backFcn->SetParameters(par);
     signalFcn->SetParameters(&par[3]);

   // draw the legend
//     TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
     legend->SetTextFont(72);
     legend->SetTextSize(0.04);
     legend->AddEntry(f1,"Data","lpe");
     legend->AddEntry(backFcn,"Background fit","l");
     legend->AddEntry(signalFcn,"Signal fit","l");
     legend->AddEntry(fitFcn,"Global Fit","l");
   }

     gStyle->SetOptFit(0011);

//      if(SimpleGauss)
// {f1->Fit("gaus"); gStyle->SetOptFit(0011);}
      if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
      if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);} ////////////////////////// VirtualTPad




      TCanvas *fCanvas = fEcanvas->GetCanvas();

      f1->Draw("elp");         

      if (Fit_enabled_bool)
       {
        if(line1_on)
          {TLine *marker1 = new TLine(marker[0],0,marker[0],20000);marker1->SetLineColor(kRed);marker1->Draw("same");marker1->SetLineWidth(2);} //TLine(x1,y1,x2,y2);
        if(line2_on)
          {TLine *marker2 = new TLine(marker[1],0,marker[1],20000);marker2->SetLineColor(kRed);marker2->Draw("same");marker2->SetLineWidth(2);}       
        if(line3_on)
          {TLine *marker3 = new TLine(marker[2],0,marker[2],20000);marker3->SetLineColor(kRed);marker3->Draw("same");marker3->SetLineWidth(2);}

       }

   if(run_fit)    
      { printf(" runfit\n");
      backFcn->Draw("same");
      signalFcn->Draw("same");
      legend->Draw();
      }


//      gStyle->SetOptStat("ne");
        


      fCanvas->cd();
      fCanvas->Update(); 
//      if(PrintOn)
//	{ PrintGui();} 
f1->Clear();

	

DoDrawInProgress=false;
}

////////////////////////////////////////////////////////// MAIN ////////// MAIN ///////////////// MAIN ///////////////

  int main(int argc, char **argv) {
  TApplication theApp("App",&argc,argv);

  ////////// STARTING SHARED MEMORY MAPPING 

  key = 6400;
  shmid = shmget (6400, SHMSZ, IPC_CREAT | 0666);
  shared_memory =(int *) shmat(shmid, NULL, 0);
  cout << "Memory attached at :" << (int *)shared_memory<< endl << " with shmid: " << shmid << endl;

  key = 6500;
  shmidDouble = shmget (6500, SHMSZ, IPC_CREAT | 0666);
  shared_memDouble =(double *) shmat(shmid, NULL, 0);
  cout << "DoubleData Memory attached at :" << (double *)shared_memDouble<< endl << " with shmid: " << shmidDouble << endl;


  xx1=2048; xx2=0; xx3=2048;
  gROOT->SetStyle("Plain"); // toglie il grigio
  amp=xx1;  min=xx2; max=xx3;
  XampH2=2048; XminH2=0; XmaxH2=2048;
  YampH2=2048; YminH2=0; YmaxH2=2048;

  cmnd=0;

  example();
  theApp.Run();
 
return 0;
}









/*

void Clicked() {
   int event = gPad->GetEvent();
   if (event != 11) return;
   TObject *select = gPad->GetSelected();
   if (!select) return;
   if (select->InheritsFrom("TH1F")) {
   TH1F *h = (TH1F*)select;
   int px = gPad->GetEventX();
   int py = gPad->GetEventY();
   cout << px << endl;
   }
}

*/


//       TTimer *timer = new TTimer();                                
//       timer->Connect("Timeout()", "MyMainFrame",DoDraw, "TimerDone()");   
//       timer->Start(2000, kTRUE);   // 2 seconds single-shot        
// Timeout signal is emitted repeadetly with minimum timeout  
// timer->Start(0, kFALSE); 
/*

bool FitOnGauss=false; bool FitOnLinear=false; bool FitOnPoliQuad=false;
bool FitOnPoliCubic=false; bool FitOnLandau=false; bool FitOnPoliGauss=false; bool FitOnExponential=false; 

      printf("\n\nal1 %d\tah1 %d\tal2 %d\tah2 %d\tal3 %d\tah3 %d\tal4 %d\tah4 %d\tal5 %d\tah5 %d\n\n",FitLower1,FitUpper1,FitLower2,FitUpper2,FitLower3,FitUpper3,AreaFitLower4,AreaFitUpper4,AreaFitLower5,AreaFitUpper5);

 */





