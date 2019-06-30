#include "mainwindow.h"
#include "../Header.h"
#include <../Shm.h>

/////////variabili e funzioni definite altrove ma necessarie/////////////////////////
extern bool MapIsOpened;
extern int point,OffsetX, OffsetY,PixelX,PixelY,Pixeldim;
extern int pixel_Xstep, pixel_Ystep;
extern int Xmap[],Ymap[],MaxX,MaxY,MinX,MinY,MaxX_ori,MinX_ori,MaxY_ori,MinY_ori;
extern double X[],Y[];
extern double Integral[],MaxIntegral;
extern int *shared_memory3;
extern int *shared_memory_cmd;
extern struct Pixel_BIG *Pointer;

void MainWindow::displayImage_SHM() {

    MinX=10000000;
    MinY=10000000;
    MaxIntegral = 0;
    point = 0;

    if (!MapIsOpened) {

        MapIsOpened=true;
        bool ok=false;
        QStringList items;
        items << tr("Colors") << tr("Gray Scale");
        QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),tr("Using:"), items, 0, false, &ok);

        if (ok && !item.isEmpty()) {
            int histopos=0;
            bool option = false;
            while(*(shared_memory3+histopos) != -2) { //EOF marked with -2, new pixel marked with -1
                if (histopos < 3 || option) {
                    X[point] = *(shared_memory3+(histopos++));
                    Y[point] = *(shared_memory3+(histopos++));
                    Integral[point] = *(shared_memory3+(histopos++));

                    if (X[point] < MinX) MinX = static_cast<int>(X[point]);
                    if (Y[point] < MinY) MinY = static_cast<int>(Y[point]);

                    if (Integral[point] > MaxIntegral) MaxIntegral = Integral[point];
                    option = false;
                }
                else if (*(shared_memory3+histopos) == -1) {
                    point++;
                    histopos++;
                    option = true;
                }
                else histopos++;
            }

            pixel_Xstep = *(shared_memory_cmd+60);
            pixel_Ystep = *(shared_memory_cmd+61);

            for (int  s = 0; s < point; s++) {
                Xmap[s] = static_cast<int>(X[s] - MinX) / pixel_Xstep;
                Ymap[s] = static_cast<int>(Y[s] - MinY) / pixel_Ystep;
            }

            Define_Pixels(); //definisce quelli che dovranno essere colorati in base a Pixeldim...ci si accede con Pointer


            MaxY = Pointer[point - 1].total[Pixeldim - 1].point_y;
            MaxX = (point * (Pixeldim * Pixeldim)) / (MaxY + 1);

           MyImage = new QImage(MaxX, MaxY+1, QImage::Format_RGB32);

            double frac=0.33333333;
            QColor myColor;
            for ( int current = 0; current < point; current++ ) {
                double intensity = (Integral[current] / MaxIntegral);

                if(item=="Colors") {
                    if ( intensity < frac ) {
                        intensity=3*intensity*255;
                        myColor.setRgb(0,int(intensity),0,255);
                    }
                    else if ( intensity >= frac && intensity < (2*frac) ) {
                        intensity=(intensity-frac)*3*255;
                        myColor.setRgb(int(intensity),255,0,255);
                    }
                    else if ( intensity >= (2*frac) ) {
                        intensity=(255-(intensity-(2.0*frac))*3*255);
                        myColor.setRgb(255,int(intensity),0,255);
                    }
                }
                else if ( item == "Gray Scale" ) {
                    intensity=intensity*255;
                    myColor.setRgb(int(intensity),int(intensity),int(intensity),255);
                }

                for (int c = 0; c < (Pixeldim * Pixeldim); c++) {
                    MyImage->setPixel(Pointer[current].total[c].point_x,Pointer[current].total[c].point_y, myColor.rgb());
                }

            }
            qDebug()<<MinX<<" "<<MaxX<<" "<<MinY<<" "<<MaxY;



            QLabel *mapLabel = new ImgLabel;
            mapLabel->setPixmap(QPixmap::fromImage(*MyImage));
            Cursor = new QCursor (QPixmap::fromImage(*MyImage),-1,-1 );
            Cursor->setShape(Qt::PointingHandCursor);
            mapLabel->setCursor(*Cursor);
            scrollArea->setWidget(mapLabel);
            scrollArea->setBackgroundRole(QPalette::Shadow);
            scrollArea->setAlignment(Qt::AlignCenter);

            QBuffer buffer(&MapImage);
            buffer.open(QIODevice::WriteOnly);
            MyImage->save(&buffer, "PNG");
        }
        else hideImage();
    }
}

