#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"



void keyence()
{

gROOT->Reset(); 


FILE *f0;
int i1;
int i=0;
int n = 100;
double x[200];
double y[200];
double ex[200];
double ey[200];


f0=fopen("telemetro","r");

while( ! feof(f0))
{
fscanf(f0,"%d\n",&i1);  
x[i]=-14.5+0.5*i;
y[i]=i1;
//ex[i]=0.005;
ey[i]=16;
i++;
}
fclose(f0);

n=i;

TGaxis::SetMaxDigits(3);

TGraphErrors *gr1 = new TGraphErrors(n, x, y, 0, ey);
//TGraph *gr1 = new TGraph (n, x, y);
gr1->SetTitle("Telemeter: linearity");
gStyle->SetOptStat(1);
gr1->Fit("pol1"); gStyle->SetOptFit(1111);

TCanvas *c0=new TCanvas("1","Calibration",50,50,800,800);
c0->SetGrid();
gr1->GetXaxis()->SetTitle("mm");
gr1->GetYaxis()->SetTitle("ADS1115 - Counts");

//gr1->GetYaxis()->SetLabelOffset(10);
gr1->Draw("A*");
gr1->Clear();
/*
TCanvas *c2=new TCanvas("Energie2","Energy2",50,50,800,800);
h2->GetXaxis()->SetTitle("canali");
h2->Draw();
*/
}
