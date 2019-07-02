#include "mainwindow.h"
#include "map.h"

extern bool MapIsOpened;
extern int *shared_memory, *shared_memory_cmd, *shared_memory3;
extern int pixel_on_map(int *x,int *y);


bool mouseleft=false; bool found1=false;

int x_image, y_image;

void ImgLabel::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton && MapIsOpened) { // Click con il tasto sx --> mi mostra l'integrale del punto
        found1 = false;
        mouseleft = true;

        x_image = event->x();
        y_image = event->y();
        int it = pixel_on_map(&x_image, &y_image);

        if (it == -1) qDebug()<<"[!] First point not found in map";
        else {
            found1 = true;
            qDebug()<<"[!] Point integral is "<<integral[it];
        }
    }

    /* Right click: moves motors to the position corresponding to the chosen pixel*/

    //    if (event->buttons() == Qt::RightButton && MapIsOpened) {

    //        if (MotoreWindowStatus==1) //&& IniXready==1 && IniYready==1)
    //        {

    //            bool found3=false;
    //            int clicked_x = event->x();
    //            int clicked_y = event->y();

    //            for(size_t c = 0; c < integral.size(); c++) {
    //                if(Appartiene(clicked_x, clicked_y, &Pointer[c])) {
    //                    found3=true;
    //                    Go_X =x_coord[c]; ///queste sono le vere posizioni dei motori
    //                    Go_Y =y_coord[c];
    //                    Go_Xc=x_coord[c] / 1000;
    //                    Go_Yc=y_coord[c] / 1000;

    //                    for (size_t s = 0; s < map_size; s++) {
    //                        x_coord[s] = (x_coord[s] - min_x) / *(shared_memory_cmd+60);
    //                        y_coord[s] = (y_coord[s] - min_y) / *(shared_memory_cmd+61);
    //                    }
    //                    qDebug()<<"Coordinate del pixel nella mappa "<<Go_Xc<<Go_Yc<<'\n';
    //                    qDebug()<<"Integrale del pixel= "<<integral[c]<<'\n';
    //                    break;
    //                }
    //            }
    //            if(found3==false)
    //                qDebug()<<"Punto 1 non compreso nella scansione\n";
    //            else
    //            {
    //                QMessageBox::StandardButton reply;
    //                reply=QMessageBox::question(this, "ATTENZIONE", "Spostare i motori alla posizione scelta?",
    //                                            QMessageBox::Yes|QMessageBox::No);

    //                if (reply == QMessageBox::Yes)
    //                {
    //                    qDebug()<<"Sposto i motori alla posizione "<<Go_Xc<<Go_Yc<<".....";
    //                    *(shared_memory+19)=1;
    //                    *(shared_memory_cmd+64)=Go_X;
    //                    *(shared_memory_cmd+65)=Go_Y;
    //                }
    //            }
    //        }
    //        else
    //            qDebug()<<"Inizializzare i motori!";

    //    }

}

int largest(int a, int b) {
    int ret;
    a > b ? ret = a : ret = b;
    return  ret;
}

int smallest(int a, int b) {
    int ret;
    a < b ? ret = a : ret = b;
    return  ret;
}


void ImgLabel::mouseReleaseEvent(QMouseEvent *event) { // Click and release in different pixel -> displays the rectangle's count integral

    if (MapIsOpened && mouseleft) {
        mouseleft=false;

        int x_image2 = event->x();
        int y_image2 = event->y();
        int it = pixel_on_map(&x_image2, &y_image2);

        if (it == -1) qDebug()<<"[!] Second point not found in map";
        else if (found1) {
            int pos = 0;
            int codePosX = 50000000, codePosY = 60000000;
            int read_x, read_y, counts, channel, spectrum[16384] = { 0 };

            /* If a rectangle is selected on the map */
            if (x_image != x_image2 || y_image != y_image2) {
                int min_x = smallest(x_image, x_image2);
                int max_x = largest(x_image, x_image2);
                int min_y = smallest(y_image, y_image2);
                int max_y = largest(y_image, y_image2);


                while (shared_memory3[pos] != -2) {
                    if (shared_memory3[pos] > codePosX) {
                        read_x = shared_memory3[pos] - codePosX; pos++;
                        read_y = shared_memory3[pos] - codePosY; pos++;
                        pos++;

                        if (read_x >= min_x && read_x <= max_x)
                            if (read_y >= min_y && read_y <= max_y)
                                while (shared_memory3[pos] != -1) {
                                    counts = shared_memory3[pos]; pos++;
                                    channel = shared_memory3[pos]; pos++;
                                    spectrum[channel] += counts;
                                }
                    }
                    else pos++;
                }
            }

            /* If only one point is clicked */
            else if (x_image == x_image2 && y_image == y_image2) {

                while (shared_memory3[pos] != -2)
                {
                    if (shared_memory3[pos] > codePosX)
                    {
                        read_x = shared_memory3[pos] - codePosX; pos++;
                        read_y = shared_memory3[pos] - codePosY; pos++;
                        pos++;

                        if (read_x == x_image && read_y == y_image)
                        {
                            while (shared_memory3[pos] != -1) {
                                counts = shared_memory3[pos]; pos++;
                                channel = shared_memory3[pos]; pos++;
                                spectrum[channel] += counts;
                            }
                        break;
                        }
                    }
                    else pos++;
                }
            }

            /* Passing the values to spectra visualization window through shared memory */
            for (int i = 0; i < 16384; i++) {
                if ( *(shared_memory_cmd+100) == 0 ) { *(shared_memory+100+i)   = spectrum[i];}
                if ( *(shared_memory_cmd+100) == 1 ) { *(shared_memory+20000+i) = spectrum[i];}
                if ( *(shared_memory_cmd+100) == 2 ) { *(shared_memory+40000+i) = spectrum[i];}
            }
            *(shared_memory+99) = 1;
        }
    }
}








