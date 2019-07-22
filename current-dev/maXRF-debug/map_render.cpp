#include "mainwindow.h"
#include "map.h"

extern bool MapIsOpened;
extern int Pixeldim;
extern int *shared_memory_cmd, *shared_memory3;

using namespace std;
vector<int> x_coord;
vector<int> y_coord;
vector<int> integral;
size_t map_size;
int x_len, y_len, x_step, y_step, min_x, min_y;

void MainWindow::displayImage_SHM() {
    int codePosX = 50000000, codePosY = 60000000;

    x_len  = *(shared_memory_cmd+51) - *(shared_memory_cmd+50);
    y_len  = *(shared_memory_cmd+53) - *(shared_memory_cmd+52);
    x_step = *(shared_memory_cmd+60);
    y_step = *(shared_memory_cmd+61);
    x_len /= x_step;
    y_len /= y_step;

    map_size = static_cast<size_t>(x_len * y_len + x_len);

    x_coord.clear(); x_coord.reserve(map_size);
    y_coord.clear(); y_coord.reserve(map_size);
    integral.clear(); integral.reserve(map_size);

    if (!MapIsOpened) hideImage();
    MapIsOpened = true;

    bool ok=false;
    QStringList items;
    items << tr("Colors") << tr("Gray Scale");
    QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),tr("Using:"), items, 0, false, &ok);

    if (ok && !item.isEmpty()) {
        int pos = 0;
        while (shared_memory3[pos] != -2) {
            if (shared_memory3[pos] > codePosX) {
                x_coord.push_back(shared_memory3[pos] - codePosX); pos++;
                y_coord.push_back(shared_memory3[pos] - codePosY); pos++;
                integral.push_back(shared_memory3[pos]); pos++;
            }
            else pos++;
        }

        min_x = *min_element(begin(x_coord), end(x_coord));
        min_y = *min_element(begin(y_coord), end(y_coord));
        int max_i = *max_element(begin(integral), end(integral));


        for (size_t s = 0; s < map_size; s++) {
            x_coord[s] = (x_coord[s] - min_x) / x_step;
            y_coord[s] = (y_coord[s] - min_y) / y_step;
        }

        x_len = x_len * Pixeldim;
        y_len = (y_len + 1) * Pixeldim;
        QImage image(x_len, y_len, QImage::Format_RGB32);

        QColor myColor;
        for (size_t current = 0; current < map_size; current++ ) {
            double intensity = static_cast<double>(integral[current]) / max_i;


            if (item == "Colors") {
                int temp = int(intensity * 255);

                int color_r = viridis[temp][0];
                int color_g = viridis[temp][1];
                int color_b = viridis[temp][2];
                myColor.setRgb(color_r, color_g, color_b, 255);
            }

            else if ( item == "Gray Scale" ) {
                intensity *= 255;
                myColor.setRgb(int(intensity),int(intensity),int(intensity),255);
            }

            int x_corner = x_coord[current] * Pixeldim;
            int y_corner = y_coord[current] * Pixeldim;

            for (int j = 0; j < Pixeldim; j++) {
                for (int i = 0; i < Pixeldim; i++){
                    image.setPixel(x_corner + i, y_corner + j, myColor.rgb());
                }
            }
        }

        QCursor cursor(QPixmap::fromImage(image));
        cursor.setShape(Qt::PointingHandCursor);

        delete imageLabel;
        imageLabel = new ImgLabel;
        imageLabel->setPixmap(QPixmap::fromImage(image));
        imageLabel->setCursor(cursor);

        scrollArea->setWidget(imageLabel);
        scrollArea->setBackgroundRole(QPalette::Dark);

        QBuffer buffer(&MapImage);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
    }
}

