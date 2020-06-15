#include "include/MAXRF/mainwindow.h"
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
extern int mempos;
extern double ChMin, ChMax;
extern int PassoX; extern int PassoY;
extern int Xmin_map, Xmax_map, Ymin_map, Ymax_map;
///////////////Variables for the composed map visualization (sum of three different elements)///////////
extern int ChMin1, ChMax1, ChMin2, ChMax2, ChMin3, ChMax3;
/////////////////////////////////////////////////////////////////////////////




void MainWindow::LoadNewFileWithNoCorrection_SHM()
{
  bool firstData=true; bool errorXup=false; bool errorXdown=false;
  int casenumber=100; int LastX=0,LastX_vector=0; double newX; int XtempStorage;
  double vectorMap[160000]={0};
  double first; int skippedLines=0;
  int lettura; int j=0; int numdati=0;
  double const_a=1, const_b=0;

  if (*(shared_memory+24)==1){
  QFile file("Calibration.txt");
       file.open(QIODevice::ReadOnly);
       QString a,b;
       a = file.readLine();
       const_a=a.toDouble();
       b = file.readLine();
       const_b=b.toDouble();
       file.close();
	if(energychanged==true){
      	 ChMin=ChMin/1000;
       	ChMax=ChMax/1000;
      	 //qDebug()<<"ChMin:"<<ChMin<<"ChMax:"<<ChMax<<"const_a:"<<const_a<<"const_b:"<<const_b<<'\n';
       	ChMin=(ChMin-const_b)/const_a;
       	ChMax=(ChMax-const_b)/const_a;
      	 //qDebug()<<"ChMin:"<<ChMin<<"ChMax:"<<ChMax<<"const_a:"<<const_a<<"const_b:"<<const_b<<'\n';
	energychanged=false;
	}

  }

  qDebug()<<"ChMin:"<<ChMin<<"ChMax:"<<ChMax<<"const_a:"<<const_a<<"const_b:"<<const_b<<'\n';



  if(OnLine==true)
    {
      OnLine=false;
      ViewOnLineAct->setChecked(false);
      hideImage();

     }
      if(MapIsOpened==true) 
            {
              hideImage();
              //MapShowAct->setChecked(false);
              //MapShowAct_SHM->setChecked(false);
              //MapHideAct->setChecked(true);
            }



      //QString outFilename="readmemory.txt";
      //QFile Positionfile(outFilename); 
      // Positionfile.open(QIODevice::WriteOnly);
      //QTextStream out(&Positionfile);

      // QFile::remove("filehisto.txt");                                                       //x scrivere filehisto...
      //QString outFilename="filehisto.txt";


  int Ntot=*(shared_memory2+4);    //// Numero dati in memoria
  qDebug()<<"NTOT "<<Ntot<<'\n'; 
  mempos=0;
  bool errore=false;
  int riga_mappa=1, colonna_mappa=1; int Xprecedente,Yprecedente;
  while(j<Ntot)
    {
      lettura=*(shared_memory2+11+j);
          if (lettura>=50000000 && firstData) casenumber=0;           //prima x letta --> mette da parte x e y
          if (lettura>=50000000 && !firstData) casenumber=1;          //altre posizioni --> scrive tutto il vettore precedente, compreso l'integrale
          if (lettura<50000000) casenumber=2;                         //energie

         
          switch (casenumber)
              {  
               case 0:    
                     {firstData=false; 
		      lettura=lettura-50000000;
                      if(lettura<0)lettura=0;
                      vectorMap[j]=lettura;
//   qDebug()<<"X"<<lettura;
                      LastX=lettura;
                      Xprecedente=lettura;   ///serve per fare bene lo shift Y
		      j++;
		      lettura=*(shared_memory2+11+j);
                      lettura=lettura-60000000;
                      if(lettura<0)lettura=0;
//   qDebug()<<"Y"<<lettura<<'\n';
                      Yprecedente=lettura;   ///serve per fare bene lo shift X
                      vectorMap[j]=lettura;
		      j++;
                      break;
		      
                     }
               case 1:    
                     { 
//    qDebug()<<"sono nel caso 1"<<'\n';
		       for(int vectorSize=0; vectorSize<20000;vectorSize++)
		       {
			 if(vectorSize<3)                                                       //x, y, I
			   {
			     *(shared_memory3+vectorSize+mempos)=vectorMap[vectorSize];
			     //out<<vectorMap[vectorSize]<<' ';
			     numdati++;
			   }
			 else                                                                   //scrive i conteggi solo se diversi da 0
			   {
			     if(vectorMap[vectorSize]!=0)
			       {
				 *(shared_memory3+numdati+mempos)=vectorMap[vectorSize];
                                 
				 //out<<vectorMap[vectorSize]<<' ';
                                 //qDebug()<<"vectorMap[vectorSize]"<<vectorMap[vectorSize]<<'\n';
                                 //qDebug()<<"[vectorSize]"<<vectorSize<<'\n';
				 numdati++;
				 *(shared_memory3+numdati+mempos)=vectorSize-3;
				//out<<vectorSize<<' ';
				 numdati++;
			       }			     
			   }
			 vectorMap[vectorSize]=0;
		       }
		       *(shared_memory3+numdati+mempos)=-1;
		       //out<<"-1\n";
		       numdati++;

		      mempos=mempos+numdati;
		      numdati=0;
                      lettura=lettura-50000000;
                      if(lettura<0)lettura=0;
//   qDebug()<<"X"<<lettura<<'\n'; 
		      vectorMap[0]=lettura;
                      LastX=lettura;
                      if(lettura>Xprecedente) colonna_mappa++; //si passa a una nuova colonna della mappa se scan YX                      
                      Xprecedente=lettura;
                      //LastX_vector=vectorMap[0]; 
                      j++;

		      lettura=*(shared_memory2+11+j);
                      lettura=lettura-60000000;   
                      if(lettura<0)lettura=0;
		    //    qDebug()<<"Y"<<lettura<<'\n'; 
                      if (colonna_mappa%2==0 && lettura>NshiftY) vectorMap[1]=lettura-NshiftY; //shift solo alle colonne pari           
                      else vectorMap[1]=lettura;
                      if(lettura>Yprecedente) riga_mappa++; //si passa a una nuova riga della mappa se scan XY
                      if (riga_mappa%2==0 && vectorMap[0]>NshiftX) vectorMap[0]=vectorMap[0]-NshiftX; //shift solo alle righe pari
                      Yprecedente=lettura;
		      j++;
                      break;
                     }


               case 2:    
                     {
//    qDebug()<<"sono nel caso 2"<<'\n';
                      vectorMap[lettura+3]=vectorMap[lettura+3]+1; // il vettore e' in shift di 3 posizioni 
		     
                      if(lettura>=ChMin && lettura<=ChMax){vectorMap[2]=vectorMap[2]+1;}//aggiorna integrale
		      j++;
                      break;
                     }
		           //qDebug()<< (*(shared_memory3+numdati+mempos));
               }
        } //chiude while

  for(int vectorSize=0; vectorSize<20000;vectorSize++) //this last for serves to write the last pixel of memory
	{
		if(vectorSize<3)                                                       //x, y, I
		{
			*(shared_memory3+vectorSize+mempos)=vectorMap[vectorSize];
			numdati++;
		}
		else                                                                   //scrive i conteggi solo se diversi da 0
		{
			if(vectorMap[vectorSize]!=0)
			{
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

    qDebug()<<"Done...!! (no pixel correction)...";
    *(shared_memory3+numdati+mempos)=-2;
    //Positionfile.close();
   
 }




void MainWindow::LoadNewFileWithCorrection_SHM()             // Funziona solo con ScanXY e fa solo shift X
{
  qDebug()<<"correction";
  bool firstData=true; bool errorXup=false; bool errorXdown=false;
  int casenumber=100; int LastX=0; double newX; int YtempStorage;
  double vectorMap[160000]={0};
  float first; int skippedLines=0;
  bool ok; int j=0; int mempos=0; int numdati=0; 
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


                      if(!firstData && (first-LastX)>PassoX) // coordinates are in microns
                        {
                          skippedLines=abs(first-LastX);   //numero di X saltate
                          errorXup=true; 
                          newX=first;
			  number_of_corrected_pixel++;
 //                         qDebug()<<"Y= "<<vectorMap[1]<<"a X= "<<LastX<<"segue "<<newX<<" UpError"; 
                              
                         }

                      else if(!firstData && ((LastX - first)>PassoX))
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

void MainWindow::LoadSHM_SumMap() //loads data from memory to display the sum map of three different elements
{
//printf("ChMin1:%d, ChMax1:%d, ChMin2:%d, ChMax2:%d, ChMin3:%d, ChMax3:%d\n", ChMin1, ChMax1, ChMin2, ChMax2, ChMin3, ChMax3);
int j=0, casenumber=10; 
int Ntot=*(shared_memory2+4);    //// Numero dati in memoria
mempos=0;
int lettura=0, LastX=0, numdati=0;
bool firstData=true;
int vectorMap[160000]={0};
int riga_mappa=1, colonna_mappa=1; int Xprecedente,Yprecedente;

/*if (*(shared_memory+24)==1){
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
  }*/

if(MapIsOpened==true) 
            {
              hideImage();
              //MapShowAct->setChecked(false);
              //MapShowAct_SHM->setChecked(false);
              //MapHideAct->setChecked(true);
            }

while(j<Ntot)
    {
      lettura=*(shared_memory2+11+j);
          if (lettura>=50000000 && firstData) casenumber=0;           //prima x letta --> mette da parte x e y
          if (lettura>=50000000 && !firstData) casenumber=1;          //altre posizioni --> scrive tutto il vettore precedente, compreso l'integrale
          if (lettura<50000000) casenumber=2;                         //energie

         
          switch (casenumber)
              {  
               case 0:    
                     {firstData=false; 
		      lettura=lettura-50000000;
                      if(lettura<0)lettura=0;
                      vectorMap[j]=lettura;
//   qDebug()<<"X"<<lettura;
                      LastX=lettura;
                      Xprecedente=lettura;   ///serve per fare bene lo shift Y
		      j++;
		      lettura=*(shared_memory2+11+j);
                      lettura=lettura-60000000;
                      if(lettura<0)lettura=0;
//   qDebug()<<"Y"<<lettura<<'\n';
                      Yprecedente=lettura;   ///serve per fare bene lo shift X
                      vectorMap[j]=lettura;
		      j++;
                      break;
		      
                     }
               case 1:    
                     { 
//    qDebug()<<"sono nel caso 1"<<'\n';
		       for(int vectorSize=0; vectorSize<20000;vectorSize++)
		       {
			 if(vectorSize<5)                                                       //x, y, I
			   {
			     *(shared_memory3+vectorSize+mempos)=vectorMap[vectorSize];
			     //out<<vectorMap[vectorSize]<<' ';
			     numdati++;
			   }
			 else                                                                   //scrive i conteggi solo se diversi da 0
			   {
			     if(vectorMap[vectorSize]!=0)
			       {
				 *(shared_memory3+numdati+mempos)=vectorMap[vectorSize];
                                 
				 //out<<vectorMap[vectorSize]<<' ';
                                 //qDebug()<<"vectorMap[vectorSize]"<<vectorMap[vectorSize]<<'\n';
                                 //qDebug()<<"[vectorSize]"<<vectorSize<<'\n';
				 numdati++;
				 *(shared_memory3+numdati+mempos)=vectorSize-5;
				//out<<vectorSize<<' ';
				 numdati++;
			       }			     
			   }
			 vectorMap[vectorSize]=0;
		       }
		       *(shared_memory3+numdati+mempos)=-1;
		       //out<<"-1\n";
		       numdati++;

		      mempos=mempos+numdati;
		      numdati=0;
                      lettura=lettura-50000000;
                      if(lettura<0)lettura=0;
//   qDebug()<<"X"<<lettura<<'\n'; 
		      vectorMap[0]=lettura;
                      LastX=lettura;
                      if(lettura>Xprecedente) colonna_mappa++; //si passa a una nuova colonna della mappa se scan YX                      
                      Xprecedente=lettura;
                      //LastX_vector=vectorMap[0]; 
                      j++;

		      lettura=*(shared_memory2+11+j);
                      lettura=lettura-60000000;   
                      if(lettura<0)lettura=0;
		    //    qDebug()<<"Y"<<lettura<<'\n'; 
                      if (colonna_mappa%2==0 && lettura>NshiftY) vectorMap[1]=lettura-NshiftY; //shift solo alle colonne pari           
                      else vectorMap[1]=lettura;
                      if(lettura>Yprecedente) riga_mappa++; //si passa a una nuova riga della mappa se scan XY
                      if (riga_mappa%2==0 && vectorMap[0]>NshiftX) vectorMap[0]=vectorMap[0]-NshiftX; //shift solo alle righe pari
                      Yprecedente=lettura;
		      j++;
                      break;
                     }


               case 2:    
                     {
//    qDebug()<<"sono nel caso 2"<<'\n';
		      vectorMap[lettura+5]=vectorMap[lettura+5]+1;
		      if(lettura>ChMin1 && lettura<ChMax1) {vectorMap[2]=vectorMap[2]+1;}
		      if(lettura>ChMin2 && lettura<ChMax2) {vectorMap[3]=vectorMap[3]+1;}
     		      if(lettura>ChMin3 && lettura<ChMax3) {vectorMap[4]=vectorMap[4]+1;}
		     
                      j++;
                      break;
                     }
		           //qDebug()<< (*(shared_memory3+numdati+mempos));
               }
        } //chiude while

  for(int vectorSize=0; vectorSize<20000;vectorSize++) //this last for serves to write the last pixel of memory
	{
		if(vectorSize<5)                                                       //x, y, I
		{
			*(shared_memory3+vectorSize+mempos)=vectorMap[vectorSize];
			numdati++;
		}
		else                                                                   //scrive i conteggi solo se diversi da 0
		{
			if(vectorMap[vectorSize]!=0)
			{
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

    qDebug()<<"Done...!! (no pixel correction)...";
    *(shared_memory3+numdati+mempos)=-2;
    //Positionfile.close();


displaySumImage_SHM();


}


