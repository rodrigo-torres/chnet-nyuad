#include "include/MAXRF/mainwindow.h"
#include <../Header.h>
#include <../Shm.h>

extern int *shm_cmd, *shmCommand_cmd, shmid_cmd, shmidString_cmd, *shared_memory_cmd; 
extern key_t key_cmd; extern key_t keyString_cmd;

extern int *shm, *shmCommand, shmid, shmidString, *shared_memory; 
extern key_t key; extern key_t keyString;

extern int *shm2, *shmCommand2, shmid2, shmidString2, *shared_memory2; 
extern key_t key2; extern key_t keyString2;

extern int *shm3, *shmCommand3, shmid3, shmidString3, *shared_memory3;
extern key_t key3; extern key_t keyString3;

extern int *shm4, *shmCommand4, shmid4, shmidString4, *shared_memory4;
extern key_t key4; extern key_t keyString4;

extern int *shm_rate, *shmCommand_rate, shmid_rate, shmidString_rate, *shared_memory_rate;
extern key_t key_rate; extern key_t keyString_rate;

//extern int MotoreWindowStatus, CentralWindowStatus;
extern int portX, portY, portZ, IniX, IniY, IniZ, IniXready, IniYready, IniZready, SerialiStatus, PassoX, PassoY, PassoZ; 


