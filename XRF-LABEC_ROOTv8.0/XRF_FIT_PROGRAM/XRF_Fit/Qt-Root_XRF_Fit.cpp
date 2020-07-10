#include <QPushButton>
#include <QLayout>
#include <QTimer>
#include <QCheckBox>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

#include <stdlib.h>

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
#include "Qt-Root_XRF_Fit.h"
#include "MultiFit_Window.h"
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
#include <errno.h>
#include <sys/ioctl.h>
#include <../Shm.h>
#include <../variables.h>

extern int *shm_cmd, *shmCommand_cmd, shmid_cmd, shmidString_cmd, *shared_memory_cmd; 
extern key_t key_cmd; extern key_t keyString_cmd;


bool histo_on=false; //se l'histo è aperto
bool single_draw=false; //se si vuole ridisegnare l'histo
bool DoDrawInProgress=false; //se si sta disegnando l'histo
bool FitInProgress=false; //se c'è un fit in corso o la calibrazione
bool DoubleFitInProgress=false; //se c'è un doppio fit in corso
bool start_calibration=false;//se c'è una calibrazione in corso
bool fit_OK=false; //se sono pronti i tre marker per il fit
bool Doublefit_Ok=false; //se sono pronti i sei marker
bool cal_Ok=false; //se sono pronti i dati per fare la calibrazione
bool Calibration_ready=false; //se lo spettro è calibrato
bool event_processed=false; //se il click del mouse è stato elaborato
bool Bkg=false; //se si vuole un bkg
bool LogOn=false; bool GridOn=false; 
bool mouse_moved_x=false; bool mouse_moved_y=false;

int Max_spectrum=0;  ///per memorizzare il massimo dello spettro
float Cal=1; //keV/ch costante di calibrazione
float offset=0;
int el_count=0; //memorizza il numero di elementi cliccati sulla tavola periodica
int color=1;
float X_xx,Y_yy;
QString xpos,ypos;
QString testo;

int bkg_number=0;
float mu1, sigma1, Integral1, chi2rid,mu2, sigma2,Integral2;
float marker[3],marker_big[6]; int i_marker_ev=0, j_marker_ev=0; 
float Fit_Xmin=0; float Fit_Xmax=0; float X_Peak_value=0;
float Emin_cal=0, Emax_cal=0;
float FWHM;

bool line1_on=false, line2_on=false, line3_on=false, line4_on=false, line5_on=false, line6_on=false;

char process[30];

bool ROI_Ok=false, BKG_Ok=false, PEAKS_Ok=false, FFUNC_Ok=false; //bottoni tutti rossi all'inizio
bool FitWindowOn=false;
bool ROIStarted=false;//se si sta definendo la ROI
bool SubtractBl=false;
bool EnROI=false, ClickROI=true;
int PMode=1;
float ROI[2]={200.,16000.}; int i_roi=0;
bool Find_for_Multi=false;
bool Click_for_Multi=false;
bool Clicked_for_Multi=false;
bool PTable_for_Multi=false;
bool PTabled_for_Multi=false;
bool Peaks_cancel=false;
bool SomeMarkersOn=false;
bool SomeLinesOn=false;
float PeakPositionX[12];
float PeakPositionY[12];
int PeaksFound=0;
int bkg=-1;
float dataSpectrum[16384], SpectrumX[16384]; ///per memorizzare lo spettro aperto
float Limit_dx[12], Limit_sx[12];
int FitFunctionsCodes[12]={0};
float Integrals[12];
int Tot_gauss=0, Tot_lor=0;

float *shm_fit;
int shmid_fit; 
key_t key_fit;
int SHMSZ_FIT=200;

int NSpectra = 0; //number of open spetra
///////posizioni usate memoria condivisa 
//shm  caso 
//shm+1 Ka
//shm+2 Kb
//shm+3 La
//shm+4 Lb
//shm+5 Ma
//shm+6 Mb
//shm+10 ---> nuove energie da controllare
//shm+11  was_already_clicked: vale 1 se l'elem era già cliccato, 0 se non lo era

//shm+19  stato della finestra Periodic_Table
//shm+20  pid della tavola periodica
//////////////////////////////////////////////////////////////////////////////////////////////////////////

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

////
Double_t GaussPeak(Double_t *x, Double_t *par) {
  return (par[0]*TMath::Exp(-0.5*((x[0]-par[1])/par[2])*((x[0]-par[1])/par[2])));
}
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
// Lorenzian Peak function
Double_t LorentzianPeak(Double_t *x, Double_t *par)
{
	return par[0]/TMath::Max( 1.e-10,(x[0]-par[1])*(x[0]-par[1]) + .25*par[2]*par[2] );
}

/******************************************************************/
// Somma di Gaussian Peaks
Double_t GaussianF(Double_t *x, Double_t *par) //par[0] numero di picchi
{
	Double_t f;
	for (int i = 1; i <= par[0]; ++i) {
		f += GaussPeak(x,&par[i*3-2]);
	}
	return f;
}
// Somma di Gaussian e/o Lorentzian Peaks
Double_t GausLorF(Double_t *x, Double_t *par) //par[0] numero di picchi
{
	Double_t f;
	for (int i = 1; i <= par[0]; ++i) 
        {
          if(FitFunctionsCodes[i]==0)
		f += GaussPeak(x,&par[i*3-2]);
          else if(FitFunctionsCodes[i]==1)
		f += LorentzianPeak(x,&par[i*3-2]);
	}
	return f;
}
/******************************************************************/
// Somma di Gaussian Peaks con bkg costante
Double_t GaussianFbkg1(Double_t *x, Double_t *par) //par[0] numero di picchi
{
	Double_t f;
	f = background_const(x,&par[1]);
	for (int i = 1; i <= par[0]; ++i) {
		f += GaussPeak(x,&par[i*3-1]);
	}
	return f;
}

// Somma di Gaussian e/o Lorentzian Peaks con bkg costante
Double_t GausLorFbkg1(Double_t *x, Double_t *par) //par[0] numero di picchi
{
	Double_t f;
	f = background_const(x,&par[1]);
	for (int i = 1; i <= par[0]; ++i) {
          if(FitFunctionsCodes[i]==0)
		f += GaussPeak(x,&par[i*3-1]);
          else if(FitFunctionsCodes[i]==1)
		f += LorentzianPeak(x,&par[i*3-1]);
	}
	return f;
}
/******************************************************************/
// Somma di Gaussian Peaks con bkg lineare
Double_t GaussianFbkg2(Double_t *x, Double_t *par) //par[0] numero di picchi
{
	Double_t f;
	f = background_line(x,&par[1]);
	for (int i = 1; i <= par[0]; ++i) {
		f += GaussPeak(x,&par[i*3]);
	}
	return f;
}

// Somma di Gaussian e/o Lorentzian Peaks con bkg lineare
Double_t GausLorFbkg2(Double_t *x, Double_t *par) //par[0] numero di picchi
{
	Double_t f;
	f = background_line(x,&par[1]);
	for (int i = 1; i <= par[0]; ++i) {
          if(FitFunctionsCodes[i]==0)
		f += GaussPeak(x,&par[i*3]);
          else if(FitFunctionsCodes[i]==1)
		f += LorentzianPeak(x,&par[i*3]);
	}
	return f;
}
//______________________________________________________________________________
QRootCanvas::QRootCanvas(QWidget *parent) : QWidget(parent, 0), fCanvas(0)
{
   // QRootCanvas constructor.

   // set options needed to properly update the canvas when resizing the widget
   // and to properly handle context menus and mouse move events
   
   //setAttribute(Qt::WA_PaintOnScreen, true);  ////questo va rimosso con Qt5, serve soloper Qt4
   setAttribute(Qt::WA_OpaquePaintEvent, true);
   setMinimumSize(800, 400);
   setUpdatesEnabled(kFALSE);
   setMouseTracking(kTRUE);

   //TVirtualX è un'interfaccia al sistema grafico, registra la finestra creata da Qt come una finestra di root
   int wid = gVirtualX->AddWindow((ULong_t)winId(), 800, 400); //vuole come parametro l'id della window
   // create the ROOT TCanvas, giving as argument the QWidget registered id
   fCanvas = new TCanvas("Root Canvas", width(), height(), wid);

   FWindow = new MultiFit_Window(0); //si crea qui ma non si apre. parent nessuno così è autonoma
}

//______________________________________________________________________________
void QRootCanvas::mouseMoveEvent(QMouseEvent *e)
{
   if (fCanvas) {
      if (e->buttons() & Qt::LeftButton) {
         fCanvas->HandleInput(kButton1Motion, e->x(), e->y());
      } else if (e->buttons() & Qt::MidButton) {
         fCanvas->HandleInput(kButton2Motion, e->x(), e->y());
      } else if (e->buttons() & Qt::RightButton) {
         fCanvas->HandleInput(kButton3Motion, e->x(), e->y());
      } else {
         fCanvas->HandleInput(kMouseMotion, e->x(), e->y());
          if(histo_on && !DoDrawInProgress)
            {              
               int px    = gPad->GetEventX();
 	        int py    = gPad->GetEventY();

                 double xd = gPad->AbsPixeltoX(px);
                 double yd = gPad->AbsPixeltoY(py);
                 X_xx = (float)gPad->PadtoX(xd);  
                 Y_yy = (float)gPad->PadtoY(yd); 
                 if(X_xx>0 && Y_yy>0 && X_xx<16000) 
                 {
                  xpos.setNum(X_xx); mouse_moved_x=true; emit sendText(xpos); 
                  ypos.setNum(Y_yy); mouse_moved_y=true; emit sendText(ypos);                  
                 }

            }
      }
   }
}

