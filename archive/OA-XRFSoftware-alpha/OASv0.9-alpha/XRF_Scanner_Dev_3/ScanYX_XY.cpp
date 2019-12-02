////////////////////////// FRAO DEC 2016
//
// removed if(XYscanning==true), the check is in the main loop (timerEvent() in mainwindow.cpp)
// removed bool VmeStatus; the DAQ STATUS is written in SHM *(shared_memory_cmd+70)
//



#include "mainwindow.h"
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

extern int serialX,serialY;
extern int send_command(int chan,const char *comando, const char *parametri, int port);
extern double numpixelforaccel;     extern double posXforacceleration;     extern double accelerationtime;	extern int accelerationtimesleep;

////////////////////////////////////////////////////////////////////////////////////


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
     send_command(1,"VEL 1 10",NULL,serialX);
     send_command(1,"VEL 1 10",NULL,serialY);
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
                send_command(1,"VEL",v,serialY);
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
                

                 else{                                                                         //La scansione � finita
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
    temp=Ymax;

    if((*(shared_memory_cmd+70)==0) && ask==false) {
        ask=true;
        if(DAQ_TYPE==1) {
            QMessageBox::StandardButton reply;
            reply=QMessageBox::question(this, "WARNING", "[!] Start USB DAQ?", QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                system("./ADCXRF_USB &");
                *(shared_memory_cmd+70)=1;
            }
        }
        if(DAQ_TYPE==0) {
            QMessageBox::StandardButton reply;
            reply=QMessageBox::question(this, "WARNING", "[!] Start Optical DAQ?", QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                system("./ADCXRF_Optical_Link &");
                *(shared_memory_cmd+70)=1;
                Sleeper::msleep(2000);
            }
        }
    }

    if(NscanX==0 && XOnTarget==true && YOnTarget==true) {
        send_command(1,"VEL 1 10",NULL,serialX);
        send_command(1,"VEL 1 10",NULL,serialY);
        qDebug()<<"... Moving motors to the scan origin with coordinates :"<<positionX/1000<<" "<<positionY/1000<<'\n';

        positionX=Xmin-posXforacceleration;

        MoveX(positionX);
        MoveY(positionY);

        YHasMoved=true;
        NscanX++;
    }

    else {
        if(XOnTarget==true && YOnTarget==true && YHasMoved==true) {
            //X motor starts to move
            Clock2=0;
            //timerPos->start(tempoPos);
            if(onlyOne==0) {
                char v[10];
                sprintf(v,"%f",V);                       
                send_command(1,"VEL",v,serialX);
                onlyOne=1;
            }

            YHasMoved=false;
            *(shared_memory2+3)=1;
            if(positionX==Xmin-posXforacceleration) {positionX=Xmax+posXforacceleration; Xo=Xmin; vx=V;}
            else if(positionX==Xmax+posXforacceleration) {positionX=Xmin-posXforacceleration; Xo=Xmax; vx=-V;}

            MoveX(positionX);
            qDebug()<<"Sleeping for a druation of 'accelerationtimesleep' :"<<accelerationtimesleep;
            Sleeper::msleep(accelerationtimesleep);


            //timerPos->start(tempoPos);
        }

        if(XOnTarget==true && YOnTarget==true && YHasMoved==false) {

            //Y motor starts to move
            qDebug()<<"Inside second loop";
            if(positionY<Ymax) {
                    positionY=positionY+Py;
                    //timerPos->stop();
                    Clock2=0;

                    while(*(shared_memory2+8)!=1) Sleeper::msleep(100);
                    qDebug()<<"... Function MoveY about to be called with position argument: "<<positionY;
                    MoveY(positionY);
                    qDebug()<<"... Function MoveY succesfully called\n"
                              "... Value of *(shared_memory2+8) is: "<<(shared_memory2+8);
                    *(shared_memory2+8)=0;
                    //YOnTarget=false;
                    YHasMoved=true;
            }

            else {

                //The scan is done
                XYscanning=false;
                NscanX=0;Clock2=0;
                //timerPos->stop();
                onlyOne=0;
                if(*(shared_memory_cmd+70)==1) {
                    //int pidVme=*(shared_memory_cmd+80);
                    //sprintf(process, "kill -s TERM %i &", pidVme);
                    //system(process);
                    int counting=0;
                    while(*(shared_memory2+8)!=1 && counting<5) {
                        Sleeper::msleep(tempoPos);
                        counting++;
                    }
                    *(shared_memory_cmd+70)=0;
                    *(shared_memory2+8)=0;
                    ask=false;
                    SaveTxt();
                }
            }
        }
    }
}



















