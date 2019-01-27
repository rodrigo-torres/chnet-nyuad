#include <mainwindow.h>
#include <../Header.h>
#include <../Shm.h>
#include <QPixmap>

extern QByteArray MapImage;

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


