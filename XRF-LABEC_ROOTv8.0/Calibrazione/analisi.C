
FILE *f0, *f1, *f2;
int i1,i2,i3,i4,i5,i6;
int i=0;

{


TH1F *h0=new TH1F("h0","Ch 0",16384,0,16384);
TH1F *h1=new TH1F("h1","Ch 1",16384,0,16384);
//TH1F *h2=new TH1F("h2","Spettro Ch 2",16384,0,16384);

f0=fopen("Energy.dat","r");
f1=fopen("prova2.txt","r");
//f2=fopen("Energy2.dat","r");

while( ! feof(f0))
{
fscanf(f0,"%d\n",&i1);  
//for(int i=0;i<i2;i++)
h0->Fill(i1,1);
}
fclose(f0);


while( ! feof(f1))
{
fscanf(f1,"%d\n",&i3);  
//for(int i=0;i<i4;i++)
h1->Fill(i3,1);
}
fclose(f1);

/*
while( ! feof(f2))
{
fscanf(f2,"%d %d\n",&i5, &i6);  
for(int i=0;i<i6;i++)
h2->Fill(i5,1);
}
fclose(f2);
*/
TCanvas *c0=new TCanvas("Energie0","Energy0",50,50,800,800);
h0->GetXaxis()->SetTitle("Channel");
h0->GetYaxis()->SetTitle("Counts");
h0->Draw();

TCanvas *c1=new TCanvas("Energie1","Energy1",50,50,800,800);
h1->GetXaxis()->SetTitle("Channel");
h1->GetYaxis()->SetTitle("Counts");
h1->Draw();
/*
TCanvas *c2=new TCanvas("Energie2","Energy2",50,50,800,800);
h2->GetXaxis()->SetTitle("canali");
h2->Draw();
*/
}
