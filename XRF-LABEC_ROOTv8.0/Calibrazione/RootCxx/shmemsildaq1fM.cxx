// Compilare come:

// rootcint -f ex2aDict.cxx -c example2a.h ex2aLinkDef.h
// g++ `root-config --cflags --glibs` -o mainframe example2a.cxx ex2aDict.cxx

/*****************************************************************************************
NEW VERSION BASED ON SHARED MEMORY
MEMORY DEFAULT SEGMENT KEY=6400 FOR NUMERIC DATA 0<jInt<7 lower and upper memory base address

 Address:     functions:               Variable:  
 jInt=0      ShmCommandWrite           MainCommand  |run   cmnd
             ShmCommandRead            MainCommand  |stop 
 jInt=1     ShmRootStatusWrite         RootStatus
            ShmRootStatusRead          RootStatus 
 jInt=2   ShmRootRecordedEventsRead   RecordedEvents
 jInt=3       ShmZoomWrite              chlow
 jInt=4       ShmZoomWrite              chhigh
 jInt=5       ShmZoomWrite              champ
 jInt=6     ShmPrintStatusWrite       PrintStatus
            ShmPrintStatusRead        PrintStatus
 jInt=7    ShmGraphHistoSelWrite     GraphHistoSel
 jInt=8    ShmFitTypeSelWrite         FitTypeChoice
 jInt=9   ShmDigitTBaseOnWrite         DigitTBon
 jInt=10    ShmNewsDaqFile            NewsDaqFile
 jInt=11    ShmExportPng            
 jInt=12    ShmExportJpg         
 jInt=13    ShmExportPs           
 jInt=14    ShmExportEps         
 jInt=15    ---------------
 jInt=16    ShmZoomChange
 jInt=17    ShmFitOnGauss Gauss=1 ;;  Gauss^2=2 ;; Gauss^3=3 ;; Linear=4           
 jInt=18    ShmExtendedFitArea1           
 jInt=19    ShmExtendedFitArea2        
 jInt=20    ShmFitLower1 
 jInt=21    ShmFitUpper1           
 jInt=22    ShmFitLower2           
 jInt=23    ShmFitUpper2         
 jInt=24    ShmFitLower3
 jInt=25    ShmFitUpper3           
 jInt=26    ShmAreaFitLower4           
 jInt=27    ShmAreaFitUpper4  
 jInt=28    ShmAreaFitLower5           
 jInt=29    ShmAreaFitUpper5         
 
// OTHER CHANNELS IN COMMAND SECTION...........!!!!

MEMORY DEFAULT SEGMENT KEY=6500 FOR STRING DATA 0<jInt<10 lower and upper memory base address
jInt=0->command ;; jInt=1->RootStatus ;;  
Associated Variable (int *shared_memory); int shmid;
Associated functions: ShmCommandWrite(); ShmCommandRead();
*****************************************************************************************/

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
//#include <TGRadioButton.h>
#include <TGButtonGroup.h>



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
int shmid; int shmidFloat;
key_t key; key_t keyString;

float *shared_memFloat;
int *shared_memory; 

bool initCanvas=true;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// START STANDARD GUI ///////////////////////////////////////
char StatusBarText1[80];

char defDaq[200];
char DefDirV;

int recorded_iev; int i;
FILE *fpx, *fpy, *fpxStart, *frecord, *fexport;
char ch;
float y;
int x; int z; int xdet1,xdet2,xdet3,xdet,xdet5,xdet6,xdet7,xdet8; // multidetector
int JArea1,A1min,A1max;int JArea2, A2min, A2max; int JArea3, A3min, A3max; int JArea4, A4min, A4max; int JArea5, A5min, A5max; 
Double_t G1Area, G2Area,G3Area,G4Area,G5Area;
int cmnd=0;
TH1F *h1x=NULL;
//TGRadioButton *fRB=NULL;

int xx1,xx2,xx3;
FILE *fzoom, *fpid1, *froot, *fileType, *fprt;

int fitStatus=0;
char ExportValue[200];
int min, max, amp; 
int XampH2,XminH2,XmaxH2,YampH2,YminH2,YmaxH2; int H2DZUP=0;

int PrType=0; int ManualOn=0; int FisrtManualOn=0; int cmndStore=0;
float channel[65536]; float Fx[65536]; float FdatoX; int iGraph; int nGraph;
int PrevOn=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////// BOOLS FOR SELECTIONS ////////////////////////////////////////// BOOLS FOR SELECTIONS  /////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool LogOn=false; bool GridOn=false; //bool H2DSURF=false;
bool PviewOn=false;  bool Histo=false; bool Graph=false; bool PrintOn=false; bool ExportPng=false; bool ExportJpg=false; 
bool ExportPs=false; bool ExportEps=false; bool ExtendedFitArea1=false; bool ExtendedFitArea2=false; bool MultiAdc=false;
bool SimpleGauss=false; bool Linear=false;
int FitOnGauss=0;
bool NewDaqFile=false; bool GFit=false; bool Gauss1=false; bool Gauss2=false; bool Gauss3=false; bool H2D=false;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////// INT FOR FIT RANGE ///////////////////////////////////////////////  INT FOR FIT RANGE  /////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int FitLower1; int FitUpper1; int FitLower2; int FitUpper2; int FitLower3; int FitUpper3;          
int AreaFitLower4; int AreaFitUpper4; int AreaFitLower5; int AreaFitUpper5;  
char TxtL1[50]; char TxtL2[50]; char TxtL3[50];char TxtL4[50]; char TxtL5[50]; char TxtL6[50];
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////// INT & BOOL FOR GRAPH ZOOM  ////////////////////////////// INT & BOOL FOR GRAPH ZOOM  ///////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GZUP=0; int GraphZoomMin=0; int GraphZoomMax=1000; bool GraphZoomUpdate=false; int GZlow=0; int GZHigh=20000;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FILE *PlotTitle; FILE *XLabel; FILE *YLabel;   
TString htitle; TString xaxis; TString yaxis;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MainCommand=0; int CommandList=0; int ZoomMin, ZoomMax, ZoomAmp; int GraphHistoSel=0;
int StartStopCmd=0;
Double_t GFitParam[8]; int iGFit=0;
/////////////////////////////////////////////////////////////////////// TextPave for fit ///////////////////////////////////////////

TPaveText *pt1 = new TPaveText(.78,.67,0.98,.82,"brNDC");
TPaveText *pt2 = new TPaveText(.78,.51,0.98,.82,"brNDC");
TPaveText *pt3 = new TPaveText(.78,.35,0.98,.82,"brNDC");
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TGString Lego, Lego1, Lego2, Surf, Surf1, Surf2, Surf3, Surf4, Surf5, Select;
 int SurfSel=0; TGString SelectedSurf;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TGString Ch0,Ch1,Ch2,Ch3,Ch4,Ch5,Ch6,Ch7,SelectCh; int PadCloneSelection=0; int SelectedPad=0; bool CanvasClone=false;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TGString AmsSel,Carbon,Iodine;



