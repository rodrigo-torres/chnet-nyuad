/// \file
/// \ingroup tutorial_fit
/// \notebook -js
/// Example for fitting signal/background.
/// This example can be executed with:
///
/// ~~~{.cpp}
/// root > .x FittingDemo.C  (using the CINT interpreter)
/// root > .x FittingDemo.C+ (using the native complier via ACLIC)
/// ~~~
///
/// \macro_image
/// \macro_output
/// \macro_code
///
/// \author Rene Brun

#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"

// Quadratic background function
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


void Fit2() {
 //Bevington Exercise by Peter Malzacher, modified by Rene Brun
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


   TCanvas *c1 = new TCanvas("c1","Fitting Demo",10,10,700,500);
//   c1->Divide(2);
//   c1->Divide(2,2);
//   c1->SetFillColor(33);
//   c1->SetFrameFillColor(41);
//   c1->cd(1);
   c1->SetGrid();

   TH1F *histo = new TH1F("histo",
      "Fit Spectrum",4092,0,4092);
   histo->SetMarkerStyle(21);
   histo->SetMarkerSize(0.8);
   histo->SetStats(0);

   for(int i=0; i < bin;  i++) histo->SetBinContent(i+1,dataSpectrum[i]);

//   TF1 *g1 = new TF1("m1","gaus",950,1150);
   // create a TF1 with the range from 0 to 3 and 6 parameters
   TF1 *fitFcn = new TF1("fitFcn",fitFunction,950,1150,6);
   fitFcn->SetNpx(500);
   fitFcn->SetLineWidth(4);
   fitFcn->SetLineColor(kMagenta);

   // first try without starting values for the parameters
   // This defaults to 1 for each param.
   // this results in an ok fit for the polynomial function
   // however the non-linear part (lorenzian) does not
   // respond well.

   fitFcn->SetParameters(1,1,0.01,30000,1050,20);
//   histo->Fit("fitFcn","R");

   // second try: set start values for some parameters
//   fitFcn->SetParameter(4,0.2); // width
//   fitFcn->SetParameter(5,1);   // peak

   histo->Fit("fitFcn","R","ep");

   // improve the picture:
   TF1 *backFcn = new TF1("backFcn",background,950,1150,3);
   backFcn->SetLineColor(kRed);
   TF1 *signalFcn = new TF1("signalFcn",GaussPeak,950,1150,3);
   signalFcn->SetLineColor(kBlue);
   signalFcn->SetNpx(500);
   Double_t par[6];

   // writes the fit results into the par array
   fitFcn->GetParameters(par);

   backFcn->SetParameters(par);
   backFcn->Draw("same");

   signalFcn->SetParameters(&par[3]);
   signalFcn->Draw("same");

//   g1->Draw("same");

   // draw the legend
   TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
   legend->SetTextFont(72);
   legend->SetTextSize(0.04);
   legend->AddEntry(histo,"Data","lpe");
   legend->AddEntry(backFcn,"Background fit","l");
   legend->AddEntry(signalFcn,"Signal fit","l");
   legend->AddEntry(fitFcn,"Global Fit","l");
   legend->Draw();




}
