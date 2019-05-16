#include "mainwindow.h"
#include <../Shm.h>
#include <QtGui>
using namespace std;


/////////variabili e funzioni definite altrove ma necessarie/////////////////////////
extern bool MapIsOpened,OnLine;
extern bool energychanged;
extern int NshiftX,NshiftY;
extern int *shared_memory2;
extern int *shared_memory3;
extern int *shared_memory;
extern int *shared_memory_cmd;
extern int mempos;
extern double ChMin, ChMax;
extern int pixel_Xstep; extern int pixel_Ystep;
extern int Xmin_map, Xmax_map, Ymin_map, Ymax_map;
///////////////Variables for the composed map visualization (sum of three different elements)///////////
extern int ChMin1, ChMax1, ChMin2, ChMax2, ChMin3, ChMax3;
/////////////////////////////////////////////////////////////////////////////
////Variables for multidetector expansion/////
extern bool digichannel0sel, digichannel1sel, digichannel0and1sel;

// Included for the sum of spectra

#include <time.h>
#define ia 106
#define ic 1283
#define im 6075

long *idum;
long seed;

int jlo, jhi;
int callstorandom = 0;


void setseed() {

    time_t timer;
    struct tm y2k = {};
    double seconds;

    y2k.tm_hour = 0;	y2k.tm_min = 0; y2k.tm_sec = 0;
    y2k.tm_year = 15;	y2k.tm_mon = 0; y2k.tm_mday = 1;

    time(&timer);

    seconds = difftime(timer,mktime(&y2k));

    printf("%.f seconds since January 1, 2000 in the current timezone\n", seconds);

    /* The following sets the seed according to the system's time. The sequence can overflow for calls over 1E5, so
     * a new seed should be set after the function has been called ~100,000 times.
     */

    seed = seconds;
    idum = &seed;
}

long ranqd1(long *idum, int jlo, int jhi) {

    long j;
    *idum=(*idum*ia+ic) % im;
    j=jlo+((jhi-jlo+1)*(*idum))/im;

    callstorandom++;
    return j;
}