void MainWindow::SHM_CREATOR()
{
   key_cmd = 6900;
   shmid_cmd = shmget (6900, SHMSZ_CMD_STATUS, IPC_CREAT | 0666);
       if (shmid_cmd == -1) qDebug()<<"Errore  "<< strerror(errno)<<'\n';
   shared_memory_cmd =(int *) shmat(shmid_cmd, NULL, 0);
   cout << "Data Memory attached at :" << (int *)shared_memory_cmd<< endl << " with shmid_cmd: " << shmid_cmd << endl;

   key = 7000;
   shmid = shmget (7000, SHMSZ, IPC_CREAT | 0666);
       if (shmid == -1) qDebug()<<"Errore  "<< strerror(errno)<<'\n';
   shared_memory =(int *) shmat(shmid, NULL, 0);
   cout << "Data Memory attached at :" << (int *)shared_memory<< endl << " with shmid2: " << shmid << endl;

   key2 = 7200;
   shmid2 = shmget (7200, SHMSZBIG, IPC_CREAT | 0666);
       if (shmid2 == -1) qDebug()<<"Errore  "<< strerror(errno)<<'\n';
   shared_memory2 =(int *) shmat(shmid2, NULL, 0);
   cout << "Data Memory attached at :" << (int *)shared_memory2<< endl << " with shmid2: " << shmid2 << endl;


  ///////////////////////////////// FOR AUTOFOCUS ///////////////////////////

   key3 = 7300;
   shmid3 = shmget (7300, SHMSZHISTO, IPC_CREAT | 0666);
       if (shmid3 == -1) qDebug()<<"Errore  "<< strerror(errno)<<'\n';
   shared_memory3 =(int *) shmat(shmid3, NULL, 0);
   cout << "Data Memory attached at :" << (int *)shared_memory3<< endl << " with shmid4: " << shmid3 << endl;

  /////////////////////////////// FOR DIGITISER /////////////////////////////

   key4 = 7400;
   shmid4 = shmget (7400, SHMSZDIGI, IPC_CREAT | 0666);
       if (shmid4 == -1) qDebug()<<"Errore  "<< strerror(errno)<<'\n';
   shared_memory4 =(int *) shmat(shmid4, NULL, 0);
   cout << "Data Memory attached at :" << (int *)shared_memory4<< endl << " with shmid5: " << shmid4 << endl;

  /////////////////////////////// FOR RATE METER /////////////////////////////

   key_rate = 7500;
   shmid_rate = shmget (7500, SHMSZRATE, IPC_CREAT | 0666);
       if (shmid_rate == -1) qDebug()<<"Errore  "<< strerror(errno)<<'\n';
   shared_memory_rate =(int *) shmat(shmid_rate, NULL, 0);
   cout << "Data Memory attached at :" << (int *)shared_memory_rate<< endl << " with shmid6: " << shmid_rate << endl;


   ////////////////   SHARED MEMORY STARTING CONFIGURATION   ///////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////// REDEFINED (DEC-2016) SHM_CMD INTRODUCED   ///////////////////////////////////////

                    // *shared_memory_cmd=MotoreWindowStatus;       /// XY Motor status//no more existent
                    // *(shared_memory_cmd+1)=MotoreWindowStatus;   /// Z Motor status//no more existent
                     *(shared_memory_cmd+2)=SerialiStatus;        /// not used

                     *(shared_memory_cmd+10)=portX;               /// X Port (default=0) initialization in variables.h
                     *(shared_memory_cmd+11)=portY;               /// Y Port (default=1)
                     *(shared_memory_cmd+12)=portZ;               /// Z Port (default=2)

                     *(shared_memory_cmd+20)=IniX;                /// X motor not inited (default=0) initialization in variables.h
                     *(shared_memory_cmd+21)=IniY;                /// Y motor not inited (default=0)
                     *(shared_memory_cmd+22)=IniZ;                /// Z motor not inited (default=0)

                     *(shared_memory_cmd+30)=IniXready;           /// X motor status (default=0) initialization in variables.h
                     *(shared_memory_cmd+31)=IniYready;           /// Y motor status (default=0)
                     *(shared_memory_cmd+32)=IniZready;           /// Z motor status (default=0)

                     *(shared_memory_cmd+40)=0;                   /// X[point] Position (default=0) 
                     *(shared_memory_cmd+41)=0;                   /// Y[point] Position (default=0)
                     *(shared_memory_cmd+42)=0;                   /// Z Position (default=0) 
                     *(shared_memory_cmd+43)=0;                   /// Integral[point] (default=0) 

                     *(shared_memory_cmd+50)=0;                   /// Xmin (default=0) 
                     *(shared_memory_cmd+51)=0;                   /// Xmax (default=0) 
                     *(shared_memory_cmd+52)=0;                   /// Ymin (default=0) 
                     *(shared_memory_cmd+53)=0;                   /// Ymax (default=0) 
                     *(shared_memory_cmd+54)=0;                   /// Zmin (default=0) 
                     *(shared_memory_cmd+55)=0;                   /// Zmax (default=0) 

                     *(shared_memory_cmd+60)=PassoX;              /// X step (default=0) 
                     *(shared_memory_cmd+61)=PassoY;              /// Y step (default=0) 
                     *(shared_memory_cmd+62)=PassoZ;              /// Z step (default=0) 
                     *(shared_memory_cmd+64)=0;                   /// X movement (default=0) 
                     *(shared_memory_cmd+65)=0;                   /// Y movement (default=0)
                     *(shared_memory_cmd+66)=0;                   /// Z movement (default=0)
		     *(shared_memory_cmd+67)=1;                   /// Scan velocity (default=1 mm/s)

                     *(shared_memory_cmd+70)=0;                   /// VME/ADCXRF STATUS (default=0) 
                     *(shared_memory_cmd+71)=0;                   /// XRF SPECTRUM STATUS (default=0) 
                     *(shared_memory_cmd+72)=0;                   /// Digitiser_interface STATUS (default=0) 
                     *(shared_memory_cmd+73)=0;                   /// Rate mater STATUS (default=0) 
                     *(shared_memory_cmd+74)=0;                   /// Xray Table STATUS (default=0) 
                     *(shared_memory_cmd+75)=0;                   /// OnLineMap STATUS (default=0) 
                     *(shared_memory_cmd+76)=0;                   /// Motor test tool STATUS (default=0) 
                     *(shared_memory_cmd+77)=0;                   /// PI parameter table STATUS (default=0) 
                     *(shared_memory_cmd+78)=0;                   /// Fit Program STATUS (default=0) 

                     *(shared_memory_cmd+80)=10000;               /// PID ADCXRF (default=10000) 
                     *(shared_memory_cmd+81)=10000;               /// PID XRF SPECTRUM (default=10000)
                     *(shared_memory_cmd+82)=10000;               /// PID Digitiser_interface (default=10000)
                     *(shared_memory_cmd+83)=10000;               /// PID Rate Meter (default=10000)
                     *(shared_memory_cmd+84)=10000;               /// PID Xray Table (default=10000)
                     *(shared_memory_cmd+85)=10000;               /// PID OnLineMap (default=10000)
                     *(shared_memory_cmd+86)=10000;               /// PID Motor test tool STATUS (default=10000) 
                     *(shared_memory_cmd+87)=10000;               /// PID PI parameter table (default=10000) 
		     *(shared_memory_cmd+88)=10000;		  /// PID XRF_Fit (default = 10000)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////


                                     *(shared_memory+13)=0;          ///...News per mappa online
				     *(shared_memory+19)=0;         //Avviso spostamento motore
                                     *(shared_memory+22)=0;          //MergePos x sommare diverse scansioni
				     *(shared_memory+24)=0;        ///Spettro in canale o energia (0 canale) (1 energia)


                                     *(shared_memory+39)=0;  //Rate meter
                                     *(shared_memory+40)=10; //Low energy cut


                                     *(shared_memory+42)=0; //MAP_ON_LINE: News
                                     *(shared_memory+43)=0; //MAP_ON_LINE: Integral
                                     *(shared_memory+44)=0; //MAP_ON_LINE: X[point]
                                     *(shared_memory+45)=0; //MAP_ON_LINE: Y[point]




                                    *(shared_memory+99)=0;           ///...News per show pixel histo  
                                    *(shared_memory+100)=0;          ///Da questo punto inizia il vettore da plottare per mostrare lo spettro




                                   *(shared_memory2+1)=0;    //// Posiz X
                                   *(shared_memory2+2)=0;    //// Posiz Y
                                   *(shared_memory2+3)=0;    //// ontarget if 1

                                   ///*(shared_memory2+4)=0;    //// Numero dati in memoria (posizioni e energie)
                                    *(shared_memory2+5)=0;     ///NEvents totali ADC per mappa online

                       		   //*(shared_memory2+100)=0;    //partenza per mappa on line       NO!!!!  
                                   //  *(shared_memory2+6);     ///N vicino a SegFault
                                   //  *(shared_memory2+7);     ///Eventi adc via via che avanza la scansione (per rate)
				   *(shared_memory2+8)=0; //usato per indicare timer stop prime di cambiare righa
				   *(shared_memory2+9)=0; //usato quando l'acquisizione viene chiamata senza scanzione

                                   // *(shared_memory2+8-10);    //// Posizioni libere se ci pungesse vaghezza di aggiungere qualche variabile
                                   // *(shared_memory2+11);      //// Da qui inizia a scrivere Position


				  // *(shared_memory3)=0; ////From here file histo





////////////////////////////////////////////// DIGITISER PARAMETERS


                     *(shared_memory4)=40;        // Trigger Threshold                                   
                     *(shared_memory4+1)=0;       // Input range digitalizzatore (0 -> 0.6V // 1 -> 1.4V                                  
                     *(shared_memory4+2)=800;     // Trapezoid Rise Time (ns) 
                     *(shared_memory4+3)=2500;    // Trapezoid Flat Top  (ns)
                     *(shared_memory4+4)=12000;   // Decay Time Constant (ns) HACK-FPEP the one expected from fitting algorithm?
                     *(shared_memory4+5)=1500;    // Peaking delay  (ns)
                     *(shared_memory4+6)=2;       // Trigger Filter smoothing factor  
                     *(shared_memory4+7)=0;       // OFFSET
                     *(shared_memory4+8)=3000;    // Trigger Hold Off  
                     *(shared_memory4+9)=256;     // BaseLine Mean - ?? 3 = bx10 = 64 samples (baseline mean del trapezio) (...?? 3)  
                     *(shared_memory4+10)=1;      // peak mean (numero punti su cui mediare per peaking del trapezio)  
                     *(shared_memory4+11)=3000;   // peak holdoff (min intervallo tra 2 trapezi...minimo=k+m)  
                     *(shared_memory4+12)=1;      // Trapezoid Gain  
                     *(shared_memory4+13)=100;    // NOT USED: Delay(b) Input Signal Rise time (ns)...sarebbe delay (b)
                     *(shared_memory4+14)=10;     // NOT USED: Energy Normalization Factor

                     *(shared_memory4+15)=0;      // Enable digitiser reading

}





