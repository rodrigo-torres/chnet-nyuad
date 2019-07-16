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

string viridis_data[256] =
{
    "0x440154",
    "0x440256",
    "0x450457",
    "0x450559",
    "0x46075a",
    "0x46085c",
    "0x460a5d",
    "0x460b5e",
    "0x470d60",
    "0x470e61",
    "0x471063",
    "0x471164",
    "0x471365",
    "0x481467",
    "0x481668",
    "0x481769",
    "0x48186a",
    "0x481a6c",
    "0x481b6d",
    "0x481c6e",
    "0x481d6f",
    "0x481f70",
    "0x482071",
    "0x482173",
    "0x482374",
    "0x482475",
    "0x482576",
    "0x482677",
    "0x482878",
    "0x482979",
    "0x472a7a",
    "0x472c7a",
    "0x472d7b",
    "0x472e7c",
    "0x472f7d",
    "0x46307e",
    "0x46327e",
    "0x46337f",
    "0x463480",
    "0x453581",
    "0x453781",
    "0x453882",
    "0x443983",
    "0x443a83",
    "0x443b84",
    "0x433d84",
    "0x433e85",
    "0x423f85",
    "0x424086",
    "0x424186",
    "0x414287",
    "0x414487",
    "0x404588",
    "0x404688",
    "0x3f4788",
    "0x3f4889",
    "0x3e4989",
    "0x3e4a89",
    "0x3e4c8a",
    "0x3d4d8a",
    "0x3d4e8a",
    "0x3c4f8a",
    "0x3c508b",
    "0x3b518b",
    "0x3b528b",
    "0x3a538b",
    "0x3a548c",
    "0x39558c",
    "0x39568c",
    "0x38588c",
    "0x38598c",
    "0x375a8c",
    "0x375b8d",
    "0x365c8d",
    "0x365d8d",
    "0x355e8d",
    "0x355f8d",
    "0x34608d",
    "0x34618d",
    "0x33628d",
    "0x33638d",
    "0x32648e",
    "0x32658e",
    "0x31668e",
    "0x31678e",
    "0x31688e",
    "0x30698e",
    "0x306a8e",
    "0x2f6b8e",
    "0x2f6c8e",
    "0x2e6d8e",
    "0x2e6e8e",
    "0x2e6f8e",
    "0x2d708e",
    "0x2d718e",
    "0x2c718e",
    "0x2c728e",
    "0x2c738e",
    "0x2b748e",
    "0x2b758e",
    "0x2a768e",
    "0x2a778e",
    "0x2a788e",
    "0x29798e",
    "0x297a8e",
    "0x297b8e",
    "0x287c8e",
    "0x287d8e",
    "0x277e8e",
    "0x277f8e",
    "0x27808e",
    "0x26818e",
    "0x26828e",
    "0x26828e",
    "0x25838e",
    "0x25848e",
    "0x25858e",
    "0x24868e",
    "0x24878e",
    "0x23888e",
    "0x23898e",
    "0x238a8d",
    "0x228b8d",
    "0x228c8d",
    "0x228d8d",
    "0x218e8d",
    "0x218f8d",
    "0x21908d",
    "0x21918c",
    "0x20928c",
    "0x20928c",
    "0x20938c",
    "0x1f948c",
    "0x1f958b",
    "0x1f968b",
    "0x1f978b",
    "0x1f988b",
    "0x1f998a",
    "0x1f9a8a",
    "0x1e9b8a",
    "0x1e9c89",
    "0x1e9d89",
    "0x1f9e89",
    "0x1f9f88",
    "0x1fa088",
    "0x1fa188",
    "0x1fa187",
    "0x1fa287",
    "0x20a386",
    "0x20a486",
    "0x21a585",
    "0x21a685",
    "0x22a785",
    "0x22a884",
    "0x23a983",
    "0x24aa83",
    "0x25ab82",
    "0x25ac82",
    "0x26ad81",
    "0x27ad81",
    "0x28ae80",
    "0x29af7f",
    "0x2ab07f",
    "0x2cb17e",
    "0x2db27d",
    "0x2eb37c",
    "0x2fb47c",
    "0x31b57b",
    "0x32b67a",
    "0x34b679",
    "0x35b779",
    "0x37b878",
    "0x38b977",
    "0x3aba76",
    "0x3bbb75",
    "0x3dbc74",
    "0x3fbc73",
    "0x40bd72",
    "0x42be71",
    "0x44bf70",
    "0x46c06f",
    "0x48c16e",
    "0x4ac16d",
    "0x4cc26c",
    "0x4ec36b",
    "0x50c46a",
    "0x52c569",
    "0x54c568",
    "0x56c667",
    "0x58c765",
    "0x5ac864",
    "0x5cc863",
    "0x5ec962",
    "0x60ca60",
    "0x63cb5f",
    "0x65cb5e",
    "0x67cc5c",
    "0x69cd5b",
    "0x6ccd5a",
    "0x6ece58",
    "0x70cf57",
    "0x73d056",
    "0x75d054",
    "0x77d153",
    "0x7ad151",
    "0x7cd250",
    "0x7fd34e",
    "0x81d34d",
    "0x84d44b",
    "0x86d549",
    "0x89d548",
    "0x8bd646",
    "0x8ed645",
    "0x90d743",
    "0x93d741",
    "0x95d840",
    "0x98d83e",
    "0x9bd93c",
    "0x9dd93b",
    "0xa0da39",
    "0xa2da37",
    "0xa5db36",
    "0xa8db34",
    "0xaadc32",
    "0xaddc30",
    "0xb0dd2f",
    "0xb2dd2d",
    "0xb5de2b",
    "0xb8de29",
    "0xbade28",
    "0xbddf26",
    "0xc0df25",
    "0xc2df23",
    "0xc5e021",
    "0xc8e020",
    "0xcae11f",
    "0xcde11d",
    "0xd0e11c",
    "0xd2e21b",
    "0xd5e21a",
    "0xd8e219",
    "0xdae319",
    "0xdde318",
    "0xdfe318",
    "0xe2e418",
    "0xe5e419",
    "0xe7e419",
    "0xeae51a",
    "0xece51b",
    "0xefe51c",
    "0xf1e51d",
    "0xf4e61e",
    "0xf6e620",
    "0xf8e621",
    "0xfbe723",
    "0xfde725"
};

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
            x_coord[s] = (x_coord[s] - min_x) / *(shared_memory_cmd+60);
            y_coord[s] = (y_coord[s] - min_y) / *(shared_memory_cmd+61);
        }

        x_len = x_len * Pixeldim;
        y_len = (y_len + 1) * Pixeldim;
        QImage image(x_len, y_len, QImage::Format_RGB32);

        QColor myColor;
        for (size_t current = 0; current < map_size; current++ ) {
            double intensity = static_cast<double>(integral[current]) / max_i;


            if (item == "Colors") {
                int temp = int(intensity * 255);
                ulong x = std::stoul(viridis_data[temp], nullptr, 16);

                int color_r = ((x >> 16) & 0xFF);
                int color_g = ((x >> 8) & 0xFF);   // Extract the GG byte
                int color_b = ((x) & 0xFF);
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