void MainWindow::displaySumImage_SHM() { // Displays a composed map (sum of up to 3 different energy peaks)

    QString l;

    pixel_Xstep = *(shared_memory_cmd+60);
    pixel_Ystep = *(shared_memory_cmd+61);

    MaxX=-1; MaxY=-1; MinX=10000000; MinY=10000000;
    int MaxIntegral1 = 0, MaxIntegral2 = 0, MaxIntegral3 = 0;
    double Integral1[50000] = {0}, Integral2[50000] = {0}, Integral3[50000] = {0};
    point=0;
    int k=0;
    int s=0;

    if (!MapIsOpened) {
        int histopos = 0;
        MapIsOpened = true;
        bool ok = false;
        QStringList items;
        QString itemLabel;
        items << tr("Colors") << tr("Gray Scale");
        QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),tr("Using:"), items, 0, false, &ok);
        if (ok && !item.isEmpty()) {
            itemLabel=item;
            int data_number=0;
            printf("MaxIntegral1:%d, MaxIntegral2:%d, MaxIntegral3:%d\n", MaxIntegral1, MaxIntegral2, MaxIntegral3);
            while(*(shared_memory3+histopos)!=-2)  // "-2" is written at the end of the file
            { //qDebug()<<*(shared_memory3+histopos); sleep(1);
                data_number++;
                if(k<5)
                {
                    X[point]=*(shared_memory3+histopos);


                    k++;
                    histopos++;
                    if(X[point]>MaxX) MaxX=X[point];
                    if(X[point]<MinX) MinX=X[point];


                    Y[point]=*(shared_memory3+histopos);

                    k++;
                    histopos++;
                    if(Y[point]>MaxY) MaxY=Y[point];
                    if(Y[point]<MinY) MinY=Y[point];

                    Integral1[point]=*(shared_memory3+histopos);
                    k++;
                    histopos++;
                    Integral2[point]=*(shared_memory3+histopos);
                    k++;
                    histopos++;
                    Integral3[point]=*(shared_memory3+histopos);
                    k++;
                    histopos++;
                    if(Integral1[point]>MaxIntegral1) {MaxIntegral1=Integral1[point];}//printf("Integral1:%f\n", Integral1[point]);
                    if(Integral2[point]>MaxIntegral2) {MaxIntegral2=Integral2[point];}//printf("Integral2:%f\n", Integral2[point]);
                    if(Integral3[point]>MaxIntegral3) {MaxIntegral3=Integral3[point];}//printf("Integral3:%f\n", Integral3[point]);

                    //	  qDebug()<<"X[point]="<<X[point]<<"Y[point]="<<Y[point]<<"Integral="<<Integral[point]<<"\n";
                }

                else
                {
                    histopos++;
                }

                if(*(shared_memory3+histopos)==-1)
                {
                    point++;
                    k=0;
                    histopos++;


                }
                //qDebug()<<data_number;
            }
            printf("MaxIntegral1:%d, MaxIntegral2:%d, MaxIntegral3:%d\n", MaxIntegral1, MaxIntegral2, MaxIntegral3);
            for (s=0; s<point; s++)
            {
                Xmap[s]=(X[s]-MinX)/pixel_Xstep;
                Ymap[s]=(Y[s]-MinY)/pixel_Ystep;
            }
            MaxX=(MaxX-MinX)/pixel_Xstep;
            MinX=0;
            MaxY=(MaxY-MinY)/pixel_Ystep;
            MinY=0;






            ////prima di chiamare Define_pixels copia dei max e min veri (della scansione)sennò li cambia///////////
            MaxX_ori=MaxX;
            MinX_ori=MinX;
            MaxY_ori=MaxY;
            MinY_ori=MinY;

            qDebug()<<"MaxX_ori:"<<MaxX_ori<<"MinX_ori:"<<MinX_ori<<"MaxY_ori:"<<MaxY_ori<<"MinY_ori:"<<MinY_ori<<"\n";

            Define_Pixels(); //definisce quelli che dovranno essere colorati in base a Pixeldim...ci si accede con Pointer

            if(Pixeldim>1)
            {
                MinX=MinX-int(Pixeldim/2);
                MaxX=MaxX+int(Pixeldim/2);
                MinY=MinY-int(Pixeldim/2);
                MaxY=MaxY+int(Pixeldim/2);
            }
            OffsetX=int((PixelX-(MaxX+MinX))/2);  //per centrare la mappa nella finestra
            OffsetY=int((PixelY-(MaxY+MinY))/2);

            qDebug()<<"MaxX"<<MaxX<<"MinX"<<MinX<<"MaxY"<<MaxY<<"MinY"<<MinY<<'\n';
            qDebug()<<"OFFSET x "<<OffsetX<<" OFFSET y "<<OffsetY<<'\n';
            qDebug()<<"pixel x:"<<PixelX<<"pixel y:"<<PixelY<<"points:"<<point<<"PixelDim:"<<Pixeldim<<'\n';


            MyImage = new QImage(PixelX, PixelY, QImage::Format_RGB32);
            MyImage->fill(QColor(Qt::black).rgb());


            QColor myColor;
            for(int current=0; current<point; current++)
            {
                    double intensity1=(Integral1[current]/MaxIntegral1)*255;if(MaxIntegral1==0)intensity1=0;
                    double intensity2=(Integral2[current]/MaxIntegral2)*255;if(MaxIntegral2==0)intensity2=0;
                    double intensity3=(Integral3[current]/MaxIntegral3)*255;if(MaxIntegral3==0)intensity3=0;

                    if(itemLabel=="Colors")
                    {
                        myColor.setRgb(int(intensity1),int(intensity2),int(intensity3),255);

                    }
                    else
                        if(itemLabel=="Gray Scale")
                        {
                            //intensity=intensity*255;
                            myColor.setRgb(0,0,0,255);
                        }

                    for(int c=0;c<(Pixeldim*Pixeldim);c++)
                        MyImage->setPixel(PixelX-Pointer[current].total[c].point_x-OffsetX,PixelY-Pointer[current].total[c].point_y-OffsetY, myColor.rgb());



            }  //chiude for

            qDebug()<<"loop ended";

            imageLabel->setPixmap(QPixmap::fromImage(*MyImage));
            scaleFactor = 1.0;
            Cursor= new QCursor (QPixmap::fromImage(*MyImage),-1,-1 );
            Cursor->setShape(Qt::PointingHandCursor);
            imageLabel->setCursor(*Cursor);


            QBuffer buffer(&MapImage);
            buffer.open(QIODevice::WriteOnly);
            MyImage->save(&buffer, "PNG"); // writes image into QByteArray MapImage in PNG format


        } //chiude if(ok...)
        else
            hideImage();

    } //chiude if(MapIsOpened)


}

