////////////////////////// FRAO DEC 2016
//
// removed if(XYscanning==true), the check is in the main loop (timerEvent() in mainwindow.cpp)
// removed bool VmeStatus; the DAQ STATUS is written in SHM *(shared_memory_cmd+70)
//



#include "include/MAXRF/mainwindow.h"
#include "../Header.h"
#include <../Shm.h>


extern bool YXscanning;    extern bool XYscanning;        extern bool XOnTarget;      extern bool YOnTarget;   extern bool FirstRun;
extern bool XHasMoved;     extern bool YHasMoved;         extern bool ask;

extern double positionX;   extern double positionY;       extern double V;            extern double Xmin1;     extern double Ymax1;
extern double Xmax1;       extern double Ymin1;           extern double Xmax;         extern double Xmin;      extern double Ymax; 
extern double Ymin;        extern double tempoPos;        extern double Px;           extern double Py;        extern double valueY;
extern double valueX;

extern int Clock2;         extern int NscanX;             extern int NscanY;          extern int casenumber;   extern int  onlyOne; 
extern int  eventionline;  extern int EventOffset;        extern int missing;         extern int deltaEventi;  extern int DAQ_TYPE;
extern int *shared_memory; extern int *shared_memory_cmd; extern int *shared_memory2;

extern float Yo,vy,Xo,vx,temp;
extern QString line1;
extern char process[30];

extern int invia_comando_X(int chan,const char *comando, const char *parametri);
extern int invia_comando_Y(int chan,const char *comando, const char *parametri);

extern double numpixelforaccel;     extern double posXforacceleration;     extern double accelerationtime;	extern int accelerationtimesleep;

////////////////////////////////////////////////////////////////////////////////////

                
extern bool Scan_started; extern void lcdTimer(); extern void Stop_Abort_DAQ();
extern bool QuitProgram_bool;


extern double millisec;
extern struct timeval tv;

extern struct itimerval it_val;	/* for setting itimer */


void MainWindow::ScanYX()
{
temp=Xmax;

{

    if((*(shared_memory_cmd+70)==0) && ask==false)
         {
	  ask=true;
          if(DAQ_TYPE==1)
            {
             QMessageBox::StandardButton reply;
             reply=QMessageBox::question(this, "WAENING", "Starting USB DAQ?", QMessageBox::Yes|QMessageBox::No);
             if (reply == QMessageBox::Yes) 
	        {system("./ADCXRF_USB &"); *(shared_memory_cmd+70)=1;}
             }
          if(DAQ_TYPE==0)
            {
             QMessageBox::StandardButton reply;
             reply=QMessageBox::question(this, "WARNING", "Starting OPTICAL DAQ?", QMessageBox::Yes|QMessageBox::No);
             if (reply == QMessageBox::Yes) 
                 {system("./ADCXRF_Optical_Link &");*(shared_memory_cmd+70)=1;}
             }
	    }
           	  


   if(NscanY==0 && XOnTarget==true && YOnTarget==true)
     {
     invia_comando_X(1,"VEL 1 10",NULL);
     invia_comando_Y(1,"VEL 1 10",NULL);
      qDebug()<<"Spostamento a Xmin e Ymin....."<<"che sarebbero "<<positionX<<" "<<positionY<<'\n'; 
      MoveX(positionX); 
      //XOnTarget=false;
      MoveY(positionY); 
      //YOnTarget=false;
      XHasMoved=true;
      NscanY++;
     }

   else
     {
      
      if(XOnTarget==true && YOnTarget==true && XHasMoved==true)       //si inizia a muovere Y        
        {
         //qDebug()<<"XonT "<<XOnTarget<<"YOnT"<<YOnTarget<<"XMoved"<<XHasMoved<<'\n';
            Clock2=0;
            //timerPos->start(tempoPos);
            if(onlyOne==0) 
               {char v[10];
                sprintf(v,"%f",V);
                invia_comando_Y(1,"VEL",v);         
                onlyOne=1;
                }
             
             XHasMoved=false;
             if(positionY==Ymin) {positionY=Ymax; Yo=Ymin; vy=V;} 
             else
             if(positionY==Ymax) {positionY=Ymin; Yo=Ymax; vy=-V;}  
             YOnTarget=false;
            //qDebug()<<"Xmin Xmax"<<Xmin<<Xmax<<"V "<<V<<'\n';
             MoveY(positionY);
             timerPos->start(tempoPos);
              
        }
                              
      if(XOnTarget==true && YOnTarget==true && XHasMoved==false)  //si inizia a muovere X
        {
             
            if(positionX<Xmax)
                       {
                     
                        positionX=positionX+Px; 
                        timerPos->stop();
                        Clock2=0;
                        MoveX(positionX);
                        //XOnTarget=false;
                        XHasMoved=true;
                       }
                

                 else{                                                                         //La scansione è finita
			YXscanning=false; NscanY=0;Clock2=0;timerPos->stop();onlyOne=0;
                           if(*(shared_memory_cmd+70)==1)
                              {   qDebug()<<*(shared_memory2+4);
                                  int pidVme=*(shared_memory_cmd+80);
                                  sprintf(process, "kill -s TERM %i &", pidVme);
                                  system(process);
                                  *(shared_memory_cmd+70)=0;
                                  ask=false;
                                  SaveTxt();
                                  qDebug()<<".... file saved....";
                              }
                      }
           }

       }///chiude else
   }


}