void MainWindow::LoadNewFileWithNoCorrection_SHM() {
    bool firstData=true;
    int casenumber=100;
    double vectorMap[20000]={0};

    long dataread ; int j=0; int numdati=0;
    double const_a=1, const_b=0;

    if (*(shared_memory+24)==1) {

        // Called if energy calibration is active in the spectrum visualization program.
        QFile file("Calibration.txt");
        file.open(QIODevice::ReadOnly);
        QString a,b;
        a = file.readLine();
        const_a=a.toDouble();
        b = file.readLine();
        const_b=b.toDouble();
        file.close();

        // The value of energychanged is couple to *(shared_memory+24), the if statement below could be appended as is above.
        if ( energychanged == true) {
            ChMin=ChMin/1000;
            ChMax=ChMax/1000;

            ChMin=(ChMin-const_b)/const_a;
            ChMax=(ChMax-const_b)/const_a;
            energychanged=false;
        }
    }
    qDebug()<<"Lower channel bound:\t"<<ChMin<<"\nUpper channel bound:\t"<<ChMax<<"\nCalibration gradient:\t"<<const_a<<"\nCalibration offset:\t"<<const_b<<'\n';

    if(OnLine==true) {
        OnLine=false;
        ViewOnLineAct->setChecked(false);
        hideImage();
    }

    if(MapIsOpened==true) {
        hideImage();
    }

    int Ntot=*(shared_memory2+4);    // Number of records in memory.
    qDebug()<<"... Records in memory: "<<Ntot;
    mempos=0;

    int riga_mappa=1,   colonna_mappa=1;
    int Xprecedente,    Yprecedente;
    int jlo, jhi;

    setseed();
    while(j<Ntot) {

        dataread =*(shared_memory2+11+j);
        if ( dataread >= 50000000 && firstData )  casenumber=0;           // First x position --> mette da parte x e y
        if ( dataread >= 50000000 && !firstData ) casenumber=1;          // Other positions  --> scrive tutto il vettore precedente, compreso l'integrale
        if ( dataread < 50000000 ) casenumber=2;                         // Energy

        switch (casenumber) {

        case 0:
            firstData= false;
            dataread = dataread - 50000000;
            // The following if statement is not needed, dataread in this case is already specified as > 50000000
            if ( dataread < 0 ) {
                dataread = 0;
            }

            vectorMap[j]=dataread;

            Xprecedente=dataread ;   ///serve per fare bene lo shift Y
            j++;

            dataread = *(shared_memory2+11+j);
            dataread = dataread - 60000000;
            if ( dataread < 0 ) {
                dataread =0;
            }
            Yprecedente=dataread ;   ///serve per fare bene lo shift X
            vectorMap[j]=dataread ;
            j++;
            break;

        case 1:

            for(int vectorSize=0; vectorSize<20000;vectorSize++) {
                // Writes the preceding vector to shared memory
                if(vectorSize<3) {
                    *(shared_memory3+vectorSize+mempos)=vectorMap[vectorSize];
                    numdati++;
                }
                else {
                    if(vectorMap[vectorSize]!=0) {
                        *(shared_memory3+numdati+mempos)=vectorMap[vectorSize];
                        numdati++;
                        *(shared_memory3+numdati+mempos)=vectorSize-3;
                        numdati++;
                    }
                }
                vectorMap[vectorSize]=0;
            }

            *(shared_memory3+numdati+mempos)=-1;
            numdati++;

		      mempos=mempos+numdati;
		      numdati=0;
                      dataread =dataread -50000000;
                      if(dataread <0)dataread =0;

              vectorMap[0]=dataread ;

                      if(dataread >Xprecedente) colonna_mappa++; //si passa a una nuova colonna della mappa se scan YX
                      Xprecedente=dataread ;

                      j++;

              dataread =*(shared_memory2+11+j);
                      dataread =dataread -60000000;
                      if(dataread <0)dataread =0;
		    //    qDebug()<<"Y"<<lettura<<'\n'; 
                      if (colonna_mappa%2==0 && dataread >NshiftY) vectorMap[1]=dataread -NshiftY; //shift solo alle colonne pari
                      else vectorMap[1]=dataread ;
                      if(dataread >Yprecedente) riga_mappa++; //si passa a una nuova riga della mappa se scan XY
                      if (riga_mappa%2==0 && vectorMap[0]>NshiftX) vectorMap[0]=vectorMap[0]-NshiftX; //shift solo alle righe pari
                      Yprecedente=dataread ;
		      j++;
                      break;

        case 2:

            if ( dataread > 19999999 && dataread < 30000000) {
                if ( *(shared_memory_cmd+100) == 1 ) {
                    j++;
                    break;
                }
                else {
                    dataread = dataread -20000000;
                }
            }

            if( dataread > 30000000 ) {
                if ( *(shared_memory_cmd+100) == 0 ) {
                    j++;
                    break;
                }
                else if ( *(shared_memory_cmd+100) == 2 ) {
                    if ( callstorandom > 50000 ) {
                        setseed();
                        callstorandom=0;
                    }

                    dataread = dataread - 30000000;

                    jlo = (((dataread-1)*(*(shared_memory_cmd+101)))+(*(shared_memory_cmd+102)))/(*(shared_memory_cmd+103));
                    jhi = ((dataread*(*(shared_memory_cmd+101)))+(*(shared_memory_cmd+102)))/(*(shared_memory_cmd+103));
                    dataread = ranqd1(idum,jlo,jhi);

                }
                else {
                    dataread =dataread -30000000;
                }
            }

            vectorMap[dataread +3]=vectorMap[dataread +3]+1; // il vettore e' in shift di 3 posizioni
            if ( dataread >= ChMin && dataread <= ChMax ) {
                vectorMap[2]=vectorMap[2]+1;
            }
            j++;
            break;
        }
    }

    for(int entry=0; entry<20000;entry++) {

        // This last for serves to write the last pixel of memory
        if(entry<3) {
            //x, y, I
            *(shared_memory3+entry+mempos)=vectorMap[entry];
			numdati++;
		}
        else {
            if(vectorMap[entry]!=0) {
                *(shared_memory3+numdati+mempos)=vectorMap[entry];
                numdati++;
                *(shared_memory3+numdati+mempos)=entry-3;
                numdati++;
            }
        }
        vectorMap[entry]=0;
    }

    *(shared_memory3+numdati+mempos)=-1;
	numdati++;

    qDebug()<<"... Map correctly loaded\n"
              "... No pixel correction applied";
    *(shared_memory3+numdati+mempos)=-2;
}

