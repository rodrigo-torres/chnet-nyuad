#include "mainwindow.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using namespace std;


#include "../Header.h"
#include "mainwindow.h"
#include "../variables.h"
#include "../QT_variables.h"
#include <../Shm.h>


int MapXSize, MapYSize;
int Map_Contrast=10;
int Map_LoVal=0;
int Map_HiVal=1000;


extern int shmid; extern int *shm; extern int *shmCommand;
extern int shmidString; extern key_t key; extern key_t keyString;
extern int *shared_memory;

extern int shmid_cmd; extern int *shm_cmd; extern int *shmCommand_cmd;
extern int shmidString_cmd; extern key_t key_cmd; extern key_t keyString_cmd;
extern int *shared_memory_cmds;



extern int Resolution_mode;

extern struct Pixel_BIG *Pointer; 

/////////variabili e funzioni definite altrove ma necessarie/////////////////////////
extern int IntR,IntG,IntB; extern int PixelX, PixelY; int PixelX1, PixelY1, PixelX2, PixelY2, PixelX3, PixelY3;
int i,j;
extern double X[],Y[];
int Xmaponline[20000],Ymaponline[20000];
extern double Integral[];
QColor myColor1;
QColor myColor2;
QColor myColor3;
QColor myColor4;
///////////////////////////////////////////////////////////////////////////////////////

int Map1=0, Map2=0, Map3=0, Map4=0;

int s=0;
int datamemory=0; //number of data in memory BIG
int dataread=0; //the number read from the shared memory
int datamemoryold=1; //the last number of data placed in memory from the last cycle
int caso=0;
int integralmap1[20000]={0}, integralmap2[20000]={0}, integralmap3[20000]={0}, integralmap4[20000]={0}; //integral value between the 4 energy intervals for the 4 different maps
int chlowmap1=0, chhighmap1=0, chlowmap2=0, chhighmap2=0, chlowmap3=0, chhighmap3=0, chlowmap4=0, chhighmap4=0;
int numberpoints=-1;
int OffsetX=0, OffsetY=0;
int Xread[20000]={0}, Yread[20000]={0};
int n=0;
int currentnew=0;
int MaxOnLineX=0, MaxOnLineY=0, MinOnLineX=0, MinOnLineY=0;
int PixelDimOnline=1;
int point;
int MaxIntegralMap1=0, MaxIntegralMap2=0, MaxIntegralMap3=0, MaxIntegralMap4=0;
bool IniOnline=true;
int c=0;
int current=0;
double intensitymap1;
double intensitymap2;
double intensitymap3;
double intensitymap4;

int ChMinBa=0, ChMaxBa=0, ChMinCa=0, ChMaxCa=0, ChMinK=0, ChMaxK=0, ChMinCo=0, ChMaxCo=0, ChMinAg=0, ChMaxAg=0, ChMinCr=0, ChMaxCr=0, ChMinCu=0, ChMaxCu=0, ChMinPbL=0, ChMaxPbL=0, ChMinAu=0, ChMaxAu=0, ChMinHg=0, ChMaxHg=0, ChMinSi=0, ChMaxSi=0, ChMinTi=0, ChMaxTi=0, ChMinSn=0, ChMaxSn=0, ChMinFe=0, ChMaxFe=0, ChMinZn=0, ChMaxZn=0, ChMinPbM=0, ChMaxPbM=0;


