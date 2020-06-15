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
float AmpScale=0.1; int ScaleF=10;
char TxtStatus[20];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// START STANDARD GUI ///////////////////////////////////////
char StatusBarText1[80];

char defDaq[200];
char DefDirV;

int recorded_iev; int i;
FILE *fpx, *fpy, *fpxStart, *frecord, *fexport;
char ch;
float y;
int x; int z; float xCal; float zCal; float CalFactor=1.0; int CalValue=1; bool TimeLinear=false;
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
int XampH2D=32; int YampH2D=32;

int PrType=0; int ManualOn=0; int FisrtManualOn=0; int cmndStore=0;
float channel[65536]; float Fx[65536]; float FdatoX; int iGraph; int nGraph;
int PrevOn=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////// BOOLS FOR SELECTIONS ////////////////////////////////////////// BOOLS FOR SELECTIONS  /////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool LogOn=false; bool GridOn=false; //bool H2DSURF=false;
bool PviewOn=false;  bool Histo=false; bool Graph=false; bool PrintOn=false; bool ExportPng=false; bool ExportJpg=false; 
bool ExportPs=false; bool ExportEps=false; bool ExtendedFitArea1=false; bool ExtendedFitArea2=false;
bool SimpleGauss=false; bool Linear=false;
int FitOnGauss=0;
bool NewDaqFile=false; bool GFit=false; bool Gauss1=false; bool Gauss2=false; bool Gauss3=false; bool H2D=false;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Calibration=false;


