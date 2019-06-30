#include "mainwindow.h"

extern int point, Pixeldim;
extern int Xmap[20000],Ymap[20000];
extern struct Pixel_BIG *Pointer;


struct Pixel_BIG *createPixel(int x, int y) {
    struct Pixel_BIG *px;
    px = static_cast<struct Pixel_BIG *>(calloc(1, sizeof(struct Pixel_BIG)));  //zona di memoria per pixel_BIG
    px->centro[0] = x * Pixeldim;
    px->centro[1] = y * Pixeldim;
    px->dimensione = Pixeldim;

    size_t contour = static_cast<size_t>(Pixeldim * Pixeldim);
    px->total = static_cast<struct punto *>(calloc(contour, sizeof(struct punto)));  //(dimensione*dimensione) punti

    for (int i = 0; i < Pixeldim; i++) {
        for (int j = 0; j < Pixeldim; j++){
            int index = (Pixeldim * i) + j;
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

    for (int z = 0; z < point; z++) {
            PixelsMappa[z] = createPixel(Xmap[z], Ymap[z]);
            Pointer[z] = *PixelsMappa[z];
    }
}

void MainWindow::Image_Export()
{
    Image_to_save = new QImage;
    if(Image_to_save->loadFromData(MapImage)) {
        QString fileName =  QFileDialog::getSaveFileName(this,tr("Export image as... *.png"), tr("png Files (*.png)"));
        if (!fileName.isEmpty()) {
            QString strSaveName, endCommand, extension;
            strSaveName= ""; extension=".png"; //endCommand="\"";
            strSaveName.append(fileName);
            if (!strSaveName.endsWith(".png")) {strSaveName.append(extension);}
            QFile file(strSaveName);
            file.open(QIODevice::WriteOnly);
            if(Image_to_save->save(&file)) {
                file.close();
                strSaveName.prepend("... File saved in: ");
                //CurrentAction->setText(strSaveName);
                status->showMessage(strSaveName, 30);
            }
        }
    }

    else //qDebug()<<"... No image loaded";
    //CurrentAction->setText("No image loaded");
    status->showMessage("No image loaded", 30);
}