void MainWindow::LoadNewFileWithCorrection_SHM()             // Funziona solo con ScanXY e fa solo shift X
{
  qDebug()<<"correction";
  bool firstData=true; bool errorXup=false; bool errorXdown=false;
  int casenumber=100; int LastX=0; double newX;
  double vectorMap[20000]={0};
  float first; int skippedLines=0;
  int j=0; int mempos=0; int numdati=0;
  int lettura;
  int riga_mappa=1; int Yprecedente;
  double const_a=1, const_b=0;

int number_of_corrected_pixel=0;

  if (*(shared_memory+24)==1){
  QFile file("Calibration.txt");
       file.open(QIODevice::ReadOnly);
       QString a,b;
       a = file.readLine();
       const_a=a.toDouble();
       b = file.readLine();
       const_b=b.toDouble();
       file.close();
       ChMin=ChMin/1000;
       ChMax=ChMax/1000;
       //qDebug()<<"ChMin:"<<ChMin<<"ChMax:"<<ChMax<<"const_a:"<<const_a<<"const_b:"<<const_b<<'\n';
       ChMin=(ChMin-const_b)/const_a;
       ChMax=(ChMax-const_b)/const_a;
       //qDebug()<<"ChMin:"<<ChMin<<"ChMax:"<<ChMax<<"const_a:"<<const_a<<"const_b:"<<const_b<<'\n';
  }

   if(OnLine==true)
    {
      OnLine=false;
//      ViewOnLineAct->setChecked(false);
      hideImage();

     }
 
      if(MapIsOpened==true) 
            {
              hideImage();
              //MapShowAct->setChecked(false);
              //MapShowAct_SHM->setChecked(false);
              //MapHideAct->setChecked(true);
            }
     
int Ntot=*(shared_memory2+4);    //// Numero dati in memoria
  qDebug()<<"NTOT "<<Ntot<<'\n'; 
  mempos=0;

     // QString outFilename="readhistoSHM.txt";
    //  QFile Positionfile(outFilename); 
    //  Positionfile.open(QIODevice::WriteOnly | QIODevice::Text);
    //  QTextStream out(&Positionfile);


  while(j<Ntot)
    {
      lettura=*(shared_memory2+11+j);
       
          if (lettura>=50000000 && firstData && !errorXup && !errorXdown) casenumber=0;                          //inizia per X ed è il primo
          if (lettura>=50000000  && !firstData && !errorXup && !errorXdown) casenumber=1;                        //inizia per X e non è il primo
          if (lettura<50000000 && !errorXup && !errorXdown) casenumber=2;  // Data lines            //energie
          if (errorXup) casenumber=3;
          if (errorXdown) casenumber=4;


          switch (casenumber)
              {  

	            case 0:    
                     {
		       firstData=false; 
		      lettura=lettura-50000000;
                      if(lettura<0)lettura=0;
                      vectorMap[j]=lettura;
                      LastX=lettura;
		      j++;
		      lettura=*(shared_memory2+11+j);
                      lettura=lettura-60000000;
                      if(lettura<0)lettura=0;
                      vectorMap[j]=lettura;
                      Yprecedente=lettura;  //per fare bene lo shift
		      j++;
                      break;
		     }

               case 1:                                              //controlla se ci sono errori
                     {lettura=lettura-50000000;
                      if(lettura<0)lettura=0;
                      first=lettura;
                      j++;
                      lettura=*(shared_memory2+11+j);
                      lettura=lettura-60000000;
                      if(lettura<0)lettura=0;
                      j++;
                      vectorMap[1]=lettura; 
                      if(lettura>Yprecedente) riga_mappa++; //si passa a una nuova riga della mappa
                      Yprecedente=lettura;
                      //qDebug()<<lettura;


                      if(!firstData && (first-LastX)>pixel_Xstep) // coordinates are in microns
                        {
                          skippedLines=abs(first-LastX);   //numero di X saltate
                          errorXup=true; 
                          newX=first;
			  number_of_corrected_pixel++;
 //                         qDebug()<<"Y= "<<vectorMap[1]<<"a X= "<<LastX<<"segue "<<newX<<" UpError"; 
                              
                         }

                      else if(!firstData && ((LastX - first)>pixel_Xstep))
                        {
                          skippedLines=(LastX-first);
                          errorXdown=true; 
                          newX=first;
                          number_of_corrected_pixel++;
//                          qDebug()<<"Y= "<<vectorMap[1]<<"a X= "<<LastX<<"segue "<<newX<<" DownError"; 
                        }



                      else
                      { 
                          for(int vectorSize=0; vectorSize<20000;vectorSize++)
		          {
			   if(vectorSize<3)
			    {
			     *(shared_memory3+vectorSize+mempos)=vectorMap[vectorSize];
			     numdati++;
                            // out<<vectorMap[vectorSize]<<" ";
			    }
			   else
			    {
			     if(vectorMap[vectorSize]!=0)
			       {
				 *(shared_memory3+numdati+mempos)=vectorMap[vectorSize];
			 	 numdati++; // out<<vectorMap[vectorSize]<<" ";
				 *(shared_memory3+numdati+mempos)=vectorSize-3;
				 numdati++; // out<<vectorSize<<" ";

			       }
			     
			    }
			   vectorMap[vectorSize]=0;
		          }//chiude for

		         *(shared_memory3+numdati+mempos)=-1;
		         // out<<"-1\n";
		         numdati++;
                         mempos=mempos+numdati;
		         numdati=0;
                         if (riga_mappa%2==0 && first>NshiftX) vectorMap[0]=qRound(first-NshiftX);  //shift
                         else vectorMap[0]=qRound(first);         
                         LastX=qRound(first);
                      }//chiude else di se non ci sono errori
                      break;
                     }



               case 2:    
                     {
                      vectorMap[lettura+3]=vectorMap[lettura+3]+1; // il vettore e' in shift di 3 posizioni 
                      if(lettura>=ChMin && lettura<=ChMax){vectorMap[2]=vectorMap[2]+1;}//aggiorna integrale
		      j++;
                      break;

                     }
               case 3:  //Correzione UpError
                     {for (int missed=0; missed<skippedLines; missed++)
                        {
                         if (riga_mappa%2==0 && LastX>NshiftX) vectorMap[0]=LastX-NshiftX+missed;//shift
                         else                                                    
                        vectorMap[0]=LastX+missed;
 //                      qDebug()<<"Integrale da dividere shm "<<vectorMap[2]<<"in "<<skippedLines<<'\n';


//qDebug()<<"Caso3 ";

		      for(int vectorSize=0; vectorSize<20000;vectorSize++)
		       {
			 if(vectorSize<3)
			   {
			     if(vectorSize<2)  *(shared_memory3+vectorSize+mempos)=vectorMap[vectorSize];
                             else  *(shared_memory3+vectorSize+mempos)=vectorMap[vectorSize]/skippedLines;
			    // out<<vectorMap[vectorSize]<<" ";
			     numdati++;
                             
			   }
			 else
			   {
			     if(vectorMap[vectorSize]!=0)
			       {
				 *(shared_memory3+numdati+mempos)=vectorMap[vectorSize]/skippedLines;
			 	 numdati++;// out<<vectorMap[vectorSize]<<" ";
				 //qDebug()<<"countsm "<<vectorMap[vectorSize]<<'\n';
				 *(shared_memory3+numdati+mempos)=vectorSize-3;
				 numdati++;// out<<vectorSize<<" ";
				 //qDebug()<<"channel "<<vectorSize<<'\n';

			       }
			     

			   }
			//// vectorMap[vectorSize]=0; messo sotto fuori dal for missing++
		       }
		       *(shared_memory3+numdati+mempos)=-1;
		   // out<<"-1\n";
		       numdati++;

		      mempos=mempos+numdati;
		      numdati=0;
                         } // fine riempimento canali mancanti
                      errorXup=false; 
                      if (riga_mappa%2==0 && newX>NshiftX) vectorMap[0]=qRound(newX-NshiftX);  //shift
                      else vectorMap[0]=qRound(newX);
                      LastX=newX;
                      for(int vectorSize=2; vectorSize<20000;vectorSize++) vectorMap[vectorSize]=0; 
                      break;
                     }

               case 4:  //Correzione DownError    
                     {for (int missed=0; missed<skippedLines; missed++)
                        {
                         if (riga_mappa%2==0 && LastX>NshiftX) vectorMap[0]=LastX-NshiftX-missed;//shift
                         else 
                         vectorMap[0]=LastX-missed;
//                         qDebug()<<"Integrale da dividere shm "<<vectorMap[2]<<"in "<<skippedLines<<'\n';

//qDebug()<<"Caso4 ";

			 for(int vectorSize=0; vectorSize<20000;vectorSize++)
		       {
			 if(vectorSize<3)
			   {
			     if(vectorSize<2) *(shared_memory3+vectorSize+mempos)=vectorMap[vectorSize];
                             else  *(shared_memory3+vectorSize+mempos)=vectorMap[vectorSize]/skippedLines;
			     numdati++;
			   }
			 else
			   {
			     if(vectorMap[vectorSize]!=0)
			       {
				 *(shared_memory3+numdati+mempos)=vectorMap[vectorSize]/skippedLines;
			 	 numdati++;
				 *(shared_memory3+numdati+mempos)=vectorSize-3;
				 numdati++;

			       }
			     

			   }
			/// vectorMap[vectorSize]=0; messo sotto fuori dal for missing
		       }
		       *(shared_memory3+numdati+mempos)=-1;
		   
		       numdati++;

		      mempos=mempos+numdati;
		      numdati=0;

                         } // fine riempimento canali mancanti
                      errorXdown=false; 
                      if (riga_mappa%2==0 && newX>NshiftX) vectorMap[0]=qRound(newX-NshiftX);
                      else vectorMap[0]=qRound(newX);
                      LastX=newX;
                      for(int vectorSize=2; vectorSize<20000;vectorSize++)vectorMap[vectorSize]=0; 
                      break;
                     }
               }
        } //chiude while
    qDebug()<<"Done!!... "<<number_of_corrected_pixel<< "corrections made";
   // Positionfile.close();
    *(shared_memory3+numdati+mempos)=-2;
    j=0;
}

