#include "mainwindow.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <../Shm.h>
using namespace std;


/////////variabili e funzioni definite altrove ma necessarie/////////////////////////
extern bool MapIsOpened, CutBorders;
extern int ChMin, ChMax,Nshift,point,OffsetX, OffsetY,PixelX,PixelY,Pixeldim,i,j;
extern int Xmap[50000],Ymap[50000],MaxX,MaxY,MinX,MinY,Xminvero,Yminvero,MaxX_ori,MinX_ori,MaxY_ori,MinY_ori;
extern double X[50000],Y[50000];
extern double Integral[50000],MaxIntegral;
extern int PassoX, PassoY;
extern int *(shared_memory3);
extern QColor myColor;
struct Pixel_BIG *Pointer;
struct Pixel_BIG *px;
struct Pixel_BIG **PixelsMappa;
extern int MaxOnLineX, MinOneLineX, MaxOnLineY, MinOnLineY;
extern int PixelDimOnline;
extern int Xmaponline[20000], Ymaponline[20000];
extern int numberpoints;

////////////////////////////////////////////////////////////////////////////////////////
int salto, saltoY, saltoX;
bool found;
int dim;
int shiftY=0;
int z=0;

struct Pixel_BIG *Crea_PX(int dim, int x, int y)
{
  
  
  px=(struct Pixel_BIG *)calloc(1,sizeof(struct Pixel_BIG));  //zona di memoria per pixel_BIG
  px->centro[0]=x; 
  px->centro[1]=y; 
  px->dimensione=dim; 

   int contour=((dim*dim));  ///numero di pixel tot
   px->total=(struct punto *)calloc(contour,sizeof(struct punto));  //(dimensione*dimensione) punti
   int k=(int)(dim/2);      
   int i=0,j=0;
   int col=1; ////è la colonna

   for(i=0;i<contour;i++)
    {
       if(i==(col*dim))
        {
         col++;
         j=0;
         k=k-1;
        }

          px->total[i].point_x =px->centro[0]-k;
          px->total[i].point_y =px->centro[1]+j-(int)(dim/2);
          j++;

   // printf("Punto total %d\n",i);
    //printf("Coordinate interne x %d   y %d\n",px->total[i].point_x,px->total[i].point_y);
    }
   
  return px;

}

bool Appartiene(int x,int y, struct Pixel_BIG *px) /////da true se x,y appartiene al pixel 
{
 found=false;
 dim=px->dimensione;
 i=0;
 for(i=0;i<(dim*dim);i++)
  {
   if(x==px->total[i].point_x  && y==px->total[i].point_y)
    {
     found=true;
     break;
     } 
  }
 return found;
}