////////////////////////////////////////////////////////////////////////////////////


void MainWindow::ScanXY()
{

if(QuitProgram_bool)
{Abort_On_Digitiser_busy(); QuitProgram_bool=false; exit;}


temp=Ymax;

    if((*(shared_memory_cmd+70)==0) && ask==false)
         {
	  ask=true;
          if(DAQ_TYPE==1)
            {
             QMessageBox::StandardButton reply;
             reply=QMessageBox::question(this, "WAENING", "Starting USB DAQ?", QMessageBox::Yes|QMessageBox::No);
             if (reply == QMessageBox::Yes) 
	        {system("./ADCXRF_USB &"); *(shared_memory_cmd+70)=1;}
             }
          if(DAQ_TYPE==0)
            {
             QMessageBox::StandardButton reply;
             reply=QMessageBox::question(this, "WARNING", "Starting OPTICAL DAQ?", QMessageBox::Yes|QMessageBox::No);
             if (reply == QMessageBox::Yes) 
                 {system("./ADCXRF_Optical_Link &");*(shared_memory_cmd+70)=1;Sleeper::msleep(2000);}
             }
	    }





   if(NscanX==0 && XOnTarget==true && YOnTarget==true)
     {
     invia_comando_X(1,"VEL 1 10",NULL);
     invia_comando_Y(1,"VEL 1 10",NULL);
      qDebug()<<"Spostamento a Xmin e Ymin....."<<"che sarebbero "<<positionX/1000<<" "<<positionY/1000<<'\n';
	positionX=Xmin-posXforacceleration;
	//printf("positionX=%f\n", positionX); 
      MoveX(positionX); 
      //XOnTarget=false;
      MoveY(positionY); 
      //YOnTarget=false;
      YHasMoved=true;
      NscanX++;
     }

   else
     {
      
      if(XOnTarget==true && YOnTarget==true && YHasMoved==true)       //si inizia a muovere X        
        {
	    
            Clock2=0;
            //timerPos->start(tempoPos);
            if(onlyOne==0) 
               {char v[10];
                sprintf(v,"%f",V);                     
                //invia_comando_Y(1,"VEL",v);  
                invia_comando_X(1,"VEL",v); 

////////////////////////////////////////////////////////////////////////////////////// INSERITO TIMER

                Scan_started=true; lcdTimer(); 

                onlyOne=1;
                }
             
             YHasMoved=false;
             if(positionX==Xmin-posXforacceleration) {positionX=Xmax+posXforacceleration; Xo=Xmin; vx=V;} 
             else
             if(positionX==Xmax+posXforacceleration) {positionX=Xmin-posXforacceleration; Xo=Xmax; vx=-V;}  

            // if(positionX==Xmin) {positionX=Xmax; Xo=Xmin; vx=V;} 
            // else
            // if(positionX==Xmax) {positionX=Xmin; Xo=Xmax; vx=-V;}  
             
            //qDebug()<<"Xmin Xmax"<<Xmin<<Xmax<<"Vx "<<vx<<'\n';
            //qDebug()<<"Xmin1 Xmax1"<<Xmin1<<Xmax1<<"V "<<V<<'\n';
	   // gettimeofday(&tv,NULL);
		//millisec=tv.tv_usec;
		//printf ("time 1:%lu,%lu\n", tv, millisec);
	    MoveX(positionX);
	    Sleeper::msleep(accelerationtimesleep);
            *(shared_memory2+3)=1;

             //timerPos->start(tempoPos);
        }
                              
      if(XOnTarget==true && YOnTarget==true && YHasMoved==false)  //si inizia a muovere Y
        {
             
            if(positionY<Ymax)
                       {
                     
                        positionY=positionY+Py; 
                        //timerPos->stop();
                        Clock2=0;
			while(*(shared_memory2+8)!=1)Sleeper::msleep(100);
                        MoveY(positionY);
			*(shared_memory2+8)=0;
                        //YOnTarget=false;
                        YHasMoved=true;
                       }
                

                 else{                                                               //La scansione è finita
			XYscanning=false; NscanX=0;Clock2=0; //timerPos->stop();
                        onlyOne=0;
                        if(*(shared_memory_cmd+70)==1)
                              {                     
                                  int pidVme=*(shared_memory_cmd+80);
                                  //sprintf(process, "kill -s TERM %i &", pidVme);
                                  //system(process);
				  int counting=0;
				  while(*(shared_memory2+8)!=1 && counting<5){Sleeper::msleep(tempoPos); counting++;}
                                  *(shared_memory_cmd+70)=0;
				 *(shared_memory2+8)=0;
                                  ask=false;
                                  SaveTxt();

                              }
                      }
           }

       }


}



