MainWindow::MainWindow( QWidget *parent ):
    QMainWindow( parent )
{

   key_cmd = 6900;
   shmid_cmd = shmget (key_cmd, SHMSZ_CMD_STATUS, IPC_CREAT | 0666);
   shared_memory_cmd =(int *) shmat(shmid_cmd, NULL, 0);
   cout<< "Data Memory attached at :" << (int *)shared_memory_cmd<< endl << " with shmid: " << shmid_cmd << endl;

   key = 7000;
   shmid = shmget (key, SHMSZ, IPC_CREAT | 0666);
   shared_memory =(int *) shmat(shmid, NULL, 0);
   cout<< "Data Memory attached at :" << (int *)shared_memory<< endl << " with shmid: " << shmid << endl;

   key2 = 7200;
   shmid2 = shmget (key2, SHMSZBIG, IPC_CREAT | 0666);
   shared_memory2 =(int *) shmat(shmid2, NULL, 0);
   cout << "Data Memory attached at :" << (int *)shared_memory2<< endl << " with shmid2: " << shmid2 << endl;

   int pid=getpid();
   *(shared_memory_cmd+85)=pid;
   *(shared_memory_cmd+75)=1;

qDebug()<<Resolution_mode;

/////////Setting Dimensions
      switch (Resolution_mode)
       {
        case(0): {PixelX=395; PixelY=395;
                  PixelX1=395; PixelY1=395;               
                  PixelX2=395; PixelY2=395;
                  PixelX3=395; PixelY3=395;
        break;}
        case(1): {PixelX=295; PixelY=295; break;}
        case(2): {PixelX=195; PixelY=195; break;}
       }

       GUI_CREATOR();

    //*(shared_memory_cmd+51)=110; *(shared_memory_cmd+50)=90; *(shared_memory_cmd+60)=1;
    //*(shared_memory_cmd+53)=110; *(shared_memory_cmd+52)=90; *(shared_memory_cmd+61)=1;

    MaxOnLineX=int((*(shared_memory_cmd+51)-*(shared_memory_cmd+50))/(*(shared_memory_cmd+60)));
    MinOnLineX=0;
    MaxOnLineY=int((*(shared_memory_cmd+53)-*(shared_memory_cmd+52))/(*(shared_memory_cmd+61)));
    MinOnLineY=0;
  
    point=(MaxOnLineX+1)*(MaxOnLineY+1);
	//printf("point:%d\n", point);
	
    //MapXSize=10*MaxOnLineX;
    //MapYSize=10*MaxOnLineY;

    if(MaxOnLineX>66 || MaxOnLineY>66) {PixelDimOnline=1;}
    else
    if((MaxOnLineX>40 && MaxOnLineX<66) || (MaxOnLineY>40 && MaxOnLineY<66)) {PixelDimOnline=3;}
    else 
    if((MaxOnLineX>28 && MaxOnLineX<40) || (MaxOnLineY>28 && MaxOnLineY<40)) {PixelDimOnline=5;}
    else
    if(MaxOnLineX<28 && MaxOnLineY<28){PixelDimOnline=7;}

	
//PixelDimOnline=3;
   /* if(PixelDimOnline>1)
 	{ 
   		
   		MaxOnLineX=MaxOnLineX*PixelDimOnline;
   		MaxOnLineY=MaxOnLineY*PixelDimOnline;
 	}*/


	
	OffsetX=int((PixelX-((MaxOnLineX*PixelDimOnline)+MinOnLineX))/2);  //per centrare la mappa nella finestra
 	OffsetY=int((PixelY-((MaxOnLineY*PixelDimOnline)+MinOnLineY))/2);

    	qDebug()<<"OffsetX: "<<OffsetX<<" OffsetY "<<OffsetY<<"\n";

int Xindex=0, Yindex=0;
printf("MaxOnlineX:%d, MaxOnLineY:%d\n", MaxOnLineX, MaxOnLineY);
j=0;
printf("point:%d\n", point);
for(i=0;i<point;i++){
              
	if(Yindex%2==0){
		Xmaponline[i]=Xindex;
		Ymaponline[i]=Yindex;
		//printf("Xindex:%d, Yindex:%d, i:%d\n", Xmaponline[i], Ymaponline[i], i);
		Xindex++;
		if(Xindex==MaxOnLineX+1){Yindex++;}}
	else	
	if(Yindex%2!=0){
		Xindex--;
		Xmaponline[i]=Xindex;
		Ymaponline[i]=Yindex;	
		//printf("Xindex:%d, Yindex:%d, i:%d\n", Xmaponline[i], Ymaponline[i],i);	
		if(Xindex==0){Yindex++;}}	

}

		//printf("Xread:%d, Yread:%d, Xmaponline:%d, Ymaponline:%d, current:%d\n", Xread[current], Yread[current], Xmaponline[current], Ymaponline[current], current);

		Define_Pixels();



   timerOnline = new QTimer(this);
   connect(timerOnline, SIGNAL(timeout()), this, SLOT(displayImageOnLine())); //è quello dello spettro online. Controlla ProvaRun
    //timerRefresh->start(500);

char element[3];
FILE *filech; //name of the file where the channel intervals are specified
filech = fopen ("../channel_intervals", "r");
fscanf(filech, "%s %d %d", element, &ChMinBa, &ChMaxBa);
fscanf(filech, "%s %d %d", element, &ChMinCa, &ChMaxCa);
fscanf(filech, "%s %d %d", element, &ChMinK, &ChMaxK);
fscanf(filech, "%s %d %d", element, &ChMinCo, &ChMaxCo);
fscanf(filech, "%s %d %d", element, &ChMinAg, &ChMaxAg);
fscanf(filech, "%s %d %d", element, &ChMinCr, &ChMaxCr);
fscanf(filech, "%s %d %d", element, &ChMinCu, &ChMaxCu);
fscanf(filech, "%s %d %d", element, &ChMinPbL, &ChMaxPbL);
fscanf(filech, "%s %d %d", element, &ChMinAu, &ChMaxAu);
fscanf(filech, "%s %d %d", element, &ChMinHg, &ChMaxHg);
fscanf(filech, "%s %d %d", element, &ChMinSi, &ChMaxSi);
fscanf(filech, "%s %d %d", element, &ChMinTi, &ChMaxTi);
fscanf(filech, "%s %d %d", element, &ChMinSn, &ChMaxSn);
fscanf(filech, "%s %d %d", element, &ChMinFe, &ChMaxFe);
fscanf(filech, "%s %d %d", element, &ChMinZn, &ChMaxZn);
fscanf(filech, "%s %d %d", element, &ChMinPbM, &ChMaxPbM);
fclose(filech);

//printf("ChMinBa:%d, ChMaxBa:%d, ChMinCa:%d, ChMaxCa:%d\n",ChMinBa, ChMaxBa, ChMinCa, ChMaxCa);

}


