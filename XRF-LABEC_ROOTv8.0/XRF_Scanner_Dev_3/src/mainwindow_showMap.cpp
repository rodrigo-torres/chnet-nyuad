// METHODS
//
// 
// MainWindow::displayImage_SHM()
//
//


#include "include/MAXRF/mainwindow.h"
#include "../Header.h"
#include <../Shm.h>
//#include <QtGui>
//using namespace std;


/////////variabili e funzioni definite altrove ma necessarie/////////////////////////
extern bool MapIsOpened, CutBorders;
extern int ChMin, ChMax,Nshift,point,OffsetX, OffsetY,PixelX,PixelY,Pixeldim,i,j;
extern int PassoX, PassoY;
extern bool NewFileLoaded;
extern int Xmap[],Ymap[],MaxX,MaxY,MinX,MinY,Xminvero, Yminvero,Xmaxvero, Ymaxvero,MaxX_ori,MinX_ori,MaxY_ori,MinY_ori;
extern double X[],Y[];
extern double Integral[],MaxIntegral;
extern int *(shared_memory3);
extern int *(shared_memory);
extern int *(shared_memory_cmd);
extern QColor myColor; extern QByteArray MapImage;
extern struct Pixel_BIG *Pointer;  //variabile globale per Pixelsmappa[point]
/////////////////////////////////////////////////////////////////////////////////
extern bool HEADER;
extern int BGColor; extern double MapOffsetValue;



extern double Integral1[], Integral2[], Integral3[];