//______________________________________________________________________________
void QRootCanvas::mousePressEvent( QMouseEvent *e )
{

   if (fCanvas) 
   {
      switch (e->button()) 
      {
         case Qt::LeftButton :
          {
            fCanvas->HandleInput(kButton1Down, e->x(), e->y());
            if(FitInProgress && !event_processed)
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
             else
             if ((start_calibration || DoubleFitInProgress) && !event_processed) //left mouse button click event =1,release event =11 
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
              else
              if(ROIStarted && !event_processed)
               {
               printf("Dentro ROIStarted\n");
                int px    = gPad->GetEventX();
                double xd = gPad->AbsPixeltoX(px);
                float x = gPad->PadtoX(xd);  
                ROI[i_roi]=x;                
                if(i_roi==0) {QString s; s.setNum(x); FWindow->E_lowBox->setText(s);i_roi++;}
                else
                if(i_roi==1)
                 {
                   QString s; s.setNum(x);
                   FWindow->E_highBox->setText(s);
                   i_roi=0; 
                   ROIStarted=false; 
                   FWindow->state1->setStyleSheet("background-color: green");
                   FWindow->ROILineEdit->setText("ROI ready");
                   ROI_Ok=true;
                  }
                event_processed=true;
                }
              else
              if(Click_for_Multi && !event_processed && PeaksFound<12)
               {
                int px    = gPad->GetEventX();
 	        int py    = gPad->GetEventY();

                 double xd = gPad->AbsPixeltoX(px);
                 double yd = gPad->AbsPixeltoY(py);
                 PeakPositionX[PeaksFound] = gPad->PadtoX(xd);  
                 PeakPositionY[PeaksFound] = gPad->PadtoY(yd); 
                 PeaksFound++;
                 emit FWindow->StopFindPeaks();
                 event_processed=true;
                }
            
            break;
          }
         case Qt::MidButton :
            fCanvas->HandleInput(kButton2Down, e->x(), e->y());
            break;
         case Qt::RightButton :
            fCanvas->HandleInput(kButton3Down, e->x(), e->y());
            break;
         default:
            break;
      }
   }
}

////per ora non utilizzato______________________________________________________________________
void QRootCanvas::mouseReleaseEvent( QMouseEvent *e )
{

   if (fCanvas) 
   {
      switch (e->button()) 
      {
         case Qt::LeftButton :
          {
            fCanvas->HandleInput(kButton1Up, e->x(), e->y());
            event_processed=false;
            break;
          }
         case Qt::MidButton :
            fCanvas->HandleInput(kButton2Up, e->x(), e->y());
            break;
         case Qt::RightButton :
            fCanvas->HandleInput(kButton3Up, e->x(), e->y());
            break;
         default:
            break;
      }
   }
}


void QRootCanvas::DoDraw()
{
  DoDrawInProgress=true; 
  gROOT->Reset();   // AZZERA TUTTE LE VARIABILI//////////////////////////////

  if (FitInProgress && histo_on) //disegna i marker se si vuola fare un fit
      {
       if(line1_on)
         {marker1 = new TLine(marker[0],0,marker[0],Max_spectrum);marker1->SetLineColor(kRed);marker1->SetLineWidth(2);marker1->Draw("same");
          testo="Two more points needed to fit the spectrum"; emit sendText(testo);}//era 20000
       if(line2_on)
         {marker2 = new TLine(marker[1],0,marker[1],Max_spectrum);marker2->SetLineColor(kRed);marker2->SetLineWidth(2);marker2->Draw("same");
          testo="One more point needed to fit the spectrum";emit sendText(testo);} //era 20000      
       if(line3_on)
          {marker3 = new TLine(marker[2],0,marker[2],Max_spectrum);marker3->SetLineColor(kRed);marker3->SetLineWidth(2);marker3->Draw("same");
           fit_OK=true;line1_on=false; line2_on=false; line3_on=false; RunFit();} //era 20000
       fCanvas->cd();
       fCanvas->Update(); 
       }
   if(single_draw) //disegna lo spettro in canali o energia
   {
    if(cal_Ok || Calibration_ready)
     {
       f1->GetXaxis()->SetTitle("Energy (keV)");
       f1->SetStats(0);

       if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
       if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);} 

       f1->Draw();  //"elp"
       printf("-------Disegno lo spettro f1_cal\n"); 
       testo=" ";
       emit sendText(testo);
       fCanvas->cd();
       fCanvas->Update();
       gPad->Update();
       single_draw=false;
      }
     else
      {
       f1->GetXaxis()->SetTitle("Channel");
       f1->SetStats(0);

       if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
       if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);} 

       f1->Draw();  //"elp"
       printf("-------Disegno lo spettro f1\n"); 
       fCanvas->cd();
       fCanvas->Update();
       gPad->Update();
       single_draw=false;
       }
    }   

 
  DoDrawInProgress=false;
}

void QRootCanvas::DoubleDoDraw() //disegna i marker se si vuole fare un doppio fit o la calibrazione
{
    DoDrawInProgress=true;
      
    if(line1_on)
     {marker1 = new TLine(marker_big[0],0,marker_big[0],Max_spectrum);marker1->SetLineColor(kRed);marker1->SetLineWidth(2);marker1->Draw("same");
      testo="Five more points needed.";emit sendText(testo);}
    if(line2_on)
     {marker2 = new TLine(marker_big[1],0,marker_big[1],Max_spectrum);marker2->SetLineColor(kRed);marker2->SetLineWidth(2);marker2->Draw("same");
      testo="Four more points needed.";emit sendText(testo);}  
    if(line3_on)
     {marker3 = new TLine(marker_big[2],0,marker_big[2],Max_spectrum);marker3->SetLineColor(kRed);marker3->SetLineWidth(2);marker3->Draw("same");
      testo="Three more points needed.";emit sendText(testo);} 
    if(line4_on)
     {marker4 = new TLine(marker_big[3],0,marker_big[3],Max_spectrum);marker4->SetLineColor(kBlue);marker4->SetLineWidth(2);marker4->Draw("same");
      testo="Two more points needed.";emit sendText(testo);}   
    if(line5_on)
     {marker5 = new TLine(marker_big[4],0,marker_big[4],Max_spectrum);marker5->SetLineColor(kBlue);marker5->SetLineWidth(2);marker5->Draw("same");
      testo="One more point needed.";emit sendText(testo);}
    if(line6_on)
     {marker6 = new TLine(marker_big[5],0,marker_big[5],Max_spectrum);marker6->SetLineColor(kBlue);marker6->SetLineWidth(2);marker6->Draw("same");
       if(start_calibration) {cal_Ok=true; CalculateCal();} 
       if(DoubleFitInProgress) {Doublefit_Ok=true; RunDoubleFit();} 
       line1_on=false; line2_on=false; line3_on=false;line4_on=false; line5_on=false; line6_on=false;
     }
      
      fCanvas->cd();
      fCanvas->Update(); 
     
  DoDrawInProgress=false;
  
}


void QRootCanvas::RunFit()
{
if(fit_OK)
 {
  printf("inizio runfit!\n");
///si assume che il primo cliccato sia xmin, il secondo peak e il terzo xmax
//altrimenti scommentare 
/*
   float maxtmp=marker[0];   
   float mintmp=marker[2];
   float peak_value=marker[1];

    if (marker[1]>marker[0]) {maxtmp=marker[1];} else {mintmp=marker[1];}
    if (marker[2]>maxtmp)    {peak_value=maxtmp; maxtmp=marker[2];}
    if (marker[2]<mintmp)    {peak_value=mintmp; mintmp=marker[2];}

Fit_Xmin=mintmp; Fit_Xmax=maxtmp; X_Peak_value=peak_value;
*/
   Fit_Xmin=marker[0]; Fit_Xmax=marker[2]; X_Peak_value=marker[1];
   printf(" min: %f-----peak val %f------max %f\n", Fit_Xmin,X_Peak_value,Fit_Xmax); 
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
 
     if(Bkg)
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
                    Integral1=signalFcn->Integral(Fit_Xmin,Fit_Xmax); 
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
                    Integral1=signalFcn->Integral(Fit_Xmin,Fit_Xmax);
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
     } //chiude if Bkg
     else
     {
        printf("case 0\n");
      	signalFcn->Draw("+same");
        legend->Draw("+same");
        mu1=signalFcn->GetParameter(1);
        sigma1=signalFcn->GetParameter(2);
        Integral1=signalFcn->Integral(Fit_Xmin,Fit_Xmax);
        chi2rid=signalFcn->GetChisquare()/signalFcn->GetNDF();
        printf("media %f\n", mu1);
      } 
 
            
      fCanvas->cd();
      fCanvas->Update();
      testo="Fit is ready...";emit sendText(testo);


      FWHM=2.355*sigma1;

      QString message="mean = ";
      QString s;
      s.setNum(mu1); message.append(s); message.append("  FWHM = ");
      s.setNum(FWHM); message.append(s); 
      

      message.append("\n Integral = ");
      s.setNum(int(Integral1)); message.append(s); message.append("\n Chi2/ndf = ");
      s.setNum(chi2rid); message.append(s); 
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(0, "Fit results", message,QMessageBox::Save | QMessageBox::Discard);
      
      if (reply == QMessageBox::Save) 
       {
        QString percorso = QFileDialog::getSaveFileName(this,tr("Save as"), QDir::currentPath());
        QFile file2(percorso); 
        file2.open(QIODevice::ReadWrite);
        QTextStream out2(&file2);      
        out2<<message<<'\n';
        file2.close();
        qDebug()<<" Fit results saved in: "<<percorso;

       }

      fCanvas->cd();
      fCanvas->Update();

      TH1F *f1_copy=(TH1F*)f1->Clone();
      f1->Reset();
      f1=(TH1F*)f1_copy->Clone();
      f1->Draw();
      testo=" ";emit sendText(testo);

   fit_OK=false;
   FitInProgress=false;
 }  
}