void MainWindow::quit() {
*(shared_memory_cmd+75)=0; // OnLineMap

//printf("oi3\n");
qApp->quit();}

void MainWindow::Element_Ba() ////////Map 1
{Map1=0; chlowmap1=ChMinBa; chhighmap1=ChMaxBa; qDebug()<< "Map_1: plotting Ba";}

void MainWindow::Element_Ca()
{Map1=1; chlowmap1=ChMinCa; chhighmap1=ChMaxCa; qDebug()<< "Map_1: plotting Ca";}

void MainWindow::Element_K()
{Map1=2; chlowmap1=ChMinK; chhighmap1=ChMaxK;qDebug()<< "Map_1: plotting K";}

void MainWindow::Element_Co()            
{Map1=3; chlowmap1=ChMinCo; chhighmap1=ChMaxCo; qDebug()<< "Map_1: plotting Co";}


void MainWindow::Element_Ag() ////////Map 2           
{Map2=0; chlowmap2=ChMinAg; chhighmap2=ChMaxAg; qDebug()<< "Map_2: plotting Ag";}

void MainWindow::Element_Cr()
{Map2=1; chlowmap2=ChMinCr; chhighmap2=ChMaxCr; qDebug()<< "Map_2: plotting Cr";}

void MainWindow::Element_Cu()
{Map2=2; chlowmap2=ChMinCu; chhighmap2=ChMaxCu; qDebug()<< "Map_2: plotting Cu";}