void MainWindow::Define_Pixels()////per creare quelli che dovranno essere disegnati sulla myImage
{
 //qDebug()<<"DEFINE PIXELS";
 //qDebug()<<"MaxX e MaxY all'inizio"<<MaxX<<MaxY<<"MinX e MinY all'inizio"<<MinX<<MinY<<'\n';

if(PixelsMappa!=NULL){
  free(PixelsMappa);}

PixelsMappa=(struct Pixel_BIG **)calloc(point,sizeof(struct Pixel_BIG *));

if(px!=NULL){
	free(px->total);
        free(px);
  }
 
 int caso=100;
 shiftY=0;
 if (Pointer!=NULL)free(Pointer);
 Pointer=(struct Pixel_BIG *)calloc(point,sizeof(struct Pixel_BIG));

 for(z=0;z<=point;z++)
  {
   if(z==0 || PixelDimOnline==1) caso=1;   ////in questo caso funziona come prima con punti di 1 pixel
   if(Xmaponline[z]>Xmaponline[z-1] && Ymaponline[z]==Ymaponline[z-1] && z>0 && PixelDimOnline>1) caso=2;  ///si e mossa la x aumentando, la y e fissa, 
   if(Ymaponline[z]>Ymaponline[z-1] && Xmaponline[z]==Xmaponline[z-1] && z>0 && PixelDimOnline>1) caso=3;  ///si e mossa la y aumentando, la x e quella di prima,
   if(Xmaponline[z]<Xmaponline[z-1] && Ymaponline[z]==Ymaponline[z-1] && z>0 && PixelDimOnline>1) caso=4;  ///si e mossa la x diminuendo, la y e fissa,
   if(Ymaponline[z]<Ymaponline[z-1] && Xmaponline[z]==Xmaponline[z-1] && z>0 && PixelDimOnline>1) caso=5;  ///si e mossa la y diminuendo, la x e fissa,
   if(Ymaponline[z]>Ymaponline[z-1] && Xmaponline[z]<Xmaponline[z-1] && z>0 && PixelDimOnline>1) caso=6;  ///y e aumentata, x e diminuita
   if(Ymaponline[z]>Ymaponline[z-1] && Xmaponline[z]>Xmaponline[z-1] && z>0 && PixelDimOnline>1) caso=7;  ///y e aumentata e x e aumentata
   if(Ymaponline[z]<Ymaponline[z-1] && Xmaponline[z]>Xmaponline[z-1] && z>0 && PixelDimOnline>1) caso=8;  ///x e aumentata e y diminuita

   switch(caso)
   {
    case(1):
     {
      //qDebug()<<"caso 1\n";
      PixelsMappa[z]=Crea_PX(PixelDimOnline,Xmaponline[z],Ymaponline[z]); 
      Pointer[z]=*PixelsMappa[z];
      break;
     }
    case(2):
     {
      //qDebug()<<"caso 2\n";
      if(Xmaponline[z]-Xmaponline[z-1]>1) 
      PixelsMappa[z]=Crea_PX(PixelDimOnline,(PixelsMappa[z-1]->centro[0]+(Xmaponline[z]-Xmaponline[z-1])*PixelDimOnline),(PixelsMappa[z-1]->centro[1]));
      else
      PixelsMappa[z]=Crea_PX(PixelDimOnline,(PixelsMappa[z-1]->centro[0]+PixelDimOnline),(PixelsMappa[z-1]->centro[1])); 
      Pointer[z]=*PixelsMappa[z];                                                            
      if(Pointer[z].centro[0]>MaxOnLineX) 
      { MaxOnLineX=Pointer[z].centro[0];}//riaggiorna il MaxX  // qDebug()<<"Nuovo MaxX"<<MaxX<<'\n';}
      break;
     }
    case(3):
     {
      //qDebug()<<"caso 3\n";
      if(Ymaponline[z]-Ymaponline[z-1]>1) 
      PixelsMappa[z]=Crea_PX(PixelDimOnline,(PixelsMappa[z-1]->centro[0]),(PixelsMappa[z-1]->centro[1]+(Ymaponline[z]-Ymaponline[z-1])* PixelDimOnline));
      else
      PixelsMappa[z]=Crea_PX(PixelDimOnline,(PixelsMappa[z-1]->centro[0]),(PixelsMappa[z-1]->centro[1] + PixelDimOnline)); 
      Pointer[z]=*PixelsMappa[z];
      if(Pointer[z].centro[1]>MaxOnLineY)
      { MaxOnLineY=Pointer[z].centro[1];}//riaggiorna il MaxY  // qDebug()<<"Nuovo MaxY"<<MaxY<<'\n';}
      break;
     }
    case(4):
     {
      //qDebug()<<"caso 4\n";
      if(Xmaponline[z-1]-Xmaponline[z]>1) 
      PixelsMappa[z]=Crea_PX(PixelDimOnline,(PixelsMappa[z-1]->centro[0]-(Xmaponline[z-1]-Xmaponline[z])*PixelDimOnline),(PixelsMappa[z-1]->centro[1]));
      else
      PixelsMappa[z]=Crea_PX(PixelDimOnline,(PixelsMappa[z-1]->centro[0]-PixelDimOnline),(PixelsMappa[z-1]->centro[1])); 
      Pointer[z]=*PixelsMappa[z];
      break;
     }
    case(5):
     {
      //qDebug()<<"caso 5\n";
      if(Ymaponline[z-1]-Ymaponline[z]>1) 
      PixelsMappa[z]=Crea_PX(PixelDimOnline,(PixelsMappa[z-1]->centro[0]),(PixelsMappa[z-1]->centro[1]-(Ymaponline[z-1]-Ymaponline[z])*PixelDimOnline));
      else
      PixelsMappa[z]=Crea_PX(PixelDimOnline,(PixelsMappa[z-1]->centro[0]),(PixelsMappa[z-1]->centro[1]-PixelDimOnline));
      Pointer[z]=*PixelsMappa[z]; 
      break;
     }
    case(6):
     {
      //qDebug()<<"caso 6\n";
      saltoY=Ymaponline[z]-Ymaponline[z-1];
      salto=Xmaponline[z-1]-Xmaponline[z];
      PixelsMappa[z]=Crea_PX(PixelDimOnline,(PixelsMappa[z-1]->centro[0] - salto*PixelDimOnline),(PixelsMappa[z-1]->centro[1] + saltoY*PixelDimOnline));
      Pointer[z]=*PixelsMappa[z]; 
      if(Pointer[z].centro[1]>MaxOnLineY)
      { MaxOnLineY=Pointer[z].centro[1];}// qDebug()<<"Nuovo MaxY"<<MaxY<<'\n';}
      break;
     }
    case(7):
     {
      //qDebug()<<"caso 7\n";
      salto=Xmaponline[z]-Xmaponline[z-1];
      saltoY=Ymaponline[z]-Ymaponline[z-1];
      PixelsMappa[z]=Crea_PX(PixelDimOnline,(PixelsMappa[z-1]->centro[0] + salto*PixelDimOnline),(PixelsMappa[z-1]->centro[1]+saltoY*PixelDimOnline));
      Pointer[z]=*PixelsMappa[z]; 
      if(Pointer[z].centro[1]>MaxOnLineY)
      { MaxOnLineY=Pointer[z].centro[1];}// qDebug()<<"Nuovo MaxY"<<MaxY<<'\n';}
      if(Pointer[z].centro[0]>MaxOnLineX) 
      { MaxOnLineX=Pointer[z].centro[0];}// qDebug()<<"Nuovo MaxX"<<MaxX<<'\n';}
      break;
     }

    case(8):
     {
      //qDebug()<<"caso 8\n";
      saltoY=Ymaponline[z-1]-Ymaponline[z];
      saltoX=Xmaponline[z]-Xmaponline[z-1];
      PixelsMappa[z]=Crea_PX(PixelDimOnline,(PixelsMappa[z-1]->centro[0] + saltoX*PixelDimOnline),(PixelsMappa[z-1]->centro[1] - saltoY*PixelDimOnline));
      Pointer[z]=*PixelsMappa[z]; 
      if(Pointer[z].centro[1]>MaxOnLineY)
      { MaxOnLineY=Pointer[z].centro[1];}// qDebug()<<"Nuovo MaxY"<<MaxY<<'\n';}
      if(Pointer[z].centro[0]>MaxOnLineX) 
      { MaxOnLineX=Pointer[z].centro[0];}// qDebug()<<"Nuovo MaxX"<<MaxX<<'\n';}
      break;
     }
   }
   
   caso=1000;
  //qDebug()<<"coordinate X Y "<<X[z]<<Y[z]<<"si devono colorare: "<<'\n';
 // for(int c=0;c<(Pixeldim*Pixeldim);c++)
 // qDebug()<<" x"<<Pointer[z].total[c].point_x<<"y"<<Pointer[z].total[c].point_y<<'\n';
 }


}
       

