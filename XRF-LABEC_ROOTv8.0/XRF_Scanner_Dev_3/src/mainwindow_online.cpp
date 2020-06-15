#include "include/MAXRF/mainwindow.h"

#include <QtGui>
using namespace std;



/////////variabili e funzioni definite altrove ma necessarie/////////////////////////
extern bool OnLine,MapIsOpened;
extern int OffsetX, OffsetY,PixelX,PixelY,Pixeldim,point,StoredPoint,IntR,IntG,IntB;
extern int *(shared_memory), *(shared_memory_cmd);
extern int i,j;
extern double X[20000],Y[20000];
extern double Integral[20000],MaxIntegral,MinIntegral;
extern QColor myColor;
///////////////////////////////////////////////////////////////////////////////////////


void MainWindow::GoOnLine()
{
 if(*(shared_memory_cmd+75)==1)
 {
//  OnLine=false;
  *(shared_memory_cmd+75)=0;
  StoredPoint=0; 
  hideImage();
 }
 else 
 {
// OnLine=true;
 MapIsOpened=true;
 qDebug()<<"OnLine Active";
 *(shared_memory_cmd+75)=1; 
 system("./OnLineMap & ");

/*
 double MaxOnLineX=(*(shared_memory_cmd+51)-*(shared_memory_cmd+50))/(*(shared_memory_cmd+60));
 double MinOnLineX=0;
 double MaxOnLineY=(*(shared_memory_cmd+53)-*(shared_memory_cmd+52))/(*(shared_memory_cmd+61));
 double MinOnLineY=0;
 OffsetX=int((PixelX-(MaxOnLineX-MinOnLineX)*Pixeldim)/2);  //per centrare la mappa online  nella finestra
 OffsetY=int((PixelY-(MaxOnLineY-MinOnLineY)*Pixeldim)/2);
*/

 }
}





/*

void MainWindow::displayImageOnLine()
{
if(OnLine==true)
{
  OnLine==false;

  point=StoredPoint;

  if (*(shared_memory+13)==1) //News
  {  
    
      X[point]=(*(shared_memory_cmd+40)-(*(shared_memory_cmd+50)))/(*(shared_memory_cmd+60)); //qDebug()<<*(shared_memory_cmd+40)<<*(shared_memory_cmd+41);
      Y[point]=(*(shared_memory_cmd+41)-(*(shared_memory_cmd+52)))/(*(shared_memory_cmd+61));
      
      Integral[point]=*(shared_memory_cmd+43); 
      qDebug()<<"X"<<*(shared_memory_cmd+40)<<"Y"<<*(shared_memory_cmd+41)<<"Integrale" <<Integral[point];
      if(Integral[point]<=0 && point>0) Integral[point]=Integral[point-1];   ////correzione in caso di integrale mancante

      if (Integral[point]>MaxIntegral) {MaxIntegral=Integral[point];   qDebug()<<"New MaxI "<<MaxIntegral<<"MinI "<<MinIntegral<<'\n';}
      if(Integral[point]<MinIntegral) {MinIntegral=Integral[point];  qDebug()<<"MaxI "<<MaxIntegral<<"New MinI "<<MinIntegral<<'\n';}
                 
      if(MinIntegral==MaxIntegral){MinIntegral=Integral[point]-5;MaxIntegral=Integral[point]+5; qDebug()<<"New MaxI "<<MaxIntegral<<"New MinI "<<MinIntegral<<'\n';}

      point++;
      StoredPoint=point;
      *(shared_memory+13)=0;

  MyImage = new QImage(PixelX, PixelY, QImage::Format_RGB32);
  MyImage->fill(QColor(Qt::black).rgb());


{
         for(int current=0; current<point; current++)
            {
             i=X[current]; j=Y[current];              

                double rel_intensity= ((Integral[current]-MinIntegral+5)*(765/(MaxIntegral-MinIntegral)));


               if(int(rel_intensity)<=255)
		{IntG=qRound(rel_intensity);IntR=0;}
               if(rel_intensity> 255 && rel_intensity<=510)
		{IntG=255;IntR=qRound(rel_intensity-255);}
               if(rel_intensity> 510 && rel_intensity<=765)
		{IntG=qRound(765-rel_intensity);IntR=255;}
               
                myColor.setRgb(IntR,IntG,0,255);
//		qDebug()<<"int(intensity)"<< rel_intensity <<"IntG "<<IntG<<"IntR "<<IntR;


////////////////se si vogliono usare 2 soli colori (da giallo a rosso)////////////////////////////////////
//                double intensity= 255 - (Integral[current]-MinIntegral)*(255/(MaxIntegral-MinIntegral));
//                myColor.setRgb(255,int(intensity),0,255); //qDebug()<<"intensity "<<int(intensity)<<point;


                MyImage->setPixel(PixelX-i-OffsetX,PixelY-j-OffsetY, myColor.rgb()); 
                  //qDebug()<<"Metto nel pixel x y"<<(PixelX-i-OffsetX)<<(PixelY-j-OffsetY)<<"La coppia "<<X[current]<<Y[current]<<'\n';

             }

    imageLabel->setPixmap(QPixmap::fromImage(*MyImage));
    scaleFactor = 1.0;
      }//chiude if point
    }///chiude if news
}//chiude if online
OnLine==true;
}


*/      ////////////////////////////////// RIMOSSO DA SPOSTARE NELLAONLINE MAP