void MainWindow::Element_PbL()
{Map2=3; chlowmap2=ChMinPbL; chhighmap2=ChMaxPbL; qDebug()<< "Map_2: plotting PbL";}


void MainWindow::Element_Au() ///////////////Map 3
{Map3=0; chlowmap3=ChMinAu; chhighmap3=ChMaxAu; qDebug()<< "Map_3: plotting Au";}

void MainWindow::Element_Hg()              
{Map3=1; chlowmap3=ChMinHg; chhighmap3=ChMaxHg;qDebug()<< "Map_3: plotting Hg";}

void MainWindow::Element_Si()
{Map3=2; chlowmap3=ChMinSi; chhighmap3=ChMaxSi; qDebug()<< "Map_3: plotting Si";}

void MainWindow::Element_Ti()
{Map3=3; chlowmap3=ChMinTi; chhighmap3=ChMaxTi;qDebug()<< "Map_3: plotting Ti";}


void MainWindow::Element_Sn()              ////////// MAP 4
{Map4=0; chlowmap4=ChMinSn; chhighmap4=ChMaxSn;qDebug()<< "Map_4: plotting Sn";}

void MainWindow::Element_Fe()
{Map4=1;  chlowmap4=6750; chhighmap4=7050;qDebug()<< "Map_4: plotting Fe";}//emulatore:chlowmap4=6750; chhighmap4=7050;

void MainWindow::Element_Zn()
{Map4=2; chlowmap4=ChMinZn; chhighmap4=ChMaxZn; qDebug()<< "Map_4: plotting Zn";}

void MainWindow::Element_PbM()
{Map4=3; chlowmap4=ChMinPbM; chhighmap4=ChMaxPbM;qDebug()<< "Map_4: plotting PbM";}





/*
void MainWindow::Lo_Ch_Value( int LoVal)
{
Map_LoVal=LoVal;
}

void MainWindow::Hi_Ch_Value( int HiVal)
{
Map_HiVal=HiVal;
}
*/


void MainWindow::enableOnTop(bool OnTp)
{ 

 Qt::WindowFlags flags = this->windowFlags();
    if (OnTp)
    {
        this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        this->show();
    }
    else
    {
        this->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
        this->show();
    }
}






void MainWindow::enableOnLine(bool OnLine)
{ 

    if (OnLine)
    {
     qDebug()<<"OnLine";
     timerOnline->start(1000);    }
    else
    {
     timerOnline->stop();
	IniOnline=true;
	
     qDebug()<<"OnLine_Off";    }
}




 MainWindow::~MainWindow()
 {
*(shared_memory_cmd+75)=0;

qApp->quit();
 }