void QRootCanvas::RunDoubleFit()
{
  if(Doublefit_Ok)
   { 
///si assume che il primo cliccato sia xmin, il secondo peak e il terzo xmax
//altrimenti scommentare  
/*
   float maxtmp1=marker_big[0];
   float mintmp1=marker_big[0];
   float peak_value1=marker_big[0];

    if (marker_big[1]>marker_big[0]) {maxtmp1=marker_big[1];} else {mintmp1=marker_big[1];}
    if (marker_big[2]>maxtmp1)    {peak_value1=maxtmp1; maxtmp1=marker_big[2];}
    if (marker_big[2]<mintmp1)    {peak_value1=mintmp1; mintmp1=marker_big[2];}

    Fit_Xmin=mintmp1; Fit_Xmax=maxtmp1; X_Peak_value=peak_value1;
*/
    Fit_Xmin=marker_big[0]; Fit_Xmax=marker_big[2]; X_Peak_value=marker_big[1];
    printf(" min1: %f-----peak val1 %f------max1 %f\n", Fit_Xmin,X_Peak_value,Fit_Xmax); 

     TF1 *signalFcn1 = new TF1("signalFcn1","gaus",Fit_Xmin,Fit_Xmax);
     signalFcn1->SetParLimits(1,(X_Peak_value-0.1*X_Peak_value),(X_Peak_value+0.1*X_Peak_value)); //il picco non lo deve spostare molto
     f1->Fit(signalFcn1,"R0");
     signalFcn1->SetLineColor(kBlue);

    float min_temp=Fit_Xmin; //in memoria min e max
    float max_temp=Fit_Xmax;

///si assume che il primo cliccato sia xmin, il secondo peak e il terzo xmax
//altrimenti scommentare 
/*
   float maxtmp2=marker_big[3];
   float mintmp2=marker_big[3];
   float peak_value2=marker_big[3];

    if (marker_big[4]>marker_big[3]) {maxtmp2=marker_big[4];} else {mintmp2=marker_big[4];}
    if (marker_big[5]>maxtmp2)    {peak_value2=maxtmp2; maxtmp2=marker_big[5];}
    if (marker_big[5]<mintmp2)    {peak_value2=mintmp2; mintmp2=marker_big[5];}

    Fit_Xmin=mintmp2; Fit_Xmax=maxtmp2; X_Peak_value=peak_value2;
*/
    Fit_Xmin=marker_big[3]; Fit_Xmax=marker_big[5]; X_Peak_value=marker_big[4];
    printf(" min2: %f-----peak val2 %f------max2 %f\n", Fit_Xmin,X_Peak_value,Fit_Xmax); 

     TF1 *signalFcn2 = new TF1("signalFcn2","gaus",Fit_Xmin,Fit_Xmax); 
     signalFcn1->SetParLimits(1,(X_Peak_value-0.1*X_Peak_value),(X_Peak_value+0.1*X_Peak_value));
     f1->Fit(signalFcn2,"R0");
     signalFcn2->SetLineColor(kCyan);

   if(max_temp>Fit_Xmax) Fit_Xmax=max_temp; //max e min per le funzioni somma totali
   if(min_temp<Fit_Xmin) Fit_Xmin=min_temp;
                        
     signalFcn1->SetRange(Fit_Xmin,Fit_Xmax); //per disegnare le due gauss su tutto il range
     signalFcn2->SetRange(Fit_Xmin,Fit_Xmax);

     TF1 *fitFcn;
     TF1 *backFcn;

     TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
     legend->SetTextFont(72);
     legend->SetTextSize(0.04);
     legend->AddEntry(f1,"Data","l");
     legend->AddEntry(signalFcn1,"Signal fit 1","l");
     legend->AddEntry(signalFcn2,"Signal fit 2","l");


    if(Bkg)   ///cioè se si vuole anche il bkg
    { 
     switch(bkg_number)
	{ 
 
          case 1: {
                    fitFcn = new TF1("fitFcn",fitFunction_0d,Fit_Xmin,Fit_Xmax,7); 
                    backFcn = new TF1("backFcn",background_const,Fit_Xmin,Fit_Xmax,1); // printf("constant\n"); 
                    fitFcn->SetParameter(1,signalFcn1->GetParameter(0));
                    fitFcn->SetParameter(2,signalFcn1->GetParameter(1));
                    float lim1=signalFcn1->GetParameter(1)-0.1*signalFcn1->GetParameter(1);
                    float lim2=signalFcn1->GetParameter(1)+0.1*signalFcn1->GetParameter(1);
                    fitFcn->SetParLimits(2,lim1,lim2); //la posizione del picco non la deve cambiare molto (messo +-10%)
                    fitFcn->SetParameter(3,signalFcn1->GetParameter(2));
                    fitFcn->SetParameter(4,signalFcn2->GetParameter(0));
                    fitFcn->SetParameter(5,signalFcn2->GetParameter(1));
                    lim1=signalFcn2->GetParameter(1)-0.1*signalFcn2->GetParameter(1);//la posizione del picco non la deve cambiare molto (+-10%)
                    lim2=signalFcn2->GetParameter(1)+0.1*signalFcn2->GetParameter(1);
                    fitFcn->SetParLimits(5,lim1,lim2);
                    fitFcn->SetParameter(6,signalFcn2->GetParameter(2));
                    f1->Fit("fitFcn","R0");  //,"ep"

                    Double_t par[7];   // risultati fit
                    fitFcn->GetParameters(par);
                    //printf("parametri: %f  %f  %f %f\n", par[0], par[1],par[2],par[3]);
                    backFcn->SetParameters(par);
                    signalFcn1->SetParameters(&par[1]);
                    signalFcn2->SetParameters(&par[4]);
                    mu1=signalFcn1->GetParameter(1);
                    sigma1=signalFcn1->GetParameter(2);
                    Integral1=signalFcn1->Integral(Fit_Xmin,Fit_Xmax);
                    mu2=signalFcn2->GetParameter(1);
                    sigma2=signalFcn2->GetParameter(2);
                    Integral2=signalFcn2->Integral(Fit_Xmin,Fit_Xmax);
                    chi2rid=fitFcn->GetChisquare()/fitFcn->GetNDF();
                    break;
                   } 
	  case 2: {
                    fitFcn = new TF1("fitFcn",fitFunction_1d,Fit_Xmin,Fit_Xmax,8); 
                    backFcn = new TF1("backFcn",background_line,Fit_Xmin,Fit_Xmax,2);  //printf("linear\n"); 
                    fitFcn->SetParameter(2,signalFcn1->GetParameter(0));
                    fitFcn->SetParameter(3,signalFcn1->GetParameter(1));
                    float lim1=signalFcn1->GetParameter(1)-0.1*signalFcn1->GetParameter(1);
                    float lim2=signalFcn1->GetParameter(1)+0.1*signalFcn1->GetParameter(1);
                    fitFcn->SetParLimits(3,lim1,lim2);
                    fitFcn->SetParameter(4,signalFcn1->GetParameter(2));
                    fitFcn->SetParameter(5,signalFcn2->GetParameter(0));
                    fitFcn->SetParameter(6,signalFcn2->GetParameter(1));
                    lim1=signalFcn2->GetParameter(1)-0.1*signalFcn2->GetParameter(1);
                    lim2=signalFcn2->GetParameter(1)+0.1*signalFcn2->GetParameter(1);
                    fitFcn->SetParLimits(6,lim1,lim2);
                    fitFcn->SetParameter(7,signalFcn2->GetParameter(2));
                    f1->Fit("fitFcn","R0");  

                    Double_t par[8];   // writes the fit results into the par array
                    fitFcn->GetParameters(par);
                    backFcn->SetParameters(par);
                    signalFcn1->SetParameters(&par[2]);
                    signalFcn2->SetParameters(&par[5]);
                    mu1=signalFcn1->GetParameter(1);
                    sigma1=signalFcn1->GetParameter(2);
                    Integral1=signalFcn1->Integral(Fit_Xmin,Fit_Xmax);
                    mu2=signalFcn2->GetParameter(1);
                    sigma2=signalFcn2->GetParameter(2);
                    Integral2=signalFcn2->Integral(Fit_Xmin,Fit_Xmax);//- backFcn->Integral(mu2-3*sigma2,mu2+3*sigma2);
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
      testo="Double fit is ready.";emit sendText(testo);

     /////stringa da scrivere nella messagebox/////     
      QString message="mean1 = ";
      QString s;
      s.setNum(mu1); message.append(s); message.append("  sigma1 = ");
      s.setNum(sigma1); message.append(s); message.append("\n Integral1 = ");
      s.setNum(int(Integral1)); message.append(s); message.append("\n mean2 = ");
      s.setNum(mu2); message.append(s); message.append("  sigma2 = ");
      s.setNum(sigma2); message.append(s); message.append("\n Integral2 = ");
      s.setNum(int(Integral2)); message.append(s); message.append("\n Chi2/ndf = ");
      s.setNum(chi2rid); message.append(s); 
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(0, "Fit results", message,QMessageBox::Save | QMessageBox::Discard);
      
      if (reply == QMessageBox::Save) 
       {
        QString percorso = QFileDialog::getSaveFileName(this,tr("Save as"), QDir::currentPath());
        QFile file2(percorso); 
        file2.open(QIODevice::ReadWrite);
        QTextStream out2(&file2);      
        out2<<message<<'\n';
        file2.close();
        qDebug()<<" Fit results saved in: "<<percorso;

       }
 

      Doublefit_Ok=false;
      DoubleFitInProgress=false;

      TH1F *f1_copy=(TH1F*)f1->Clone();
      f1->Reset();
      f1=(TH1F*)f1_copy->Clone();
      f1->Draw();
      testo=" ";emit sendText(testo);

    }//chiude if(Doublefit_Ok)
 else
 {
  testo="Six points are needed to double_fit the data. Click on the spectrum to fix the points.";emit sendText(testo);

 }

}

void QRootCanvas:: CalculateCal()
{

  if(cal_Ok)
   { 
    Calibration_ready=false;
///si assume che il primo cliccato sia xmin, il secondo peak e il terzo xmax
//altrimenti scommentare
/*
   float maxtmp1=marker_big[0];
   float mintmp1=marker_big[0];
   float peak_value1=marker_big[0];

    if (marker_big[1]>marker_big[0]) {maxtmp1=marker_big[1];} else {mintmp1=marker_big[1];}
    if (marker_big[2]>maxtmp1)    {peak_value1=maxtmp1; maxtmp1=marker_big[2];}
    if (marker_big[2]<mintmp1)    {peak_value1=mintmp1; mintmp1=marker_big[2];}

    Fit_Xmin=mintmp1; Fit_Xmax=maxtmp1; X_Peak_value=peak_value1;
*/
    Fit_Xmin=marker_big[0]; Fit_Xmax=marker_big[2]; X_Peak_value=marker_big[1];
    printf(" min1: %f-----peak val1 %f------max1 %f\n", Fit_Xmin,X_Peak_value,Fit_Xmax); 

     TF1 *signalFcn1 = new TF1("signalFcn1","gaus",Fit_Xmin,Fit_Xmax); 
     f1->Fit(signalFcn1,"R0");
     signalFcn1->SetLineColor(kBlue);

    float min_temp=Fit_Xmin; //in memoria min e max
    float max_temp=Fit_Xmax;

///si assume che il primo cliccato sia xmin, il secondo peak e il terzo xmax
//altrimenti scommentare
/*
   float maxtmp2=marker_big[3];
   float mintmp2=marker_big[3];
   float peak_value2=marker_big[3];

    if (marker_big[4]>marker_big[3]) {maxtmp2=marker_big[4];} else {mintmp2=marker_big[4];}
    if (marker_big[5]>maxtmp2)    {peak_value2=maxtmp2; maxtmp2=marker_big[5];}
    if (marker_big[5]<mintmp2)    {peak_value2=mintmp2; mintmp2=marker_big[5];}

    Fit_Xmin=mintmp2; Fit_Xmax=maxtmp2; X_Peak_value=peak_value2;
*/
    Fit_Xmin=marker_big[3]; Fit_Xmax=marker_big[5]; X_Peak_value=marker_big[4];
    printf(" min2: %f-----peak val2 %f------max2 %f\n", Fit_Xmin,X_Peak_value,Fit_Xmax); 

     TF1 *signalFcn2 = new TF1("signalFcn2","gaus",Fit_Xmin,Fit_Xmax); 
     f1->Fit(signalFcn2,"R");
     signalFcn1->Draw("+same");

     float mean1=signalFcn1->GetParameter(1);
     float mean2=signalFcn2->GetParameter(1);

    if(mean1>mean2) {float temp=mean1; mean1=mean2; mean2=temp;} //così 1 è quello minore
    float energy1, energy2;

/////Input energie per fit///////
 QString mess1="ChLow: ";
 QString s1; s1.setNum(mean1);
 mess1.append(s1);
 bool ok1;
      double chan1 = QInputDialog::getDouble(this, mess1, tr("ELow (keV):"), 0, 0, 60, 2, &ok1);
      if (ok1)
        {energy1=chan1;}
 QString mess2="ChHigh: ";
 QString s2; s2.setNum(mean2);
 mess2.append(s2);
 bool ok2;
      double chan2 = QInputDialog::getDouble(this, mess2, tr("EHigh (keV):"), 60, 0, 60, 2, &ok2);
      if (ok2)
        {energy2=chan2;}


////////////calibrazione istogramma///////////////////////////
    Cal=(energy2-energy1)/(mean2-mean1); //keV/ch
    offset=(mean2*energy1-mean1*energy2)/(mean2-mean1); //keV
    int w=0;
   Emin_cal=offset;
   Emax_cal=(16000.*Cal)+offset;
   printf("costante di cal %.5f, offset %.5f\n", Cal,offset);
   printf("emin %f, emax %f\n", Emin_cal,Emax_cal);
   int nbins = (f1->GetXaxis()->GetNbins()); 

   TH1F *f1_copy= (TH1F*)f1->Clone();
   delete f1;
   f1 = new TH1F("f1"," ",nbins,Emin_cal,Emax_cal); 
   for (int d=1;d<=nbins;d++) 
   {
      float z = f1_copy->GetBinContent(d); 
      float k = f1_copy->GetXaxis()->GetBinCenter(d);
      float xnew = Cal*k + offset;  //trasformazione sulle x
      for(w=1;w<=z;w++) f1->Fill(xnew); 
      dataSpectrum[d]=z;
      SpectrumX[d]=xnew;
   }
   ROI[0]=ROI[0]*Cal+offset;
   ROI[1]=ROI[1]*Cal+offset;
   ///////////////////////////////////////////////////////////////////
    single_draw=true;
    DoDraw();
    cal_Ok=false;
    Calibration_ready=true;
    start_calibration=false;
    testo="Calibration ready."; emit sendText(testo);
   }//chiude if (cal_ok)

  
}



//______________________________________________________________________________
void QRootCanvas::resizeEvent( QResizeEvent * )
{
   // Handle resize events.

   if (fCanvas) {
      fCanvas->Resize();
      fCanvas->Update();
   }
}

//______________________________________________________________________________
void QRootCanvas::paintEvent( QPaintEvent * )
{
   // Handle paint events.

   if (fCanvas) {
      fCanvas->Resize();
      fCanvas->Update();
   }
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//__INIZIO___MAINWINDOW______///////////////////////////////////////_________

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)   
{
 ////////////memoria condivisa con multifit e tavola periodica//////////////////
   key_fit = 6000;
   shmid_fit = shmget (key_fit, SHMSZ_FIT, IPC_CREAT | 0666);
       if (shmid_fit == -1) qDebug()<<"Errore  "<< strerror(errno)<<'\n';
   shm_fit =(float *) shmat(shmid_fit, NULL, 0);
   cout << "Data Memory for FIT attached at :" << (float *)shm_fit<< endl << " with shmid FIT: " << shmid_fit << endl;

///////posizioni usate
//shm  caso 
//shm+1 Ka
//shm+2 Kb
//shm+3 La
//shm+4 Lb
//shm+5 Ma
//shm+6 Mb
//shm+10 ---> nuove energie da controllare
//shm+11 was_already clicked (0 se non era cliccato, 1 se era già cliccato)

//shm+19 stato di PTable
//shm+20 pid di PTable
/////////////////////

for(int j=0;j<21;j++) *(shm_fit+j)=0;  //pulizia memoria

   key_cmd = 6900;
   shmid_cmd = shmget (key_cmd, SHMSZ_CMD_STATUS, IPC_CREAT | 0666);
   shared_memory_cmd =(int *) shmat(shmid_cmd, NULL, 0);

   cout<< "Data Memory attached at :" << (int *)shared_memory_cmd<< endl << " with shmid: " << shmid_cmd << endl;
    int pid=getpid();
    *(shared_memory_cmd+88)=pid;

*(shared_memory_cmd+78) = 1; //XRF_Fit on
//////Actions////////////////////////
/// In menu FILE
//qDebug()<<"inizio create actions\n";
    openAct = new QAction(tr("Open spectrum"), this);
    openAct->setStatusTip(tr("Open a new spectrum from file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(LoadSpectrum()));

    openCaenAct = new QAction(tr("Open 5780 raw file"), this);
    openCaenAct->setStatusTip(tr("Open a new spectrum from Caen file"));
    connect(openCaenAct, SIGNAL(triggered()), this, SLOT(LoadCaen()));
 

    exitAct = new QAction(tr("Exit"), this);
    exitAct->setStatusTip(tr("Quit the program"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

/// In menu Fit
    BkgSelAct = new QAction(tr("Select Background"), this);
    BkgSelAct->setStatusTip(tr("Choose background function for fit"));
    connect(BkgSelAct, SIGNAL(triggered()), this, SLOT(Select_Background()));

    SingleFitAct = new QAction(tr("Single Fit"), this);
    SingleFitAct->setStatusTip(tr("Fit peak with a single Gauss function"));
    connect(SingleFitAct, SIGNAL(triggered()), this, SLOT(SingleFit()));

    DoubleFitAct = new QAction(tr("Double Fit"), this);
    DoubleFitAct->setStatusTip(tr("Fit peak with two Gauss functions"));
    connect(DoubleFitAct, SIGNAL(triggered()), this, SLOT(DoubleFit()));

    MultiFitAct = new QAction(tr("Multi Fit"),this);
    MultiFitAct->setStatusTip("Fit many peaks");
    connect(MultiFitAct, SIGNAL(triggered()), this, SLOT(openMultiFitWindow()));
    

/// In menu Calibration
    CalAct = new QAction(tr("Calibrate"), this);
    CalAct->setStatusTip(tr("Start calibration"));
    connect(CalAct, SIGNAL(triggered()), this, SLOT(Calibrate()));

/// In menu Spectrum

    PeaksAct = new QAction(tr("Find Peaks"), this);
    PeaksAct->setStatusTip(tr("Find peaks in the spectrum"));
    connect(PeaksAct, SIGNAL(triggered()), this, SLOT(FindPeaks()));

    TableAct = new QAction(tr("Periodic Table"), this);
    TableAct->setStatusTip(tr("Open the periodic table of elements"));
    connect(TableAct, SIGNAL(triggered()), this, SLOT(OpenTable()));

    ClearAct = new QAction(tr("Clear"), this);
    ClearAct->setStatusTip(tr("Clear markers and lines from the spectrum"));
    connect(ClearAct, SIGNAL(triggered()), this, SLOT(ClearSpectrum()));

//////////////////////////////////
///////////MENU///////////////////
///////////////////////////////////////////////////
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(openCaenAct);
    fileMenu->addSeparator();
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    FitMenu = menuBar()->addMenu(tr("Fit"));
    FitMenu->addAction(BkgSelAct);
    FitMenu->addSeparator();
    FitMenu->addAction(SingleFitAct);
    FitMenu->addAction(DoubleFitAct);
    FitMenu->addAction(MultiFitAct);
    //FitMenu->addSeparator();

    CalMenu = menuBar()->addMenu(tr("Calibration"));
    CalMenu->addAction(CalAct);
    CalMenu->addSeparator();

    SpectrumMenu = menuBar()->addMenu(tr("Spectrum"));
    SpectrumMenu->addAction(ClearAct);
    SpectrumMenu->addSeparator();
    SpectrumMenu->addAction(PeaksAct);
    SpectrumMenu->addSeparator();
    SpectrumMenu->addAction(TableAct);
///////////////////////////////

        window = new QWidget;
        setCentralWidget(window);  ///wigdet contenitore centrale (contiene il canvas di root, line edit ecc..)

        window->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        canvas = new QRootCanvas(window); //canvas di root
        canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        x_label = new QLabel(window);
        x_label->setText("X coordinate = ");
        x_label->setAlignment(Qt::AlignRight);
        xLineEdit = new QLineEdit(window);
        xLineEdit->setEnabled(false);  
        xLineEdit->setAlignment(Qt::AlignLeft);
        xLineEdit->setGeometry(QRect(245, 120, 60, 40));     
        spacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Ignored);

        y_label = new QLabel(window);
        y_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        y_label->setAlignment(Qt::AlignRight);
        y_label->setText("Y coordinate = ");
        yLineEdit = new QLineEdit(window);
        yLineEdit->setEnabled(false);
        yLineEdit->setAlignment(Qt::AlignLeft);
        yLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        line1 = new QFrame(window);
        line1->setFrameShape(QFrame::VLine);
        line1->setFrameShadow(QFrame::Sunken);
        line2 = new QFrame(window);
        line2->setFrameShape(QFrame::VLine);
        line2->setFrameShadow(QFrame::Sunken);

        Log_label = new QLabel(window);
        Log_label->setAlignment(Qt::AlignRight);
        Log_label->setText("Log scale ");
        Log_checkBox = new QCheckBox(window);
        connect(Log_checkBox, SIGNAL(stateChanged(int)), this, SLOT(LogStateChanged(int)));
        Log_checkBox->setCheckable(false);

        Grid_label = new QLabel(window);
        Grid_label->setAlignment(Qt::AlignRight);
        Grid_label->setText("Grid ");
        Grid_checkBox = new QCheckBox(window);
        connect(Grid_checkBox, SIGNAL(stateChanged(int)), this, SLOT(GridStateChanged(int)));
        Grid_checkBox->setCheckable(false);

        Spe2_pushButton = new QPushButton(window);
        connect(Spe2_pushButton, SIGNAL(clicked()), this, SLOT(SuperImposeNewSpectrum()));
	Spe2_pushButton->setText("+ New spectrum");
        Spe2_pushButton->setEnabled(false);

        infoLabel = new QLabel(window); //info status bar
        infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        //infoLabel->setAlignment(Qt::AlignCenter);
        infoLabel->setText("<i>Choose a menu option</i>");

        l = new QGridLayout;

        l->addWidget(canvas,0,0,1,7);
        l->addWidget(infoLabel,1,0,1,7);
        l->addWidget(x_label,2,0,1,1);
        l->addWidget(xLineEdit,2,1,1,1);
        l->addWidget(line1,2,2,1,1);
        l->addWidget(Log_label,2,3,1,1);
        l->addWidget(Log_checkBox,2,4,1,1);
        l->addItem(spacer,2,5,1,1);
        l->addWidget(y_label,3,0,1,1);
        l->addWidget(yLineEdit,3,1,1,1);
        l->addWidget(line2,3,2,1,1);
        l->addWidget(Grid_label,3,3,1,1);
        l->addWidget(Grid_checkBox,3,4,1,1);
	l->addWidget(Spe2_pushButton, 2,5,1,1);

        window->setLayout(l);
       

   fRootTimer = new QTimer( this );   ///timer per root
   QObject::connect( fRootTimer, SIGNAL(timeout()), this, SLOT(handle_root_events()) );
   fRootTimer->start( 20 );

   PTabletimer = new QTimer(this);   //timer per comunicare con Periodic_Table 
   connect(PTabletimer, SIGNAL(timeout()), this, SLOT(PTtimerEvent()));


   connect(canvas, SIGNAL(sendText(QString & )), this, SLOT(SetCurrentAction(QString & ))); ///per usare gli emit 
   connect(canvas->FWindow, SIGNAL(FP()), this, SLOT(FP_rimbalzo()));
   connect(canvas->FWindow, SIGNAL(StopFindPeaks()), this, SLOT(IndicatePeaks()));
   connect(canvas->FWindow, SIGNAL(Fit_tutti()), this, SLOT(Fit_All_Peaks()));
   connect(canvas->FWindow, SIGNAL(ClearAll()), this, SLOT(Clear_MultiFit()));
   connect(canvas->FWindow, SIGNAL(PTable()), this, SLOT(OpenTable()));
}


void MainWindow::SetCurrentAction(QString & text)  // SET INFO ON BAR
  {
   if(mouse_moved_x)   //se è cambiata la posizione x del cursore sullo spettro aggiorna la lineedit x
    {xLineEdit->setText(text); mouse_moved_x=false;}
   else
   if(mouse_moved_y)   //se è cambiata la posizione y del cursore sullo spettro aggiorna la lineedit y
    {yLineEdit->setText(text); mouse_moved_y=false;}
   else    //sennò nel caso generale scrive il messaggio sulla infolabel
   infoLabel->setText(text);
  }


void MainWindow::LoadSpectrum()
{
DoDrawInProgress=true; 
gROOT->Reset();
const int bin= 16384;

int ch =0;

  QString text = QFileDialog::getOpenFileName(this,tr("Open Spectrum"), QDir::currentPath());
   if (!text.isEmpty())
    {
     QFile file(text);
     if(file.exists())
      {
       Max_spectrum=0;
       qDebug()<<"....loading file...\n";
       file.open(QIODevice::ReadOnly);
       QString line;
       while( !file.atEnd() ) 
         {
          line = file.readLine();
          int numero=line.toInt();
          dataSpectrum[ch]=numero;
          SpectrumX[ch]=ch; 
          if(numero>Max_spectrum && ch>0) Max_spectrum=numero;
          ch++;
         }
       file.close();
       dataSpectrum[0]=0; ///corregge errore nei file (bin 0 sempre 16384)
      }
    }
    
    canvas->f1 = new TH1F("histo","",16000,0,16000);
    canvas->f1->Reset();
    canvas->f1->SetMarkerStyle(21);
    canvas->f1->SetMarkerSize(0.8);
    canvas->f1->SetStats(0);
    canvas->f1->GetXaxis()->SetTitle("Channel");

    for(int i=0; i < bin;  i++) canvas->f1->SetBinContent(i+1,dataSpectrum[i]);
    if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
    if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);} 

    canvas->f1->Draw();  //"elp"
    printf("-------Disegno lo spettro f1\n"); 
    canvas->fCanvas->cd();
    canvas->fCanvas->Update();
    
    Calibration_ready=false; //è stato aperto un nuovo spettro non calibrato
    Cal=1; offset=0;
    ROI[0]=200; ROI[1]=16000;
    PeaksFound=0; //non ci sono picchi segnati
    el_count=0; //non ci sono elementi cliccati
    if(*(shm_fit+19)) 
     {int pidPT=*(shm_fit+20);
      sprintf(process, "kill -s TERM %i &", pidPT);
      system(process);}//se è aperta la tavola periodica la chiude
    if(FitWindowOn) {canvas->FWindow->ResetAll(); canvas->FWindow->close(); FitWindowOn=false;}

    histo_on=true;
    Log_checkBox->setCheckable(true);
    Grid_checkBox->setCheckable(true);
    DoDrawInProgress=false;
    Spe2_pushButton->setEnabled(true);
}



void MainWindow::LoadCaen()
{
DoDrawInProgress=true; 
gROOT->Reset();
const int bin= 16384;

int ch =0;



  QString text = QFileDialog::getOpenFileName(this,tr("Open Spectrum"), QDir::currentPath());
   if (!text.isEmpty())
    {
    printf("qui");
     QFile file(text);
     if(file.exists())
      {
           printf("il file esiste\n");
       Max_spectrum=0;
       qDebug()<<"....loading file...\n";
       file.open(QIODevice::ReadOnly);
       QString line;

       while( !file.atEnd() ) 
         {
          line = file.readLine();
          line.remove(0,5);
          int numero=line.toInt();
          dataSpectrum[ch]=numero;
          SpectrumX[ch]=ch; 
          if(numero>Max_spectrum && ch>0) Max_spectrum=numero;
          ch++;
         }
      file.close();
      dataSpectrum[0]=0; ///corregge errore nei file (bin 0 sempre 16384)
      }
    }






/*
  QString text = QFileDialog::getOpenFileName(this,tr("Open Spectrum"), QDir::currentPath());
   if (!text.isEmpty())
    {
     QFile file(text);
     if(file.exists())
      {
       Max_spectrum=0;
       qDebug()<<"....loading file...\n";
       file.open(QIODevice::ReadOnly);
       QString line;

       while( !file.atEnd() ) 
         {
          line = file.readLine();

          int numero=line.toInt();
          dataSpectrum[ch]=numero;
          SpectrumX[ch]=ch; 
          if(numero>Max_spectrum && ch>0) Max_spectrum=numero;
          ch++;
         }
      file.close();
       dataSpectrum[0]=0; ///corregge errore nei file (bin 0 sempre 16384)
      }
    }
*/    
    canvas->f1 = new TH1F("histo","",16000,0,16000);
    canvas->f1->Reset();
    canvas->f1->SetMarkerStyle(21);
    canvas->f1->SetMarkerSize(0.8);
    canvas->f1->SetStats(0);
    canvas->f1->GetXaxis()->SetTitle("Channel");

    for(int i=0; i < bin;  i++) canvas->f1->SetBinContent(i+1,dataSpectrum[i]);
    if(LogOn){gPad->SetLogy();} else{gPad->SetLogy(0);}
    if(GridOn){gPad->SetGrid();} else{gPad->SetGrid(0,0);} 

    canvas->f1->Draw();  //"elp"
    printf("-------Disegno lo spettro f1\n"); 
    canvas->fCanvas->cd();
    canvas->fCanvas->Update();
    
    Calibration_ready=false; //è stato aperto un nuovo spettro non calibrato
    Cal=1; offset=0;
    ROI[0]=200; ROI[1]=16000;
    PeaksFound=0; //non ci sono picchi segnati
    el_count=0; //non ci sono elementi cliccati
    if(*(shm_fit+19)) 
     {int pidPT=*(shm_fit+20);
      sprintf(process, "kill -s TERM %i &", pidPT);
      system(process);}//se è aperta la tavola periodica la chiude
    if(FitWindowOn) {canvas->FWindow->ResetAll(); canvas->FWindow->close(); FitWindowOn=false;}

    histo_on=true;
    Log_checkBox->setCheckable(true);
    Grid_checkBox->setCheckable(true);
    DoDrawInProgress=false;
    Spe2_pushButton->setEnabled(true);
}




///superimpose new spectrum i
void MainWindow::SuperImposeNewSpectrum(){
DoDrawInProgress=true; 
const int nbins= (canvas->f1->GetXaxis()->GetNbins()); 
float dataSpectrumNew[16384], SpectrumXNew[16384];
int ch =0;
int Max_spectrumNew=0;
int i = NSpectra;
            
int color = NSpectra + 3; if(color==10)color++;

  QString text = QFileDialog::getOpenFileName(this,tr("+ Spectrum"), QDir::currentPath());
   if (!text.isEmpty())
    {
     QFile file(text);
     if(file.exists())
      {
       Max_spectrumNew=0;
       qDebug()<<"....loading file...\n";
       file.open(QIODevice::ReadOnly);
       QString line;
       while( !file.atEnd() ) 
         {
          line = file.readLine();
          int numero=line.toInt();
          dataSpectrumNew[ch]=numero;
          SpectrumXNew[ch]=ch; 
          if(numero>Max_spectrumNew && ch>0) Max_spectrumNew=numero;
          ch++;
         }
       file.close();
       dataSpectrumNew[0]=0; ///corregge errore nei file (bin 0 sempre 16384)
      }
    }
	if(Max_spectrumNew > Max_spectrum){
	Max_spectrum = Max_spectrumNew;
	canvas->f1->GetYaxis()->SetRangeUser(0,Max_spectrum);
	}
 //disegna lo spettro in canali o energia
  
    canvas->fnew[i] = new TH1F("histo","",16000,0,16000);
    canvas->fnew[i]->Reset();
    canvas->fnew[i]->SetMarkerStyle(21);
    canvas->fnew[i]->SetMarkerSize(0.8);
    canvas->fnew[i]->SetStats(0);

    for(int j=0; j < nbins;  j++) canvas->fnew[i]->SetBinContent(j+1,dataSpectrumNew[j]); 



    if(cal_Ok || Calibration_ready)
     {
  	 TH1F *f1_copy= (TH1F*)canvas->fnew[i]->Clone();
   	 delete canvas->fnew[i];
   	 canvas->fnew[i] = new TH1F(""," ",nbins,Emin_cal,Emax_cal); 
  	 for (int d=1;d<=nbins;d++) 
  		 {
     		 float z = f1_copy->GetBinContent(d); 
      		 float k = f1_copy->GetXaxis()->GetBinCenter(d);
      		 float xnew = Cal*k + offset;  //trasformazione sulle x
      		 for(int w=1;w<=z;w++) canvas->fnew[i]->Fill(xnew); 
     		 dataSpectrumNew[d]=z;
      		 SpectrumXNew[d]=xnew;
  		 }
      }

       canvas->fnew[i]->SetLineColor(color); 
       canvas->fnew[i]->Draw("same");  //"elp"
       printf("-------Disegno lo spettro sovrapposto\n"); 
       testo=" ";
       emit canvas->sendText(testo);
       canvas->fCanvas->cd();
       canvas->fCanvas->Update();
       gPad->Update();
       single_draw=false;
       DoDrawInProgress=false;
       NSpectra++;
    if(NSpectra < MAX_NEWSPE)
    Spe2_pushButton->setEnabled(true);
    else
    Spe2_pushButton->setEnabled(false);

}








void MainWindow::ClearSpectrum()
{
if(SomeMarkersOn)
 {
  Peaks_cancel=true;
  Find_for_Multi=true;
  IndicatePeaks();
  SomeMarkersOn=false;
 }
if(SomeLinesOn)
 {
  Peaks_cancel=true;
  PTabled_for_Multi=true;
  IndicatePeaks();
  SomeLinesOn=false;
 }
if(NSpectra>0){
 for(int s=0;s<NSpectra;s++)
  delete canvas->fnew[s];
  canvas->fCanvas->GetCanvas();
  canvas->fCanvas->Update();
  NSpectra=0;
  }
}

void MainWindow::Select_Background()
{
  QStringList items;
  items << tr("No background") << tr("Constant background") << tr("Linear background");

  bool ok;
  QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),tr("Background function:"), items, 0, false, &ok);
  if (ok && !item.isEmpty())
  {
   if(item=="No background") Bkg=false;
   else 
   if(item=="Constant background"){Bkg=true; bkg_number=1;}
   else 
   if(item=="Linear background"){Bkg=true; bkg_number=2;}
  }
}

void MainWindow::SingleFit()
{
FitInProgress=true;  //per attivare il disegno dei marker a Dodraw
testo="Define the fitting range: xlow, peak, xhigh.";
infoLabel->setText(testo);
}

void MainWindow::DoubleFit()
{
DoubleFitInProgress=true; //per attivare il disegno dei marker a DoubleDodraw
testo="Define the fitting ranges: xlow1, peak1, xhigh1, xlow2, peak2, xhigh2. ";
infoLabel->setText(testo);
}

void MainWindow::Calibrate()
{
 if(histo_on) 
   {
    start_calibration=true; //per attivare il disegno dei marker a DoubleDodraw
    testo="Choose the peaks to calibrate the spectrum and define the fitting ranges"; 
    infoLabel->setText(testo);
   }
}


void MainWindow::FindPeaks() //trovata dal sito di root, usa la classe TSpectrum...va capito come usarla se serve
{
SomeMarkersOn=true;
 PeaksFound=0;
float a, bin;
QString n,en;

int Nbins=canvas->f1->GetXaxis()->FindBin(int(ROI[1])) - canvas->f1->GetXaxis()->FindBin(int(ROI[0])); //int Nbins = int (ROI[1]-ROI[0]); // 
int start= canvas->f1->GetXaxis()->FindBin(int(ROI[0])); //int(ROI[0]); //
int stop =canvas->f1->GetXaxis()->FindBin(int(ROI[1]));  //int(ROI[1]); // //
printf("Nbins %d, start %d, stop %d\n",Nbins, start,stop);
Double_t  source[Nbins], dest[Nbins]; //ROI di default è dal bin 10 al bin 16000
for (int i = start; i < stop; i++) source[i-start]=canvas->f1->GetBinContent(i + 1);

   s = new TSpectrum(20);//2*npeaks
   PeaksFound = s->SearchHighRes(source, dest, Nbins, 8, 2, kTRUE, 3, kTRUE, 3);//picchi distanti 3 sigma dal precedente 
                                                                             //picchi alti almeno thr/100*(picco+alto) (sigma 8 thr 2 iterations 3)

if(PeaksFound>12) PeaksFound=12;
printf("Found %d candidate peaks to fit\n",PeaksFound);
//float fPositionX[nfound];
//float fPositionY[nfound];
 Double_t *xpeaks = s->GetPositionX();
for (int i = 0; i < PeaksFound; i++) 
{
a=xpeaks[i];
bin = 1 + Int_t(a + 0.5);
PeakPositionX[i] = canvas->f1->GetBinCenter(bin+start);
PeakPositionY[i] = canvas->f1->GetBinContent(bin+start);

if(Find_for_Multi) 
 {
  n.setNum(i);n.append("\n");
  en.setNum(PeakPositionX[i]); en.append("\n");
  canvas->FWindow->textEditP->append(n);
  canvas->FWindow->textEditE->append(en);
 }
} //chiude for

pm = new TPolyMarker(PeaksFound, PeakPositionX, PeakPositionY);
canvas->f1->GetListOfFunctions()->Add(pm);
pm->SetMarkerStyle(23);
pm->SetMarkerColor(kRed);
pm->SetMarkerSize(1.3);
pm->Draw("same");
canvas->getCanvas();
canvas->fCanvas->Update();

if(Find_for_Multi) 
 {
  canvas->FWindow->state3->setStyleSheet("background-color: green"); 
  canvas->FWindow->state4->setStyleSheet("background-color: green");
  PEAKS_Ok=true;
  FFUNC_Ok=true;
  if(ROI_Ok && BKG_Ok) {printf("Tutto ok"); canvas->FWindow->BigStart->setEnabled(true);}
 }
}

void MainWindow::IndicatePeaks()
{
if(Peaks_cancel) //deve cancellare i marker
{
 if(Clicked_for_Multi) //cancellare i marker posizionati a mano
 {
  for(int j=0;j<PeaksFound;j++)
  pm_one[j]->Delete();               
  canvas->getCanvas();
  canvas->fCanvas->Update();
  Clicked_for_Multi=false;
 }
 else if(Find_for_Multi) //cancellare i marker posizionati in automatico
 {
  pm->Delete();
  canvas->getCanvas();
  canvas->fCanvas->Update();
  Find_for_Multi=false;
 }
 else if(PTabled_for_Multi)//cancellare i marker (linee) posizionate dalla tavola peiodica
 {
  for(int k=0;k<el_count;k++) //ricerca trai puntatori 
  {markerEn[k]->Delete();  markerEn[k]=NULL;}
  el_count=0;
  PTabled_for_Multi=false;
  canvas->getCanvas();
  canvas->fCanvas->Update();
 }
Peaks_cancel=false;
}
else
{
pm_one[PeaksFound-1] = new TMarker(PeakPositionX[PeaksFound-1], PeakPositionY[PeaksFound-1],23);
canvas->f1->GetListOfFunctions()->Add(pm_one[PeaksFound-1]);
//pm_one[PeaksFound-1]->SetMarkerStyle(23);
pm_one[PeaksFound-1]->SetMarkerColor(kRed);
pm_one[PeaksFound-1]->SetMarkerSize(1.3);
pm_one[PeaksFound-1]->Draw("same");
canvas->getCanvas();
canvas->fCanvas->Update();
QString n,en;
n.setNum(PeaksFound-1);n.append("\n");
en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
canvas->FWindow->textEditP->append(n);
canvas->FWindow->textEditE->append(en);
}
}


void MainWindow::LogStateChanged(int state)
{
  if(state==Qt::Checked) 
    {LogOn=true; single_draw=true; canvas->DoDraw();}
  else
  if(state==Qt::Unchecked)
    {LogOn=false; single_draw=true; canvas->DoDraw();}
}

void MainWindow::GridStateChanged(int state)
{
  if(state==Qt::Checked) 
    {GridOn=true; single_draw=true; canvas->DoDraw();}
  else
  if(state==Qt::Unchecked)
    {GridOn=false; single_draw=true; canvas->DoDraw();}
}
//______________________________________________________________________________
void MainWindow::handle_root_events() //importante sennò tutto il pc va in crash...deve elaborare gli eventi di root!!
{
  
   gSystem->ProcessEvents();
}

//______________________________________________________________________________
void MainWindow::changeEvent(QEvent * e)
{
   if (e->type() == QEvent ::WindowStateChange) {
      QWindowStateChangeEvent * event = static_cast< QWindowStateChangeEvent * >( e );
      if (( event->oldState() & Qt::WindowMaximized ) ||
          ( event->oldState() & Qt::WindowMinimized ) ||
          ( event->oldState() == Qt::WindowNoState && 
            this->windowState() == Qt::WindowMaximized )) {
         if (canvas->getCanvas()) {
            canvas->getCanvas()->Resize();
            canvas->getCanvas()->Update();
         }
      }
   }
}

void MainWindow::OpenTable()
{
system("./Periodic_Table  & ");
*(shm_fit+19)=1; //il programma Periodic_Table è aperto
PTabletimer->start(250);
}

///timer attivo solo se la tavola periodica è aperta (*(shm+19)) 
///se si accorge che è stato cliccato un elemento legge dalla shm le energie delle sue righe e le segna sullo spettro 
void MainWindow::PTtimerEvent()
{
if(!*(shm_fit+19)) //se la tavola è stata chiusa il timer si ferma e cancella i marker rimasti
 {
  PTabletimer->stop(); 
  if(el_count>0)
    for(int k=0;k<el_count;k++) //ricerca trai puntatori se era già stato disegnato
      {
        markerEn[k]->Delete(); 
        markerEn[k]=NULL;
       }
   el_count=0;
   SomeLinesOn=false;
   canvas->getCanvas();
   canvas->fCanvas->Update();  
 }
else
if(*(shm_fit+10)) //se è stato cliccato un nuovo elemento
  {
   SomeLinesOn=true;
   PTabletimer->stop();
   switch (int(*shm_fit))
     {
      case (1):  ///caso 1: l'elemento ha solo Ka e Kb
        {

         if(Calibration_ready)
          {
            if(*(shm_fit+11)==0) //non era cliccato, devo disegnare i marker
            {
             if(el_count==99) el_count=0;

             markerEn[el_count] = new TLine(*(shm_fit+1),0,*(shm_fit+1),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color); 
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;

             markerEn[el_count] = new TLine(*(shm_fit+2),0,*(shm_fit+2),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color); //kGreen
             color++; if(color==10)color++;
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;
             if(PTable_for_Multi && PeaksFound<12) //se si vuole un multi fit lo aggiunge ai picchi scelti
              {
               if(ROI[0]<*(shm_fit+1) && *(shm_fit+1)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+1); 
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+1)));  
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               if(ROI[0]<*(shm_fit+2) && *(shm_fit+2)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+2); 
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+2)));  
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               }
             }
            else  //era già cliccato, devo cancellare i marker
             {
              for(int k=0;k<el_count-1;k++) //ricerca trai puntatori se era già stato disegnato
              {
               if(markerEn[k]->GetX1()==*(shm_fit+1)) 
                  {
                  //qDebug()<<"cancello"<<markerEn[k]->GetX1()<<"e "<<markerEn[k+1]->GetX1();
                   //qDebug()<<"metto in "<<y-2<<markerEn[y]->GetX1();
                   markerEn[k]->Delete(); markerEn[k+1]->Delete(); 
                   if(el_count>k+2) for(int y=k+2;y<el_count;y++){markerEn[y-2]=markerEn[y]; markerEn[y]=NULL;}//se sono più di 2 si shifta indietro
                   el_count=el_count-2;
                   if(el_count==0)SomeLinesOn=false;
                   //qDebug()<<"rimossi "<<k<<k+1;
                  }
               }
               canvas->getCanvas();
               canvas->fCanvas->Update();
              }
           }
         else qDebug()<<"Spettro non calibrato!!\n";
        
         break;
         }
       case (2):  ///caso 2: l'elemento ha ka, Kb e La
        {

         if(Calibration_ready)
          {
            if(*(shm_fit+11)==0) //non era cliccato, devo disegnare i marker
            {
             if(el_count==99) el_count=0;

             markerEn[el_count] = new TLine(*(shm_fit+1),0,*(shm_fit+1),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color); 
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;

             markerEn[el_count] = new TLine(*(shm_fit+2),0,*(shm_fit+2),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color); 
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;

             markerEn[el_count] = new TLine(*(shm_fit+3),0,*(shm_fit+3),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color); //kGreen
             color++; if(color==10)color++;
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;
             if(PTable_for_Multi && PeaksFound<12)
              {
               if(ROI[0]<*(shm_fit+1) && *(shm_fit+1)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+1); 
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+1)));   
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               if(ROI[0]<*(shm_fit+2) && *(shm_fit+2)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+2); 
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+2)));  
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               if(ROI[0]<*(shm_fit+3) && *(shm_fit+3)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+3); 
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+3)));  
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               }
             }
            else  //era già cliccato, devo cancellare i marker
             {
              for(int k=0;k<el_count-1;k++) //ricerca trai puntatori se era già stato disegnato
              {
               if(markerEn[k]->GetX1()==*(shm_fit+1)) 
                  {
                  //qDebug()<<"cancello"<<markerEn[k]->GetX1()<<"e "<<markerEn[k+1]->GetX1();
                   //qDebug()<<"metto in "<<y-2<<markerEn[y]->GetX1();
                   markerEn[k]->Delete(); markerEn[k+1]->Delete(); markerEn[k+2]->Delete();
                   if(el_count>k+3) for(int y=k+3;y<el_count;y++){markerEn[y-3]=markerEn[y]; markerEn[y]=NULL;}//se sono più di 3 si shifta indietro
                   el_count=el_count-3;
                   if(el_count==0)SomeLinesOn=false;
                   //qDebug()<<"rimossi "<<k<<k+1;
                  }
               }
               canvas->getCanvas();
               canvas->fCanvas->Update();
              }
           }
         else qDebug()<<"Spettro non calibrato!!\n";
        
         break;
         }
       case (3):  ///caso 3: l'elemento ha ka, Kb, La e Lb
        {

         if(Calibration_ready)
          {
            if(*(shm_fit+11)==0) //non era cliccato, devo disegnare i marker
            {
             if(el_count==99) el_count=0;

             markerEn[el_count] = new TLine(*(shm_fit+1),0,*(shm_fit+1),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color); 
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;

             markerEn[el_count] = new TLine(*(shm_fit+2),0,*(shm_fit+2),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color);
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;

             markerEn[el_count] = new TLine(*(shm_fit+3),0,*(shm_fit+3),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color); 
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;

             markerEn[el_count] = new TLine(*(shm_fit+4),0,*(shm_fit+4),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color); //kGreen
             color++; if(color==10)color++;
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;
             if(PTable_for_Multi && PeaksFound<12)
              {
               if(ROI[0]<*(shm_fit+1) && *(shm_fit+1)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+1);  
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+1)));  
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               if(ROI[0]<*(shm_fit+2) && *(shm_fit+2)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+2); 
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+2)));  
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               if(ROI[0]<*(shm_fit+3) && *(shm_fit+3)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+3); 
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+3)));  
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               if(ROI[0]<*(shm_fit+4) && *(shm_fit+4)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+4); 
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+4)));  
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               }
             }
            else  //era già cliccato, devo cancellare i marker
             {
              for(int k=0;k<el_count-1;k++) //ricerca trai puntatori se era già stato disegnato
              {
               if(markerEn[k]->GetX1()==*(shm_fit+1)) 
                  {
                  //qDebug()<<"cancello"<<markerEn[k]->GetX1()<<"e "<<markerEn[k+1]->GetX1();
                   //qDebug()<<"metto in "<<y-2<<markerEn[y]->GetX1();
                   markerEn[k]->Delete(); markerEn[k+1]->Delete(); markerEn[k+2]->Delete(); markerEn[k+3]->Delete();
                   if(el_count>k+4) for(int y=k+4;y<el_count;y++){markerEn[y-4]=markerEn[y]; markerEn[y]=NULL;}//se sono più di 4 si shifta indietro
                   el_count=el_count-4;
                   if(el_count==0)SomeLinesOn=false;
                   //qDebug()<<"rimossi "<<k<<k+1;
                  }
               }
               canvas->getCanvas();
               canvas->fCanvas->Update();
              }
           }
         else qDebug()<<"Spettro non calibrato!!\n";
        
         break;
         }
       case (4):  ///caso 4: l'elemento ha ka, Kb, La, Lb, Ma e Mb
        {

         if(Calibration_ready)
          {
            if(*(shm_fit+11)==0) //non era cliccato, devo disegnare i marker
            {
             if(el_count==99) el_count=0;

             markerEn[el_count] = new TLine(*(shm_fit+1),0,*(shm_fit+1),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color); 
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;

             markerEn[el_count] = new TLine(*(shm_fit+2),0,*(shm_fit+2),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color);
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;

             markerEn[el_count] = new TLine(*(shm_fit+3),0,*(shm_fit+3),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color); 
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;

             markerEn[el_count] = new TLine(*(shm_fit+4),0,*(shm_fit+4),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color);
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;

             markerEn[el_count] = new TLine(*(shm_fit+5),0,*(shm_fit+5),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color);
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;

             markerEn[el_count] = new TLine(*(shm_fit+6),0,*(shm_fit+6),Max_spectrum);
             canvas->f1->GetListOfFunctions()->Add(markerEn[el_count]);
             canvas->getCanvas();
             canvas->fCanvas->Update();
             markerEn[el_count]->SetLineColor(color); //kGreen
             color++; if(color==10)color++;
             markerEn[el_count]->SetLineWidth(2);
             markerEn[el_count]->Draw("same");
             el_count++;
             if(PTable_for_Multi && PeaksFound<12)
              {
               if(ROI[0]<*(shm_fit+1) && *(shm_fit+1)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+1); 
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+1)));   
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               if(ROI[0]<*(shm_fit+2) && *(shm_fit+2)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+2); 
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+2)));  
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               if(ROI[0]<*(shm_fit+3) && *(shm_fit+3)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+3);
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+3)));   
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               if(ROI[0]<*(shm_fit+4) && *(shm_fit+4)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+4);  
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+4))); 
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               if(ROI[0]<*(shm_fit+5) && *(shm_fit+5)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+5);
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+5)));   
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               if(ROI[0]<*(shm_fit+6) && *(shm_fit+6)<ROI[1])
                {
                PeakPositionX[PeaksFound] = *(shm_fit+6);  
                PeakPositionY[PeaksFound] = canvas->f1->GetBinContent(canvas->f1->GetXaxis()->FindBin(*(shm_fit+6))); 
                PeaksFound++;
                QString n,en;
                n.setNum(PeaksFound-1);n.append("\n");
                en.setNum(PeakPositionX[PeaksFound-1]); en.append("\n");
                canvas->FWindow->textEditP->append(n);
                canvas->FWindow->textEditE->append(en);
                }
               }

             }
            else  //era già cliccato, devo cancellare i marker
             {
              for(int k=0;k<el_count-1;k++) //ricerca trai puntatori se era già stato disegnato
              {
               if(markerEn[k]->GetX1()==*(shm_fit+1)) 
                  {
                  //qDebug()<<"cancello"<<markerEn[k]->GetX1()<<"e "<<markerEn[k+1]->GetX1();
                   //qDebug()<<"metto in "<<y-2<<markerEn[y]->GetX1();
                   markerEn[k]->Delete(); markerEn[k+1]->Delete(); markerEn[k+2]->Delete(); markerEn[k+3]->Delete();
                   markerEn[k+4]->Delete(); markerEn[k+5]->Delete();
                   if(el_count>k+6) for(int y=k+6;y<el_count;y++){markerEn[y-6]=markerEn[y]; markerEn[y]=NULL;}//se sono più di 6 si shifta indietro
                   el_count=el_count-6;
                   if(el_count==0)SomeLinesOn=false;
                   //qDebug()<<"rimossi "<<k<<k+1;
                  }
               }
               canvas->getCanvas();
               canvas->fCanvas->Update();
              }
           }
         else qDebug()<<"Spettro non calibrato!!\n";
        
         break;
         }

     } //chiude switch
 *(shm_fit+10)=0; //il click è stato elaborato
  PTabletimer->start(250);
 }
}