bool DoDrawInProgress=false;

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {  //Tutte le modifiche fatte qui dentro devono passare dal ROOTCINT
  // Create a main frame
  fMain = new TGMainFrame(p,w,h);

  fMain->SetIconPixmap("Pictures/Infn2.gif");
  Lego="LEGO"; Lego1="LEGO1"; Lego2="LEGO2"; Select="Select Surf"; Surf="SURF";  
  Surf1="SURF1"; Surf2="SURF2"; Surf3="SURF3"; Surf4="SURF4"; Surf5="SURF5";
  AmsSel="AmsSel";Carbon="Carbon";Iodine="Iodine";

  Ch0="Ch0";Ch1="Ch1";Ch2="Ch2";Ch3="Ch3";Ch4="Ch4";Ch5="Ch5";Ch6="Ch6";Ch7="Ch7";SelectCh="ChSel";

  // Create canvas widget
  fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,850,460);
  fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX| kLHintsExpandY,
					      10,10,10,1)); //10,10,10,1

  fMain->Connect("CloseWindow()", "MyMainFrame", this, "CloseMainWindow()");

  TTimer *timer1 = new TTimer();
  timer1->Start(250, kFALSE); //era500
  timer1->Connect("Timeout()", "MyMainFrame",this, "TimeOut()");

  
  
  // Create a horizontal frame widget with buttons
  TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,850,30);
  //  TGHorizontalFrame *hframe1 = new TGHorizontalFrame(fMain,680,430);

  TGComboBox *H2Surf = new TGComboBox(hframe,"SelectSurf");
  H2Surf->AddEntry(Select,0);
  H2Surf->AddEntry(Lego,1);
  H2Surf->AddEntry(Lego1,2); //  
  H2Surf->AddEntry(Lego2,3); //  
  H2Surf->AddEntry(Surf,4); 
  H2Surf->AddEntry(Surf1,5); // 
  H2Surf->AddEntry(Surf2,6); // 
  H2Surf->AddEntry(Surf3,7); 
  H2Surf->AddEntry(Surf4,8); 
  H2Surf->AddEntry(Surf5,9); // 
  H2Surf-> Resize(80,20);
  H2Surf->Connect("Selected(int)","MyMainFrame",this,"H2DSurf(int)");
  hframe->AddFrame(H2Surf, new TGLayoutHints(kLHintsCenterX,5,5,3,3));
  ///////////////////////////////////////////////////////////////////////////

  TGComboBox *AMS = new TGComboBox(hframe,"AmsSel");
  AMS->AddEntry(AmsSel,0);
  AMS->AddEntry(Carbon,1);
  AMS->AddEntry(Iodine,2); 
  AMS-> Resize(70,20);
  AMS->Connect("Selected(int)","MyMainFrame",this,"AMS(int)");
//  hframe->AddFrame(AMS, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  
//////////// Clone Draw Selection ////////////////
  TGComboBox *CDraw = new TGComboBox(hframe,"ChSel");
  CDraw->AddEntry(SelectCh,0);
  CDraw->AddEntry(Ch0,1);
  CDraw->AddEntry(Ch1,2); 
  CDraw->AddEntry(Ch2,3); 
  CDraw->AddEntry(Ch3,4); 
  CDraw->AddEntry(Ch4,5);
  CDraw->AddEntry(Ch5,6); 
  CDraw->AddEntry(Ch6,7); 
  CDraw->AddEntry(Ch7,8); 
  // H2Surf->AddEntry(Surf5,9);
  CDraw-> Resize(60,20);
  CDraw->Connect("Selected(int)","MyMainFrame",this,"CloneDrawSel(int)");
//  hframe->AddFrame(CDraw, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