void MainWindow::displayImageOnLine()
{
	
// if(OnLine){	
  //while(*(shared_memory_cmd+70)!=0)
  //{  
	// Sleeper::msleep(500);

  if(*(shared_memory_cmd+70)==1 && IniOnline)
  {
	//printf("oi\n");
	datamemoryold=1; 
	numberpoints=-1; 
	MaxIntegralMap1=0; MaxIntegralMap2=0; MaxIntegralMap3=0; MaxIntegralMap4=0;
	IniOnline=false;
        
	MyImage1 = new QImage(PixelX, PixelY, QImage::Format_RGB32);
	MyImage2 = new QImage(PixelX, PixelY, QImage::Format_RGB32);
	MyImage3 = new QImage(PixelX, PixelY, QImage::Format_RGB32);
	MyImage4 = new QImage(PixelX, PixelY, QImage::Format_RGB32);
		
	MyImage1->fill(QColor(Qt::black).rgb());
        MyImage2->fill(QColor(Qt::black).rgb());
        MyImage3->fill(QColor(Qt::black).rgb());
        MyImage4->fill(QColor(Qt::black).rgb());
	imageLabel->setPixmap(QPixmap::fromImage(*MyImage1)); 
    	imageLabel1->setPixmap(QPixmap::fromImage(*MyImage2));
   	imageLabel2->setPixmap(QPixmap::fromImage(*MyImage3));
    	imageLabel3->setPixmap(QPixmap::fromImage(*MyImage4));

  }
  else if (*(shared_memory_cmd+70)==0){IniOnline=true;}


  if (*(shared_memory+42)==1) //News
  {  
      *(shared_memory+42)=0;
      datamemory=*(shared_memory2+4);
      //printf("datamemory:%d, datamemoryold:%d\n", datamemory, datamemoryold);

	for(n=datamemoryold;n<=datamemory;n++)
	{   
		 
		dataread=*(shared_memory2+10+n);
		//printf("dataread:%d, n:%d\n", dataread, n); 
		if(dataread>49999999 && dataread<59999999) caso=0;
		if(dataread>59999999) caso=1;
		if(dataread<50000000) caso=2;
		switch(caso)
		{

			case 0:
			{       
				numberpoints++;
				//printf("dataread:%d\n", dataread);
				Xread[numberpoints]=dataread-50000000;
				//printf("Xread1:%d, numberpoints:%d, n:%d\n", Xread[numberpoints],numberpoints, n);
				caso=10;
				break;
			}
			
			case 1:
			{
				Yread[numberpoints]=dataread-60000000;
				//printf("Yread1:%d, numberpoints:%d, n:%d\n", Yread[numberpoints], numberpoints, n);
				caso=10;
				break;
			}
			
			case 2:
			{       
				if (dataread>chlowmap1 && dataread<chhighmap1) {integralmap1[numberpoints]=integralmap1[numberpoints]+1;}
				if(integralmap1[numberpoints]>MaxIntegralMap1) MaxIntegralMap1=integralmap1[numberpoints];
				if (dataread>chlowmap2 && dataread<chhighmap2) {integralmap2[numberpoints]=integralmap2[numberpoints]+1;}
				if(integralmap2[numberpoints]>MaxIntegralMap2) MaxIntegralMap2=integralmap2[numberpoints];
				if (dataread>chlowmap3 && dataread<chhighmap3) {integralmap3[numberpoints]=integralmap3[numberpoints]+1;}
				if(integralmap3[numberpoints]>MaxIntegralMap3) MaxIntegralMap3=integralmap3[numberpoints];
				if (dataread>chlowmap4 && dataread<chhighmap4) {integralmap4[numberpoints]=integralmap4[numberpoints]+1;}
				if(integralmap4[numberpoints]>MaxIntegralMap4) MaxIntegralMap4=integralmap4[numberpoints];
				caso=10;
				break;
			}
		
		}
	
	}

	
      

        for(current=0; current<=numberpoints; current++)
            {
                //printf("numberpoints:%d, current:%d\n", numberpoints, current);
		//printf("integralmap1:%d\n", integralmap1[current]);
		//printf("integralmap2:%d\n", integralmap2[current]);
		//printf("integralmap3:%d\n", integralmap3[current]);
		//printf("integralmap4:%d\n", integralmap4[current]);

             intensitymap1= ((((double)integralmap1[current]-Map_LoVal)/MaxIntegralMap1)*255); if (intensitymap1>255) intensitymap1=255;
	     intensitymap2= ((((double)integralmap2[current]-Map_LoVal)/MaxIntegralMap2)*255); if (intensitymap2>255) intensitymap2=255;
	     intensitymap3= ((((double)integralmap3[current]-Map_LoVal)/MaxIntegralMap3)*255); if (intensitymap3>255) intensitymap3=255;
	     intensitymap4= ((((double)integralmap4[current]-Map_LoVal)/MaxIntegralMap4)*255); if (intensitymap4>255) intensitymap4=255;
		
//qDebug()<<"intensitymap1: "<<intensitymap1<<"intensitymap2"<<intensitymap2<<"intensitymap3"<<intensitymap3<<"intensitymap4"<<intensitymap4<<"\n";
             
		//int Map_Contrast=10;

		     if(intensitymap1<255/3) {myColor1.setRgb(0,int(intensitymap1),0,255);}
                     else
                     if(intensitymap1>=255/3 && intensitymap1<(2*255/3)) {myColor1.setRgb(int(intensitymap1),255,0,255);}  
                     else
                     if(intensitymap1>=(2*255/3)) {myColor1.setRgb(255,int(intensitymap1),0,255);} 

		     if(intensitymap2<255/3) {myColor2.setRgb(0,int(intensitymap2),0,255);}
                     else
                     if(intensitymap2>=255/3 && intensitymap2<(2*255/3)) {myColor2.setRgb(int(intensitymap2),255,0,255);}  
                     else
                     if(intensitymap2>=(2*255/3)) {myColor2.setRgb(255,int(intensitymap2),0,255);} 

		     if(intensitymap3<255/3) {myColor3.setRgb(0,int(intensitymap3),0,255);}
                     else
                     if(intensitymap3>=255/3 && intensitymap3<(2*255/3)) {myColor3.setRgb(int(intensitymap3),255,0,255);}  
                     else
                     if(intensitymap3>=(2*255/3)) {myColor3.setRgb(255,int(intensitymap3),0,255);} 

		     if(intensitymap4<255/3) {myColor4.setRgb(0,int(intensitymap4),0,255);}
                     else
                     if(intensitymap4>=255/3 && intensitymap4<(2*255/3)) {myColor4.setRgb(int(intensitymap4),255,0,255);}  
                     else
                     if(intensitymap4>=(2*255/3)) {myColor4.setRgb(255,int(intensitymap4),0,255);} 
		
                 
           // myColor.setRgb(int(intensitymap1),int(intensity),int(intensity),255);
		

		/*Xmaponline[current]=(Xread[current]-(*(shared_memory_cmd+50)))/(*(shared_memory_cmd+60));
		Ymaponline[current]=(Yread[current]-(*(shared_memory_cmd+52)))/(*(shared_memory_cmd+61));
		//printf("Xread:%d, Yread:%d, Xmaponline:%d, Ymaponline:%d, current:%d\n", Xread[current], Yread[current], Xmaponline[current], Ymaponline[current], current);

		Define_Pixels();*/
	
          
	//qDebug()<<"OffsetX: "<<OffsetX<<" OffsetY "<<OffsetY<<"\n";
            
	   for(c=0;c<(PixelDimOnline*PixelDimOnline);c++){
          //printf("current:%d\n", current);
	     MyImage1->setPixel(PixelX-Pointer[current].total[c].point_x-OffsetX,PixelY-Pointer[current].total[c].point_y-OffsetY, myColor1.rgb());
	     MyImage2->setPixel(PixelX-Pointer[current].total[c].point_x-OffsetX,PixelY-Pointer[current].total[c].point_y-OffsetY, myColor2.rgb());
	     MyImage3->setPixel(PixelX-Pointer[current].total[c].point_x-OffsetX,PixelY-Pointer[current].total[c].point_y-OffsetY, myColor3.rgb());
	     MyImage4->setPixel(PixelX-Pointer[current].total[c].point_x-OffsetX,PixelY-Pointer[current].total[c].point_y-OffsetY, myColor4.rgb());
		}
				
             }//closes for int current

	
		imageLabel->setPixmap(QPixmap::fromImage(*MyImage1)); 
    		imageLabel1->setPixmap(QPixmap::fromImage(*MyImage2));
   		imageLabel2->setPixmap(QPixmap::fromImage(*MyImage3));
    		imageLabel3->setPixmap(QPixmap::fromImage(*MyImage4));
    		scaleFactor = 1.0;
		    
  //  currentnew=numberpoints;
  	
	
  datamemoryold=datamemory+1;

    }///chiude if news

}
















