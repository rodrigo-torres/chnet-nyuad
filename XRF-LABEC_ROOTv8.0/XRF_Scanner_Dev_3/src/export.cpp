#include "include/MAXRF/mainwindow.h"
#include <../Header.h>
#include <../Shm.h>
#include <QPixmap>

extern QByteArray MapImage;

void MainWindow::Image_Export() //le esstensioni sono in coda al file a Image_export /////CAMBIARE IL FORmAt SCRITTO IN GRAPHEXPORT

{

  Image_to_save = new QImage;
  if(Image_to_save->loadFromData(MapImage)) //qDebug()<<"image loaded";
  {
    QString fileName =  QFileDialog::getSaveFileName(this,tr("Export image as... *.png"), tr("png Files (*.png)"));
    if (!fileName.isEmpty())
     {
     QString strSaveName, endCommand, extension;
     strSaveName= ""; extension=".png"; //endCommand="\"";
     strSaveName.append(fileName);
     if (!strSaveName.endsWith(".png")) {strSaveName.append(extension);} 
     QFile file(strSaveName);
     file.open(QIODevice::WriteOnly);
     if(Image_to_save->save(&file)) //qDebug()<<"ok";
     {file.close(); strSaveName.prepend("file saved in: "); CurrentAction->setText(strSaveName);}
     }
  }
  else qDebug()<<"no image loaded"; CurrentAction->setText("no image loaded");
}



/* {                                                                    // Orig DefWorkDir -> "/home/frao/QtRoot/Qt/root_ui/Saved"
   switch(checkImageFormat)
     { 
     case 0:                                                                   // DefWorkDir "/home/frao/QtRoot/Qt/root_ui/Saved"
       { 	 
	 QString fileName =  QFileDialog::getSaveFileName(this,tr("Export image as... *.png"),DefWorkDir, tr("png Files (*.png)"));
	 if (!fileName.isEmpty())
	   {
	     QString strSaveName, endCommand, extension;
	     strSaveName= "\""; extension=".png"; endCommand="\"";
	     strSaveName.append(fileName);
	     if (!strSaveName.endsWith(".png")) {strSaveName.append(extension);} 
	     strSaveName.append(endCommand);
	     //fileName.append(extension);
	     QFile file1("GraphExport");
	     file1.open(QIODevice::WriteOnly | QIODevice::Text);
	     QTextStream out(&file1);
	     out << strSaveName << "\n";
	     file1.close();
	   }
	 break; 
       }
     case 1:
       { 	 
	 QString fileName =  QFileDialog::getSaveFileName(this,tr("Export image as... *.tiff"),DefWorkDir, tr("tiff Files (*.tiff)"));
	 if (!fileName.isEmpty())
	   {
	     QString strSaveName, endCommand, extension;
	     strSaveName= "\""; extension=".tiff"; endCommand="\"";
	     strSaveName.append(fileName);
	     if (!strSaveName.endsWith(".tiff")) {strSaveName.append(extension);} 
	     strSaveName.append(endCommand);
	     //fileName.append(extension);
	     QFile file1("GraphExport");
	     file1.open(QIODevice::WriteOnly | QIODevice::Text);
	     QTextStream out(&file1);
	     out << strSaveName << "\n";
	     file1.close();
	   }
	 break;
       }
     case 2:
       {
	 QString fileName =  QFileDialog::getSaveFileName(this,tr("Export image as... *.ps"),DefWorkDir, tr("ps Files (*.ps)"));
	 if (!fileName.isEmpty())
	   {
	     QString strSaveName, endCommand, extension;
	     strSaveName= "\""; extension=".ps"; endCommand="\"";
	     strSaveName.append(fileName);
	     if (!strSaveName.endsWith(".ps")) {strSaveName.append(extension);} 
	     strSaveName.append(endCommand);
	     //fileName.append(extension);
	     QFile file1("GraphExport");
	     file1.open(QIODevice::WriteOnly | QIODevice::Text);
	     QTextStream out(&file1);
	     out << strSaveName << "\n";
	     file1.close();
	   }
	 break;
       }
     case 3:
       {
	 QString fileName =  QFileDialog::getSaveFileName(this,tr("Export image as... *.eps"),DefWorkDir, tr("eps Files (*.eps)"));
	 if (!fileName.isEmpty())
	   {
	     QString strSaveName, endCommand, extension;
	     strSaveName= "\""; extension=".eps"; endCommand="\"";
	     strSaveName.append(fileName); 
	     if (!strSaveName.endsWith(".eps")) {strSaveName.append(extension);} 
	     //		 strSaveName.append(extension); 
	     strSaveName.append(endCommand);
	     //fileName.append(extension);
	     QFile file1("GraphExport");
	     file1.open(QIODevice::WriteOnly | QIODevice::Text);
	     QTextStream out(&file1);
	     out << strSaveName << "\n";
	     file1.close();
	   } 
	 break;
       }
     }
 }   
}

*/