void MainWindow::LoadSHM_SumMap() {

    int j=0, casenumber=10;
    int Ntot=*(shared_memory2+4);    //// Numero dati in memoria
    mempos=0;
    int dataread=0, numdati=0;
    bool firstData=true;
    int vectorMap[20000]={0};
    int riga_mappa=1, colonna_mappa=1; int Xprecedente,Yprecedente;

    if(MapIsOpened==true) {
        hideImage();
    }

    while ( j < Ntot ) {

        dataread=*(shared_memory2+11+j);

        if ( dataread >= 50000000 && firstData ) casenumber=0;           //prima x letta --> mette da parte x e y
        if ( dataread >= 50000000 && !firstData ) casenumber=1;          //altre posizioni --> scrive tutto il vettore precedente, compreso l'integrale
        if ( dataread < 50000000 ) casenumber=2;

        qDebug()<<"... To call switch in LoadSHM_SumMap()";

        switch (casenumber) {

        case 0:

            firstData=false;
            dataread=dataread-50000000;
            if ( dataread < 0 ) {
                dataread=0;
            }

            vectorMap[j]=dataread;
            Xprecedente=dataread;   ///serve per fare bene lo shift Y
            j++;

            dataread=*(shared_memory2+11+j);
            dataread=dataread-60000000;
            if ( dataread < 0 ) {
                dataread=0;
            }

            Yprecedente=dataread;   ///serve per fare bene lo shift X
            vectorMap[j]=dataread;
            j++;
            break;

        case 1:

            for ( int vectorSize=0; vectorSize<20000; vectorSize++) {
                if(vectorSize<5) {
                    //x, y, I
                    *(shared_memory3+vectorSize+mempos)=vectorMap[vectorSize];
                    numdati++;
                }
                else {
                    if ( vectorMap[vectorSize] != 0 ) {
                        *(shared_memory3+numdati+mempos)=vectorMap[vectorSize];
                        numdati++;
                        *(shared_memory3+numdati+mempos)=vectorSize-5;
                        numdati++;
                    }
                }

                vectorMap[vectorSize]=0;
            }

            *(shared_memory3+numdati+mempos)=-1;
            numdati++;

            mempos=mempos+numdati;
            numdati=0;

            dataread=dataread-50000000;
            if (dataread < 0 ) {
                dataread=0;
            }

            vectorMap[0]=dataread;
            if ( dataread > Xprecedente ) {
                //si passa a una nuova colonna della mappa se scan YX
                colonna_mappa++;
            }
            Xprecedente=dataread;
            j++;

            dataread=*(shared_memory2+11+j);
            dataread=dataread-60000000;

            if ( dataread < 0 ) {
                dataread=0;
            }
            if ( colonna_mappa%2==0 && dataread > NshiftY) {
                vectorMap[1]=dataread-NshiftY;
                //shift solo alle colonne pari
            }
            else vectorMap[1]=dataread;

            if ( dataread > Yprecedente ) {
                riga_mappa++;
                //si passa a una nuova riga della mappa se scan XY
            }

            if (riga_mappa%2==0 && vectorMap[0] > NshiftX ) {
                vectorMap[0]=vectorMap[0]-NshiftX;
                //shift solo alle righe pari
            }

            Yprecedente=dataread;
            j++;
            break;

        case 2:
            if ( dataread > 19999999 && dataread < 30000000) {
                if ( *(shared_memory_cmd+100) == 1 ) {
                    j++;
                    break;
                }
                else {
                    dataread = dataread -20000000;
                }
            }

            if( dataread > 30000000 ) {
                if ( *(shared_memory_cmd+100) == 0 ) {
                    j++;
                    break;
                }
                else if ( *(shared_memory_cmd+100) == 2 ) {
                    if ( callstorandom > 50000 ) {
                        setseed();
                        callstorandom=0;
                    }

                    dataread = dataread - 30000000;

                    jlo = (((dataread-1)*(*(shared_memory_cmd+101)))+(*(shared_memory_cmd+102)))/(*(shared_memory_cmd+103));
                    jhi = ((dataread*(*(shared_memory_cmd+101)))+(*(shared_memory_cmd+102)))/(*(shared_memory_cmd+103));
                    dataread = ranqd1(idum,jlo,jhi);

                }
                else {
                    dataread =dataread -30000000;
                }
            }

            vectorMap[dataread+5]=vectorMap[dataread+5]+1;
            if ( dataread > ChMin1 && dataread < ChMax1 ) {
                vectorMap[2]=vectorMap[2]+1;
            }
            if ( dataread > ChMin2 && dataread < ChMax2 ) {
                vectorMap[3]=vectorMap[3]+1;
            }
            if ( dataread > ChMin3 && dataread < ChMax3 ) {
                vectorMap[4]=vectorMap[4]+1;
            }

            j++;
            break;
        }
    }


    for ( int vectorSize=0; vectorSize<20000; vectorSize++ ) {

        if(vectorSize<5) {
            *(shared_memory3+vectorSize+mempos)=vectorMap[vectorSize];
            numdati++;
        }
        else {
            if(vectorMap[vectorSize]!=0) {
                *(shared_memory3+numdati+mempos)=vectorMap[vectorSize];
                numdati++;
                *(shared_memory3+numdati+mempos)=vectorSize-5;
                numdati++;
            }
        }

        vectorMap[vectorSize]=0;
    }

    *(shared_memory3+numdati+mempos)=-1;
    numdati++;

    qDebug()<<"... Composed element map loaded";
    *(shared_memory3+numdati+mempos)=-2;
    displaySumImage_SHM();
}


