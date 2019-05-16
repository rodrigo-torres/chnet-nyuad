// Frao 31/1/2016
//
// motor_INIT_XXX.cpp manages the calls from mainwindos.cpp 
// 
// 
// 
// THREE IDENTICAL MODULES ARE PRESENT IN THIS FILE TO INIT THE MOTORS (X-Y-Z). 
//


#include "mainwindow.h"
#include "../Header.h"
#include <../Shm.h>
#include <time.h>

////////////variabili e funzioni definite altrove ma necessarie////////////////////////////

extern int serialX,interval; extern bool InitX; extern bool InitPhase;
extern bool TimerActive, okX;
extern int invia_comando_X(int chan,const char *comando, const char *parametri);
extern char *read_Xanswer();
extern bool XConnected; extern bool Xmoving; 
//////////////////////////////////////////////////////////////////////////////////////////////////
bool selected_motor_is_valid=false;

extern int motor_selected;

void MainWindow::InizializzazioneX()
{
  //pushButton_tab_2_2Y->setEnabled(false); // disabling Y and Z init button to avoid conflicts
  //INIT_Z_pushButton->setEnabled(false);


  qDebug()<<"...entering motor selection switch...";

      switch (motor_selected)
       {
        case(1): { //"M404-2PD" 50mm

		  ANSWER_lineEdit->setText("Initialising: M-404.2PD - 50mm");

                  invia_comando_X(1,"SPA"," 60 M-404.2PD");  
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for M-404.2PD linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);
                  invia_comando_X(1,"SPA"," 1 220"); //primo numero canale SPA secondo numero valore
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 2 45");          
                  Sleeper::msleep(100);
                  invia_comando_X(1,"SPA"," 3 200");         
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
                  invia_comando_X(1,"SPA"," 47 25.");    //Distanza tra Ref e Pos-lim
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


                  selected_motor_is_valid=true;

                 break;}

        case(2): { //"M404-4PD" 100mm code:

		 // ANSWER_lineEdit->setText("Initialising: M-404.4PD - 100mm");

                  invia_comando_X(1,"SPA"," 60 M-404.4PD");  
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for M-404.4PD linear stage...";
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

                  selected_motor_is_valid=true;

                 break;}

        case(3): { //"M404-8PD" 200mm code:

		  ANSWER_lineEdit->setText("Initialising: M-404.8PD - 200mm");

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

                  selected_motor_is_valid=true;

                 break;}

        case(4): { //"VT80-100" 100mm code: 62309140

		  ANSWER_lineEdit->setText("Initialising: VT80-100 - 100mm code: 62309140");

                  invia_comando_X(1,"SPA"," 62309140"); 
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for VT80-100 linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);
                  invia_comando_X(1,"SPA"," 1 1000"); //primo numero canale SPA secondo numero valore
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

                  selected_motor_is_valid=true;

                 break;}

        case(5): { //"VT80-150" 150mm code: 62309150

                  ANSWER_lineEdit->setText("Initialising: VT80-150 - 150mm code: 62309150");

                  invia_comando_X(1,"SPA"," 62309150"); 
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for VT80-150 linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);
                  invia_comando_X(1,"SPA"," 1 1000"); //primo numero canale SPA secondo numero valore
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

                  selected_motor_is_valid=true;

                 break;}

        case(6): { //"VT80-200" 200mm code: 62309160

                  invia_comando_X(1,"SPA"," 62309160"); 
                  Sleeper::msleep(1200);
                  qDebug()<<"...loading general parameters for VT80-200 linear stage...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...wait while finishing...";
                  Sleeper::msleep(1200);
                  qDebug()<<"...start loading specific parameters...";
                  Sleeper::msleep(500);
                  invia_comando_X(1,"SPA"," 1 1000"); //primo numero canale SPA secondo numero valore
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

                  selected_motor_is_valid=true;

                 break;}

       }

  qDebug()<<"...leaving motor selection switch...";
  Sleeper::msleep(500);

                   

  if(selected_motor_is_valid)   //// to avoid case 0 initing
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



      switch (motor_selected)
       {
        case(1): { 

                  char sx[100];
                  sprintf(sx,"%f",25.0000);
                  invia_comando_X(1,"MOV",sx);
                  
                  CheckXOnTarget(); Sleeper::msleep(100); 
                  while(Xmoving)
                     { CheckXOnTarget(); Sleeper::msleep(100);}
                  break;
                 }

        case(2): { 
                  char sx[100];
                  sprintf(sx,"%f",50.0000);
                  invia_comando_X(1,"MOV",sx);
                  
                  CheckXOnTarget(); Sleeper::msleep(100);   
                  while(Xmoving)
                     { CheckXOnTarget(); Sleeper::msleep(100);}
                  break;
                 }

        case(3): { 

                  char sx[100];
                  sprintf(sx,"%f",100.0000);
                  invia_comando_X(1,"MOV",sx);
                  
                  CheckXOnTarget(); Sleeper::msleep(100);   
                  while(Xmoving)
                     { CheckXOnTarget(); Sleeper::msleep(100);}
                  break;
                 }

       }

    



  
  InitPhase=true;
  if(TimerActive==false)
  {timer->start(interval); TimerActive=true;}
  okX=false;
  InitX=true;
  }
  else qDebug()<<".... motor selection not valid..  ..or driver missing....";
   {selected_motor_is_valid=false; okX=false;}


}





////////////////////// ORIGINAL

/*  if(selected_motor_is_valid)   //// to avoid case 0 initing
  {
  qDebug()<<"Changing X velocity....."<<'\n'; invia_comando_X(1,"VEL 1 5",NULL);   sleep(0.5);
  qDebug()<<"Starting X servo motor....."<<'\n'; invia_comando_X(1,"SVO 1 1",NULL);   sleep(1);
  if(motor_selected==4 || motor_selected==5 || motor_selected==6) //for VT80 series that need to reach the negative limit of the stages
    {qDebug()<<"Going to X negative limit......"<<'\n'; invia_comando_X(1,"FNL 1",NULL);
     CheckXOnTarget(); Sleeper::msleep(250);
      while(Xmoving)
       { CheckXOnTarget(); Sleeper::msleep(100);
       }
    }
  qDebug()<<"Going to X reference position......"<<'\n'; invia_comando_X(1,"FRF 1",NULL); 
  qDebug()<<"wait........................"<<'\n';   
  InitPhase=true;
  if(TimerActive==false)
  {timer->start(interval); TimerActive=true;}
  okX=false;
  InitX=true;
  }
  else qDebug()<<".... motor selection not valid..  ..or driver missing....";
   {selected_motor_is_valid=false; okX=false;}


*/