FILE *PlotTitle; FILE *XLabel; FILE *YLabel;   
TString htitle; TString xaxis; TString yaxis;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MainCommand=0; int CommandList=0; int ZoomMin, ZoomMax, ZoomAmp; int GraphHistoSel=0;
int StartStopCmd=0;
Double_t GFitParam[8]; int iGFit=0;
/////////////////////////////////////////////////////////////////////// TextPave for fit ///////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TGString Lego, Lego1, Lego2, Surf, Surf1, Surf2, Surf3, Surf4, Surf5, Select;
 int SurfSel=0; TGString SelectedSurf;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool DoDrawInProgress=false;

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {  //Tutte le modifiche fatte qui dentro devono passare dal ROOTCINT
  // Create a main frame
  fMain = new TGMainFrame(p,w,h);

  fMain->SetIconPixmap("Pictures/Infn2.gif");
  Lego="LEGO"; Lego1="LEGO1"; Lego2="LEGO2"; Select="Select Surf"; Surf="SURF";  
  Surf1="SURF1"; Surf2="SURF2"; Surf3="SURF3"; Surf4="SURF4"; Surf5="SURF5";

  // Create canvas widget
  fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,800,460);
  fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX| kLHintsExpandY,
					      10,10,10,1)); //10,10,10,1

  fMain->Connect("CloseWindow()", "MyMainFrame", this, "CloseMainWindow()");

  TTimer *timer1 = new TTimer();
  timer1->Start(250, kFALSE); //era500
  timer1->Connect("Timeout()", "MyMainFrame",this, "TimeOut()");


  
  // Create a horizontal frame widget with buttons
  TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,800,30);
  //  TGHorizontalFrame *hframe1 = new TGHorizontalFrame(fMain,680,430);

  TGCheckButton *EnTimeCal = new TGCheckButton(hframe,"&EnTimeCal");
  EnTimeCal->Connect("Clicked()","MyMainFrame",this,"EnTimeCal()");
  hframe->AddFrame(EnTimeCal, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGCheckButton *TimeLinear = new TGCheckButton(hframe,"&t^(-1/2)");
  TimeLinear->Connect("Clicked()","MyMainFrame",this,"TimeSqrt()");
  hframe->AddFrame(TimeLinear, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  TGComboBox *H2Surf = new TGComboBox(hframe,"SelectSurf");
  H2Surf->AddEntry(Select,0);
  H2Surf->AddEntry(Lego,1);
  //  H2Surf->AddEntry(Lego1,2); 
  //  H2Surf->AddEntry(Lego2,3); 
  H2Surf->AddEntry(Surf,4); 
  H2Surf->AddEntry(Surf1,5);
  // H2Surf->AddEntry(Surf2,6); 
  H2Surf->AddEntry(Surf3,7); 
  H2Surf->AddEntry(Surf4,8); 
  // H2Surf->AddEntry(Surf5,9);
  H2Surf-> Resize(80,20);
  H2Surf->Connect("Selected(int)","MyMainFrame",this,"H2DSurf(int)");
  hframe->AddFrame(H2Surf, new TGLayoutHints(kLHintsCenterX,5,5,3,3));

  

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
  // StatusBar->SetTextSize(10);
  

  StatusBar->SetParts(parts,3);
  StatusBar->SetText("AmsDefel RootGui StatusBar",0);
  //  StatusBar->Connect("MyMainFrame",this,"BarStatus()");

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
{if(!LogOn){LogOn=true;} else{LogOn=false;}}
void MyMainFrame::Grid()
{if(!GridOn){GridOn=true;} else{GridOn=false;}}

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

//  printf("MainCommand: %i\n",MainCommand);  
//  if (MainCommand==1) {GFit=false;}/////////////////////////////////////////////////////////////////////////////////////////

  CommandList=*(shared_memory+16); //Zoom change
  if(CommandList==1)
    { //printf("Run zoom\t cmnd%d\n",CommandList);
      ZoomMin=*(shared_memory+3);
      ZoomMax=*(shared_memory+4);
      ZoomAmp=*(shared_memory+5);
      *(shared_memory+16)=0; // si killa la news dopo averla letta
    }
  GraphHistoSel=*(shared_memory+7); // GraphHisto  printf("cmnd %d\t GraphHistoSel %d\n", cmnd, GraphHistoSel);
  if (GraphHistoSel==0)
    {Histo=true; H2D=false;}
  if (GraphHistoSel==3) 
    {Histo=false; H2D=true; if(Calibration) {CalValue=*(shared_memory+42); CalFactor= CalValue*0.0001;}} 
  //printf("CalValue %i\t %f\n",CalValue,CalFactor);}
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////// ATTENZIONE GraphHistoSel==2 per FIT

  if ((CommandList=*(shared_memory+10))==1) NewDaqFile=true; // DefVmeDaqDir Changed
  if ((CommandList=*(shared_memory+11))==1) {ExportPng=true; PrintOn=true;} // Export png
  if ((CommandList=*(shared_memory+12))==1) {ExportJpg=true; PrintOn=true;}  // Export jpg
  if ((CommandList=*(shared_memory+13))==1) {ExportPs=true; PrintOn=true;}  // Export ps
  if ((CommandList=*(shared_memory+14))==1) {ExportEps=true; PrintOn=true;}  // Export eps



  H2DZUP=*(shared_memory+40);
  if(H2DZUP==1)
    { XminH2=*(shared_memory+36);  
      XmaxH2=*(shared_memory+37); 
      YminH2=*(shared_memory+38); 
      YmaxH2=*(shared_memory+39);
//      AmpScale=*(shared_memDouble);printf("AmpScale %f\n",AmpScale);
      ScaleF=*(shared_memory+41); AmpScale=(ScaleF*0.01);
//      printf("ROOTGui--> Fscale %i\tAmpScale %f\n",ScaleF,AmpScale);

      XampH2=int((abs(XmaxH2-XminH2)*AmpScale)); YampH2=int((abs(YmaxH2-YminH2)*AmpScale)); 
      if(XampH2<128){XampH2=128;} if(XampH2>384){/*printf("Xamp Overflow\n");*/ XampH2=384;}
      if(YampH2<128){YampH2=128;} if(YampH2>384){/*printf("Yamp Overflow\n");*/ YampH2=384;}
 //     printf("Xamp %i\tYamp %i\n", XampH2, YampH2);
 //     printf("Xmin %i\tXmax %i\tYmin %i\t Ymax %i\n",XminH2,XmaxH2,YminH2,YmaxH2);
      *(shared_memory+40)=0; 
      sprintf(TxtStatus,"H2D resolution: %ix%i",XampH2,YampH2);
      StatusBar->SetText(TxtStatus,1);
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
  if(H2D)   StatusBar->SetText("Histo2D",2);

  if (NewDaqFile)
    {  fileType = fopen("DefVmeDaqFile", "r");
      fscanf(fileType,"%s\n",defDaq); 		// Definizione Daq Formato File e VmeDir //////////////////////////////////////////
      fclose(fileType);
      NewDaqFile=false; 
      *(shared_memory+10)=0;
    }

  if(Histo)             
    {
      min=ZoomMin; max=ZoomMax; amp=ZoomAmp; if (amp<64) amp=64;
      TH1F *f1=new TH1F("f1","AmsDefel",amp,min,max);
      f1->Reset();/*fflush(fpx); */  recorded_iev =0;    
      fpx = fopen(defDaq, "r");
      while(! feof(fpx))
	{
//	  fscanf(fpx,"%i\n",&x); 		//in origine era float
//	  f1->Fill(x); recorded_iev++;
	  fscanf(fpx,"%f\n",&xf); 		//in origine era float
	  f1->Fill(xf); recorded_iev++;
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
  ////////////////////////////////// START H2D ///////////////////// H2D ///////////////// H2D //////////

  if(H2D)             
    {  //    printf("IniSel %i\n",SurfSel);
      gROOT->SetStyle("Plain");
      switch(SurfSel)
	{
	case 0:
	  { XminH2Cal=int(XminH2*CalFactor); XmaxH2Cal=int (XmaxH2*CalFactor);
            TH2F *f1=new TH2F("f1","AmsDefel",XampH2,XminH2Cal,XmaxH2Cal,YampH2,YminH2,YmaxH2);
	    f1->Reset();/*fflush(fpx); */  recorded_iev =0;    

	    if(TimeLinear){
	    fpx = fopen(defDaq, "r"); 
	    while(! feof(fpx))
	      { fscanf(fpx,"%f\t",&xf); xCal=xf*CalFactor; fscanf(fpx,"%f\n",&zf); zCal=(1/(sqrt(zf)));  
		f1->Fill(xCal,zCal); recorded_iev++;}
	    fclose(fpx);}
            else {
	    fpx = fopen(defDaq, "r"); 
	    while(! feof(fpx))
	      { fscanf(fpx,"%f\t",&xf); xCal=xf*CalFactor; fscanf(fpx,"%f\n",&zf);  		
		f1->Fill(xCal,zf); recorded_iev++;}
	    fclose(fpx);}

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


	    XampH2D=int (XampH2*.125);YampH2D=int (YampH2*.125);
	    //COLTZ;
	    TH2F *f1=new TH2F("f1","AmsDefel",XampH2D,XminH2,XmaxH2,YampH2D,YminH2,YmaxH2);
	    f1->Reset();/*fflush(fpx); */  recorded_iev =0;    
	    fpx = fopen(defDaq, "r");
	    while(! feof(fpx))
	      { fscanf(fpx,"%f\t",&xf); fscanf(fpx,"%f\n",&zf);  		//in origine era float
		f1->Fill(xf,zf); recorded_iev++;}
	    fclose(fpx);
	    *(shared_memory+2)=recorded_iev; // Writing collected events in ShmMemory
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
