#include "../Header.h"
#include "mainwindow.h"

extern int *shared_memory; 
extern int *shared_memory2; 

extern int Xmap[],Ymap[],MaxX,MaxY,MinX,MinY,Xminvero, Yminvero,Xmaxvero, Ymaxvero,MaxX_ori,MinX_ori,MaxY_ori,MinY_ori, PassoX, PassoY;
extern int i; extern int j;

void MainWindow::exportpymca()
{

bool xyscan=false;
QStringList items;
QLabel itemLabel;
items << tr("X-Y Scan") << tr("Y-X Scan");

bool ok;
    QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),
                                         tr("Scan Modality:"), items, 0, false, &ok);
    if (ok && !item.isEmpty()){
        itemLabel.setText(item);


if(item.startsWith("X"))xyscan=true;

int chmax=16384;
int linesnumber=*(shared_memory2+4); //number of integers loaded in the SHM
int sample=0;
char fname[50]; 
int spettro[16384]={0};
int l=1;
int righa=0; //line number
int cambiorigha=0; //if 0, first position in an even line, if 1, first position in an odd line
bool firstline=true;
bool firstposition=true;
int caso=-1;
int numerorigha=0;//line number
bool write=false;
FILE *fout;
float Xread, Yread;
int read=0;


if(xyscan==true){
	

for(read=1;read<=linesnumber;read++){

        
	sample=*(shared_memory2+10+read);
	//printf("read:%d\n",read);
	
	if(sample>50000000 && sample<51000000) caso=0; //when the line on the file is an X position
	else
	if(sample>60000000 && sample<61000000 && ((((sample-60000000)-Yminvero)/PassoY) % 2 == 0)) caso=1; //checks to see if the line is even
	else
	if(sample>60000000 && sample<61000000 && ((((sample-60000000)-Yminvero)/PassoY) % 2 !=0)) caso=2; //checks if line is odd
	
	else 
	if(sample<50000000)caso=3; //when the number read on the file is an energy
	

	switch(caso)
	{
		case 0:
		{	Xread=sample-50000000;
			if(firstposition==true){firstposition=false;break;} //if it is the first time it reads an X position, it breaks, if it is the second time, it writes a file
			//printf("l:%d sample:%d\n",l, sample);
			if(write==true){
				
				fout=fopen(fname,"w+");
				for(j=0;j<chmax;j++){
				fprintf(fout, "%d\n",spettro[j]);
				}
				fclose(fout);
				write=false;
			}
			for(j=0;j<chmax;j++)spettro[j]=0;
			break;
		}
		
		case 1:
		{	
			
			//printf("test1:%f\n",((sample-60000000)-Yminvero)/1000);
			//printf("test1:%f\n", test1);
			if((Xread-Xminvero)>0 && (Xmaxvero-Xread)>PassoX){
				
				//if (l<200)printf("l:%d sample:%d\n",l, sample);
				if(cambiorigha==0 && firstline==true){righa++; cambiorigha=1;}
				if(cambiorigha==0 && firstline==false){righa++; cambiorigha=1;l=l+(MaxX_ori-1);}
				sprintf(fname,"Histo_1_%d.txt",l);
				//if (l<200) printf("%s\n", fname);
				l++;
				write=true;
			}
			break;
		}
		
		case 2:
		{	
			
			//printf("test2:%f\n",((sample-60000000)-Yminvero)/1000);
			//printf("test2:%f\n", test2);
			if((Xmaxvero-Xread)>0 && (Xread-Xminvero)>PassoX){
				
				if(cambiorigha==1){righa++; cambiorigha=0; l=l+(MaxX_ori-3);}	
				firstline=false;
				sprintf(fname,"Histo_1_%d.txt",l);
				//if (l<200)printf("%s\n", fname);
				l--;
				write=true;
			}
			break;
		}
		
		case 3:
		{       if(sample>19999999 && sample<30000000)
			{
				sample=sample-20000000;
			}

			if(sample>29999999)
		        {
		 		sample=sample-30000000;
				sample=qRound((sample*0.9386189258)+7.9641944476);	
			}
		 
			spettro[sample]=spettro[sample]+1;
			break;
		}
caso=-1;
	} 
}

			fout=fopen(fname,"w+");
			for(j=0;j<chmax;j++){
			fprintf(fout, "%d\n",spettro[j]);
			spettro[j]=0;}
			fclose(fout);

}
else
if(xyscan==false){
//printf("maxy_ori:%d\n", MaxY_ori);
for(read=1;read<=linesnumber;read++){

        
	sample=*(shared_memory2+10+read);
	//printf("read:%d\n",read);
	
	if(sample>60000000 && sample<61000000) caso=0; //when the line on the file is an Y position
	else
	if(sample>50000000 && sample<51000000 && ((((sample-50000000)-Xminvero)/PassoX) % 2 == 0)) caso=1; //checks to see if the line is even
	else
	if(sample>50000000 && sample<51000000 && ((((sample-50000000)-Xminvero)/PassoX) % 2 !=0)) caso=2; //checks if line is odd
	
	else 
	if(sample<50000000)caso=3; //when the number read on the file is an energy
	

	switch(caso)
	{
		case 0:
		{	Yread=sample-60000000;
			if(firstposition==true){firstposition=false;break;} //if it is the first time it reads an X position, it breaks, if it is the second time, it writes a file
			
			if(write==true){
				
				fout=fopen(fname,"w+");
				for(j=0;j<chmax;j++){
				fprintf(fout, "%d\n",spettro[j]);
				}
				fclose(fout);
				write=false;
			}
			for(j=0;j<chmax;j++)spettro[j]=0;
			break;
		}
		
		case 1:
		{	
			
			
			//printf("test1:%f\n", test1);
			if((Yread-Yminvero)>0 && (Ymaxvero-Yread)>PassoY){
				
				//if (l<200)printf("l:%d sample:%d\n",l, sample);
				if(cambiorigha==0 && firstline==true){righa++; cambiorigha=1;}
				if(cambiorigha==0 && firstline==false){righa++; cambiorigha=1;l=l+(MaxY_ori-1);}
				sprintf(fname,"Histo_1_%d.txt",l);
				//printf("%s, l:%d\n",fname, l);
				//if (l<200) printf("%s\n", fname);
				l++;
				write=true;
			}
			break;
		}
		
		case 2:
		{	
			
			
			//printf("test2:%f\n", test2);
			if((Ymaxvero-Yread)>0 && (Yread-Yminvero)>PassoY){
			
				//if (l<200) printf("l:%d sample:%d\n",l, sample);
				if(cambiorigha==1){righa++; cambiorigha=0; l=l+(MaxY_ori-3);}	
				firstline=false;
				sprintf(fname,"Histo_1_%d.txt",l);
				//printf("%s, l:%d\n",fname, l);
				l--;
				write=true;
			}
			break;
		}
		
		case 3:
		{
			spettro[sample]=spettro[sample]+1;
			break;
		}
caso=-1;
	} 
}

			fout=fopen(fname,"w+");
			for(j=0;j<chmax;j++){
			fprintf(fout, "%d\n",spettro[j]);
			spettro[j]=0;}
			fclose(fout);

}

} //closes if OK was clicked 

//printf("Export Pymca ended\n");

}
