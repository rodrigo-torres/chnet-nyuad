#include "mainwindow.h"
#include "map.h"

extern int Pixeldim, *shared_memory_cmd;


int pixel_on_map(int &x, int &y) { // Returns true if the coordinate (x, y) belongs to a pixel
    if (x <= x_len && y <= y_len) {
        x /= Pixeldim;
        y /= Pixeldim;

        /* Calculate the iterator for the array member containing these points */
        int true_length = x_len / Pixeldim;
        int it = y * true_length;
        y % 2 == 0 ? it += x : it += (true_length - x - 1);

        /* Compute the position in physical units corresponding to this point */
        x = min_x + x * x_step;
        y = min_y + y * y_step;

        return it;
    }
    return -1;
}

void MainWindow::export_map()
{
    QImage image;
    QString path("/home/frao/Desktop/XRFData");

    if (image.loadFromData(MapImage))
    {
        QString fileName =  QFileDialog::getSaveFileName(this, "Save image as PNG", path, tr("Images (*.png)"));

        if (!fileName.isEmpty())
        {
            if (!fileName.endsWith(".png")) fileName.append(".png");

            QFile file(fileName);
            file.open(QIODevice::WriteOnly);

            if (image.save(&file))
            {
                file.close();
                fileName.prepend("... File saved in: ");
                status->showMessage(fileName, 30);
            }
        }
    }
    else status->showMessage("[!] No image loaded", 30000);
}