void MainWindow::displayImage_SHM()
{


  PassoX=*(shared_memory_cmd+60);
  PassoY=*(shared_memory_cmd+61);


double x, y;
QString l;
MaxX=-1;MaxY=-1;MinX=10000000;MinY=10000000;
MaxIntegral=0;
point=0;
 int k=0;
int s=0;
int r=0;
if(MapIsOpened==false)
{
int histopos=0;
MapIsOpened=true;
bool ok=false;
QStringList items;
QString itemLabel;
items << tr("Colors") << tr("Gray Scale");
QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),tr("Using:"), items, 0, false, &ok);
if (ok && !item.isEmpty()) 
{
 itemLabel=item;
 int data_number=0;
 while(*(shared_memory3+histopos)!=-2)  // "-2" is written at the end of the file 
    { //qDebug()<<*(shared_memory3+histopos); sleep(1);
      data_number++;      
      if(k<3)
	{ 
	  X[point]=*(shared_memory3+histopos);
         
	  k++;
	  histopos++;
	  if(X[point]>MaxX) MaxX=X[point];
	  if(X[point]<MinX) MinX=X[point];
      
     
	  Y[point]=*(shared_memory3+histopos);
        
	  k++;
	  histopos++;
	  if(Y[point]>MaxY) MaxY=Y[point];
	  if(Y[point]<MinY) MinY=Y[point];
	  
	  Integral[point]=*(shared_memory3+histopos);
	  k++;
	  histopos++;
	  if(Integral[point]>MaxIntegral) MaxIntegral=Integral[point];

//	  qDebug()<<"X[point]="<<X[point]<<"Y[point]="<<Y[point]<<"Integral="<<Integral[point]<<"\n";
	}
     
      else
	{
	   histopos++;
	}

      if(*(shared_memory3+histopos)==-1)
	{
	  point++;
	  k=0;
	  histopos++;
	  
	}
     //qDebug()<<data_number;
    }

for (s=0; s<point; s++)
	{
		Xmap[s]=(X[s]-MinX)/PassoX;
		Ymap[s]=(Y[s]-MinY)/PassoY;
	}
 

printf("MaxX: %i\tMinX: %i\tMaxY: %i\tMinY: %i\n",MaxX, MinX, MaxY, MinY);



/*

printf("MaxX: %i\tMinX: %i\tMaxY: %i\tMinY: %i\n",*(shared_memory_cmd+51), *(shared_memory_cmd+50),  *(shared_memory_cmd+53), *(shared_memory_cmd+52));


printf("XStep: %i\tYStep: %i\n", *(shared_memory_cmd+60), *(shared_memory_cmd+61));

if(HEADER)

{

     MinX = *(shared_memory_cmd+50); //Xmin
     MaxX = *(shared_memory_cmd+51); //Xmax
     MinY = *(shared_memory_cmd+52); //Ymin
     MaxY = *(shared_memory_cmd+53); //Ymax
//     *(shared_memory_cmd+60); //Xstep
//     *(shared_memory_cmd+61); //Ystep

HEADER=false;
}
*/


Xmaxvero=MaxX; //memorizes the X max in micrometers
 MaxX=(MaxX-MinX)/PassoX;
 Xminvero=MinX;
 MinX=0;
 Ymaxvero=MaxY; //memorizes the Y max in micrometers
 MaxY=(MaxY-MinY)/PassoY;
 Yminvero=MinY;
 MinY=0;


 ////prima di chiamare Define_pixels copia dei max e min veri (della scansione)sennò li cambia///////////
 MaxX_ori=MaxX;
 MinX_ori=MinX;
 MaxY_ori=MaxY;
 MinY_ori=MinY;

//qDebug()<<"MaxX_ori:"<<MaxX_ori<<"MinX_ori:"<<MinX_ori<<"MaxY_ori:"<<MaxY_ori<<"MinY_ori:"<<MinY_ori<<"\n";

// printf("MaxX_ori: %i\tMinX_ori: %i\tMaxY_ori: %i\tMinY_ori: %i\n",MaxX_ori, MinX_ori, MaxY_ori, MinY_ori);


// OffsetX=int((PixelX-(MaxX+MinX))/2);  //per centrare la mappa nella finestra
// OffsetY=int((PixelY-(MaxY+MinY))/2);

// printf("PixelX: %i\t%PixelY: %i\tOffsetX: %i\tOffsetY: %i\n",PixelX, PixelY, OffsetX,OffsetY);


//sleep(10);


 Define_Pixels(); //definisce quelli che dovranno essere colorati in base a Pixeldim...ci si accede con Pointer
                 
 if(Pixeldim>1)
 { 
   MinX=MinX-int(Pixeldim/2);
   MaxX=MaxX+int(Pixeldim/2);
   MinY=MinY-int(Pixeldim/2);
   MaxY=MaxY+int(Pixeldim/2);
 }
 OffsetX=int((PixelX-(MaxX+MinX))/2);  //per centrare la mappa nella finestra
 OffsetY=(int((PixelY-(MaxY+MinY)*MapOffsetValue)/2));

printf("PixelX: %i\t%PixelY: %i\tOffsetX: %i\tOffsetY: %i\n",PixelX, PixelY, OffsetX,OffsetY);


 double frac=0.33333333;
//////////////////////////////////////////////////////////////////////////////////////



 qDebug()<<"MaxX"<<MaxX<<"MinX"<<MinX<<"MaxY"<<MaxY<<"MinY"<<MinY<<'\n';
 qDebug()<<"OFFSET x "<<OffsetX<<" OFFSET y "<<OffsetY<<'\n';
 qDebug()<<"pixel x:"<<PixelX<<"pixel y:"<<PixelY<<"points:"<<point<<"PixelDim:"<<Pixeldim<<'\n';


  MyImage = new QImage(PixelX, PixelY, QImage::Format_RGB32);

if (BGColor==0)  MyImage->fill(QColor(Qt::black).rgb());
if (BGColor==1)  MyImage->fill(QColor(Qt::white).rgb());

  bool I_updated=false;


         for(int current=0; current<point; current++)
            {
                i=Xmap[current]; j=Ymap[current]; 

                if(CutBorders)
                {
                   ///riaggiorna il colore///
                   if(I_updated==false)
                     {
                      MaxIntegral=-1;
                      for(int v=0;v<point;v++)
                      if ((Ymap[v]>MinY_ori+1)&& (Ymap[v]<MaxY_ori-1) && (Xmap[v]>MinX_ori+1) && (Xmap[v]<MaxX_ori-1) && Integral[v]>MaxIntegral) MaxIntegral=Integral[v];
                      I_updated=true;
                     }

                 if ((Ymap[current]>MinY_ori+1)&& (Ymap[current]<MaxY_ori-1)&& (Xmap[current]>MinX_ori+1) && (Xmap[current]<MaxX_ori-1))
                  {
                  double intensity=(Integral[current]/MaxIntegral);

                   if(itemLabel=="Colors")
                     {
                     if(intensity<frac) {intensity=3*intensity*255; myColor.setRgb(0,int(intensity),0,255);}
                     else
                     if(intensity>=frac && intensity<(2*frac)) {intensity=(intensity-frac)*3*255; myColor.setRgb(int(intensity),255,0,255);}  
                     else
                     if(intensity>=(2*frac)) {intensity=(255-(intensity-(2.0*frac))*3*255); myColor.setRgb(255,int(intensity),0,255);}            
                     //qDebug()<<"Metto nel pixel x y"<<(PixelX-i-OffsetX)<<(PixelY-j-OffsetY)<<"La coppia "<<X[current]<<Y[current]<<'\n';
                     } 
                   else
                   if(itemLabel=="Gray Scale") 
                     {
                      intensity=intensity*255;
                      myColor.setRgb(int(intensity),int(intensity),int(intensity),255);
                     }
                   //qDebug()<<"coordinate X Y "<<X[current]<<Y[current]<<'\n';
                  // qDebug()<<"centro "<<Pointer[current].centro[0]<<Pointer[current].centro[1]<<'\n';
                   for(int c=0;c<(Pixeldim*Pixeldim);c++)
                   MyImage->setPixel(PixelX-Pointer[current].total[c].point_x-OffsetX,PixelY-Pointer[current].total[c].point_y-OffsetY, myColor.rgb());
                    
                  }
                 }///chiude if cutborders 

                else
                  {
                    double intensity=(Integral[current]/MaxIntegral);
		
       
                    if(itemLabel=="Colors")
                      {
                       if(intensity<frac) {intensity=3*intensity*255; myColor.setRgb(0,int(intensity),0,255);}
                       else
                       if(intensity>=frac && intensity<(2*frac)) {intensity=(intensity-frac)*3*255; myColor.setRgb(int(intensity),255,0,255);}  
                       else
                       if(intensity>=(2*frac)) {intensity=(255-(intensity-(2.0*frac))*3*255); myColor.setRgb(255,int(intensity),0,255);} 
                      }
                    else
                     if(itemLabel=="Gray Scale") 
                      {
                       intensity=intensity*255;
                       myColor.setRgb(int(intensity),int(intensity),int(intensity),255);
                       }
                  
                   for(int c=0;c<(Pixeldim*Pixeldim);c++)
                   MyImage->setPixel(PixelX-Pointer[current].total[c].point_x-OffsetX,PixelY-Pointer[current].total[c].point_y-OffsetY, myColor.rgb());
                   //qDebug()<<" coloro x"<<PixelX-Pointer[current].total[c].point_x-OffsetX<<"coloro y"<<PixelY-Pointer[current].total[c].point_y-OffsetY<<'\n';}

                   //MyImage->setPixel(PixelX-i-OffsetX,PixelY-j-OffsetY, myColor.rgb()); 
                   //qDebug()<<"Metto nel pixel x y"<<(PixelX-i-OffsetX)<<(PixelY-j-OffsetY)<<"La coppia "<<X[current]<<Y[current]<<'\n';
                   }


            }  //chiude for

           qDebug()<<"loop ended";

          imageLabel->setPixmap(QPixmap::fromImage(*MyImage));
          scaleFactor = 1.0;
          Cursor= new QCursor (QPixmap::fromImage(*MyImage),-1,-1 );
          Cursor->setShape(Qt::PointingHandCursor);
          imageLabel->setCursor(*Cursor);

 
        QBuffer buffer(&MapImage);
        buffer.open(QIODevice::WriteOnly);
        MyImage->save(&buffer, "PNG"); // writes image into QByteArray MapImage in PNG format


 } //chiude if(ok...)
