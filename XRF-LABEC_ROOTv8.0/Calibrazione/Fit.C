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


// Lorenzian Peak function
Double_t lorentzianPeak(Double_t *x, Double_t *par) {
  return (0.5*par[0]*par[1]/TMath::Pi()) /
    TMath::Max( 1.e-10,(x[0]-par[2])*(x[0]-par[2])
   + .25*par[1]*par[1]);
}

// Gauss  f(x)= p0*exp(-0.5*((x-p1)/p2)^2)

Double_t GaussPeak(Double_t *x, Double_t *par) {
  return (par[0]*exp(-0.5*(x[0]-par[1]/par[2])*(x[0]-par[1]/par[2])));
}


// Sum of background and peak function
Double_t fitFunction(Double_t *x, Double_t *par) {
  return background(x,par) + lorentzianPeak(x,&par[3]);
}

Double_t fitFunctionGauss(Double_t *x, Double_t *par) {
  return background(x,par) + GaussPeak(x,&par[3]);
}


void Fit() {
 //Bevington Exercise by Peter Malzacher, modified by Rene Brun
   int j; const int bin= 16384;
   const int nBins = 16384;
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



   Double_t data[nBins] = { 6, 1,10,12, 6,13,23,22,15,21,
                           23,26,36,25,27,35,40,44,66,81,
                           75,57,48,45,46,41,35,36,53,32,
                           40,37,38,31,36,44,42,37,32,32,
                           43,44,35,33,33,39,29,41,32,44,
                           26,39,29,35,32,21,21,15,25,15};
   TCanvas *c1 = new TCanvas("c1","Fitting Demo",10,10,700,500);
   c1->Divide(2);
//   c1->Divide(2,2);
//   c1->SetFillColor(33);
//   c1->SetFrameFillColor(41);
   c1->cd(1);
   c1->SetGrid();

   TH1F *histo = new TH1F("histo",
      "Lorentzian Peak on Quadratic Background",60,0,3);
   histo->SetMarkerStyle(21);
   histo->SetMarkerSize(0.8);
   histo->SetStats(0);

   for(int i=0; i < nBins;  i++) histo->SetBinContent(i+1,data[i]);

   // create a TF1 with the range from 0 to 3 and 6 parameters
   TF1 *fitFcn = new TF1("fitFcn",fitFunction,0,3,6);
   fitFcn->SetNpx(500);
   fitFcn->SetLineWidth(4);
   fitFcn->SetLineColor(kMagenta);

   // first try without starting values for the parameters
   // This defaults to 1 for each param.
   // this results in an ok fit for the polynomial function
   // however the non-linear part (lorenzian) does not
   // respond well.
   fitFcn->SetParameters(1,1,1,1,1,1);
   histo->Fit("fitFcn","0");

   // second try: set start values for some parameters
   fitFcn->SetParameter(4,0.2); // width
   fitFcn->SetParameter(5,1);   // peak

   histo->Fit("fitFcn","V+","ep");

   // improve the picture:
   TF1 *backFcn = new TF1("backFcn",background,0,3,3);
   backFcn->SetLineColor(kRed);
   TF1 *signalFcn = new TF1("signalFcn",lorentzianPeak,0,3,3);
   signalFcn->SetLineColor(kBlue);
   signalFcn->SetNpx(500);
   Double_t par[6];

   // writes the fit results into the par array
   fitFcn->GetParameters(par);

   backFcn->SetParameters(par);
   backFcn->Draw("same");

   signalFcn->SetParameters(&par[3]);
   signalFcn->Draw("same");

   // draw the legend
   TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
   legend->SetTextFont(72);
   legend->SetTextSize(0.04);
   legend->AddEntry(histo,"Data","lpe");
   legend->AddEntry(backFcn,"Background fit","l");
   legend->AddEntry(signalFcn,"Signal fit","l");
   legend->AddEntry(fitFcn,"Global Fit","l");
   legend->Draw();

   c1->cd(2);

   TH1F *histo2 = new TH1F("histo2",
      "Prova Spettro",4092,0,4092);
   histo2->SetMarkerStyle(21);
   histo2->SetMarkerSize(0.8);
   histo2->SetStats(0);
   for(int i=0; i < bin;  i++) histo2->SetBinContent(i+1,dataSpectrum[i]);

 
   TF1 *fitFcn2 = new TF1("fitFcnG",fitFunctionGauss,950,1150,6);
//   fitFcn2->SetNpx(500);
   fitFcn2->SetLineWidth(4);
   fitFcn2->SetLineColor(kMagenta);

   // first try without starting values for the parameters
   // This defaults to 1 for each param.
   // this results in an ok fit for the polynomial function
   // however the non-linear part (lorenzian) does not
   // respond well.
   fitFcn2->SetParameters(1,1,1,1,1,1);

   fitFcn2->SetParameter(3,10000);   // peak
   fitFcn2->SetParameter(4,1000); // width
   fitFcn2->SetParameter(5,20);   // peak


   histo2->Fit("fitFcnG","V+","ep");

   TF1 *backFcn2 = new TF1("backFcn2",background,950,1150,3);
   backFcn2->SetLineColor(kRed);
   TF1 *signalFcn2 = new TF1("signalFcn2",GaussPeak,950,1150,3);
   signalFcn2->SetLineColor(kBlue);
   signalFcn2->SetNpx(500);
   Double_t par2[6];

   // writes the fit results into the par array
   fitFcn2->GetParameters(par2);

   backFcn2->SetParameters(par2);
   backFcn2->Draw("same");

   signalFcn2->SetParameters(&par2[3]);
   signalFcn2->Draw("same");



//   histo2->Draw();




}
