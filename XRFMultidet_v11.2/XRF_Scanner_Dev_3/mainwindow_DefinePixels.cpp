#include "mainwindow.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <../Shm.h>
using namespace std;

extern int point, Pixeldim;
extern int Xmap[20000],Ymap[20000];
extern struct Pixel_BIG *Pointer;


struct Pixel_BIG *createPixel(int dim, int x, int y) {
    struct Pixel_BIG *px;
    px = static_cast<struct Pixel_BIG *>(calloc(1, sizeof(struct Pixel_BIG)));  //zona di memoria per pixel_BIG
    px->centro[0] = x;
    px->centro[1] = y;
    px->dimensione = dim;

    size_t contour = static_cast<size_t>(dim * dim);
    px->total = static_cast<struct punto *>(calloc(contour, sizeof(struct punto)));  //(dimensione*dimensione) punti

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++){
            int index = (dim * i) + j;
            px->total[index].point_x = px->centro[0]+i;
            px->total[index].point_y = px->centro[1]+j;
        }
    }
    return px;
}

bool Appartiene(int x, int y, struct Pixel_BIG *px) { // Returns true if the coordinate (x, y) belongs to a pixel
    int dim = px->dimensione;
    for(int i = 0; i < (dim * dim); i++) {
        if (x == px->total[i].point_x  && y == px->total[i].point_y) return true;
    }
    return false;
}

void MainWindow::Define_Pixels() {
    struct Pixel_BIG *PixelsMappa[point];
    size_t temp = static_cast<size_t>(point);
    Pointer = static_cast<struct Pixel_BIG *>(calloc(temp, sizeof(struct Pixel_BIG)));

    int caso = 0;
    for (int z = 0; z < point; z++) {
        if (z == 0 || Pixeldim == 1)                            caso = 1;   // First pixels or pixel with unit dimensions
        else if (Xmap[z] > Xmap[z-1] && Ymap[z] == Ymap[z-1])   caso = 2;   // si e mossa la x aumentando, la y e fissa,
        else if (Ymap[z] > Ymap[z-1] && Xmap[z] == Xmap[z-1])   caso = 3;   // si e mossa la y aumentando, la x e quella di prima,
        else if (Xmap[z] < Xmap[z-1] && Ymap[z] == Ymap[z-1])   caso = 4;   // si e mossa la x diminuendo, la y e fissa,
        else continue;

        switch (caso) {
        case 1:
            PixelsMappa[z] = createPixel(Pixeldim,Xmap[z],Ymap[z]);
            Pointer[z] = *PixelsMappa[z];
            break;
        case 2:
            if (Xmap[z] - Xmap[z-1] > 1)
                PixelsMappa[z]=createPixel(Pixeldim,(PixelsMappa[z-1]->centro[0]+(Xmap[z]-Xmap[z-1])*Pixeldim),(PixelsMappa[z-1]->centro[1]));
            else
                PixelsMappa[z]=createPixel(Pixeldim,(PixelsMappa[z-1]->centro[0]+Pixeldim),(PixelsMappa[z-1]->centro[1]));
            Pointer[z]=*PixelsMappa[z];
            break;
        case 3:
            if(Ymap[z]-Ymap[z-1]>1)
                PixelsMappa[z]=createPixel(Pixeldim,(PixelsMappa[z-1]->centro[0]),(PixelsMappa[z-1]->centro[1]+(Ymap[z]-Ymap[z-1])* Pixeldim));
            else
                PixelsMappa[z]=createPixel(Pixeldim,(PixelsMappa[z-1]->centro[0]),(PixelsMappa[z-1]->centro[1] + Pixeldim));
            Pointer[z]=*PixelsMappa[z];
            break;
        case 4:
            if(Xmap[z-1]-Xmap[z]>1)
                PixelsMappa[z]=createPixel(Pixeldim,(PixelsMappa[z-1]->centro[0]-(Xmap[z-1]-Xmap[z])*Pixeldim),(PixelsMappa[z-1]->centro[1]));
            else
                PixelsMappa[z]=createPixel(Pixeldim,(PixelsMappa[z-1]->centro[0]-Pixeldim),(PixelsMappa[z-1]->centro[1]));
            Pointer[z]=*PixelsMappa[z];
            break;
        }
    }
}


