


// Fitting function with gaussian signal and quadratic background

Double_t figaus(Double_t *x, Double_t *par){
  float xx=x[0];
  float bkg = par[0]+par[1]*xx+par[2]*xx*xx;
  float arg = -0.5*(xx-par[4])*(xx-par[4])/(par[5]/2.35)/(par[5]/2.35);
  float gau = par[3]*exp(arg);
  float val = bkg+gau;
  return val;
}

// main program

void FitUserDefined(){
  gROOT->Reset();

  // define the fitting functions
  TF1 *user=new TF1("user",figaus,0.,0.,6);
  user->SetLineColor(2);
  user->SetParName(0,"P0");
  user->SetParName(1,"P1");
  user->SetParName(2,"P2");
  user->SetParName(3,"peak height");
  user->SetParName(4,"peak position");
  user->SetParName(5,"FWHM");
  user->SetParameters(0.,0.,0.,0.,0.,-1.);

  // prepare an histogram
   c1 = new TCanvas("c1","test",10,10,700,500);
   c1->SetFillColor(42);
   c1->SetFrameFillColor(33);

   if (!gROOT->FindObject("hMaestro")){
       TH1F *hMaestro=new TH1F("hMaestro","hMaestro",100,0.,20.);
       hMaestro->SetFillColor(3);
       for (int i=0;i<1000;i++) {
	 float x=gRandom->Gaus()+10.;
	 hMaestro->Fill(x);
       }
       for (int i=0;i<5000;i++) {
	 float x=gRandom->Gaus(2.,10.);
	 hMaestro->Fill(x);
       }
   }

   // plot and open the fit panel
   hMaestro->Draw();
   gPad->Update();

   // menu (test di esistenza suggerito da F.Ferri)
   if (!gROOT->FindObject("fit control panel")) { 
     TControlBar *bar = new TControlBar("vertical", "fit control panel");
     gROOT->Add(bar);
     bar->AddButton("Reset Parameters","{ResetParam();}","Reset parameters");
     bar->AddButton("Show Parameters","{ShowParam();}","Show parameters");
     bar->AddButton("Fit","{hMaestro->Fit(\"user\");}","execute fitting");
     bar->Show();
   }

   //    //Add a TExec object to the canvas
   c1->AddExec("mouse test","FitSelected()");
}

void FitSelected()
{
  //example of macro called when a pad is redrawn
  //one must create a TExec object in the following way
  // TExec ex("ex",".x exec1.C");
  // ex.Draw();
  // this macro prints the bin number and the bin content when one clicks
  //on the histogram contour of any histogram in a pad

  TF1 *ff = gROOT->FindObject("user");
  float x0,y0;

  int  event = gPad->GetEvent();
  //  cout << event << endl;
  if (event != kButton1Up) return;

  int px = gPad->GetEventX();
  TObject *select = gPad->GetSelected();
  if (!select) return;
  if (select->InheritsFrom("TH1")) {
    TH1 *h = (TH1*)select;
    Float_t xx = gPad->AbsPixeltoX(px);
    Float_t x  = gPad->PadtoX(xx);
    Int_t binx = h->GetXaxis()->FindBin(x);

    x0=h->GetBinCenter(binx);
    y0=h->GetBinContent(binx);

    if (ff->GetParameter(0)==0) {
      ff->SetParameter(0,y0);
      //	ff->SetRange(x0,gPad->GetUxmax());
      //	cout << "set P0 to " << ff->GetParameter(0) << endl;
      ShowParam();
    }
    else if (ff->GetParameter(3)==0) {
      ff->SetParameter(3,y0);
      ff->SetParameter(4,x0);
      ShowParam();
      //	printf("Set peak(x,y) to (%f,%f)\n",x0,y0);
    }
    else if (ff->GetParameter(5)==-1) {
      ff->SetParameter(5,fabs(x0-ff->GetParameter(4)));
      ShowParam();
      //	printf("Set sigma to %f\n",ff->GetParameter(5));
    }
  }
  return;
}

void ShowParam(){
  TF1 *ff=gROOT->FindObject("user");
  printf("*********\n"); 
  for (int i=0;i<ff->GetNumberFreeParameters();i++) 
    printf("%20s %7f \n",user->GetParName(i),user->GetParameter(i));
  return;
}

void ResetParam(){
  TF1 *ff=gROOT->FindObject("user");
  ff->SetParameters(0.,0.,0.,0.,0.,-1.);
  ShowParam();
  return;
}