void MainWindow::openMultiFitWindow()
{
 if(histo_on && Calibration_ready)
  {
   canvas->FWindow->show();
   FitWindowOn=true;
  }
 else infoLabel->setText("A calibrated spectrum is needed before starting Multi fit!");

}

void MainWindow::FP_rimbalzo()
{
FindPeaks();
}


void MainWindow::Fit_All_Peaks()
{
int j=0;
TF1  *totalF;
for(int i=0;i<PeaksFound;i++) //ricerca valli a dx e sx di ogni picco
 {
  printf("Picco numero %d:\n",i);
  j= canvas->f1->GetXaxis()->FindBin(PeakPositionX[i])-20;  //PeakPositionX[i]-20; //questo funziona solo in canali
  printf("inizio da j=%d\n",j);
  while(dataSpectrum[j-1]<dataSpectrum[j] && j>0) j--;// printf("j = %d contenuto %f\n", j,dataSpectrum[j]);}
  Limit_sx[i]=SpectrumX[j]; printf("Limit sx = %f\n", Limit_sx[i]);
  j=canvas->f1->GetXaxis()->FindBin(PeakPositionX[i])+20;   //PeakPositionX[i]+20; //questo funziona solo in canali
  while(dataSpectrum[j+1]<dataSpectrum[j]) j++; // printf("j = %dcontenuto %f\n", j,dataSpectrum[j]);}
  Limit_dx[i]=SpectrumX[j]; printf("Limit dx = %f\n", Limit_dx[i]);
  
 }
 switch(bkg)
  { 
      
    case 0: 
     {
      Double_t Parameters[3*PeaksFound+1];
      Parameters[0]=PeaksFound;
      for(int i=0;i<PeaksFound;i++) //fit singole gauss 
       {
        TF1 *p = new TF1("p","gaus",Limit_sx[i],Limit_dx[i]);
        p->SetParameter(0,PeakPositionY[i]);
        p->SetParLimits(0,PeakPositionY[i]-100,PeakPositionY[i]+100); //il max non lo deve spostare molto
        p->SetParameter(1,PeakPositionX[i]);
  
        canvas->f1->Fit(p,"R0");
        Parameters[3*i+1]= p->GetParameter(0);
        Parameters[3*i+1+1]= p->GetParameter(1);
        Parameters[3*i+2+1]= p->GetParameter(2);

       }
      //for(int y=0;y<(3*PeaksFound);y++) printf(" %f\n",Parameters[y]);

      int N=PeaksFound*3+1;
      //totalF = new TF1("totalF",GaussianF,ROI[0],ROI[1],N); //picchi gaussiani
      totalF = new TF1("totalF",GausLorF,ROI[0],ROI[1],N); //caso generale, picchi gauss o lorentz
      totalF->SetParameters(Parameters);
      for(int s=2;s<PeaksFound;s=s+3) //limiti sulle posizioni dei picchi
      totalF->SetParLimits(s,(Parameters[s]-0.05*Parameters[s]),(Parameters[s]+0.05*Parameters[s]));
      break;
     }

    case 1: 
     {
      Double_t Parameters[3*PeaksFound+2];
      Parameters[0]=PeaksFound;
      Parameters[1]=(dataSpectrum[canvas->f1->GetXaxis()->FindBin(Limit_sx[0])-10]+dataSpectrum[canvas->f1->GetXaxis()->FindBin(Limit_dx[0])+10])/2;
//int(Limit_sx[0]-10)]+dataSpectrum[int(Limit_dx[0]+10)])/2;
      for(int i=0;i<PeaksFound;i++) //fit singole gauss 
       {
        TF1 *p = new TF1("p","gaus",Limit_sx[i],Limit_dx[i]);
        p->SetParameter(0,PeakPositionY[i]);
        p->SetParLimits(0,PeakPositionY[i]-100,PeakPositionY[i]+100); //il picco non lo deve spostare molto
        p->SetParameter(1,PeakPositionX[i]);
  
        canvas->f1->Fit(p,"R0");
        Parameters[3*i+2]= p->GetParameter(0);
        Parameters[3*i+1+2]= p->GetParameter(1);
        Parameters[3*i+2+2]= p->GetParameter(2);

       }
      //for(int y=0;y<(3*PeaksFound);y++) printf(" %f\n",Parameters[y]);

      int N=PeaksFound*3+2;
  //    totalF = new TF1("totalF",GaussianFbkg1,ROI[0],ROI[1],N); //picchi gussiani
      totalF = new TF1("totalF",GausLorFbkg1,ROI[0],ROI[1],N); //caso generale, picchi gauss o lorentz
      totalF->SetParameters(Parameters);
      for(int s=3;s<PeaksFound;s=s+3) //limiti sulle posizioni dei picchi
      totalF->SetParLimits(s,(Parameters[s]-0.05*Parameters[s]),(Parameters[s]+0.05*Parameters[s]));
      break;
     }

    case 2: 
     {
      Double_t Parameters[3*PeaksFound+3];
      Parameters[0]=PeaksFound;
      Parameters[2]=(dataSpectrum[canvas->f1->GetXaxis()->FindBin(int(ROI[1]))]-dataSpectrum[canvas->f1->GetXaxis()->FindBin(int(ROI[0]))])/(SpectrumX[canvas->f1->GetXaxis()->FindBin(int(ROI[1]))]-SpectrumX[canvas->f1->GetXaxis()->FindBin(int(ROI[0]))]);
      Parameters[1]=dataSpectrum[canvas->f1->GetXaxis()->FindBin(int(ROI[1]))]-Parameters[2]*SpectrumX[canvas->f1->GetXaxis()->FindBin(int(ROI[1]))];

      for(int i=0;i<PeaksFound;i++) //fit singole gauss 
       {
        TF1 *p = new TF1("p","gaus",Limit_sx[i],Limit_dx[i]);
        p->SetParameter(0,PeakPositionY[i]);
        p->SetParLimits(0,PeakPositionY[i]-100,PeakPositionY[i]+100); //il picco non lo deve spostare molto
        p->SetParameter(1,PeakPositionX[i]);
  
        canvas->f1->Fit(p,"R0");
        Parameters[3*i+3]= p->GetParameter(0);
        Parameters[3*i+1+3]= p->GetParameter(1);
        Parameters[3*i+2+3]= p->GetParameter(2);

       }
      //for(int y=0;y<(3*PeaksFound);y++) printf(" %f\n",Parameters[y]);

      int N=PeaksFound*3+3;
      //totalF = new TF1("totalF",GaussianFbkg2,ROI[0],ROI[1],N); //picchi solo gaussiani
      totalF = new TF1("totalF",GausLorFbkg2,ROI[0],ROI[1],N); //caso generale, picchi gauss o lorentz
      totalF->SetParameters(Parameters);
      for(int s=4;s<PeaksFound;s=s+3) //limiti sulle posizioni dei picchi
      totalF->SetParLimits(s,(Parameters[s]-0.05*Parameters[s]),(Parameters[s]+0.05*Parameters[s]));
      break;
     }
  }///chiude switch bkg
 canvas->f1->Fit(totalF,"R0");
 canvas->getCanvas();
 totalF->SetNpx(1000);
 totalF->Draw("+same");
 canvas->fCanvas->Update();
 gPad->Update();

///scrittura risultati nel file MultiFit_Results.txt/////
   QFile file("MultiFit_Results.txt"); 
   file.open(QIODevice::WriteOnly | QIODevice::Text);  //ReadWrite);
   QTextStream out(&file);
   out<<"Peak\t\t\t"<<"Sigma\t\t\t"<<"Integral"<<"\n";
   switch(bkg)
  { 
      
    case 0: 
     {
      Double_t Parameters[3*PeaksFound+1];
      totalF->GetParameters(Parameters);
      int c=0;
      for(int s=2;s<(PeaksFound*3);s=s+3)     
      {
       Integrals[c]=totalF->Integral(Limit_sx[c],Limit_dx[c]);
       out<<Parameters[s]<<"\t\t\t"<<Parameters[s+1]<<"\t\t\t"<<Integrals[c]<<'\n';
       //printf("s%d: scritto %f  e  %f\n",s,Parameters[s],Parameters[s+1]);
       c++;
       }
       break;
     }
    case 1: 
     {
      Double_t Parameters[3*PeaksFound+2];
      totalF->GetParameters(Parameters);
      TF1 *bg = new TF1("bg",background_const,ROI[0],ROI[1],1);
      bg->SetParameter(0,Parameters[1]);
      int c=0;
      for(int s=3;s<(PeaksFound*3+1);s=s+3)  
      {   
       Integrals[c]=totalF->Integral(Limit_sx[c],Limit_dx[c]);
       if(SubtractBl) Integrals[c]=Integrals[c]-bg->Integral(Limit_sx[c],Limit_dx[c]);
       out<<Parameters[s]<<"\t\t\t"<<Parameters[s+1]<<"\t\t\t"<<Integrals[c]<<'\n';
       c++;
      }
      break;
     }
    case 2: 
     {
      Double_t Parameters[3*PeaksFound+3];
      totalF->GetParameters(Parameters);
      TF1 *bg = new TF1("bg",background_line,ROI[0],ROI[1],2);
      bg->SetParameter(0,Parameters[1]);
      bg->SetParameter(1,Parameters[2]);
      int c=0;
      for(int s=4;s<(PeaksFound*3+2);s=s+3)     
      {   
       Integrals[c]=totalF->Integral(Limit_sx[c],Limit_dx[c]);
       if(SubtractBl) Integrals[c]=Integrals[c]-bg->Integral(Limit_sx[c],Limit_dx[c]);
       out<<Parameters[s]<<"\t\t\t"<<Parameters[s+1]<<"\t\t"<<Integrals[c]<<'\n';
       c++;
      }
      break;
     }
  }
  file.close();
}

void MainWindow::Clear_MultiFit()
{
single_draw=true;
canvas->DoDraw();
}

void MainWindow::closeEvent(QCloseEvent *event)  ///per chiudere la finestra multifit se è aperta
{
 if(FitWindowOn) canvas->FWindow->close(); 
 event->accept();

}

MainWindow::~MainWindow()
{
   if(*(shm_fit+19)) 
       {int pidPT=*(shm_fit+20);
       sprintf(process, "kill -s TERM %i &", pidPT);
       system(process);printf("...killing periodic table...");}
   else printf("periodic table is already off...");

   char clearShmCmd[30];
   printf("...clearing shared memory segment...");
   sprintf(clearShmCmd, "ipcrm shm %i &", shmid_fit);
   system(clearShmCmd);
   *(shared_memory_cmd+78) = 0;
 
}