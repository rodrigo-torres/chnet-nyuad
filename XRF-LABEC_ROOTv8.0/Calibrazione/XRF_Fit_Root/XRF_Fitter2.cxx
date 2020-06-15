///////////////LARA MOB/////////////////////////////////////////////
//   ./thisroot.sh
//   rootcint -f XRF_Fitter2Dict.cxx -c example2a.h ex2aLinkDef.h
//   g++ -DSTANDALONE -O3 -Wall $(root-config --cflags --glibs) XRF_Fitter2.cxx XRF_Fitter2Dict.cxx -o mainframe
/////////////////////////////£££££££££/////////////////////////////////////////////

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
#include <TString.h>

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


#include <TGLabel.h>
#include <TGTextBuffer.h>
#include <TGTextEntry.h>
#include <TGFileDialog.h>
#include <TGMsgBox.h>
#include <TGInputDialog.h>



#include <fstream>
#include <iostream>
#include <sstream>
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

#include "TEnv.h"


int *shm; 
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

float mu1, sigma1, Integral1, chi2rid,mu2, sigma2,Integral2;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////// BOOLS FOR SELECTIONS ////////////////////////////////////////// BOOLS FOR SELECTIONS  /////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool LogOn=false; bool GridOn=false; 
bool PviewOn=false;  bool Histo=false; bool Graph=false; bool PrintOn=false;
bool single_draw=false; bool histo_on=false; //questo è true quando c'è uno spettro disegnato
bool timer_stop=false;

bool SimpleGauss=false; bool Linear=false;
int FitOnGauss=0;
bool GFit=false; bool H2D=false;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Calibration_ready=false; bool start_calibration=false;

bool Fit_enabled_bool=false; bool DoubleFitEnabled_bool=false; bool fit_OK=false; bool cal_Ok=false; bool Doublefit_Ok=false; 
bool event_processed=false;

FILE *PlotTitle; FILE *XLabel; FILE *YLabel;   
TString htitle; TString xaxis; TString yaxis;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MainCommand=0; int CommandList=0; int GraphHistoSel=0;
int StartStopCmd=0;
Double_t GFitParam[8]; int iGFit=0;
/////////////////////////////////////////////////////////////////////// TextPave for fit ///////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TGString Lego0, Lego, Lego1, Lego2, Surf, Select;
 int SurfSel=0; int bkg_number=0; TGString SelectedSurf;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool DoDrawInProgress=false; float marker[3],marker_big[6]; int i_marker_ev=0, j_marker_ev=0; bool line1_on=false, line2_on=false, line3_on=false, line4_on=false, line5_on=false, line6_on=false;
float Fit_Xmin=0; float Fit_Xmax=0; float X_Peak_value=0; bool run_fit=false;
float Emin_cal=0, Emax_cal=0;
//////////////////////////////////////////////////////// MOUSE ON GPAD







//////////////////////////// FIT FUNCTIONS ///////////////////////////////////////////


/*/ Polinomial 2 background function          ////////////ELIMINATO bkg pol2
Double_t background_pol2(Double_t *x, Double_t *par) 
{
   return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}
*/
// Constant background function
Double_t background_const(Double_t *x, Double_t *par) 
{
   return par[0];
}
// Linear background function
Double_t background_line(Double_t *x, Double_t *par) 
{
   return par[0]+par[1]*x[0];
}


Double_t GaussPeak(Double_t *x, Double_t *par) {
  return (par[0]*TMath::Exp(-0.5*((x[0]-par[1])/par[2])*((x[0]-par[1])/par[2])));
}


/////funzioni somma//////////////////////////////////////

/*Double_t fitFunction_2(Double_t *x, Double_t *par) {   ////ELIMINATO bkg2
  
  return background_pol2(x,par) + GaussPeak(x,&par[3]);
}
*/
///////Singole gaussiane
Double_t fitFunction_1(Double_t *x, Double_t *par) {
  
  return background_line(x,par) + GaussPeak(x,&par[2]);
}
Double_t fitFunction_0(Double_t *x, Double_t *par) {
  
  return background_const(x,par) + GaussPeak(x,&par[1]);
}
///////////////////////////////////////////////////////////
//////Doppie gaussiane
Double_t fitFunction_d(Double_t *x, Double_t *par) {
  
  return GaussPeak(x,par) + GaussPeak(x,&par[3]);
}
Double_t fitFunction_1d(Double_t *x, Double_t *par) {
  
  return background_line(x,par) + GaussPeak(x,&par[2]) + GaussPeak(x,&par[5]);
}
Double_t fitFunction_0d(Double_t *x, Double_t *par) {
  
  return background_const(x,par) + GaussPeak(x,&par[1]) + GaussPeak(x,&par[4]);
}
/////////////////////////////////////////////////////////

  MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {  //Tutte le modifiche fatte qui dentro devono passare dal ROOTCINT




  // Create a main frame
  fMain = new TGMainFrame(p,w,h);

//  fMain->SetIconPixmap("Pictures/Infn2.gif");
  Lego0="Constant"; Lego="Linear";  Select="no BG Function"; //Lego1="Quadratic"; Lego2="Cubic";

// Create canvas widget
  fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,1200,460);
  fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX| kLHintsExpandY,
					      10,10,10,1)); //10,10,10,1





  fMain->Connect("CloseWindow()", "MyMainFrame", this, "CloseMainWindow()");

  timer1 = new TTimer();
  timer1->Start(250, kFALSE); //era500
  timer1->Connect("Timeout()", "MyMainFrame",this, "TimeOut()");