//////////////////////////////////////////////////

  TGCheckButton *LogScale = new TGCheckButton(hframe,"&LogScale");
  LogScale->Connect("Clicked()","MyMainFrame",this,"LogScale()");
  hframe->AddFrame(LogScale, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGCheckButton *Grid = new TGCheckButton(hframe,"&Grid");
  Grid->Connect("Clicked()","MyMainFrame",this,"Grid()");
  hframe->AddFrame(Grid, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGCheckButton *ManMode = new TGCheckButton(hframe,"&ManMode");
  ManMode->Connect("Clicked()","MyMainFrame",this,"ManualDraw()");
  hframe->AddFrame(ManMode, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGTextButton *draw = new TGTextButton(hframe,"&Draw/ReDraw");
  draw->Connect("Clicked()","MyMainFrame",this,"SingleDraw()");
  hframe->AddFrame(draw, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGCheckButton *PreviewOn = new TGCheckButton(hframe,"&PrintPview");
  PreviewOn->Connect("Clicked()","MyMainFrame",this,"PreviewOn()");
  hframe->AddFrame(PreviewOn, new TGLayoutHints(kLHintsCenterX,5,5,3,3));



  //  TGTextButton *exit = new TGTextButton(hframe,"&Exit",
  //					"gApplication->Terminate(0)");

  hframe->AddFrame(CDraw, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGTextButton *ClonePad = new TGTextButton(hframe,"&ClonePad");
  ClonePad->Connect("Clicked()","MyMainFrame",this,"CloneGPad()");
  hframe->AddFrame(ClonePad, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  hframe->AddFrame(AMS, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGTextButton *exit = new TGTextButton(hframe,"&Exit");
  exit->Connect("Clicked()","MyMainFrame",this,"endRoot()");
  hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,3));



  //Int_t parts[] = {45, 15, 10, 30}; fStatusBar->SetParts(parts, 4) SetStatusText(text3,3); 
  Int_t parts[]={24,68,8};

  /*  TGStatusBar * */StatusBar = new TGStatusBar(fMain,5,5,kVerticalFrame);         
  fMain->AddFrame(StatusBar, new TGLayoutHints(kLHintsExpandX,10,10,2,2));
  // StatusBar->SetTextSize(10);
  

  StatusBar->SetParts(parts,3);
  StatusBar->SetText("AmsDefel RootGui StatusBar",0);
  //  StatusBar->Connect("MyMainFrame",this,"BarStatus()");


///////////////////////  RADIO BUTTON EXAMPLE //////////////////////
/*
   TGHorizontalFrame *hframe2 = new TGHorizontalFrame(fMain,720,30); //new
   TGButtonGroup *horizontal = new TGButtonGroup(hframe2, "Channel Expansion Selection",kHorizontalFrame);
   horizontal->SetTitlePos(TGGroupFrame::kCenter);
   new TGRadioButton(horizontal, "Ch1  ", kTextCenterX);
   new TGRadioButton(horizontal, "Ch2  ", kTextCenterX);
   new TGRadioButton(horizontal, "Ch3  ", kTextCenterX);
   new TGRadioButton(horizontal, "Ch4  ", kTextCenterX);
   new TGRadioButton(horizontal, "Ch5  ", kTextCenterX);
   new TGRadioButton(horizontal, "Ch6  ", kTextCenterX);
   new TGRadioButton(horizontal, "Ch7  ", kTextCenterX);
   new TGRadioButton(horizontal, "Ch8  ", kTextCenterX);

   horizontal->SetButton(kTextCenterX);
   horizontal->Connect("Pressed(Int_t)", "ButtonWindow", this, "DoHPosition(Int_t)");
   hframe2->AddFrame(horizontal, new TGLayoutHints(kLHintsExpandX)); // end new
   fMain->AddFrame(hframe2, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
*/
////////////////////////////////////////////////////////////////////////////////////////////

  // add hframe to main
  fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));


  // Set a name to the main frame
  fMain->SetWindowName("AmsDefel: RootGui (alpha)");
  // Map all subwindows of main frame
  fMain->MapSubwindows();
  // Initialize the layout algorithm
  fMain->Resize(fMain->GetDefaultSize());

  // Map main frame
  fMain->MapWindow();

 
  //SettingUp standard labels 
  std::string line1; ifstream myfile1 ("PlotTitle");
  getline(myfile1,line1); htitle=line1; myfile1.close();
  std::string line2; ifstream myfile2 ("XLabel");
  getline(myfile2,line2); xaxis=line2; myfile2.close();
  std::string line3; ifstream myfile3 ("YLabel");
  getline(myfile3,line3); yaxis=line3; myfile3.close();

  //Aquiring starting zoom 
  ZoomMin=*(shared_memory+3);
  ZoomMax=*(shared_memory+4);
  ZoomAmp=*(shared_memory+5);

  fpid1=fopen("RootGuiPid","w+");
  fprintf(fpid1,"%d\n",getpid());
  fclose(fpid1);
}

/////////// END INITIALIZATION ////////////////////////////////////////////////////////////////////


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
{if(!LogOn){LogOn=true;} else{LogOn=false;}}
void MyMainFrame::Grid()
{if(!GridOn){GridOn=true;} else{GridOn=false;}}

 
void MyMainFrame::Refresh()
{  if(!DoDrawInProgress)  //if(!PrintOn)  // To let the program process printing data....// meglio mettere un if sul DODRAW
    { CmndList();  //printf("MCmnd %d\n", MainCommand); 
      if (MainCommand==1) {GFit=false; DoDraw();}}
}

void MyMainFrame::CmndList()
{
/* jInt=0    ShmCommandWrite           MainCommand  
             ShmCommandRead            MainCommand  |stop 
 jInt=1     ShmRootStatusWrite         RootStatus
            ShmRootStatusRead          RootStatus 
 jInt=2   ShmRootRecordedEventsRead   RecordedEvents
 jInt=3       ShmZoomWrite              chlow
 jInt=4       ShmZoomWrite              chhigh
 jInt=5       ShmZoomWrite              champ
 jInt=6     ShmPrintStatusWrite       PrintStatus
            ShmPrintStatusRead        PrintStatus
 jInt=7    ShmGraphHistoSelWrite     GraphHistoSel
 jInt=8      -------------
 jInt=9   ShmDigitTBaseOnWrite         DigitTBon
 jInt=10    ShmNewsDaqFile            NewsDaqFile
 jInt=11    ShmExportPng             
 jInt=12    ShmExportJpg            
 jInt=13    ShmExportPs           
 jInt=14    ShmExportEps         
 jInt=15    ---------------
 jInt=16    ShmZoomChange
 jInt=17    ShmFitOnGauss Gauss=1 ;;  Gauss^2=2 ;; Gauss^3=3 ;; Linear=4            
 jInt=18     ----------         
 jInt=19    ---------        
 jInt=20    ShmFitLower1 
 jInt=21    ShmFitUpper1           
 jInt=22    ShmFitLower2           
 jInt=23    ShmFitUpper2         
 jInt=24    ShmFitLower3
 jInt=25    ShmFitUpper3           
 jInt=26    ShmAreaFitLower4           
 jInt=27    ShmAreaFitUpper4  
 jInt=28    ShmAreaFitLower5           
 jInt=29    ShmAreaFitUpper5  
 jInt=30    ShmGraphZoomMin  
 jInt=31    ShmGraphZoomMax           
 jInt=32    ShmGraphZoomUpdate  
 jInt=33    ShmUpdateTitle
 jInt=34    ShmUpdateXLabel
 jInt=35    ShmUpdateYLabel 

 ///// H2D SECTION /////////////////////////

 jInt=36    ShmXminH2           
 jInt=37    ShmXmaxH2  
 jInt=38    ShmYminH2
 jInt=39    ShmYmaxH2
 jInt=40    ShmH2DZUP 		H2D on 

 ////// MULTIGRAPH Start From 50 ////////////////////
 jInt=50    ShmMultiOn

  */

  MainCommand=*shared_memory;
  //  if (MainCommand==1) {GFit=false;}/////////////////////////////////////////////////////////////////////////////////////////

  CommandList=*(shared_memory+16); //Zoom change
  if(CommandList==1)
    { //printf("Run zoom\t cmnd%d\n",CommandList);
      ZoomMin=*(shared_memory+3);
      ZoomMax=*(shared_memory+4);
      ZoomAmp=*(shared_memory+5);
    }
  GraphHistoSel=*(shared_memory+7); // GraphHisto  printf("cmnd %d\t GraphHistoSel %d\n", cmnd, GraphHistoSel);
  if (GraphHistoSel==0)
    {Histo=true; Graph=false; GFit=false; H2D=false; MultiAdc=false; initCanvas=true;}
  if (GraphHistoSel==1) 
    {Histo=false; Graph=true; GFit=false; H2D=false; MultiAdc=false; initCanvas=true;}
  if (GraphHistoSel==2) 
    {Histo=false; Graph=false; GFit=true; H2D=false; MultiAdc=false; initCanvas=true;}
  if (GraphHistoSel==3) 
    {Histo=false; Graph=false; GFit=false; H2D=true; MultiAdc=false; initCanvas=true;}
  if (GraphHistoSel==4) 
    {Histo=false; Graph=false; GFit=false; H2D=false; MultiAdc=true;}
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////// ATTENZIONE GraphHistoSel==2 per FIT

  if ((CommandList=*(shared_memory+10))==1) NewDaqFile=true; // DefVmeDaqDir Changed
  if ((CommandList=*(shared_memory+11))==1) {ExportPng=true; PrintOn=true;} // Export png
  if ((CommandList=*(shared_memory+12))==1) {ExportJpg=true; PrintOn=true;}  // Export jpg
  if ((CommandList=*(shared_memory+13))==1) {ExportPs=true; PrintOn=true;}  // Export ps
  if ((CommandList=*(shared_memory+14))==1) {ExportEps=true; PrintOn=true;}  // Export eps

  FitOnGauss=*(shared_memory+17); 
  if(FitOnGauss==0)
    {SimpleGauss=false; Gauss1=false; Gauss2=false;Gauss3=false;ExtendedFitArea1=false; ExtendedFitArea2=false; Linear=false;}
  if(FitOnGauss==1)
    {SimpleGauss=true; Gauss1=false; Gauss2=false;Gauss3=false;ExtendedFitArea1=false; ExtendedFitArea2=false; Linear=false;}
  if(FitOnGauss==2)
    { FitLower1=*(shared_memory+20); FitUpper1=*(shared_memory+21);
      Gauss1=true; Gauss2=false;Gauss3=false;ExtendedFitArea1=false; ExtendedFitArea2=false; SimpleGauss=false; Linear=false;}
  if(FitOnGauss==3)
    { FitLower1=*(shared_memory+20); FitUpper1=*(shared_memory+21);
      FitLower2=*(shared_memory+22); FitUpper2=*(shared_memory+23);
      Gauss2=true; Gauss1=false;Gauss3=false;ExtendedFitArea1=false; ExtendedFitArea2=false;SimpleGauss=false; Linear=false;}
  if(FitOnGauss==4)
    { FitLower1=*(shared_memory+20); FitUpper1=*(shared_memory+21);
      FitLower2=*(shared_memory+22); FitUpper2=*(shared_memory+23);
      FitLower3=*(shared_memory+24); FitUpper3=*(shared_memory+25);
      Gauss3=true;Gauss1=false;Gauss2=false;ExtendedFitArea1=false; ExtendedFitArea2=false;SimpleGauss=false; Linear=false;}
  if (FitOnGauss==5) 
    { FitLower1=*(shared_memory+20); FitUpper1=*(shared_memory+21);
      FitLower2=*(shared_memory+22); FitUpper2=*(shared_memory+23);
      FitLower3=*(shared_memory+24); FitUpper3=*(shared_memory+25);
      AreaFitLower4=*(shared_memory+26); AreaFitUpper4=*(shared_memory+27);
      ExtendedFitArea1=true;Gauss1=false; Gauss2=false;Gauss3=false;ExtendedFitArea2=false;SimpleGauss=false; Linear=false;}    
  if (FitOnGauss==6) 
    { FitLower1=*(shared_memory+20); FitUpper1=*(shared_memory+21);
      FitLower2=*(shared_memory+22); FitUpper2=*(shared_memory+23);
      FitLower3=*(shared_memory+24); FitUpper3=*(shared_memory+25);
      AreaFitLower4=*(shared_memory+26); AreaFitUpper4=*(shared_memory+27);
      AreaFitLower5=*(shared_memory+28); AreaFitUpper5=*(shared_memory+29);
      ExtendedFitArea2=true;Gauss1=false; Gauss2=false;Gauss3=false;ExtendedFitArea1=false;SimpleGauss=false; Linear=false;}
  if(FitOnGauss==7)
    {Linear=true; Gauss1=false; Gauss2=false;Gauss3=false;ExtendedFitArea1=false; ExtendedFitArea2=false; SimpleGauss=false;}

  GZUP=*(shared_memory+32);
  if(GZUP==1)
    { GraphZoomMin=*(shared_memory+30);  
      GraphZoomMax=*(shared_memory+31);           
      GraphZoomUpdate=true; 
    }
  H2DZUP=*(shared_memory+40);
  if(H2DZUP==1)
    { XminH2=*(shared_memory+36);  
      XmaxH2=*(shared_memory+37); 
      YminH2=*(shared_memory+38); 
      YmaxH2=*(shared_memory+39); 
      XampH2=abs(XmaxH2-XminH2); YampH2=abs(YmaxH2-YminH2);
      *(shared_memory+40)=0; printf("Xmin %i\tXmax %i\tYmin %i\t Ymax %i\n",XminH2,XmaxH2,YminH2,YmaxH2);
    }

  if(*(shared_memory+33)==1)
    { std::string line1; ifstream myfile1 ("PlotTitle");
      getline(myfile1,line1); htitle=line1; myfile1.close(); *(shared_memory+33)=0;}
  if(*(shared_memory+34)==1)
    { std::string line2; ifstream myfile2 ("XLabel");
      getline(myfile2,line2); xaxis=line2; myfile2.close(); *(shared_memory+34)=0;}
  if(*(shared_memory+35)==1)
    { std::string line3; ifstream myfile3 ("YLabel");
      getline(myfile3,line3); yaxis=line3; myfile3.close(); *(shared_memory+35)=0;}
}

void MyMainFrame::SingleDraw() ////////////// QUi ///////////////////////////////  e gira
{ if(!DoDrawInProgress){CmndList(); DoDraw();}}
//{ if(!PrintOn){CmndList(); DoDraw();}} // meglio mettere un if sul DODRAW

void MyMainFrame::ManualDraw()
{
  switch(ManualOn)
    {
    case 0: {ManualOn=1; break;}
    case 1: {ManualOn=0; break;}
    }
}

void MyMainFrame::TimeOut()   // Refresh Data Input File
{ if(ManualOn==0)  Refresh();}

void MyMainFrame::PreviewOn()
{
  if(PviewOn)
    {PviewOn=false; PrevOn=0; /*printf("%i\n",PrevOn); */}
  else
    {PviewOn=true; PrevOn=1; /*printf("%i\n",PrevOn);*/}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MyMainFrame::PrintGui()
{
  fexport = fopen("GraphExport", "r");
  fscanf(fexport,"%s\n",ExportValue);
  fclose(fexport);
  if(ExportPng)
    {
      *(shared_memory+6)=1; // writing into memory OpDone 
      TCanvas *fCanvas = fEcanvas->GetCanvas();    
      fCanvas->Print("pippo.png"); 
      fCanvas->cd();
      fCanvas->Update();
      TString MvPng= ("cp pippo.png ");
      MvPng.Append(ExportValue);  gSystem->Exec(MvPng);
      if(PviewOn) {gSystem->Exec("eog pippo.png &");}  // if preview ON
      *(shared_memory+11)=0; PrintOn=false; ExportPng=false; 
    }
  if(ExportJpg) 
    {
      *(shared_memory+6)=1; //ShmMem+6->PrintStatus 
      TCanvas *fCanvas = fEcanvas->GetCanvas();
      fCanvas->Print("pippo.tiff"); 
      fCanvas->Update();
      TString MvPng= ("cp pippo.tiff ");
      MvPng.Append(ExportValue); gSystem->Exec(MvPng);
      if(PviewOn) {gSystem->Exec("eog pippo.tiff &");}  // if preview ON
      *(shared_memory+12)=0; PrintOn=false; ExportJpg=false; 
    }
  if(ExportPs) 
    {	
      *(shared_memory+6)=1;
      TCanvas *fCanvas = fEcanvas->GetCanvas();
      fCanvas->Print("pippo.ps");
      TString MvPng= ("cp pippo.ps ");
      MvPng.Append(ExportValue); gSystem->Exec(MvPng);
      if(PviewOn) {gSystem->Exec("evince pippo.ps &");}  // if preview ON
      *(shared_memory+13)=0; PrintOn=false; ExportPs=false; 
    }
  if(ExportEps) 
     {	
      *(shared_memory+6)=1;
      TCanvas *fCanvas = fEcanvas->GetCanvas();
      fCanvas->Print("pippo.eps");
      TString MvPng= ("cp pippo.eps ");
      MvPng.Append(ExportValue); gSystem->Exec(MvPng);
      if(PviewOn) {gSystem->Exec("gv pippo.eps &");}  //  if preview ON
      *(shared_memory+14)=0; PrintOn=false; ExportEps=false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MyMainFrame::FitLabel()
{
  if(Gauss1)
    {
      sprintf(TxtL1,"Mean:(%4.3e)",GFitParam[0]);
      sprintf(TxtL2,"Sigma: (%2.1e)",GFitParam[1]);
      pt1->Clear();
      pt1->SetTextSize(0.035);
      pt1->AddText("PeakFit1:");  pt1->AddText(TxtL1); pt1->AddText(TxtL2);
      //pt1->Draw();
    }
  if(Gauss2)
    {
      sprintf(TxtL1,"Mean:(%4.3e)",GFitParam[0]);
      sprintf(TxtL2,"Sigma: (%2.1e)",GFitParam[1]);
      sprintf(TxtL3,"Mean:(%4.3e)",GFitParam[2]);
      sprintf(TxtL4,"Sigma: (%2.1e)",GFitParam[3]);
      pt2->Clear();
      pt2->SetTextSize(0.035);
      pt2->AddText("PeakFit1:");  pt2->AddText(TxtL1); pt2->AddText(TxtL2);
      pt2->AddText("PeakFit2:");  pt2->AddText(TxtL3); pt2->AddText(TxtL4);
    }
  if((Gauss3)||(ExtendedFitArea1)||(ExtendedFitArea2))
    {
      sprintf(TxtL1,"Mean:(%4.3e)",GFitParam[0]);
      sprintf(TxtL2,"Sigma: (%2.1e)",GFitParam[1]);
      sprintf(TxtL3,"Mean:(%4.3e)",GFitParam[2]);
      sprintf(TxtL4,"Sigma: (%2.1e)",GFitParam[3]);
      sprintf(TxtL5,"Mean:(%4.3e)",GFitParam[4]);
      sprintf(TxtL6,"Sigma: (%2.1e)",GFitParam[5]);
      pt3->Clear();
      pt3->SetTextSize(0.035);
      pt3->AddText("PeakFit1:");  pt3->AddText(TxtL1); pt3->AddText(TxtL2);
      pt3->AddText("PeakFit2:");  pt3->AddText(TxtL3); pt3->AddText(TxtL4);
      pt3->AddText("PeakFit3:");  pt3->AddText(TxtL5); pt3->AddText(TxtL6);
    }
  }

void MyMainFrame::H2DSurf(int Pippo)
{
  SurfSel=Pippo;
  if(Pippo==0) {SurfSel=0;}
  else
    { SurfSel=1;
      switch(Pippo)
	{ case 1: {SelectedSurf="LEGO";break;}
	  case 2: {SelectedSurf="LEGO1";break;}
	  case 3: {SelectedSurf="LEGO2";break;}
	  case 4: {SelectedSurf="SURF";break;}
	  case 5: {SelectedSurf="SURF1";break;}
	  case 6: {SelectedSurf="SURF2";break;}
	  case 7: {SelectedSurf="SURF3";break;}
	  case 8: {SelectedSurf="SURF4";break;}
	  case 9: {SelectedSurf="SURF5";break;}
	}
    }
  printf("Pippo %i\tComboSel %i\n",Pippo,SurfSel);
}


void MyMainFrame::CloneDrawSel(int PadClone)
{ 
  PadCloneSelection=PadClone;
  SelectedPad=PadClone;
  if(PadClone==0) {PadCloneSelection=0;}
  else
    { PadCloneSelection=1;
    }
  printf("PadClone %i\tPadCloneSelection %i\n",PadClone,PadCloneSelection);
}
void MyMainFrame::CloneGPad()
{
  if((PadCloneSelection==1) && (MultiAdc))
    { CanvasClone=true;   }
  printf("PadClone %i\tPadCloneSelection %i\n",SelectedPad,PadCloneSelection);
}

void MyMainFrame::AMS(int AmsSelection)
{
//  if((PadCloneSelection==1) && (MultiAdc))
//    { CanvasClone=true;   }
  printf("AmsSelection %i\n",AmsSelection);
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

  if(Histo) StatusBar->SetText("Histo",2);
  if(Graph) StatusBar->SetText("Graph",2);
  if(GFit)  StatusBar->SetText("Fit",2);
  if(H2D)   StatusBar->SetText("Histo2D",2);  // Gui Status Bar
  if(MultiAdc)   StatusBar->SetText("MultiAdc",2); 

  if (NewDaqFile)
    {  fileType = fopen("DefVmeDaqFile", "r");
      fscanf(fileType,"%s\n",defDaq); 		// Definizione Daq Formato File e VmeDir //////////////////////////////////////////
      fclose(fileType);
      NewDaqFile=false; 
      *(shared_memory+10)=0;
    }

  if(Histo)             
    {
      min=ZoomMin; max=ZoomMax; amp=ZoomAmp;
      TH1F *f1=new TH1F("f1","AmsDefel",amp,min,max);
      f1->Reset();/*fflush(fpx); */  recorded_iev =0;    
      fpx = fopen(defDaq, "r");
      while(! feof(fpx))
	{
	  fscanf(fpx,"%i\n",&x); 		//in origine era float
	  f1->Fill(x); recorded_iev++;
	}
      fclose(fpx);
     
      *(shared_memory+2)=recorded_iev; // Writing collected events in ShmMemory

      f1->SetFillColor(19);
      f1->SetFillStyle(1);
      f1->SetLineWidth(1);
      f1->GetYaxis()->SetTitle(yaxis); 
      f1->GetYaxis()->CenterTitle();
      f1->GetXaxis()->SetTitle(xaxis);
      f1->SetTitle(htitle);
      if(SimpleGauss) {f1->Fit("gaus"); gStyle->SetOptFit(0011);}
      if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
      if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);} ////////////////////////// VirtualTPad
      TCanvas *fCanvas = fEcanvas->GetCanvas();
      f1->Draw();      
      fCanvas->cd();
      fCanvas->Update();
      if(PrintOn)
	{ PrintGui();} f1->Clear();
    }
	
  ////////////////////////// END OF if(Histo)      //////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////// START MULTI ///////////////////// MULTI ///////////////// MULTI ////
  if(MultiAdc)             
    {
      min=ZoomMin; max=ZoomMax; amp=ZoomAmp;
      TH1F *f1=new TH1F("f1","AmsDefel",amp,min,max); ///////// da scrivere
      TH1F *f2=new TH1F("f2","AmsDefel",amp,min,max);
      TH1F *f3=new TH1F("f3","AmsDefel",amp,min,max); 
      TH1F *f4=new TH1F("f4","AmsDefel",amp,min,max);
      TH1F *f5=new TH1F("f5","AmsDefel",amp,min,max); 
      TH1F *f6=new TH1F("f6","AmsDefel",amp,min,max);
//      TH1F *f7=new TH1F("f7","AmsDefel",amp,min,max); 
//      TH1F *f8=new TH1F("f8","AmsDefel",amp,min,max);

      recorded_iev =0;    
      f1->Reset(); f2->Reset(); f3->Reset(); f4->Reset();    
      f5->Reset(); f6->Reset(); // f7->Reset(); f8->Reset();    

      fpx = fopen(defDaq, "r");
      while(! feof(fpx))
	{
	  fscanf(fpx,"%i\n",&xdet1); x=(int)(xdet1/10000);	//in origine era float
	  switch(x)
	   { case 0: {f1->Fill((xdet1)-x*10000); break;}
	     case 1: {f2->Fill((xdet1)-x*10000); break;}
	     case 2: {f3->Fill((xdet1)-x*10000); break;}
	     case 3: {f4->Fill((xdet1)-x*10000); break;}	  
	     case 4: {f5->Fill((xdet1)-x*10000); break;}
	     case 5: {f6->Fill((xdet1)-x*10000); break;}
	    // case 6: {f7->Fill((xdet1)-x*10000); break;}
	    // case 7: {f8->Fill((xdet1)-x*10000); break;}
	   }
          recorded_iev++;
	}
      fclose(fpx);
     
      *(shared_memory+2)=recorded_iev; // Writing collected events in ShmMemory

      f1->SetFillColor(19);
      f1->SetFillStyle(1);
      f1->SetLineWidth(1);
      f1->GetYaxis()->SetTitle(yaxis); 
      f1->GetYaxis()->CenterTitle();
      f1->GetXaxis()->SetTitle(xaxis);
      f1->SetTitle(htitle);
//      if(SimpleGauss) {f1->Fit("gaus"); gStyle->SetOptFit(0011);}
      if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
      if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);} ////////////////////////// VirtualTPad

         
      TCanvas *fCanvas = fEcanvas->GetCanvas();   //
      if(initCanvas)
      {fCanvas->Divide(2,3); initCanvas=false;}

      fCanvas-> cd(1);	
      f1->Draw();  // printf("Printing f1\t"); 
      fCanvas-> cd(2);	//TPad *k1 = gPad;
      f2->Draw();  // printf("Printing f2\t");
      fCanvas-> cd(3);	
      f3->Draw();  // printf("Printing f3\t");
      fCanvas-> cd(4);	
      f4->Draw();  // printf("Printing f4\t");        
      fCanvas-> cd(5);	
      f5->Draw();  // printf("Printing f5\t");
      fCanvas-> cd(6);	
      f6->Draw();  // printf("Printing f6\n");  
      gPad->Modified(); // informa root ch qualcuno ha cambiato il pad, tante volte non se ne fosse accorto
      gPad->Update(); 
      fCanvas->cd();
      fCanvas->Update();
	if(CanvasClone)
  	{TCanvas *CloneCanvas= new TCanvas ("c6","DrawClone Expanded View",200,200,800,600);
	CloneCanvas->cd();
	switch (SelectedPad)
	   {
	    case 1:{f1->Draw(); CloneCanvas->SetTitle("DrawClone Expanded View: Channel 0"); break;}
	    case 2:{f2->Draw(); CloneCanvas->SetTitle("DrawClone Expanded View: Channel 1"); break;}
	    case 3:{f3->Draw(); CloneCanvas->SetTitle("DrawClone Expanded View: Channel 2"); break;}
	    case 4:{f4->Draw(); CloneCanvas->SetTitle("DrawClone Expanded View: Channel 3"); break;}
	    case 5:{f5->Draw(); CloneCanvas->SetTitle("DrawClone Expanded View: Channel 4"); break;}
	    case 6:{f6->Draw(); CloneCanvas->SetTitle("DrawClone Expanded View: Channel 5"); break;}
	    //case 7:{f7->Draw(); f1->SetTitle("Channel 0"); break;}
	    //case 8:{f8->Draw(); f1->SetTitle("Channel 0"); break;}
	   }
	CanvasClone=false;
	}
      if(PrintOn)
	{ PrintGui();} 
	f1->Clear(); f2->Clear();f3->Clear(); f4->Clear();f5->Clear(); f6->Clear(); // f7->Clear(); f8->Clear();


    }


  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////// START H2D ///////////////////// H2D ///////////////// H2D //////////

  if(H2D)             
    {  //    printf("IniSel %i\n",SurfSel);
      switch(SurfSel)
	{
	case 0:
	  { TH2F *f1=new TH2F("f1","AmsDefel",XampH2,XminH2,XmaxH2,YampH2,YminH2,YmaxH2);
	    f1->Reset();/*fflush(fpx); */  recorded_iev =0;    
	    fpx = fopen(defDaq, "r");
	    while(! feof(fpx))
	      { fscanf(fpx,"%i\t",&x); fscanf(fpx,"%i\n",&z);  		//in origine era float
		f1->Fill(x,z); recorded_iev++;}
	    fclose(fpx);
	    *(shared_memory+2)=recorded_iev; // Writing collected events in ShmMemory
	    if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);}
	    f1->GetYaxis()->SetTitle(yaxis); 
	    f1->GetYaxis()->CenterTitle();
	    f1->GetXaxis()->SetTitle(xaxis);
	    f1->SetTitle(htitle); 
	    TCanvas *fCanvas = fEcanvas->GetCanvas();
	    f1->Draw();
	    fCanvas->cd();
	    fCanvas->Update();
	    f1->Clear();
	    if(PrintOn)
	      { PrintGui();} 
	    break;
	  }
	case 1:             
	  { //if(XampH2>=50){XampH2=50;} if(YampH2>=50){YampH2=50;}
	    TH2F *f1=new TH2F("f1","AmsDefel",32,XminH2,XmaxH2,32,YminH2,YmaxH2);
	    f1->Reset();/*fflush(fpx); */  recorded_iev =0;    
	    fpx = fopen(defDaq, "r");
	    while(! feof(fpx))
	      { fscanf(fpx,"%i\t",&x); fscanf(fpx,"%i\n",&z);  		//in origine era float
		f1->Fill(x,z); recorded_iev++;}
	    fclose(fpx);
	    *(shared_memory+2)=recorded_iev; // Writing collected events in ShmMemory
	    gStyle->SetPalette(1); // tentativo di cambio colori (arcobaleno)
	    if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);}
	    f1->GetYaxis()->SetTitle(yaxis); 
	    f1->GetYaxis()->CenterTitle();
	    f1->GetXaxis()->SetTitle(xaxis);
	    f1->SetTitle(htitle); 
	    TCanvas *fCanvas = fEcanvas->GetCanvas();
	    f1->Draw(SelectedSurf);
	    fCanvas->cd();
	    fCanvas->Update();
	    f1->Clear();
	    if(PrintOn)
	      { PrintGui();} 
	    break;
	  }
	}
    }

  if(Graph)
    { iGraph=0;
      fpx = fopen(defDaq, "r");
      while(!feof(fpx))
	{
	  fscanf(fpx,"%f\n",&FdatoX);
	  Fx[iGraph]=FdatoX; channel[iGraph]=iGraph;
	  iGraph++;
	}
      fclose(fpx);
      nGraph=iGraph;
      TGraph *gr1 =new TGraph(nGraph-1,channel,Fx);
      gStyle->SetPalette(1);
      gROOT->SetStyle("Plain"); //toglie il grigio
      gr1->GetYaxis()->SetTitle(yaxis);
      gr1->GetYaxis()->CenterTitle();
      gr1->GetXaxis()->SetTitle(xaxis);
      gr1->SetTitle(htitle);
      gr1->SetLineColor(1);
      gr1->SetFillColor(40);
      if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
      if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);} 
      if(GraphZoomUpdate)
	{GZlow=GraphZoomMin;GZHigh=GraphZoomMax;GraphZoomUpdate=false; *(shared_memory+32)=0;}
      gr1->GetXaxis()->SetRange(GZlow,GZHigh);

      //{printf("GZmin %d\tGZmax %d\n",GraphZoomMin,GraphZoomMax); GraphZoomUpdate=false;*(shared_memory+32)=0;}

      TCanvas *fCanvas = fEcanvas->GetCanvas();
      gr1->Draw("AC"); //La A e' obbligatoria....
      fCanvas->cd();
      fCanvas->Update();
      if(PrintOn)
	{ PrintGui();}
    }
  if(GFit)
    {                                     //////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////// inizio multiFIT///////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      TF1 *g1 = new TF1("m1","gaus",FitLower1,FitUpper1);
      TF1 *g2 = new TF1("m2","gaus",FitLower2,FitUpper2);
      TF1 *g3 = new TF1("m3","gaus",FitLower3,FitUpper3);

      //      THF1 *f1= new TH1F("f1","MultiFitSingleGauss",2048,0,2048);
      //gStyle->SetOptStat("nem");


      if(Gauss1)
	{ JArea1=0;
	  TF1 *total = new TF1("mstotal","gaus(0)",0,2048);
	  TH1F *f1= new TH1F("f1","AmsDefel",2048,0,2048);
	  f1->Reset();/*fflush(fpx); */  recorded_iev =0;    
	  fpx = fopen(defDaq, "r");
	  while(! feof(fpx))
	    {
	      fscanf(fpx,"%i\n",&x);
	      if((x>=FitLower1)&&(x<=FitUpper1))
		JArea1=JArea1+1;
	      f1->Fill(x); recorded_iev++;
	    }
 	  fclose(fpx);
	  Double_t par[3];
	  gStyle->SetOptFit(0000);
	  f1->Fit(g1,"R");
	  g1->GetParameters(&par[0]);
	  GFitParam[0]=par[1];GFitParam[1]=par[2];
	  total->SetParameters(par);
	  if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
	  if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);}
	  f1->Fit(total,"R+");
	  FitLabel(); ///////////////////////////////////////DEVE STARE DOPO FIT(TOTAL,"R+")
	  TCanvas *fCanvas = fEcanvas->GetCanvas();
	  fCanvas->cd();
	  pt1->Draw();
	  f1->GetYaxis()->SetTitle(yaxis);
	  f1->GetYaxis()->CenterTitle(); //gr1->GetYaxis()->CenterTitle();
	  f1->GetXaxis()->SetTitle(xaxis);
	  f1->SetTitle(htitle);
	  fCanvas->Update();
	  G1Area=100*(float)JArea1/recorded_iev;  
	  sprintf(StatusBarText1,"Relative Peak Areas: G1:(%3.1f)",G1Area);         //Ratio1,Ratio2);
	  StatusBar->SetText(StatusBarText1,1);
	  if(PrintOn)
	    {PrintGui();} f1->Clear();
	}
      if(Gauss2)
	{ JArea1=0; JArea2=0; recorded_iev=0;
	  TF1 *total = new TF1("mstotal","gaus(0)+gaus(3)",0,2048);
	  TH1F *f1=new TH1F("f1","MultiFitDoubleGauss",2048,0,2048);
	  f1->Reset();/*fflush(fpx); */  recorded_iev =0;    
	  fpx = fopen(defDaq, "r");
	  while(! feof(fpx))
	    {
	      fscanf(fpx,"%i\n",&x);
	      if((x>=FitLower1)&&(x<=FitUpper1))
		JArea1=JArea1+1;
	      if((x>=FitLower2)&&(x<=FitUpper2))
		JArea2=JArea2+1;
	      f1->Fill(x);
	      recorded_iev++;
	    }
	  fclose(fpx);
	  Double_t par[6];
	  gStyle->SetOptFit(0000);
	  f1->Fit(g1,"R");
	  f1->Fit(g2,"R+");
	  g1->GetParameters(&par[0]);
	  g2->GetParameters(&par[3]);
	  GFitParam[0]=par[1];GFitParam[1]=par[2];
	  GFitParam[2]=par[4]; GFitParam[3]=par[5];
	  total->SetParameters(par);
	  if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
	  if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);}
	  f1->Fit(total,"R+");
	  FitLabel();
	  TCanvas *fCanvas = fEcanvas->GetCanvas();
	  fCanvas->cd();
	  pt2->Draw();
	  f1->GetYaxis()->SetTitle(yaxis); 
	  f1->GetYaxis()->CenterTitle();
	  f1->GetXaxis()->SetTitle(xaxis);
	  f1->SetTitle(htitle);
	  fCanvas->Update(); 
	  G1Area=100*(float)JArea1/recorded_iev;  
	  G2Area=100*(float)JArea2/recorded_iev;
	  sprintf(StatusBarText1,"Relative Peak Areas: G1:(%3.1f) | G2:(%3.1f)",G1Area,G2Area);         //Ratio1,Ratio2);
	  StatusBar->SetText(StatusBarText1,1);
	  if(PrintOn)
	    {PrintGui();} f1->Clear();
	}
      if(Gauss3)
	{  JArea1=0; JArea2=0; JArea3=0;    
	  TF1 *total = new TF1("mstotal","gaus(0)+gaus(3)+gaus(6)",0,2048);
	  TH1F *f1= new TH1F("f1","MultiFitTripleGauss",2048,0,2048);
	  f1->Reset();/*fflush(fpx); */  recorded_iev =0;    
	  fpx = fopen(defDaq, "r");
	  while(! feof(fpx))
	    {
	      fscanf(fpx,"%i\n",&x);
	      if((x>=FitLower1)&&(x<=FitUpper1))
		JArea1=JArea1+1;
	      if((x>=FitLower2)&&(x<=FitUpper2))
		JArea2=JArea2+1;	      
	      if((x>=FitLower3)&&(x<=FitUpper3))
		JArea3=JArea3+1;
	      f1->Fill(x); recorded_iev++;
	    }
	  fclose(fpx); 

	  Double_t par[9];
	  gStyle->SetOptFit(0000);
	  f1->Fit(g1,"R");
	  f1->Fit(g2,"R+");
	  f1->Fit(g3,"R+");
	  g1->GetParameters(&par[0]);
	  g2->GetParameters(&par[3]);
	  g3->GetParameters(&par[6]);
	  //if(viewPeak)
	  GFitParam[0]=par[1];GFitParam[1]=par[2];GFitParam[2]=par[4];
	  GFitParam[3]=par[5];GFitParam[4]=par[7];GFitParam[5]=par[8];
	  total->SetParameters(par);
	  if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
	  if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);}
	  f1->Fit(total,"R+");	// pt->Draw();//  leg->Draw();
	  FitLabel();
	  TCanvas *fCanvas = fEcanvas->GetCanvas();
	  fCanvas->cd();
	  pt3->Draw();
	  f1->GetYaxis()->SetTitle(yaxis);
	  f1->GetYaxis()->CenterTitle(); 
	  f1->GetXaxis()->SetTitle(xaxis);
	  f1->SetTitle(htitle);
	  fCanvas->Update();
	  G1Area=100*(float)JArea1/recorded_iev;  
	  G2Area=100*(float)JArea2/recorded_iev;
	  G3Area=100*(float)JArea3/recorded_iev;
	  sprintf(StatusBarText1,"Relative Peak Areas: G1:(%3.1f) | G2:(%3.1f) | G3:(%3.1f)",G1Area,G2Area,G3Area);         //Ratio1,Ratio2);
	  StatusBar->SetText(StatusBarText1,1);
	  if(PrintOn)
	    {PrintGui();} f1->Clear();
	}

      if(ExtendedFitArea1)
	{  JArea1=0; JArea2=0; JArea3=0; JArea4=0;    
	  TF1 *total = new TF1("mstotal","gaus(0)+gaus(3)+gaus(6)",0,2048);
	  TH1F *f1= new TH1F("f1","MultiFit  3Gauss + 1ExtendedArea",2048,0,2048);
	  f1->Reset();/*fflush(fpx); */  recorded_iev =0;    
	  fpx = fopen(defDaq, "r");
	  while(! feof(fpx))
	    {
	      fscanf(fpx,"%i\n",&x);
	      if((x>=FitLower1)&&(x<=FitUpper1))
		JArea1=JArea1+1;
	      if((x>=FitLower2)&&(x<=FitUpper2))
		JArea2=JArea2+1;	      
	      if((x>=FitLower3)&&(x<=FitUpper3))
		JArea3=JArea3+1;
	      if((x>=AreaFitLower4)&&(x<=AreaFitUpper4))
		JArea4=JArea4+1;
	      f1->Fill(x); recorded_iev++;
	    }
	  fclose(fpx); 
	  Double_t par[9];
	  gStyle->SetOptFit(0000);
	  f1->Fit(g1,"R");
	  f1->Fit(g2,"R+");
	  f1->Fit(g3,"R+");
	  g1->GetParameters(&par[0]);
	  g2->GetParameters(&par[3]);
	  g3->GetParameters(&par[6]);
	  GFitParam[0]=par[1];GFitParam[1]=par[2];GFitParam[2]=par[4];
	  GFitParam[3]=par[5];GFitParam[4]=par[7];GFitParam[5]=par[8];
	  total->SetParameters(par);
	  if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
	  if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);}
	  FitLabel();
	  TCanvas *fCanvas = fEcanvas->GetCanvas();
	  fCanvas->cd();
	  f1->Fit(total,"R+");
	  f1->GetYaxis()->SetTitle(yaxis); 
	  f1->GetYaxis()->CenterTitle();
	  f1->GetXaxis()->SetTitle(xaxis);
	  f1->SetTitle(htitle);
	  pt3->Draw();
	  fCanvas->Update();
	  G1Area=100*(float)JArea1/recorded_iev;  
	  G2Area=100*(float)JArea2/recorded_iev;
	  G3Area=100*(float)JArea3/recorded_iev;
	  G4Area=100*(float)JArea4/recorded_iev;
	  sprintf(StatusBarText1,"Relative Peak Areas: G1:(%3.1f) | G2:(%3.1f) | G3:(%3.1f) | G4:(%3.1f)",G1Area,G2Area,G3Area,G4Area);      
	  StatusBar->SetText(StatusBarText1,1);
	  if(PrintOn)
	    {PrintGui();} f1->Clear();
	}
      if(ExtendedFitArea2)
	{  JArea1=0; JArea2=0; JArea3=0; JArea4=0; JArea5=0;    
	  TF1 *total = new TF1("mstotal","gaus(0)+gaus(3)+gaus(6)",0,2048);
	  TH1F *f1= new TH1F("f1","MultiFit 3Gauss + 2ExtendedArea",2048,0,2048);
	  f1->Reset();/*fflush(fpx); */ recorded_iev =0;    
	  fpx = fopen(defDaq, "r");
	  while(! feof(fpx))
	    {
	      fscanf(fpx,"%i\n",&x);
	      if((x>=FitLower1)&&(x<=FitUpper1))
		JArea1=JArea1+1;
	      if((x>=FitLower2)&&(x<=FitUpper2))
		JArea2=JArea2+1;	      
	      if((x>=FitLower3)&&(x<=FitUpper3))
		JArea3=JArea3+1;
	      if((x>=AreaFitLower4)&&(x<=AreaFitUpper4))
		JArea4=JArea4+1;
	      if((x>=AreaFitLower5)&&(x<=AreaFitUpper5))
		JArea5=JArea5+1;
	      f1->Fill(x); recorded_iev++;
	    }
	  fclose(fpx); 
	  Double_t par[9];
	  gStyle->SetOptFit(0000);
	  f1->Fit(g1,"R");
	  f1->Fit(g2,"R+");
	  f1->Fit(g3,"R+");
	  g1->GetParameters(&par[0]);
	  g2->GetParameters(&par[3]);
	  g3->GetParameters(&par[6]);
	  GFitParam[0]=par[1];GFitParam[1]=par[2];GFitParam[2]=par[4];
	  GFitParam[3]=par[5];GFitParam[4]=par[7];GFitParam[5]=par[8];
	  total->SetParameters(par);
	  if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
	  if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);}
	  FitLabel();
	  TCanvas *fCanvas = fEcanvas->GetCanvas();
	  fCanvas->cd();
       	  f1->Fit(total,"R+");
	  f1->GetYaxis()->SetTitle(yaxis); 
	  f1->GetYaxis()->CenterTitle();
	  f1->GetXaxis()->SetTitle(xaxis);
	  f1->SetTitle(htitle);
	  pt3->Draw();
	  fCanvas->Update();
	  G1Area=100*(float)JArea1/recorded_iev;  
	  G2Area=100*(float)JArea2/recorded_iev;
	  G3Area=100*(float)JArea3/recorded_iev;
	  G4Area=100*(float)JArea4/recorded_iev;
	  G5Area=100*(float)JArea5/recorded_iev;
	  //printf("Area5 %f\n",(float)JArea5);
	  sprintf(StatusBarText1,"Relative Peak Areas: G1:(%3.1f)  ||  G2:(%3.1f)  ||  G3:(%3.1f)  ||  G4:(%3.1f)  ||  G5:(%3.1f)",G1Area,G2Area,G3Area,G4Area,G5Area);     
	  StatusBar->SetText(StatusBarText1,1);
	  if(PrintOn)
	    {PrintGui();} f1->Clear();
	}
    }
DoDrawInProgress=false;
}

////////////////////////////////////////////////////////// MAIN ////////// MAIN ///////////////// MAIN ///////////////

  int main(int argc, char **argv) {
  TApplication theApp("App",&argc,argv);

  ////////// STARTING SHARED MEMORY MAPPING 

  key = 6400;
  shmid = shmget (6400, SHMSZ, IPC_CREAT | 0666);
  shared_memory =(int *) shmat(shmid, NULL, 0);
  cout << "Command Memory attached at :" << (int *)shared_memory<< endl << " with shmid: " << shmid << endl;

  key = 6500;
  shmidFloat = shmget (6500, SHMSZ, IPC_CREAT | 0666);
  shared_memFloat =(float *) shmat(shmid, NULL, 0);
  cout << "Data Memory attached at :" << (float *)shared_memFloat<< endl << " with shmid: " << shmid << endl;

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
