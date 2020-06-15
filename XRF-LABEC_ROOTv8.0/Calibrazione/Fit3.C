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
  return (par[0]*exp(-0.5*(x[0]-par[1]/par[2])*(x[0]-par[1]/par[2])));
}

Double_t fitFunction(Double_t *x, Double_t *par) {
  return background(x,par) + GaussPeak(x,&par[3]);
}


void Fit3() {
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
   c1->SetGrid();

   TH1F *histo = new TH1F("histo",
   "Fit",4092,0,4092);
   histo->SetMarkerStyle(21);
   histo->SetMarkerSize(0.8);
   histo->SetStats(0);

   for(int i=0; i < bin;  i++) histo->SetBinContent(i+1,dataSpectrum[i]);

   TF1 *g1 = new TF1("m1","gaus",950,1150);
   histo->Fit(g1,"R");
//   histo->Fit("m1","V+","ep");
//   g1->Draw("same");

   // draw the legend
   TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
   legend->SetTextFont(72);
   legend->SetTextSize(0.04);
   legend->AddEntry(histo,"Data","lpe");
//   legend->AddEntry(backFcn,"Background fit","l");
//   legend->AddEntry(signalFcn,"Signal fit","l");
//   legend->AddEntry(fitFcn,"Global Fit","l");
   legend->Draw();




}