void MainWindow::displaySumImage_SHM() { // Displays a composed map (sum of up to 3 different energy peaks)


    //    QString l;

    //    pixel_Xstep = *(shared_memory_cmd+60);
    //    pixel_Ystep = *(shared_memory_cmd+61);

    //    int MaxX=-1, MaxY=-1, MinX=10000000, MinY=10000000;
    //    int MaxIntegral1 = 0, MaxIntegral2 = 0, MaxIntegral3 = 0;
    //    double Integral1[50000] = {0}, Integral2[50000] = {0}, Integral3[50000] = {0};
    //    point=0;
    //    int k=0;
    //    int s=0;

    //    if (!MapIsOpened) {
    //        int histopos = 0;
    //        MapIsOpened = true;
    //        bool ok = false;
    //        QStringList items;
    //        QString itemLabel;
    //        items << tr("Colors") << tr("Gray Scale");
    //        QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),tr("Using:"), items, 0, false, &ok);
    //        if (ok && !item.isEmpty()) {
    //            itemLabel=item;
    //            int data_number=0;
    //            printf("MaxIntegral1:%d, MaxIntegral2:%d, MaxIntegral3:%d\n", MaxIntegral1, MaxIntegral2, MaxIntegral3);
    //            while(*(shared_memory3+histopos)!=-2)  // "-2" is written at the end of the file
    //            { //qDebug()<<*(shared_memory3+histopos); sleep(1);
    //                data_number++;
    //                if(k<5)
    //                {
    //                    X[point]=*(shared_memory3+histopos);


    //                    k++;
    //                    histopos++;
    //                    if(X[point]>MaxX) MaxX=X[point];
    //                    if(X[point]<MinX) MinX=X[point];


    //                    Y[point]=*(shared_memory3+histopos);

    //                    k++;
    //                    histopos++;
    //                    if(Y[point]>MaxY) MaxY=Y[point];
    //                    if(Y[point]<MinY) MinY=Y[point];

    //                    Integral1[point]=*(shared_memory3+histopos);
    //                    k++;
    //                    histopos++;
    //                    Integral2[point]=*(shared_memory3+histopos);
    //                    k++;
    //                    histopos++;
    //                    Integral3[point]=*(shared_memory3+histopos);
    //                    k++;
    //                    histopos++;
    //                    if(Integral1[point]>MaxIntegral1) {MaxIntegral1=Integral1[point];}//printf("Integral1:%f\n", Integral1[point]);
    //                    if(Integral2[point]>MaxIntegral2) {MaxIntegral2=Integral2[point];}//printf("Integral2:%f\n", Integral2[point]);
    //                    if(Integral3[point]>MaxIntegral3) {MaxIntegral3=Integral3[point];}//printf("Integral3:%f\n", Integral3[point]);

    //                    //	  qDebug()<<"X[point]="<<X[point]<<"Y[point]="<<Y[point]<<"Integral="<<Integral[point]<<"\n";
    //                }

    //                else
    //                {
    //                    histopos++;
    //                }

    //                if(*(shared_memory3+histopos)==-1)
    //                {
    //                    point++;
    //                    k=0;
    //                    histopos++;


    //                }
    //                //qDebug()<<data_number;
    //            }
    //            printf("MaxIntegral1:%d, MaxIntegral2:%d, MaxIntegral3:%d\n", MaxIntegral1, MaxIntegral2, MaxIntegral3);
    //            for (s=0; s<point; s++)
    //            {
    //                Xmap[s]=(X[s]-MinX)/pixel_Xstep;
    //                Ymap[s]=(Y[s]-MinY)/pixel_Ystep;
    //            }
    //            MaxX=(MaxX-MinX)/pixel_Xstep;
    //            MinX=0;
    //            MaxY=(MaxY-MinY)/pixel_Ystep;
    //            MinY=0;

    //            Define_Pixels(); //definisce quelli che dovranno essere colorati in base a Pixeldim...ci si accede con Pointer

    //            if(Pixeldim>1)
    //            {
    //                MinX=MinX-int(Pixeldim/2);
    //                MaxX=MaxX+int(Pixeldim/2);
    //                MinY=MinY-int(Pixeldim/2);
    //                MaxY=MaxY+int(Pixeldim/2);
    //            }
    //            OffsetX=int((PixelX-(MaxX+MinX))/2);  //per centrare la mappa nella finestra
    //            OffsetY=int((PixelY-(MaxY+MinY))/2);

    //            qDebug()<<"MaxX"<<MaxX<<"MinX"<<MinX<<"MaxY"<<MaxY<<"MinY"<<MinY<<'\n';
    //            qDebug()<<"OFFSET x "<<OffsetX<<" OFFSET y "<<OffsetY<<'\n';
    //            qDebug()<<"pixel x:"<<PixelX<<"pixel y:"<<PixelY<<"points:"<<point<<"PixelDim:"<<Pixeldim<<'\n';


    //            MyImage = new QImage(PixelX, PixelY, QImage::Format_RGB32);
    //            MyImage->fill(QColor(Qt::black).rgb());


    //            QColor myColor;
    //            for(int current=0; current<point; current++)
    //            {
    //                    double intensity1=(Integral1[current]/MaxIntegral1)*255;if(MaxIntegral1==0)intensity1=0;
    //                    double intensity2=(Integral2[current]/MaxIntegral2)*255;if(MaxIntegral2==0)intensity2=0;
    //                    double intensity3=(Integral3[current]/MaxIntegral3)*255;if(MaxIntegral3==0)intensity3=0;

    //                    if(itemLabel=="Colors")
    //                    {
    //                        myColor.setRgb(int(intensity1),int(intensity2),int(intensity3),255);

    //                    }
    //                    else
    //                        if(itemLabel=="Gray Scale")
    //                        {
    //                            //intensity=intensity*255;
    //                            myColor.setRgb(0,0,0,255);
    //                        }

    //                    for(int c=0;c<(Pixeldim*Pixeldim);c++)
    //                        MyImage->setPixel(PixelX-Pointer[current].total[c].point_x-OffsetX,PixelY-Pointer[current].total[c].point_y-OffsetY, myColor.rgb());



    //            }  //chiude for

    //            qDebug()<<"loop ended";

    //            imageLabel->setPixmap(QPixmap::fromImage(*MyImage));
    //            scaleFactor = 1.0;
    //            Cursor= new QCursor (QPixmap::fromImage(*MyImage),-1,-1 );
    //            Cursor->setShape(Qt::PointingHandCursor);
    //            imageLabel->setCursor(*Cursor);


    //            QBuffer buffer(&MapImage);
    //            buffer.open(QIODevice::WriteOnly);
    //            MyImage->save(&buffer, "PNG"); // writes image into QByteArray MapImage in PNG format


    //        } //chiude if(ok...)
    //        else
    //            hideImage();

    //    } //chiude if(MapIsOpened)


}