// Create a horizontal frame widget with buttons
  TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,1200,300);


 // TGCheckButton *EnTimeCal = new TGCheckButton(hframe,"&EnTimeCal");   ////eliminate le checkbox
 // EnTimeCal->Connect("Clicked()","MyMainFrame",this,"EnTimeCal()");
 // hframe->AddFrame(EnTimeCal, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

 // TGCheckButton *TimeLinear = new TGCheckButton(hframe,"&t^(-1/2)");
 // TimeLinear->Connect("Clicked()","MyMainFrame",this,"TimeSqrt()");
 // hframe->AddFrame(TimeLinear, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGCheckButton *LogScale = new TGCheckButton(hframe,"&LogScale");
  LogScale->Connect("Clicked()","MyMainFrame",this,"LogScale()");
  hframe->AddFrame(LogScale, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGCheckButton *Grid = new TGCheckButton(hframe,"&Grid");
  Grid->Connect("Clicked()","MyMainFrame",this,"Grid()");
  hframe->AddFrame(Grid, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGComboBox *H2Surf = new TGComboBox(hframe,"SelectBGFunction");
  H2Surf->AddEntry(Select,0);
  H2Surf->AddEntry(Lego0,1);
  H2Surf->AddEntry(Lego,2);
  H2Surf-> Resize(150,20);
  H2Surf->Connect("Selected(int)","MyMainFrame",this,"H2DSurf(int)");
  hframe->AddFrame(H2Surf, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

/*
  TGCheckButton *EnablingFit = new TGCheckButton(hframe,"&EnableFit");   ///eliminate le checkbox per snellire
  EnablingFit->Connect("Clicked()","MyMainFrame",this,"Fit_enabled()");
  hframe->AddFrame(EnablingFit, new TGLayoutHints(kLHintsCenterX,5,5,3,3));
*/
  DoFit= new TGTextButton(hframe,"&Fit");
  DoFit->Connect("Clicked()","MyMainFrame",this,"RunFit()"); 
  hframe->AddFrame(DoFit, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

/*
  TGCheckButton *EnablingDoubleFit = new TGCheckButton(hframe,"&EnableDoubleFit");
  EnablingDoubleFit->Connect("Clicked()","MyMainFrame",this,"DoubleFit_enabled()");
  hframe->AddFrame(EnablingDoubleFit, new TGLayoutHints(kLHintsCenterX,5,5,3,3));
*/
  DoDoubleFit = new TGTextButton(hframe,"&DoubleFit");
  DoDoubleFit->Connect("Clicked()","MyMainFrame",this,"RunDoubleFit()"); 
  hframe->AddFrame(DoDoubleFit, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

/*
  TGCheckButton *EnablingCal = new TGCheckButton(hframe,"&EnableCal");
  EnablingCal->Connect("Clicked()","MyMainFrame",this,"Calibration()");
  hframe->AddFrame(EnablingCal, new TGLayoutHints(kLHintsCenterX,5,5,3,3));
*/
  CalButton = new TGTextButton(hframe,"&Calibrate");
  CalButton->Connect("Clicked()","MyMainFrame",this,"CalculateCal()");
  hframe->AddFrame(CalButton, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGTextButton *draw = new TGTextButton(hframe,"&Draw/ReDraw");
  draw->Connect("Clicked()","MyMainFrame",this,"SingleDraw()");
  hframe->AddFrame(draw, new TGLayoutHints(kLHintsCenterX,5,5,3,3)); ////////////////

 // TGCheckButton *PreviewOn = new TGCheckButton(hframe,"&Preview on Export");
 // PreviewOn->Connect("Clicked()","MyMainFrame",this,"PreviewOn()");
 // hframe->AddFrame(PreviewOn, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGTextButton *exit = new TGTextButton(hframe,"&Exit");
  exit->Connect("Clicked()","MyMainFrame",this,"endRoot()");
  hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,3));



  //Int_t parts[] = {45, 15, 10, 30}; fStatusBar->SetParts(parts, 4) SetStatusText(text3,3); 
  Int_t parts[]={24,68,8};

  StatusBar = new TGStatusBar(fMain,5,5,kVerticalFrame);         
  fMain->AddFrame(StatusBar, new TGLayoutHints(kLHintsExpandX,10,10,2,2));
  //StatusBar->SetTextSize(14);
  StatusBar->SetParts(parts,3);
  StatusBar->SetText("XRF Fitter: RootGui StatusBar",0);

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


void MyMainFrame::Refresh() // timer event
{  //printf("Faccio refresh\n");
  if(!DoDrawInProgress)  
    { CmndList();  //printf("MCmnd %d\n", MainCommand); 
      if (MainCommand==1) {DoDraw();}
    }
}


void MyMainFrame::DoubleDoDraw()
{
    DoDrawInProgress=true;
      
    if(line1_on)
     {marker1 = new TLine(marker_big[0],0,marker_big[0],20000);marker1->SetLineColor(kRed);marker1->SetLineWidth(2);marker1->Draw("same");
      StatusBar->SetText("Five more points needed.",1);}
    if(line2_on)
     {marker2 = new TLine(marker_big[1],0,marker_big[1],20000);marker2->SetLineColor(kRed);marker2->SetLineWidth(2);marker2->Draw("same");
      StatusBar->SetText("Four more points needed.",1);}  
    if(line3_on)
     {marker3 = new TLine(marker_big[2],0,marker_big[2],20000);marker3->SetLineColor(kRed);marker3->SetLineWidth(2);marker3->Draw("same");
      StatusBar->SetText("Three more points needed.",1);} 
    if(line4_on)
     {marker4 = new TLine(marker_big[3],0,marker_big[3],20000);marker4->SetLineColor(kBlue);marker4->SetLineWidth(2);marker4->Draw("same");
      StatusBar->SetText("Two more points needed.",1);}    
    if(line5_on)
     {marker5 = new TLine(marker_big[4],0,marker_big[4],20000);marker5->SetLineColor(kBlue);marker5->SetLineWidth(2);marker5->Draw("same");
      StatusBar->SetText("One more point needed.",1);} 
    if(line6_on)
     {marker6 = new TLine(marker_big[5],0,marker_big[5],20000);marker6->SetLineColor(kBlue);marker6->SetLineWidth(2);marker6->Draw("same");
       if(start_calibration) {StatusBar->SetText("Starting calibration...",1); cal_Ok=true; CalculateCal();}
       if(DoubleFitEnabled_bool) {StatusBar->SetText("Starting double_fit...",1); Doublefit_Ok=true; RunDoubleFit();}
       line1_on=false; line2_on=false; line3_on=false;line4_on=false; line5_on=false; line6_on=false;
     }
      
      fCanvas->cd();
      fCanvas->Update(); 
     
  DoDrawInProgress=false;
  
}

void MyMainFrame:: CalculateCal()
{
  if(cal_Ok)
   { 
    Calibration_ready=false;
   float maxtmp1=marker_big[0];
   float mintmp1=marker_big[0];
   float peak_value1=marker_big[0];

    if (marker_big[1]>marker_big[0]) {maxtmp1=marker_big[1];} else {mintmp1=marker_big[1];}
    if (marker_big[2]>maxtmp1)    {peak_value1=maxtmp1; maxtmp1=marker_big[2];}
    if (marker_big[2]<mintmp1)    {peak_value1=mintmp1; mintmp1=marker_big[2];}

    Fit_Xmin=mintmp1; Fit_Xmax=maxtmp1; X_Peak_value=peak_value1;
    printf(" min1: %f-----peak val1 %f------max1 %f\n", mintmp1,peak_value1,maxtmp1); 

     TF1 *signalFcn1 = new TF1("signalFcn1","gaus",Fit_Xmin,Fit_Xmax); 
     f1->Fit(signalFcn1,"R");
     signalFcn1->Draw("+same");

     float mean1=signalFcn1->GetParameter(1);

   float maxtmp2=marker_big[3];
   float mintmp2=marker_big[3];
   float peak_value2=marker_big[3];

    if (marker_big[4]>marker_big[3]) {maxtmp2=marker_big[4];} else {mintmp2=marker_big[4];}
    if (marker_big[5]>maxtmp2)    {peak_value2=maxtmp2; maxtmp2=marker_big[5];}
    if (marker_big[5]<mintmp2)    {peak_value2=mintmp2; mintmp2=marker_big[5];}

    Fit_Xmin=mintmp2; Fit_Xmax=maxtmp2; X_Peak_value=peak_value2;
    printf(" min2: %f-----peak val2 %f------max2 %f\n", mintmp2,peak_value2,maxtmp2); 

     TF1 *signalFcn2 = new TF1("signalFcn2","gaus",Fit_Xmin,Fit_Xmax); 
     f1->Fit(signalFcn2,"R");
     signalFcn1->Draw("+same");

     float mean2=signalFcn2->GetParameter(1);

    if(mean1>mean2) {float temp=mean1; mean1=mean2; mean2=temp;} //così 1 è quello minore
    float energy1, energy2;

/////Input energie per fit///////
static char answer[10];
TGString mess1=Form("Enter energy value (keV) corresponding to ch %.2f: ", mean1);
new TGInputDialog(gClient->GetRoot(), fMain,mess1,"0.00", answer);
energy1=atof(answer);

TGString mess2=Form("Enter energy value (keV) corresponding to ch %.2f: ", mean2);
new TGInputDialog(gClient->GetRoot(), fMain,mess2,"0.00", answer);
energy2=atof(answer);

////versione prova con scanf da terminale/////////////////////
//    printf(" A quale energia (keV) corrisponde il canale %f?\n", mean1);
//    scanf("%f", &energy1);
//    printf(" A quale energia (keV) corrisponde il canale %f?\n", mean2);
//    scanf("%f", &energy2);


////////////calibrazione istogramma///////////////////////////
    float Cal=(energy2-energy1)/(mean2-mean1); //keV/ch
    float offset=(mean2*energy1-mean1*energy2)/(mean2-mean1); //keV
    int w=0;
   Emin_cal=offset;
   Emax_cal=(4092.*Cal)+offset;
   printf("costante di cal %.5f, offset %.5f\n", Cal,offset);
   printf("emin %f, emax %f\n", Emin_cal,Emax_cal);
   int nbins = (f1->GetXaxis()->GetNbins()); 

   TH1F *f1_copy= (TH1F*)f1->Clone();
   delete f1;
   f1 = new TH1F("f1","c",nbins,Emin_cal,Emax_cal); 
   for (int d=1;d<=nbins;d++) 
   {
      double z = f1_copy->GetBinContent(d); 
      double k = f1_copy->GetXaxis()->GetBinCenter(d);
      double xnew = Cal*k + offset;  //trasformazione sulle x
      for(w=1;w<=z;w++) f1->Fill(xnew); 
   }
   ///////////////////////////////////////////////////////////////////
    single_draw=true;
    DoDraw();

    cal_Ok=false;
    Calibration_ready=true;
    start_calibration=false;
    //CalButton-> SetEnabled(kTRUE);  //mai più abilitato dopo che è stato calibrato
    StatusBar->SetText("Calibration ready.",1);
   }//chiude if (cal_ok)
  else
   {
    if(histo_on) 
     {
     start_calibration=true;
     CalButton-> SetEnabled(kFALSE);
     StatusBar->SetText("Six points are needed to calibrate the spectrum. Click on the spectrum to fix the points.",1);
     }    

   }
  
}


void MyMainFrame::CmndList()
{
  MainCommand=*shared_memory;


   // get event information
   int event = gPad->GetEvent();

   //printf("event %d\n", event);
if (event==1 && Fit_enabled_bool && !event_processed) //left mouse button click event =1,release event =11 
                                                      //funziona  con il fit o la calibrazione abilitati
        {       
                int px    = gPad->GetEventX();
 	        int py    = gPad->GetEventY();

                 double xd = gPad->AbsPixeltoX(px);
                 double yd = gPad->AbsPixeltoY(py);
                 float x = gPad->PadtoX(xd);  
                 float y = gPad->PadtoY(yd); 
               printf("Ascissa x %f, Ordinata y %f\n", x,y);
               marker[i_marker_ev]= x;
               i_marker_ev++;
               if (i_marker_ev > 2) i_marker_ev=0;

               if(line2_on) line3_on=true;
               if(line1_on) line2_on=true;               
               line1_on=true;

               //printf("linea 1  %s ", line1_on? "true":"false");
               //printf("linea 2  %s ", line2_on? "true":"false");
               //printf("linea 3  %s\n", line3_on? "true":"false");
               DoDraw();

               event_processed=true;
          }

if (event==1 && (start_calibration || DoubleFitEnabled_bool) && !event_processed) //left mouse button click event =1,release event =11 
                                                                             //funziona  con il fit o la calibrazione abilitati
        {
                int px    = gPad->GetEventX();
 	        int py    = gPad->GetEventY();

                 double xd = gPad->AbsPixeltoX(px);
                 double yd = gPad->AbsPixeltoY(py);
                 float x = gPad->PadtoX(xd);  
                 float y = gPad->PadtoY(yd); 
               printf("Ascissa x %f, Ordinata y %f\n", x,y);
               marker_big[j_marker_ev]= x;
               j_marker_ev++;
               if (j_marker_ev > 5) j_marker_ev=0;

               if(line5_on) line6_on=true;
               if(line4_on) line5_on=true;
               if(line3_on) line4_on=true;
               if(line2_on) line3_on=true;
               if(line1_on) line2_on=true;               
               line1_on=true;

               //printf("linea 1  %s ", line1_on? "true":"false");
               //printf("linea 2  %s ", line2_on? "true":"false");
               //printf("linea 3  %s\n", line3_on? "true":"false");
               DoubleDoDraw();
               event_processed=true;
          
        }

if (event == 11) event_processed=false;
}

void MyMainFrame::SingleDraw() 
{ 
  if(!DoDrawInProgress)
    {
      //CmndList();  //per adesso non fa niente ma si può usare in seguito con la shm
      histo_on=false;
      single_draw=true; //non è ancora stato disegnato-> lo disegno
      DoDraw();
     }
}



void MyMainFrame::TimeOut()   
{Refresh();} //printf("timer\n");



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MyMainFrame::H2DSurf(int BG_Sel)
{
  bkg_number=BG_Sel;   //bkg_number rappresenta il tipo di bkg scelto
  if(BG_Sel==0) {SurfSel=0;}// printf("No bacgrnd\n");}
  else SurfSel=1;

}



void MyMainFrame::RunFit()
{
 if(fit_OK) //cioè siamo pronti per fare il fit ///line1_on && line2_on && line3_on
{
   //printf("Sono dentro Runfit!!!!!!!!\n");
   //for (int i = 0; i < 3; i++)
   //   printf("Value...%d, ", marker[i]);   

   float maxtmp=marker[0];
   float mintmp=marker[0];
   float peak_value=marker[0];

    if (marker[1]>marker[0]) {maxtmp=marker[1];} else {mintmp=marker[1];}
    if (marker[2]>maxtmp)    {peak_value=maxtmp; maxtmp=marker[2];}
    if (marker[2]<mintmp)    {peak_value=mintmp; mintmp=marker[2];}

Fit_Xmin=mintmp; Fit_Xmax=maxtmp; X_Peak_value=peak_value;
printf(" min: %f-----peak val %f------max %f\n", mintmp,peak_value,maxtmp); 
run_fit=true;
fit_OK=false;

Fit_enabled_bool=false;
DoDraw();
//    cout <<" min: "<< mintmp << " ---- peak val "    << peak_value << " ---- max " <<  maxtmp;   
}
else
{
DoFit->SetEnabled(kFALSE);
Fit_enabled_bool=true; printf("fit enabled\n");
StatusBar->SetText("Three points are needed to fit the data. Click on the spectrum to fix the points.",1);
}

}

void MyMainFrame::RunDoubleFit()
{
  if(Doublefit_Ok)
   { 
 
   float maxtmp1=marker_big[0];
   float mintmp1=marker_big[0];
   float peak_value1=marker_big[0];

    if (marker_big[1]>marker_big[0]) {maxtmp1=marker_big[1];} else {mintmp1=marker_big[1];}
    if (marker_big[2]>maxtmp1)    {peak_value1=maxtmp1; maxtmp1=marker_big[2];}
    if (marker_big[2]<mintmp1)    {peak_value1=mintmp1; mintmp1=marker_big[2];}

    Fit_Xmin=mintmp1; Fit_Xmax=maxtmp1; X_Peak_value=peak_value1;
    printf(" min1: %f-----peak val1 %f------max1 %f\n", mintmp1,peak_value1,maxtmp1); 

     TF1 *signalFcn1 = new TF1("signalFcn1","gaus",Fit_Xmin,Fit_Xmax); 
     f1->Fit(signalFcn1,"R0");
     signalFcn1->SetLineColor(kBlue);

    float min_temp=Fit_Xmin; //in memoria min e max
    float max_temp=Fit_Xmax;

   float maxtmp2=marker_big[3];
   float mintmp2=marker_big[3];
   float peak_value2=marker_big[3];

    if (marker_big[4]>marker_big[3]) {maxtmp2=marker_big[4];} else {mintmp2=marker_big[4];}
    if (marker_big[5]>maxtmp2)    {peak_value2=maxtmp2; maxtmp2=marker_big[5];}
    if (marker_big[5]<mintmp2)    {peak_value2=mintmp2; mintmp2=marker_big[5];}


    Fit_Xmin=mintmp2; Fit_Xmax=maxtmp2; X_Peak_value=peak_value2;
    printf(" min2: %f-----peak val2 %f------max2 %f\n", mintmp2,peak_value2,maxtmp2); 

     TF1 *signalFcn2 = new TF1("signalFcn2","gaus",Fit_Xmin,Fit_Xmax); 
     f1->Fit(signalFcn2,"R0");
     signalFcn2->SetLineColor(kCyan);

   if(max_temp>Fit_Xmax) Fit_Xmax=max_temp; //max e min per le funzioni somma totali
   if(min_temp<Fit_Xmin) Fit_Xmin=min_temp;
    
     TF1 *fitFcn;
     TF1 *backFcn;

     TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
     legend->SetTextFont(72);
     legend->SetTextSize(0.04);
     legend->AddEntry(f1,"Data","l");
     legend->AddEntry(signalFcn1,"Signal fit 1","l");
     legend->AddEntry(signalFcn2,"Signal fit 2","l");


    if(SurfSel==1)   ///cioè se si vuole anche il bkg
    { 
     switch(bkg_number)
	{ 
 
          case 1: {
                    fitFcn = new TF1("fitFcn",fitFunction_0d,Fit_Xmin,Fit_Xmax,7); 
                    backFcn = new TF1("backFcn",background_const,Fit_Xmin,Fit_Xmax,1); // printf("constant\n"); 
                    fitFcn->SetParameter(1,signalFcn1->GetParameter(0));
                    fitFcn->SetParameter(2,signalFcn1->GetParameter(1));
                    fitFcn->SetParameter(3,signalFcn1->GetParameter(2));
                    fitFcn->SetParameter(4,signalFcn2->GetParameter(0));
                    fitFcn->SetParameter(5,signalFcn2->GetParameter(1));
                    fitFcn->SetParameter(6,signalFcn2->GetParameter(2));
                    f1->Fit("fitFcn","R0");  //,"ep"

                    Double_t par[7];   // writes the fit results into the par array
                    fitFcn->GetParameters(par);
                    //printf("parametri: %f  %f  %f %f\n", par[0], par[1],par[2],par[3]);
                    backFcn->SetParameters(par);
                    signalFcn1->SetParameters(&par[1]);
                    signalFcn2->SetParameters(&par[4]);
                    mu1=signalFcn1->GetParameter(1);
                    sigma1=signalFcn1->GetParameter(2);
                    Integral1=signalFcn1->Integral(Fit_Xmin,Fit_Xmax)- backFcn->Integral(mu1-3*sigma1,mu1+3*sigma1);
                    mu2=signalFcn2->GetParameter(1);
                    sigma2=signalFcn2->GetParameter(2);
                    Integral2=signalFcn2->Integral(Fit_Xmin,Fit_Xmax)- backFcn->Integral(mu2-3*sigma2,mu2+3*sigma2);
                    chi2rid=fitFcn->GetChisquare()/fitFcn->GetNDF();
                    break;
                   } 
	  case 2: {
                    fitFcn = new TF1("fitFcn",fitFunction_1d,Fit_Xmin,Fit_Xmax,8); 
                    backFcn = new TF1("backFcn",background_line,Fit_Xmin,Fit_Xmax,2);  //printf("linear\n"); 
                    fitFcn->SetParameter(2,signalFcn1->GetParameter(0));
                    fitFcn->SetParameter(3,signalFcn1->GetParameter(1));
                    fitFcn->SetParameter(4,signalFcn1->GetParameter(2));
                    fitFcn->SetParameter(5,signalFcn2->GetParameter(0));
                    fitFcn->SetParameter(6,signalFcn2->GetParameter(1));
                    fitFcn->SetParameter(7,signalFcn2->GetParameter(2));
                    f1->Fit("fitFcn","R0");  

                    Double_t par[8];   // writes the fit results into the par array
                    fitFcn->GetParameters(par);
                    backFcn->SetParameters(par);
                    signalFcn1->SetParameters(&par[2]);
                    signalFcn2->SetParameters(&par[5]);
                    mu1=signalFcn1->GetParameter(1);
                    sigma1=signalFcn1->GetParameter(2);
                    Integral1=signalFcn1->Integral(Fit_Xmin,Fit_Xmax)- backFcn->Integral(mu1-3*sigma1,mu1+3*sigma1);
                    mu2=signalFcn2->GetParameter(1);
                    sigma2=signalFcn2->GetParameter(2);
                    Integral2=signalFcn2->Integral(Fit_Xmin,Fit_Xmax)- backFcn->Integral(mu2-3*sigma2,mu2+3*sigma2);
                    chi2rid=fitFcn->GetChisquare()/fitFcn->GetNDF();
                    break;
                   }
	 /* case 3: {    /////ELIMINATO bkg pol2  */

	 }  //chiuse switch

     fitFcn->SetNpx(500);
     fitFcn->SetLineWidth(2);
     fitFcn->SetLineColor(kMagenta);

     backFcn->SetLineColor(kRed);

     legend->AddEntry(backFcn,"Background fit","l");
     legend->AddEntry(fitFcn,"Global Fit","l");

      backFcn->Draw("+same");
      signalFcn1->Draw("+same");
      signalFcn2->Draw("+same");
      fitFcn->Draw("+same");
      legend->Draw("+same");
     } //chiude if surfsel==1

     else    //se non si vuole il bkg
     {
       fitFcn = new TF1("fitFcn",fitFunction_d,Fit_Xmin,Fit_Xmax,6);  
       fitFcn->SetParameter(0,signalFcn1->GetParameter(0));
       fitFcn->SetParameter(1,signalFcn1->GetParameter(1));
       fitFcn->SetParameter(2,signalFcn1->GetParameter(2));
       fitFcn->SetParameter(3,signalFcn2->GetParameter(0));
       fitFcn->SetParameter(4,signalFcn2->GetParameter(1));
       fitFcn->SetParameter(5,signalFcn2->GetParameter(2));
       f1->Fit("fitFcn","R0");  

       Double_t par[6];   // writes the fit results into the par array
       fitFcn->GetParameters(par);
       //printf("parametri: %f  %f  %f %f\n", par[0], par[1],par[2],par[3]);
       signalFcn1->SetParameters(par);
       signalFcn2->SetParameters(&par[3]);
       signalFcn1->Draw("+same");
       signalFcn2->Draw("+same");
       legend->Draw("+same");
       mu1=fitFcn->GetParameter(1);
       sigma1=fitFcn->GetParameter(2);
       Integral1=signalFcn1->Integral(Fit_Xmin,Fit_Xmax);
       mu2=fitFcn->GetParameter(4);
       sigma2=fitFcn->GetParameter(5);
       Integral2=signalFcn2->Integral(Fit_Xmin,Fit_Xmax);
       chi2rid=fitFcn->GetChisquare()/fitFcn->GetNDF();

       fitFcn->SetNpx(500);
       fitFcn->SetLineWidth(2);
       fitFcn->SetLineColor(kMagenta);

       legend->AddEntry(fitFcn,"Global Fit","l");
       signalFcn1->Draw("+same");
       signalFcn2->Draw("+same");
       fitFcn->Draw("+same");
       legend->Draw("+same");
     }

      fCanvas->cd();
      fCanvas->Update();
      StatusBar->SetText("Double-fit is ready.",1);
     /////stringa da scrivere nella messagebox/////     
     TString msgg=Form("mean1 = %.2f  sigma1 = %.2f Integral1 = %.2f \n mean2 = %.2f  sigma2 = %.2f Integral2 =%.2f\n\n Chi2/ndf = %f\n",mu1,sigma1,Integral1,mu2,sigma2,Integral2,chi2rid);
     //cout<<msgg<<'\n';
      new TGMsgBox(gClient->GetDefaultRoot(), fMain, "Fit Results", msgg, kMBIconAsterisk,kMBOk); 

      Doublefit_Ok=false;
      DoubleFitEnabled_bool=false;
      DoDoubleFit->SetEnabled(kTRUE);

      TH1F *f1_copy=(TH1F*)f1->Clone();
      f1->Reset();
      f1=(TH1F*)f1_copy->Clone();
      f1->Draw();
      StatusBar->SetText(" ",1);
    }//chiude if(Doublefit_Ok)
 else
 {
  DoubleFitEnabled_bool=true; printf("Double fit enabled\n");
  DoDoubleFit->SetEnabled(kFALSE);
  StatusBar->SetText("Six points are needed to double_fit the data. Click on the spectrum to fix the points.",1);
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

  if(single_draw)
  {
   if(cal_Ok || Calibration_ready)
    {
       f1->GetXaxis()->SetTitle("Energy (keV)");
       f1->SetStats(0);

       if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
       if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);} ////////////////////////// VirtualTPad

       fCanvas = fEcanvas->GetCanvas();
       fCanvas->cd();
       f1->Draw();  //"elp"
       printf("-------Disegno lo spettro f1_cal\n"); 
       fCanvas->Update();
       gPad->Update();
       histo_on=true;
       single_draw=false; 
     }
   else
    {
      int j; const int bin= 16384;
      Double_t dataSpectrum[bin];
      int ch =0;

   TGFileInfo file_info_;
   //const char *filetypes[] = {"txt files", "*.txt", 0, 0};  ///se si vuole accettare solo un certo tipo di file
   //file_info_.fFileTypes = filetypes;
   file_info_.fIniDir = StrDup(".");
   new TGFileDialog(gClient->GetDefaultRoot(), gClient->GetDefaultRoot(),kFDOpen, &file_info_);
   if( file_info_.fFilename )
   {
   //   cout << "'" << file_info_.fFilename << "' selected." << endl;

    char *filename=file_info_.fFilename;
      FILE *f0;
      //f0=fopen("data_prova","r");
      f0=fopen(filename,"r");
      printf("apro il file\n");
      while( ! feof(f0))
        {
         fscanf(f0,"%d\n",&j);
         dataSpectrum[ch]=j; ch++;
        }
       fclose(f0);
    }
       f1 = new TH1F("histo","Fit",4092,0,4092);
       f1->Reset();
       f1->SetMarkerStyle(21);
       f1->SetMarkerSize(0.8);
       f1->SetStats(0);
       f1->GetXaxis()->SetTitle("Channel");

       for(int i=0; i < bin;  i++) f1->SetBinContent(i+1,dataSpectrum[i]);
       if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
       if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);} ////////////////////////// VirtualTPad

       fCanvas = fEcanvas->GetCanvas();
       f1->Draw();  //"elp"
       printf("-------Disegno lo spettro f1\n"); 
       fCanvas->cd();
       fCanvas->Update();
       histo_on=true;
       single_draw=false; 
    } //chiude else
 }
   if (run_fit) //////////////////////////////////////////////////////////////////////////////////////////////////////////////
   {
    printf(" runfit\n");
     
//Picco gaussiano, usata anche per trovare valori iniziali dei parametri gaussiani
    

     TF1 *signalFcn = new TF1("signalFcn","gaus",Fit_Xmin,Fit_Xmax); 
     signalFcn->SetLineColor(kBlue);
     signalFcn->SetNpx(500);

     f1->Fit(signalFcn,"R0");

     TF1 *fitFcn;
     TF1 *backFcn;

     TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
     legend->SetTextFont(72);
     legend->SetTextSize(0.04);
     legend->AddEntry(f1,"Data","l");
     legend->AddEntry(signalFcn,"Signal fit","l");


    if(SurfSel==1)   ///cioè se si vuole anche il bkg
    { 
     switch(bkg_number)
	{ 
 
          case 1: {
                    fitFcn = new TF1("fitFcn",fitFunction_0,Fit_Xmin,Fit_Xmax,4); 
                    backFcn = new TF1("backFcn",background_const,Fit_Xmin,Fit_Xmax,1); // printf("constant\n"); 
                    fitFcn->SetParameter(1,signalFcn->GetParameter(0));
                    fitFcn->SetParameter(2,signalFcn->GetParameter(1));
                    fitFcn->SetParameter(3,signalFcn->GetParameter(2));
                    f1->Fit("fitFcn","R0");  //,"ep"

                    Double_t par[4];   // writes the fit results into the par array
                    fitFcn->GetParameters(par);                    
                    //printf("parametri: %f  %f  %f %f\n", par[0], par[1],par[2],par[3]);
                    backFcn->SetParameters(par);
                    signalFcn->SetParameters(&par[1]);
                    mu1=signalFcn->GetParameter(1);
                    sigma1=signalFcn->GetParameter(2);
                    Integral1=signalFcn->Integral(Fit_Xmin,Fit_Xmax) - backFcn->Integral(mu1-3*sigma1,mu1+3*sigma1);
                    chi2rid=fitFcn->GetChisquare()/fitFcn->GetNDF();
                    break;
                   } 
	  case 2: {
                    fitFcn = new TF1("fitFcn",fitFunction_1,Fit_Xmin,Fit_Xmax,5); 
                    backFcn = new TF1("backFcn",background_line,Fit_Xmin,Fit_Xmax,2);  //printf("linear\n"); 
                    fitFcn->SetParameter(2,signalFcn->GetParameter(0));
                    fitFcn->SetParameter(3,signalFcn->GetParameter(1));
                    fitFcn->SetParameter(4,signalFcn->GetParameter(2));
                    f1->Fit("fitFcn","R0");  //,"ep"

                    Double_t par[5];   // writes the fit results into the par array
                    fitFcn->GetParameters(par);
                    backFcn->SetParameters(par);
                    signalFcn->SetParameters(&par[2]);
                    mu1=signalFcn->GetParameter(1);
                    sigma1=signalFcn->GetParameter(2);
                    Integral1=signalFcn->Integral(Fit_Xmin,Fit_Xmax)- backFcn->Integral(mu1-3*sigma1,mu1+3*sigma1);
                    chi2rid=fitFcn->GetChisquare()/fitFcn->GetNDF();
                    break;
                   }
	 /* case 3: {                                    /////ELIMINATO bkg pol2
                    fitFcn = new TF1("fitFcn",fitFunction_2,Fit_Xmin,Fit_Xmax,6); //950,1150,6)
                    backFcn = new TF1("backFcn",background_line,Fit_Xmin,Fit_Xmax,3);  //printf("quadratic\n"); 
                    fitFcn->SetParameter(3,signalFcn->GetParameter(0));
                    fitFcn->SetParameter(4,signalFcn->GetParameter(1));
                    fitFcn->SetParameter(5,signalFcn->GetParameter(2));
                    f1->Fit("fitFcn","R0");  //,"ep"

                    Double_t par[5];   // writes the fit results into the par array
                    fitFcn->GetParameters(par);
                    backFcn->SetParameters(par);
                    signalFcn->SetParameters(&par[3]);
                    break;
                   } 
          */
	 }  //chiuse switch

     fitFcn->SetNpx(500);
     fitFcn->SetLineWidth(2);
     fitFcn->SetLineColor(kMagenta);

     backFcn->SetLineColor(kRed);

     legend->AddEntry(backFcn,"Background fit","l");
     legend->AddEntry(fitFcn,"Global Fit","l");

      backFcn->Draw("+same");
      signalFcn->Draw("+same");
      fitFcn->Draw("+same");
      legend->Draw("+same");

     } //chiude if surfsel==1
     else    //se non si vuole il bkg
     {
      signalFcn->Draw("+same");
      legend->Draw("+same");
      mu1=signalFcn->GetParameter(1);
      sigma1=signalFcn->GetParameter(2);
      Integral1=signalFcn->Integral(Fit_Xmin,Fit_Xmax);
      chi2rid=signalFcn->GetChisquare()/signalFcn->GetNDF();
     }
            
      fCanvas->cd();
      fCanvas->Update();
      StatusBar->SetText("Fit is ready.",1);
     /////stringa da scrivere nella messagebox/////     
     TString msgg=Form("mean = %.2f    sigma = %.2f  Integral = %.2f \n\n Chi2/ndf = %f\n",mu1,sigma1,Integral1,chi2rid);
     //cout<<msgg<<'\n';
      new TGMsgBox(gClient->GetDefaultRoot(), fMain, "Fit Results", msgg, kMBIconAsterisk,kMBOk); 
      
      fCanvas->cd();
      fCanvas->Update();
      run_fit=false; 
      DoFit->SetEnabled(kTRUE);

      TH1F *f1_copy=(TH1F*)f1->Clone();
      f1->Reset();
      f1=(TH1F*)f1_copy->Clone();
      f1->Draw();
      StatusBar->SetText(" ",1);
     //printf("finito runfit\n");
   }

 if (Fit_enabled_bool)
      {
       if(line1_on)
         {marker1 = new TLine(marker[0],0,marker[0],20000);marker1->SetLineColor(kRed);marker1->SetLineWidth(2);marker1->Draw("same");
          StatusBar->SetText("Two more points needed to fit the spectrum.",1);}
       if(line2_on)
         {marker2 = new TLine(marker[1],0,marker[1],20000);marker2->SetLineColor(kRed);marker2->SetLineWidth(2);marker2->Draw("same");
          StatusBar->SetText("One more point needed to fit the spectrum.",1);}       
       if(line3_on)
          {marker3 = new TLine(marker[2],0,marker[2],20000);marker3->SetLineColor(kRed);marker3->SetLineWidth(2);marker3->Draw("same");
           StatusBar->SetText("Starting fit...",1); fit_OK=true;line1_on=false; line2_on=false; line3_on=false; RunFit();}
      fCanvas->cd();
      fCanvas->Update(); 
       }

//      gStyle->SetOptStat("ne");
 
DoDrawInProgress=false;
}


//gEnv->SetValue("Gui.DefaultFont", "-*-helvetica-large-r-*-*-48-*-*-*-*-*-iso8859-1");
////////////////////////////////////////////////////////// MAIN ////////// MAIN ///////////////// MAIN ///////////////

  int main(int argc, char **argv) {
  TApplication theApp("App",&argc,argv);


  ////////// STARTING SHARED MEMORY MAPPING 


  key = 6400;
  shmid = shmget (6400, SHMSZ, IPC_CREAT | 0666);
  shared_memory =(int *) shmat(shmid, NULL, 0);
  cout << "Memory attached at :" << (int *)shared_memory<< endl << " with shmid: " << shmid << endl;

  *shared_memory=0;

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
