else
hideImage();

 } //chiude if(MapIsOpened)
}


void MainWindow::displaySumImage_SHM() //displays the composed map (sum of three different elements)
{
  PassoX=*(shared_memory_cmd+60);
  PassoY=*(shared_memory_cmd+61);


for (i=0; i<632025; i++){Integral1[i]=0; Integral2[i]=0; Integral3[i]=0;}

double x, y;
QString l;
MaxX=-1;MaxY=-1;MinX=10000000;MinY=10000000;
int MaxIntegral1=0, MaxIntegral2=0, MaxIntegral3=0;
//double Integral1[50000]={0}, Integral2[50000]={0}, Integral3[50000]={0};
point=0;
 int k=0;
int s=0;
int r=0;
if(MapIsOpened==false)
{
int histopos=0;
MapIsOpened=true;
bool ok=false;
QStringList items;
QString itemLabel;
items << tr("Colors") << tr("Gray Scale");
QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),tr("Using:"), items, 0, false, &ok);
if (ok && !item.isEmpty()) 
{
 itemLabel=item;
 int data_number=0;
printf("MaxIntegral1:%d, MaxIntegral2:%d, MaxIntegral3:%d\n", MaxIntegral1, MaxIntegral2, MaxIntegral3);
 while(*(shared_memory3+histopos)!=-2)  // "-2" is written at the end of the file 
    { //qDebug()<<*(shared_memory3+histopos); sleep(1);
      data_number++;      
      if(k<5)
	{ 
	  X[point]=*(shared_memory3+histopos);
		
         
	  k++;
	  histopos++;
	  if(X[point]>MaxX) MaxX=X[point];
	  if(X[point]<MinX) MinX=X[point];
      
     
	  Y[point]=*(shared_memory3+histopos);
        
	  k++;
	  histopos++;
	  if(Y[point]>MaxY) MaxY=Y[point];
	  if(Y[point]<MinY) MinY=Y[point];
	  
	  Integral1[point]=*(shared_memory3+histopos);
	  k++;
	  histopos++;
	  Integral2[point]=*(shared_memory3+histopos);
	  k++;
	  histopos++;
	  Integral3[point]=*(shared_memory3+histopos);
	  k++;
	  histopos++;
	  if(Integral1[point]>MaxIntegral1) {MaxIntegral1=Integral1[point];}//printf("Integral1:%f\n", Integral1[point]);
	  if(Integral2[point]>MaxIntegral2) {MaxIntegral2=Integral2[point];}//printf("Integral2:%f\n", Integral2[point]);
	  if(Integral3[point]>MaxIntegral3) {MaxIntegral3=Integral3[point];}//printf("Integral3:%f\n", Integral3[point]);

//	  qDebug()<<"X[point]="<<X[point]<<"Y[point]="<<Y[point]<<"Integral="<<Integral[point]<<"\n";
	}
     
      else
	{
	   histopos++;
	}

      if(*(shared_memory3+histopos)==-1)
	{
	  point++;
	  k=0;
	  histopos++;
	
	  
	}
     //qDebug()<<data_number;
    }
printf("MaxIntegral1:%d, MaxIntegral2:%d, MaxIntegral3:%d\n", MaxIntegral1, MaxIntegral2, MaxIntegral3);
for (s=0; s<point; s++)
	{
		Xmap[s]=(X[s]-MinX)/PassoX;
		Ymap[s]=(Y[s]-MinY)/PassoY;
	}
 Xmaxvero=MaxX; //memorizes the X max in micrometers
 MaxX=(MaxX-MinX)/PassoX;
 Xminvero=MinX;
 MinX=0;
 Ymaxvero=MaxY; //memorizes the Y max in micrometers
 MaxY=(MaxY-MinY)/PassoY;
 Yminvero=MinY;
 MinY=0;

 




 ////prima di chiamare Define_pixels copia dei max e min veri (della scansione)sennò li cambia///////////
 MaxX_ori=MaxX;
 MinX_ori=MinX;
 MaxY_ori=MaxY;
 MinY_ori=MinY;

qDebug()<<"MaxX_ori:"<<MaxX_ori<<"MinX_ori:"<<MinX_ori<<"MaxY_ori:"<<MaxY_ori<<"MinY_ori:"<<MinY_ori<<"\n";

 Define_Pixels(); //definisce quelli che dovranno essere colorati in base a Pixeldim...ci si accede con Pointer
                 
 if(Pixeldim>1)
 { 
   MinX=MinX-int(Pixeldim/2);
   MaxX=MaxX+int(Pixeldim/2);
   MinY=MinY-int(Pixeldim/2);
   MaxY=MaxY+int(Pixeldim/2);
 }
 OffsetX=int((PixelX-(MaxX+MinX))/2);  //per centrare la mappa nella finestra
 OffsetY=int((PixelY-(MaxY+MinY)*MapOffsetValue)/2);


 double frac=0.33333333;
//////////////////////////////////////////////////////////////////////////////////////



 qDebug()<<"MaxX"<<MaxX<<"MinX"<<MinX<<"MaxY"<<MaxY<<"MinY"<<MinY<<'\n';
 qDebug()<<"OFFSET x "<<OffsetX<<" OFFSET y "<<OffsetY<<'\n';
 qDebug()<<"pixel x:"<<PixelX<<"pixel y:"<<PixelY<<"points:"<<point<<"PixelDim:"<<Pixeldim<<'\n';


  MyImage = new QImage(PixelX, PixelY, QImage::Format_RGB32);
//  MyImage->fill(QColor(Qt::black).rgb());

if (BGColor==0)  MyImage->fill(QColor(Qt::black).rgb());
if (BGColor==1)  MyImage->fill(QColor(Qt::white).rgb());


  bool I_updated=false;


         for(int current=0; current<point; current++)
            {
                i=Xmap[current]; j=Ymap[current]; 

                if(CutBorders)
                {
                   ///riaggiorna il colore///
                   if(I_updated==false)
                     {
                      MaxIntegral=-1;
                      for(int v=0;v<point;v++)
                      if ((Ymap[v]>MinY_ori+1)&& (Ymap[v]<MaxY_ori-1) && (Xmap[v]>MinX_ori+1) && (Xmap[v]<MaxX_ori-1) && Integral[v]>MaxIntegral) MaxIntegral=Integral[v];
                      I_updated=true;
                     }

                 if ((Ymap[current]>MinY_ori+1)&& (Ymap[current]<MaxY_ori-1)&& (Xmap[current]>MinX_ori+1) && (Xmap[current]<MaxX_ori-1))
                  {
                    double intensity1=(Integral1[current]/MaxIntegral1)*255;
		    double intensity2=(Integral2[current]/MaxIntegral2)*255;
		    double intensity3=(Integral3[current]/MaxIntegral3)*255;

                   if(itemLabel=="Colors")
                     {
                     myColor.setRgb(int(intensity1),int(intensity2),int(intensity3),255);        
                     //qDebug()<<"Metto nel pixel x y"<<(PixelX-i-OffsetX)<<(PixelY-j-OffsetY)<<"La coppia "<<X[current]<<Y[current]<<'\n';
                     } 
                   else
                   if(itemLabel=="Gray Scale") 
                     {
                      
                      myColor.setRgb(0,0,0,255);
                     }
                   //qDebug()<<"coordinate X Y "<<X[current]<<Y[current]<<'\n';
                  // qDebug()<<"centro "<<Pointer[current].centro[0]<<Pointer[current].centro[1]<<'\n';
                   for(int c=0;c<(Pixeldim*Pixeldim);c++)
                   MyImage->setPixel(PixelX-Pointer[current].total[c].point_x-OffsetX,PixelY-Pointer[current].total[c].point_y-OffsetY, myColor.rgb());
                    
                  }
                 }///chiude if cutborders 

                else
                  {
                    double intensity1=(Integral1[current]/MaxIntegral1)*255;if(MaxIntegral1==0)intensity1=0; 
		    double intensity2=(Integral2[current]/MaxIntegral2)*255;if(MaxIntegral2==0)intensity2=0;
		    double intensity3=(Integral3[current]/MaxIntegral3)*255;if(MaxIntegral3==0)intensity3=0;
       
                    if(itemLabel=="Colors")
                      {
                       myColor.setRgb(int(intensity1),int(intensity2),int(intensity3),255);
                       
                      }
                    else
                     if(itemLabel=="Gray Scale") 
                      {
                       //intensity=intensity*255;
                       myColor.setRgb(0,0,0,255);
                       }
                  
                   for(int c=0;c<(Pixeldim*Pixeldim);c++)
                   MyImage->setPixel(PixelX-Pointer[current].total[c].point_x-OffsetX,PixelY-Pointer[current].total[c].point_y-OffsetY, myColor.rgb());
                   //qDebug()<<" coloro x"<<PixelX-Pointer[current].total[c].point_x-OffsetX<<"coloro y"<<PixelY-Pointer[current].total[c].point_y-OffsetY<<'\n';}

                   //MyImage->setPixel(PixelX-i-OffsetX,PixelY-j-OffsetY, myColor.rgb()); 
                   //qDebug()<<"Metto nel pixel x y"<<(PixelX-i-OffsetX)<<(PixelY-j-OffsetY)<<"La coppia "<<X[current]<<Y[current]<<'\n';
                   }


            }  //chiude for

           qDebug()<<"loop ended";

          imageLabel->setPixmap(QPixmap::fromImage(*MyImage));
          scaleFactor = 1.0;
          Cursor= new QCursor (QPixmap::fromImage(*MyImage),-1,-1 );
          Cursor->setShape(Qt::PointingHandCursor);
          imageLabel->setCursor(*Cursor);

 
        QBuffer buffer(&MapImage);
        buffer.open(QIODevice::WriteOnly);
        MyImage->save(&buffer, "PNG"); // writes image into QByteArray MapImage in PNG format


 } //chiude if(ok...)
else
hideImage();

 } //chiude if(MapIsOpened)


}


