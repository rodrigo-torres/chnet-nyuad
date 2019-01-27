#include "mainwindow.h"
//#include "variables.h"
#include "../Header.h"
#include <QString>
#include <../Shm.h>

/////////variabili e funzioni definite altrove ma necessarie/////////////////////////
extern bool MapIsOpened;
extern int OffsetX, OffsetY,PixelX,PixelY,point,X1,Y1,X2,Y2,MotoreWindowStatus,IniXready,IniYready;
extern int pixel_Xstep,pixel_Ystep,Xminvero,Yminvero;
extern double X[20000],Y[20000];
extern double Integral[20000],MaxIntegral,x_image,y_image,x_image2,y_image2;
extern int *(shared_memory), *(shared_memory_cmd);
extern int *(shared_memory3);
extern struct Pixel_BIG *Pointer;  //variabile globale per Pixelsmappa[point]
extern int Appartiene(int x,int y, struct Pixel_BIG *px);

//////////////////////////////////////////////////////////////////////////////////////

QString TextYes;
double Go_Xc, Go_Yc;
bool mouseleft=false; bool selectionStarted=false; bool found1=false; bool found2=false;


void ImgLabel::mousePressEvent(QMouseEvent *event)
{
    int I;

    if (event->buttons() == Qt::LeftButton && MapIsOpened) { // Click con il tasto sx --> mi mostra l'integrale del punto

        mouseleft=true;
        found1=false;

        x_image = event->x(); // queste non sono le vere posizioni dei motori se Pixeldim>1!!!
        y_image = event->y();
        qDebug()<<"... Pixel coordinates clicked (x, y): "<<x_image<<y_image;


        for (int c = 0; c < point; c++) {
            if(Appartiene(x_image,y_image, &Pointer[c]))
            {
                found1=true;
                x_image=X[c]/1000; ///il Pointer[c] a cui appartiene il punto è associato a quaesta coppia X[c],Y[c]
                y_image=Y[c]/1000; ///queste sono le vere posizioni dei motori
                I=Integral[c];

                qDebug()<<"Integrale del pixel= "<<I<<'\n';
                break;
            }


        }
        if(found1==false) //CurrentAction->setText(TextNo);
            qDebug()<<"Punto 1 non compreso nella scansione\n";
    }

    if (event->buttons() == Qt::RightButton && MapIsOpened==true)  //// Click con il tasto dx --> sposta i motori alla posizione selezionata
    {

        if (MotoreWindowStatus==1 && IniXready==1 && IniYready==1)
        {

            bool found3=false;
            int X_chosen,Y_chosen, Go_X, Go_Y;
            X_chosen = event->x();
            Y_chosen = event->y();

            Go_X=PixelX-X_chosen-OffsetX; ///NB:queste non sono le vere posizioni dei motori se Pixeldim>1!!!
            Go_Y=PixelY-Y_chosen-OffsetY;
            for(int c=0;c<point;c++)
            {
                if(Appartiene(Go_X,Go_Y, &Pointer[c]))
                {
                    found3=true;
                    Go_X=X[c]; ///queste sono le vere posizioni dei motori
                    Go_Y=Y[c];
                    Go_Xc=X[c]/1000;
                    Go_Yc=Y[c]/1000;
                    qDebug()<<"Coordinate del pixel nella mappa "<<Go_Xc<<Go_Yc<<'\n';
                    qDebug()<<"Integrale del pixel= "<<Integral[c]<<'\n';
                    break;
                }
            }
            if(found3==false)
                qDebug()<<"Punto 1 non compreso nella scansione\n";
            else
            {
                QMessageBox::StandardButton reply;
                reply=QMessageBox::question(this, "ATTENZIONE", "Spostare i motori alla posizione scelta?",
                                            QMessageBox::Yes|QMessageBox::No);

                if (reply == QMessageBox::Yes)
                {
                    qDebug()<<"Sposto i motori alla posizione "<<Go_Xc<<Go_Yc<<".....";
                    *(shared_memory+19)=1;
                    *(shared_memory_cmd+64)=Go_X;
                    *(shared_memory_cmd+65)=Go_Y;
                }
            }
        }
        else
            qDebug()<<"Inizializzare i motori!";

    }
}

