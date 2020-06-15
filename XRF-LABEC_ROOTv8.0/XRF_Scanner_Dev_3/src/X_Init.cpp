// Frao 31/12/2016
//
// motor_INIT_XXX.cpp manages the calls from mainwindos.cpp 
// 
// 
// 
// THREE IDENTICAL MODULES ARE PRESENT IN THIS FILE TO INIT THE MOTORS (X-Y-Z). 
//


#include "include/MAXRF/mainwindow.h"
#include "../Header.h"
#include <../Shm.h>
#include <time.h>

////////////variabili e funzioni definite altrove ma necessarie////////////////////////////

extern int serialX,interval; extern bool InitX; extern bool InitPhaseX;
extern bool TimerActive, okX;
extern int invia_comando_X(int chan,const char *comando, const char *parametri);
extern char *read_Xanswer();
extern bool XConnected; extern bool Xmoving; 
extern int *shared_memory_cmd; extern int Xmin; extern int Xmax;
//////////////////////////////////////////////////////////////////////////////////////////////////

extern double V; extern double tempoPos; extern double Px;
extern int invia_comando_X(int chan,const char *comando, const char *parametri);




bool X_selected_motor_is_valid=false;

extern int motor_selectedX;

void MainWindow::InizializzazioneX()
{
  pushButton_tab_2_2Y->setEnabled(false); // disabling Y and Z init button to avoid conflicts
  INIT_Z_pushButton->setEnabled(false);
  Sleeper::msleep(200);
  //qDebug()<<motor_selectedX;
  qDebug()<<"...entering motor selection switch...";

      switch (motor_selectedX)
       {
        case(1): { //"M404-2PD" 50mm

		  CurrentAction->setText("Initialising: M-404.2PD - 50mm");

                  invia_comando_X(1,"SPA"," 60 M-404.2PD");  
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for M-404.2PD linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);                  
                  invia_comando_X(1,"SPA"," 1 180"); //primo numero canale SPA secondo numero valore
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 2 45");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 3 300");         
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 4 2000");       
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 14 4000");     
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 15 1"); ////0.064285257
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 19 0");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 20 1");        //si usano i limit switches dell'hardware (top e bottom) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 21 50.");       //Max spostamento in positivo da home (0)  
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 22 25.");   //Value at Reference position
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 23 25.");  //Distanza tra Ref e Neg-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 47 25");    //Distanza tra Ref e Pos-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 24 0");          //0 i limit switches sono attivi alti...va come 49
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 48 0.000000");  //Max spostamento in negativo da home (0) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 49 0");        //0 non si inverte il riferimento 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 50 0");      //0 Abilita lo stop ai limit switches dell'hardware    
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 8 0.5");    //8 10 e 11 per ultimi senno' li ricambia 0.035156
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 10 50");   // velocita'
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 11 30");  //Accelerazione
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 117442049 MM");  //Unita'
                  Sleeper::msleep(100);
  
                    invia_comando_X(1,"SPA? 1 1",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 2",NULL);
                  read_Xanswer();
                   invia_comando_X(1,"SPA? 1 3",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 4",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 8",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 10",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 11",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 14",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 15",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 19",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 20",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 21",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 22",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 23",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 24",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 48",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 49",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 50",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 60",NULL);
                  read_Xanswer();


                  X_selected_motor_is_valid=true;

                 break;}

        case(2): { //"M404-4PD" 100mm code:

		  CurrentAction->setText("Initialising: M-404.4PD - 100mm");

                  invia_comando_X(1,"SPA"," 60 M-404.4PD");  
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for M-404.4PD linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);
                  invia_comando_X(1,"SPA"," 1 180"); 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 2 45");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 3 300");         
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 4 2000");       
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 14 4000");     
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 15 1"); ////0.064285257
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 19 0");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 20 1");        //si usano i limit switches dell'hardware (top e bottom) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 21 100.");       //Max spostamento in positivo da home (0)  
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 22 50.");   //Value at Reference position
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 23 50.");  //Distanza tra Ref e Neg-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 47 50");    //Distanza tra Ref e Pos-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 24 0");          //0 i limit switches sono attivi alti...va come 49
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 48 0.000000");  //Max spostamento in negativo da home (0) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 49 0");        //0 non si inverte il riferimento 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 50 0");      //0 Abilita lo stop ai limit switches dell'hardware    
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 8 0.5");    //8 10 e 11 per ultimi senno' li ricambia 0.035156
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 10 50");   // velocita'
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 11 30");  //Accelerazione
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 117442049 MM");  //Unita'
                  Sleeper::msleep(100);
  
                    invia_comando_X(1,"SPA? 1 1",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 2",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 3",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 4",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 8",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 10",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 11",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 14",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 15",NULL);
                  read_Xanswer();
                   invia_comando_X(1,"SPA? 1 19",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 20",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 21",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 22",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 23",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 24",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 48",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 49",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 50",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 60",NULL);
                  read_Xanswer();

                  X_selected_motor_is_valid=true;

                 break;}

        case(3): { //"M404-8PD" 200mm code:

		  CurrentAction->setText("Initialising: M-404.8PD - 200mm");

                  invia_comando_X(1,"SPA"," 60 M-404.8PD");  
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for M-404.8PD linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);
                  invia_comando_X(1,"SPA"," 1 180"); //primo numero canale SPA secondo numero valore
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 2 45");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 3 300");         
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 4 2000");       
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 14 4000");     
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 15 1"); ////0.064285257
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 19 0");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 20 1");        //si usano i limit switches dell'hardware (top e bottom) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 21 200.");       //Max spostamento in positivo da home (0)  
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 22 100.");   //Value at Reference position
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 23 100.");  //Distanza tra Ref e Neg-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 47 100");    //Distanza tra Ref e Pos-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 24 0");          //0 i limit switches sono attivi alti...va come 49
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 48 0.000000");  //Max spostamento in negativo da home (0) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 49 0");        //0 non si inverte il riferimento 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 50 0");      //0 Abilita lo stop ai limit switches dell'hardware    
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 8 0.5");    //8 10 e 11 per ultimi senno' li ricambia 0.035156
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 10 50");   // velocita'
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 11 200");  //Accelerazione
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 117442049 MM");  //Unita'
                  Sleeper::msleep(100);
  
                    invia_comando_X(1,"SPA? 1 1",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 2",NULL);
                  read_Xanswer();
                   invia_comando_X(1,"SPA? 1 3",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 4",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 8",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 10",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 11",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 14",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 15",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 19",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 20",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 21",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 22",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 23",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 24",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 48",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 49",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 50",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 60",NULL);
                  read_Xanswer();

                  X_selected_motor_is_valid=true;

                 break;}

        case(4): { //"VT80-100" 100mm code: 62309140

		  CurrentAction->setText("Initialising: VT80-100 - 100mm code: 62309140");

                  invia_comando_X(1,"SPA"," 62309140"); 
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for VT80-100 linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);
                  invia_comando_X(1,"SPA"," 1 1000"); 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 2 1000");
                  Sleeper::msleep(100);          
                  invia_comando_X(1,"SPA"," 3 200");
                  Sleeper::msleep(100);         
                  invia_comando_X(1,"SPA"," 4 1000");       
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 14 2000");
                  Sleeper::msleep(100);     
                  invia_comando_X(1,"SPA"," 15 1"); ////0.064285257
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 19 0");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 20 1");        //si usano i limit switches dell'hardware (top e bottom) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 21 100");       //Max spostamento in positivo da home (0)  
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 22 0.5");   //Value at Reference position
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 23 0.5");  //Distanza tra Ref e Neg-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 47 99.5");    //Distanza tra Ref e Pos-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 24 0");          //0 i limit switches sono attivi alti...va come 49
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 48 0.0000");  //Max spostamento in negativo da home (0) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 49 0");        //0 non si inverte il riferimento 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 50 0");      //0 Abilita lo stop ai limit switches dell'hardware    
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 8 0.5");    //8 10 e 11 per ultimi senno' li ricambia 0.035156
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 10 50");   // velocita'
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 11 200");  //Accelerazione
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 117442049 MM");  //Unita'
                  Sleeper::msleep(100);
  
                    invia_comando_X(1,"SPA? 1 1",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 2",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 3",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 4",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 8",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 10",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 11",NULL);
                  read_Xanswer();
                   invia_comando_X(1,"SPA? 1 14",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 15",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 19",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 20",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 21",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 22",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 23",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 24",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 48",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 49",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 50",NULL);
                  read_Xanswer();
                      invia_comando_X(1,"SPA? 1 60",NULL);
                  read_Xanswer();

                  X_selected_motor_is_valid=true;

                 break;}

        case(5): { //"VT80-150" 150mm code: 62309150

                  CurrentAction->setText("Initialising: VT80-150 - 150mm code: 62309150");

                  invia_comando_X(1,"SPA"," 62309150"); 
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for VT80-150 linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);
                  invia_comando_X(1,"SPA"," 1 1000"); 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 2 1000");
                  Sleeper::msleep(100);          
                  invia_comando_X(1,"SPA"," 3 200");
                  Sleeper::msleep(100);         
                  invia_comando_X(1,"SPA"," 4 1000");       
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 14 2000");
                  Sleeper::msleep(100);     
                  invia_comando_X(1,"SPA"," 15 1"); ////0.064285257
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 19 0");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 20 1");        //si usano i limit switches dell'hardware (top e bottom) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 21 150");       //Max spostamento in positivo da home (0)  
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 22 0.5");   //Value at Reference position
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 23 0.5");  //Distanza tra Ref e Neg-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 47 149.5");    //Distanza tra Ref e Pos-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 24 0");          //0 i limit switches sono attivi alti...va come 49
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 48 0.0000");  //Max spostamento in negativo da home (0) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 49 0");        //0 non si inverte il riferimento 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 50 0");      //0 Abilita lo stop ai limit switches dell'hardware    
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 8 0.5");    //8 10 e 11 per ultimi senno' li ricambia 0.035156
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 10 50");   // velocita'
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 11 200");  //Accelerazione
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 117442049 MM");  //Unita'
                  Sleeper::msleep(100);
  
                    invia_comando_X(1,"SPA? 1 1",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 2",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 3",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 4",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 8",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 10",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 11",NULL);
                  read_Xanswer();
                   invia_comando_X(1,"SPA? 1 14",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 15",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 19",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 20",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 21",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 22",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 23",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 24",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 48",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 49",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 50",NULL);
                  read_Xanswer();
                      invia_comando_X(1,"SPA? 1 60",NULL);
                  read_Xanswer();

                  X_selected_motor_is_valid=true;

                 break;}

        case(6): { //"VT80-200" 200mm code: 62309160

                  CurrentAction->setText("Initialising: VT80-200 - 200mm code: 62309160");

                  invia_comando_X(1,"SPA"," 62309160"); 
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for VT80-200 linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);
                  invia_comando_X(1,"SPA"," 1 1000");
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 2 1000");
                  Sleeper::msleep(100);          
                  invia_comando_X(1,"SPA"," 3 200");
                  Sleeper::msleep(100);         
                  invia_comando_X(1,"SPA"," 4 1000");       
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 14 2000");
                  Sleeper::msleep(100);     
                  invia_comando_X(1,"SPA"," 15 1"); ////0.064285257
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 19 0");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 20 1");        //si usano i limit switches dell'hardware (top e bottom) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 21 200");       //Max spostamento in positivo da home (0)  
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 22 0.5");   //Value at Reference position
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 23 0.5");  //Distanza tra Ref e Neg-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 47 199.5");    //Distanza tra Ref e Pos-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 24 0");          //0 i limit switches sono attivi alti...va come 49
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 48 0.0000");  //Max spostamento in negativo da home (0) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 49 0");        //0 non si inverte il riferimento 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 50 0");      //0 Abilita lo stop ai limit switches dell'hardware    
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 8 0.5");    //8 10 e 11 per ultimi senno' li ricambia 0.035156
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 10 50");   // velocita'
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 11 200");  //Accelerazione
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 117442049 MM");  //Unita'
                  Sleeper::msleep(100);
  
                    invia_comando_X(1,"SPA? 1 1",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 2",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 3",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 4",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 8",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 10",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 11",NULL);
                  read_Xanswer();
                   invia_comando_X(1,"SPA? 1 14",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 15",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 19",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 20",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 21",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 22",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 23",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 24",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 48",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 49",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 50",NULL);
                  read_Xanswer();
                      invia_comando_X(1,"SPA? 1 60",NULL);
                  read_Xanswer();

                  X_selected_motor_is_valid=true;

                 break;}

        case(7): { //"VT80-300" 300mm code: 62309180

                  CurrentAction->setText("Initialising: VT80-300 - 300mm code: 62309180");

                  invia_comando_X(1,"SPA"," 62309180"); 
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for VT80-300 linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);
                  invia_comando_X(1,"SPA"," 1 1000");
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 2 1000");
                  Sleeper::msleep(100);          
                  invia_comando_X(1,"SPA"," 3 200");
                  Sleeper::msleep(100);         
                  invia_comando_X(1,"SPA"," 4 1000");       
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 14 2000");
                  Sleeper::msleep(100);     
                  invia_comando_X(1,"SPA"," 15 1"); ////0.064285257
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 19 0");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 20 1");        //si usano i limit switches dell'hardware (top e bottom) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 21 300");       //Max spostamento in positivo da home (0)  
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 22 0.5");   //Value at Reference position
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 23 0.5");  //Distanza tra Ref e Neg-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 47 299.5");    //Distanza tra Ref e Pos-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 24 0");          //0 i limit switches sono attivi alti...va come 49
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 48 0.0000");  //Max spostamento in negativo da home (0) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 49 0");        //0 non si inverte il riferimento 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 50 0");      //0 Abilita lo stop ai limit switches dell'hardware    
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 8 0.5");    //8 10 e 11 per ultimi senno' li ricambia 0.035156
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 10 50");   // velocita'
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 11 200");  //Accelerazione
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 117442049 MM");  //Unita'
                  Sleeper::msleep(100);
  
                    invia_comando_X(1,"SPA? 1 1",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 2",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 3",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 4",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 8",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 10",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 11",NULL);
                  read_Xanswer();
                   invia_comando_X(1,"SPA? 1 14",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 15",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 19",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 20",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 21",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 22",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 23",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 24",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 48",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 49",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 50",NULL);
                  read_Xanswer();
                      invia_comando_X(1,"SPA? 1 60",NULL);
                  read_Xanswer();

                  X_selected_motor_is_valid=true;

                 break;}

        case(8): { //"VT80-400" 400mm code: 62309160

                  CurrentAction->setText("Initialising: VT75-400 - 400mm code: 62309150");

                  invia_comando_X(1,"SPA"," 62309160"); 
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for VT80-200 linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);
                  invia_comando_X(1,"SPA"," 1 1000");
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 2 1000");
                  Sleeper::msleep(100);          
                  invia_comando_X(1,"SPA"," 3 200");
                  Sleeper::msleep(100);         
                  invia_comando_X(1,"SPA"," 4 1000");       
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 14 2000");
                  Sleeper::msleep(100);     
                  invia_comando_X(1,"SPA"," 15 1"); ////0.064285257
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 19 0");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 20 1");        //si usano i limit switches dell'hardware (top e bottom) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 21 400");       //Max spostamento in positivo da home (0)  
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 22 0.5");   //Value at Reference position
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 23 0.5");  //Distanza tra Ref e Neg-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 47 399.5");    //Distanza tra Ref e Pos-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 24 0");          //0 i limit switches sono attivi alti...va come 49
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 48 0.0000");  //Max spostamento in negativo da home (0) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 49 0");        //0 non si inverte il riferimento 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 50 0");      //0 Abilita lo stop ai limit switches dell'hardware    
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 8 0.5");    //8 10 e 11 per ultimi senno' li ricambia 0.035156
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 10 50");   // velocita'
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 11 200");  //Accelerazione
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 117442049 MM");  //Unita'
                  Sleeper::msleep(100);
  
                    invia_comando_X(1,"SPA? 1 1",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 2",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 3",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 4",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 8",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 10",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 11",NULL);
                  read_Xanswer();
                   invia_comando_X(1,"SPA? 1 14",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 15",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 19",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 20",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 21",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 22",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 23",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 24",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 48",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 49",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 50",NULL);
                  read_Xanswer();
                      invia_comando_X(1,"SPA? 1 60",NULL);
                  read_Xanswer();

                  X_selected_motor_is_valid=true;

                 break;}

        case(9): { //"VT80-600" 200mm code: 62309160

                  CurrentAction->setText("Initialising: VT75-600 - 600mm code: 62309150");

                  invia_comando_X(1,"SPA"," 62309160"); 
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for VT80-200 linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);
                  invia_comando_X(1,"SPA"," 1 1000");
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 2 1000");
                  Sleeper::msleep(100);          
                  invia_comando_X(1,"SPA"," 3 200");
                  Sleeper::msleep(100);         
                  invia_comando_X(1,"SPA"," 4 1000");       
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 14 2000");
                  Sleeper::msleep(100);     
                  invia_comando_X(1,"SPA"," 15 1"); ////0.064285257
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 19 0");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 20 1");        //si usano i limit switches dell'hardware (top e bottom) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 21 600");       //Max spostamento in positivo da home (0)  
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 22 0.5");   //Value at Reference position
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 23 0.5");  //Distanza tra Ref e Neg-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 47 599.5");    //Distanza tra Ref e Pos-lim
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 24 0");          //0 i limit switches sono attivi alti...va come 49
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 48 0.0000");  //Max spostamento in negativo da home (0) 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 49 0");        //0 non si inverte il riferimento 
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 50 0");      //0 Abilita lo stop ai limit switches dell'hardware    
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 8 0.5");    //8 10 e 11 per ultimi senno' li ricambia 0.035156
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 10 50");   // velocita'
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 11 200");  //Accelerazione
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 117442049 MM");  //Unita'
                  Sleeper::msleep(100);
  
                    invia_comando_X(1,"SPA? 1 1",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 2",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 3",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 4",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 8",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 10",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 11",NULL);
                  read_Xanswer();
                   invia_comando_X(1,"SPA? 1 14",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 15",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 19",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 20",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 21",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 22",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 23",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 24",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 48",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 49",NULL);
                  read_Xanswer();
                    invia_comando_X(1,"SPA? 1 50",NULL);
                  read_Xanswer();
                      invia_comando_X(1,"SPA? 1 60",NULL);
                  read_Xanswer();

                  X_selected_motor_is_valid=true;

                 break;}



       }

  qDebug()<<"...leaving motor selection switch...";
  Sleeper::msleep(500);

  if(X_selected_motor_is_valid)   //// to avoid case 0 initing
   {
    qDebug()<<"Changing X velocity....."<<'\n'; invia_comando_X(1,"VEL 1 5",NULL);   Sleeper::msleep(100); 
    qDebug()<<"Starting X servo motor....."<<'\n'; invia_comando_X(1,"SVO 1 1",NULL);   Sleeper::msleep(500); ;
    qDebug()<<"Going to X negative limit......"<<'\n'; invia_comando_X(1,"FNL 1",NULL);
    CheckXOnTarget(); Sleeper::msleep(50); 
      while(Xmoving)
       { CheckXOnTarget(); Sleeper::msleep(100);
       }
   qDebug()<<"Defining reference position......"<<'\n';
   invia_comando_X(1,"DEF 1",NULL); 
   Sleeper::msleep(300);

   qDebug()<<"Going to reference position......"<<'\n'; //invia_comando_X(1,"FRF 1",NULL); 
   qDebug()<<"wait........................"<<'\n'; 

   switch (motor_selectedX)
      {
       case(1): 
          { 
           char sx[100];
           sprintf(sx,"%f",25.0000);
           invia_comando_X(1,"MOV",sx);
                  
           CheckXOnTarget(); Sleeper::msleep(100); 
           while(Xmoving)
                { CheckXOnTarget(); Sleeper::msleep(100);}
           break;
           }

        case(2): 
           { 
            char sx[100];
            sprintf(sx,"%f",50.0000);
            invia_comando_X(1,"MOV",sx);
                  
            CheckXOnTarget(); Sleeper::msleep(100);   
             while(Xmoving)
                  { CheckXOnTarget(); Sleeper::msleep(100);}
            break;
           }

        case(3): 
           { 
            char sx[100];
            sprintf(sx,"%f",100.0000);
            invia_comando_X(1,"MOV",sx);
                  
            CheckXOnTarget(); Sleeper::msleep(100);   
            while(Xmoving)
                 { CheckXOnTarget(); Sleeper::msleep(100);}
            break;
           }

        case(4): 
           { 
            char sx[100];
            sprintf(sx,"%f",50.0000);
            invia_comando_X(1,"MOV",sx);
                  
            CheckXOnTarget(); Sleeper::msleep(100);   
            while(Xmoving)
                 { CheckXOnTarget(); Sleeper::msleep(100);}
            break;
           }
        case(5): 
           { 
            char sx[100];
            sprintf(sx,"%f",75.0000);
            invia_comando_X(1,"MOV",sx);
                  
            CheckXOnTarget(); Sleeper::msleep(100);   
            while(Xmoving)
                 { CheckXOnTarget(); Sleeper::msleep(100);}
            break;
           }

        case(6): 
           { 
            char sx[100];
            sprintf(sx,"%f",100.0000);
            invia_comando_X(1,"MOV",sx);
                  
            CheckXOnTarget(); Sleeper::msleep(100);   
            while(Xmoving)
                 { CheckXOnTarget(); Sleeper::msleep(100);}
            break;
           }
        case(7): 
           { 
            char sx[100];
            sprintf(sx,"%f",150.0000);
            invia_comando_X(1,"MOV",sx);
                  
            CheckXOnTarget(); Sleeper::msleep(100);   
            while(Xmoving)
                 { CheckXOnTarget(); Sleeper::msleep(100);}
            break;
           }
        case(8): 
           { 
            char sx[100];
            sprintf(sx,"%f",200.0000);
            invia_comando_X(1,"MOV",sx);
                  
            CheckXOnTarget(); Sleeper::msleep(100);   
            while(Xmoving)
                 { CheckXOnTarget(); Sleeper::msleep(100);}
            break;
           }
        case(9): 
           { 
            char sx[100];
            sprintf(sx,"%f",300.0000);
            invia_comando_X(1,"MOV",sx);
                  
            CheckXOnTarget(); Sleeper::msleep(100);   
            while(Xmoving)
                 { CheckXOnTarget(); Sleeper::msleep(100);}
            break;
           }
       }




   switch (motor_selectedX)
       {
        case(1): 
           { //"M404-2PD" 50mm
            MOVE_TO_X_doubleSpinBox->setMaximum(50);
            MOVE_TO_X_doubleSpinBox->setValue(25);
            SET_SCAN_X_MIN_doubleSpinBox->setMaximum(50);
            SET_SCAN_X_MIN_doubleSpinBox->setValue(25);
            *(shared_memory_cmd+50)=25000;
            Xmin =25000;

            SET_SCAN_X_MAX_doubleSpinBox->setMaximum(50);
            SET_SCAN_X_MAX_doubleSpinBox->setValue(25);
            *(shared_memory_cmd+51)=25000;
            Xmax =25000;
            break;
        }
        case(2): 
           { //"M404-4PD" 100mm code:
            MOVE_TO_X_doubleSpinBox->setMaximum(100);
            MOVE_TO_X_doubleSpinBox->setValue(50);
            SET_SCAN_X_MIN_doubleSpinBox->setMaximum(100);
            SET_SCAN_X_MIN_doubleSpinBox->setValue(50);
            *(shared_memory_cmd+50)=50000;
            Xmin =50000;
            SET_SCAN_X_MAX_doubleSpinBox->setMaximum(100);
            SET_SCAN_X_MAX_doubleSpinBox->setValue(50);
            *(shared_memory_cmd+51)=50000;
            Xmax =50000;
            break;
           }
        case(3): 
           { //"M404-8PD" 200mm code:
            MOVE_TO_X_doubleSpinBox->setMaximum(200);
            MOVE_TO_X_doubleSpinBox->setValue(100);
            SET_SCAN_X_MIN_doubleSpinBox->setMaximum(200);
            SET_SCAN_X_MIN_doubleSpinBox->setValue(100);
            *(shared_memory_cmd+50)=100000;
            Xmin =100000;
            SET_SCAN_X_MAX_doubleSpinBox->setMaximum(200);
            SET_SCAN_X_MAX_doubleSpinBox->setValue(100);
            *(shared_memory_cmd+51)=100000;
            Xmax =100000;
            break;
           }
        case(4): 
           { //"VT80-100" 100mm code: 62309140
            MOVE_TO_X_doubleSpinBox->setMaximum(100);
            MOVE_TO_X_doubleSpinBox->setValue(50);
            SET_SCAN_X_MIN_doubleSpinBox->setMaximum(100);
            SET_SCAN_X_MIN_doubleSpinBox->setValue(50);
            *(shared_memory_cmd+50)=50000;
            Xmin =50000;
            SET_SCAN_X_MAX_doubleSpinBox->setMaximum(100);
            SET_SCAN_X_MAX_doubleSpinBox->setValue(50);
            *(shared_memory_cmd+51)=50000;
            Xmax =50000;
            break;
           }
        case(5): 
           { //"VT80-150" 150mm code: 62309150
            MOVE_TO_X_doubleSpinBox->setMaximum(150);
            MOVE_TO_X_doubleSpinBox->setValue(75);
            SET_SCAN_X_MIN_doubleSpinBox->setMaximum(150);
            SET_SCAN_X_MIN_doubleSpinBox->setValue(75);
            *(shared_memory_cmd+50)=75000;
            Xmin =75000;
            SET_SCAN_X_MAX_doubleSpinBox->setMaximum(150);
            SET_SCAN_X_MAX_doubleSpinBox->setValue(75);
            *(shared_memory_cmd+51)=75000;
            Xmax =75000;
            break;
           }
        case(6): 
           { //"VT80-200" 200mm code: 62309160
            MOVE_TO_X_doubleSpinBox->setMaximum(200);
            MOVE_TO_X_doubleSpinBox->setValue(100);
            SET_SCAN_X_MIN_doubleSpinBox->setMaximum(200);
            SET_SCAN_X_MIN_doubleSpinBox->setValue(100);
            *(shared_memory_cmd+50)=100000;
            Xmin =100000;
            SET_SCAN_X_MAX_doubleSpinBox->setMaximum(200);
            SET_SCAN_X_MAX_doubleSpinBox->setValue(100);
            *(shared_memory_cmd+51)=100000;
            Xmax =100000;
            break;
           }
        case(7): 
           { //"VT80-300" 300mm code: 62309180
            MOVE_TO_X_doubleSpinBox->setMaximum(300);
            MOVE_TO_X_doubleSpinBox->setValue(150);
            SET_SCAN_X_MIN_doubleSpinBox->setMaximum(300);
            SET_SCAN_X_MIN_doubleSpinBox->setValue(150);
            *(shared_memory_cmd+50)=150000;
            Xmin =150000;
            SET_SCAN_X_MAX_doubleSpinBox->setMaximum(300);
            SET_SCAN_X_MAX_doubleSpinBox->setValue(150);
            *(shared_memory_cmd+51)=150000;
            Xmax =150000;
            break;
           }
        case(8): 
           { //"VT75-400" 400mm code: 62309160
            MOVE_TO_X_doubleSpinBox->setMaximum(400);
            MOVE_TO_X_doubleSpinBox->setValue(200);
            SET_SCAN_X_MIN_doubleSpinBox->setMaximum(400);
            SET_SCAN_X_MIN_doubleSpinBox->setValue(200);
            *(shared_memory_cmd+50)=200000;
            Xmin =200000;
            SET_SCAN_X_MAX_doubleSpinBox->setMaximum(400);
            SET_SCAN_X_MAX_doubleSpinBox->setValue(200);
            *(shared_memory_cmd+51)=200000;
            Xmax =200000;
            break;
           }
        case(9): 
           { //"VT75-600" 600mm code: 62309160
            MOVE_TO_X_doubleSpinBox->setMaximum(600);
            MOVE_TO_X_doubleSpinBox->setValue(300);
            SET_SCAN_X_MIN_doubleSpinBox->setMaximum(600);
            SET_SCAN_X_MIN_doubleSpinBox->setValue(300);
            *(shared_memory_cmd+50)=300000;
            Xmin =300000;
            SET_SCAN_X_MAX_doubleSpinBox->setMaximum(600);
            SET_SCAN_X_MAX_doubleSpinBox->setValue(300);
            *(shared_memory_cmd+51)=300000;
            Xmax =300000;
            break;
           }


       }

  Enable_TabWidget_3_4_XY();


   V=1;
   int V_adc=V*1000;
	printf("V_adc:%d\n", V_adc);
   *(shared_memory_cmd+67)=V_adc;
   tempoPos=Px/V;
   qDebug()<<"velocità "<<V<<"mm/s"<<"Scrittura posizione ogni "<<tempoPos<<" ms";
   char v[10];
   sprintf(v,"%f",V);
   invia_comando_X(1,"VEL",v);  



  InitPhaseX=true;
	
  if(TimerActive==false)
  {timer->start(interval); TimerActive=true;}
  okX=false;
  InitX=true;
  invia_comando_X(1,"ERR?",NULL);
  read_Xanswer();
  }
  else qDebug()<<".... motor selection not valid..  ..or driver missing....";
   {X_selected_motor_is_valid=false; okX=false;}

  pushButton_tab_2_2Y->setEnabled(true); // re-enabling Y and Z init button
  INIT_Z_pushButton->setEnabled(true);

}
















