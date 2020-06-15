{
   Int_t n = 20;
   Double_t x[n], y[n];
   for (Int_t i=0;i<n;i++) {
      x[i] = i*0.1;
      y[i] = 10*sin(x[i]+0.2);
   }

   // create graph
   TGraph *gr  = new TGraph(n,x,y);
   TCanvas *c1 = new TCanvas("c1","Graph Draw Options",
                             200,10,600,400);

   // draw the graph with axis, continuous line, and put
   // a * at each point
   gr->Draw("AC*");
}