void ImgLabel::mouseReleaseEvent(QMouseEvent *event) { // Click and release in different pixel -> displays the rectangle's count integral
    int histopos=0, u=0, k=0;
    int xshm,yshm,conteggi,canale;
    int Spettro[16000]={0};
    found2=false;

    if(MapIsOpened==true && mouseleft==true) {
        int I,max_y,max_x,min_y,min_x;
        mouseleft=false;
        x_image2 = event->x();
        y_image2 = event->y();;

        for(int c=0;c<point;c++) {
            if(Appartiene(x_image2,y_image2, &Pointer[c])) {
                found2=true;
                x_image2=X[c]/1000; ///queste sono le vere posizioni dei motori
                y_image2=Y[c]/1000;
                I=Integral[c];
                break;
            }
        }

        if (!found2) qDebug()<<"[!] Second point selected is not within the scan dimensions";
        if (found2 && found1) {

            if (x_image!=x_image2 || y_image!=y_image2) {
                // If I select a rectangle on the map.
                if (x_image<x_image2) {min_x=x_image; max_x=x_image2;}
                else {max_x=x_image; min_x=x_image2;}
                if (y_image<y_image2) {min_y=y_image; max_y=y_image2;}
                else {max_y=y_image; min_y=y_image2;}

                while(*(shared_memory3+histopos)!=-2) {
                    if(k<2) {
                        xshm=*(shared_memory3+histopos);
                        k++;
                        histopos++;

                        yshm=*(shared_memory3+histopos);
                        k++;
                        histopos++;

                        if(xshm<=max_x*1000 && xshm>=min_x*1000 && yshm<=max_y*1000 && yshm>=min_y*1000 ) {

                            histopos++;  //salto l'integrale
                            while(*(shared_memory3+histopos)!=-1) {
                                conteggi=*(shared_memory3+histopos);
                                histopos++;
                                canale=*(shared_memory3+histopos);
                                Spettro[canale]=Spettro[canale]+conteggi;
                                histopos++;
                            }
                        }
                    }

                    else {
                        histopos++;
                    }

                    if (*(shared_memory3+histopos)==-1) {
                        k=0;
                        histopos++;
                    }
                }

                // puts the value in shared memory
                for (u=0; u<16000;u++) {
                    if ( *(shared_memory_cmd+100) == 0 ) { *(shared_memory+100+u) = Spettro[u];}
                    if ( *(shared_memory_cmd+100) == 1 ) { *(shared_memory+20000+u) = Spettro[u];}
                    if ( *(shared_memory_cmd+100) == 2 ) { *(shared_memory+40000+u) = Spettro[u];}
                }

                *(shared_memory+99)=1;
            }



            else
                if (x_image==x_image2 && y_image==y_image2)      //se invece ho cliccato solo su un punto



                {

                    while(*(shared_memory3+histopos)!=-2)
                    {

                        if(k<2)
                        {
                            xshm=*(shared_memory3+histopos);
                            k++;
                            histopos++;

                            yshm=*(shared_memory3+histopos);
                            k++;
                            histopos++;

                            if (xshm==x_image*1000 && yshm==y_image*1000)
                            {
                                histopos++;  //salto l'integrale
                                while(*(shared_memory3+histopos)!=-1)
                                {
                                    conteggi=*(shared_memory3+histopos);
                                    histopos++;
                                    canale=*(shared_memory3+histopos);
                                    Spettro[canale]=conteggi;
                                    histopos++;
                                }
                                for (u=0; u<16000;u++) {
                                    if ( *(shared_memory_cmd+100) == 0 ) { *(shared_memory+100+u) = Spettro[u];}
                                    if ( *(shared_memory_cmd+100) == 1 ) { *(shared_memory+20000+u) = Spettro[u];}
                                    if ( *(shared_memory_cmd+100) == 2 ) { *(shared_memory+40000+u) = Spettro[u];}
                                }
                                *(shared_memory+99)=1;
                                break;
                            }

                        }

                        else
                        {
                            histopos++;
                        }

                        if(*(shared_memory3+histopos)==-1)
                        {
                            k=0;
                            histopos++;
                        }


                    }
                }
            qDebug()<<"Coordinate del pixel "<<x_image<<y_image<<'\n';


        }
        else qDebug()<<"[!] One of the selected points is not within the scan dimensions";
    }

}









