#include "mainwindow.h"
#include "map.h"

extern int Pixeldim, *shared_memory_cmd;
Pixel_BIG *Pointer;

void createPixel(size_t pix) {
    Pixel_BIG *px = &Pointer[pix];
    px->dimensione = Pixeldim;

    size_t contour = static_cast<size_t>(Pixeldim * Pixeldim);
    px->total = new punto[contour];

    for (int i = 0; i < Pixeldim; i++) {
        for (int j = 0; j < Pixeldim; j++){
            int index = (Pixeldim * i) + j;
            px->total[index].point_x = x_coord[pix] * Pixeldim + i;
            px->total[index].point_y = y_coord[pix] * Pixeldim + j;
        }
    }
}

int pixel_on_map(int *x, int *y) { // Returns true if the coordinate (x, y) belongs to a pixel
    if (*x <= x_len && *y <= y_len) {
        *x /= Pixeldim;
        *y /= Pixeldim;

        /* Calculate the iterator for the array member containing these points */
        int it = *y * (x_len / Pixeldim);
        *y % 2 == 0 ? it += *x : it += (x_len - *x - 1);

        /* Compute the position in physical units corresponding to this point */
        *x = min_x + *x * x_step;
        *y = min_y + *y * y_step;

        return it;
    }
    return -1;
}

void MainWindow::Define_Pixels() {
    if (Pointer != nullptr) delete[] Pointer;

    size_t size = x_coord.size();
    Pointer = new Pixel_BIG[size];

    for (size_t z = 0; z < size; z++) createPixel(z);
}

void MainWindow::export_map()
{
    QImage *saved_image = new QImage;
    if(saved_image->loadFromData(MapImage)) {
        QString fileName =  QFileDialog::getSaveFileName(this,tr("Export image as... *.png"), tr("png Files (*.png)"));
        if (!fileName.isEmpty()) {
            QString strSaveName, endCommand, extension;
            strSaveName= ""; extension=".png";
            strSaveName.append(fileName);
            if (!strSaveName.endsWith(".png")) {strSaveName.append(extension);}
            QFile file(strSaveName);
            file.open(QIODevice::WriteOnly);
            if(saved_image->save(&file)) {
                file.close();
                strSaveName.prepend("... File saved in: ");
                //CurrentAction->setText(strSaveName);
                status->showMessage(strSaveName, 30);
            }
        }
    }

    else //qDebug()<<"... No image loaded";
        status->showMessage("No image loaded", 30);
}
