#include <QtGui>
#include <QSpinBox>
#include "mainwindow.h"

extern int Resolution_mode;

void MainWindow::GuiCreator()
{

    QWidget *mywidget= new QWidget;
    setCentralWidget(mywidget);

    QString styleSheet_H = "::section {" // "QHeaderView::section {"
                         "spacing: 10px;"
                         "background-color: lightgray;"
                         //"color: white;"
                         //"border: 1px solid red;"
                         //"margin: 1px;"
                         //"font-size: 12px;"
                         //"font-family: arial;"
                          "text-align: center;}";
     
    QString styleSheet_V = "::section {" // "QHeaderView::section {"
                         "spacing: 10px;"
                         "background-color: lightgray;"
                         //"color: white;"
                         //"border: 1px solid red;"
                         //"margin: 1px;"
                         //"font-size: 12px;"
                         //"font-family: arial;"
                          "text-align: center;}";




//////////////// INPUT SIGNAL/////////////////////////////



    QLabel   *labelSpin = new QLabel(mywidget);
    labelSpin->setText("  Standard parameters of C-863 PI controllers for M404 and VT-80 motors");


    QPushButton *button2 = new QPushButton(mywidget);
    button2->setText("quit");


      QTableWidget* table = new QTableWidget(mywidget);
      table->setRowCount(61);
      table->setColumnCount(7);

      QStringList HList;
      HList << "" << "SPA" << "M404.2PD"<< "SPA" << "VT80-100mm"<< "SPA" << "VT80-150mm";
      table->horizontalHeader()->setStyleSheet(styleSheet_H); 
      table->verticalHeader()->setStyleSheet(styleSheet_V);
      table->setHorizontalHeaderLabels(HList);

///////////////////////////////////// LOADING DATA FILES
       for(int i=0; i<60; i++)
        {
         table->setItem(i, 0, new QTableWidgetItem());
//         table->setItem(i, 0, new QTableWidgetItem(numero));
        }

      QString textdata="Program_Data_Files/SPA_CMD";
      QFile filedata(textdata);
      filedata.open(QIODevice::ReadOnly);
      if(filedata.exists())
      {
         QString Ini_line; //////////////////////// first line removed
         Ini_line = filedata.readLine();

       for(int i=0; i<60; i++)
        {
         QString line;
         line = filedata.readLine();
//         int numero=line.toInt();
         table->setItem(i, 1, new QTableWidgetItem(line));
//         table->setItem(i, 0, new QTableWidgetItem(numero));
        }
      }
     else {qDebug()<<"SPA data missing....!!!\n"; exit ;}
     filedata.close();



      QString textM404_2PD="Program_Data_Files/M404.2PD";
      QFile file(textM404_2PD);
      file.open(QIODevice::ReadOnly);
      if(file.exists())
      {
         QString Ini_line;
         Ini_line = file.readLine();
       for(int i=0; i<60; i++)
        {
         QString line;
         line = file.readLine();
         table->setItem(i, 2, new QTableWidgetItem(line));
        }
      }
     else {qDebug()<<"M404.2PD data missing....!!!\n"; exit ;}
     file.close();


/////////////////////////////////////////////////////////////////////LINE 2

      filedata.open(QIODevice::ReadOnly);
      if(filedata.exists())
      {
         QString line;
         line = filedata.readLine();

       for(int i=0; i<60; i++)
        {
         line = filedata.readLine();
        table->setItem(i, 3, new QTableWidgetItem(line));
        }
      }
     else {qDebug()<<"SPA data missing....!!!\n"; exit ;}
     filedata.close();



      QString textVT80_100="Program_Data_Files/vt80-100";
      QFile fileVT80_100(textVT80_100);
      fileVT80_100.open(QIODevice::ReadOnly);
      if(fileVT80_100.exists())
      {
         QString Ini_line;
         Ini_line = fileVT80_100.readLine();

       for(int i=0; i<60; i++)
        {
         QString line;
         line = fileVT80_100.readLine();
         table->setItem(i, 4, new QTableWidgetItem(line));
        }
      }
     else {qDebug()<<"vt80-100 data missing....!!!\n"; exit ;}
     fileVT80_100.close();

/////////////////////////////////////////////////////////////////////LINE 3

      filedata.open(QIODevice::ReadOnly);
      if(filedata.exists())
      {
         QString line;
         line = filedata.readLine();

       for(int i=0; i<60; i++)
        {
         line = filedata.readLine();
         table->setItem(i, 5, new QTableWidgetItem(line));
        }
      }
     else {qDebug()<<"SPA data missing....!!!\n"; exit ;}
     filedata.close();



      QString textVT80_150="Program_Data_Files/vt80-150";
      QFile fileVT80_150(textVT80_150);
      fileVT80_150.open(QIODevice::ReadOnly);
      if(fileVT80_150.exists())
      {
         QString Ini_line;
         Ini_line = fileVT80_150.readLine();

       for(int i=0; i<60; i++)
        {
         QString line;
         line = fileVT80_150.readLine();
         table->setItem(i, 6, new QTableWidgetItem(line));
        }
      }
     else {qDebug()<<"vt80-150 data missing....!!!\n"; exit ;}
     fileVT80_150.close();




// GEOMETRY ///////////////////////////////////////////////////////////////////

//qDebug()<<Resolution_mode;

   switch (Resolution_mode)
   {
    case(0):
    {
/////INPUT

    table->setColumnWidth(0,30);
    table->setColumnWidth(1,80);
    table->setColumnWidth(2,200);
    table->setColumnWidth(3,80);
    table->setColumnWidth(4,200);
    table->setColumnWidth(5,80);
    table->setColumnWidth(6,200);


    table->setGeometry(QRect(30, 60, 960, 380));

    button2->setGeometry(QRect(20, 480, 960, 56));
    break;
    }

    case(1): ////////// LOW RESOLUTION CASES
    {
    table->setGeometry(QRect(262, 67, 285, 270));
    button2->setGeometry(QRect(15, 360, 540, 42));
    break;
    }

    case(2):
    {
    table->setColumnWidth(2, 40);
    button2->setGeometry(QRect(10, 240, 360, 28));
    break;
    }
   }

     connect(button2, SIGNAL(clicked()), this, SLOT(Exit())); ///aggiungere check se è fermo qApp, SLOT(quit())




}



