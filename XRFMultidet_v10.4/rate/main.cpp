#include <qapplication.h>
#include <qtabwidget.h>
#include "dialtab.h"
#include <QFile>
#include <QDebug>

int Resolution_mode=0;


int main ( int argc, char **argv )
{
    QApplication a( argc, argv );

    QTabWidget tabWidget;

    DialTab *dialTab = new DialTab();
    dialTab->setAutoFillBackground( true );
    dialTab->setPalette( Qt::darkGray );

    tabWidget.addTab( dialTab, "XRF rate-meter" );

     QString ResMod;

     QString text="ScreenResolution";
      QFile file(text);
      if(file.exists())
      {
        file.open(QIODevice::ReadOnly);
        QString line;
         line = file.readLine();
         int numero=line.toInt();
         file.close();
         if (numero >  2400)                    {Resolution_mode=0; ResMod="high";}
         if (numero <  2400 && numero >= 1750)  {Resolution_mode=1; ResMod="medium";}
         if (numero <= 1750)                    {Resolution_mode=2; ResMod="low";}
      }
     else {qDebug()<<"ScreenResolution not defined....!!!\n"; exit ;}


    if(Resolution_mode==0)
    tabWidget.resize( 720, 820 );  
    if(Resolution_mode==1)
    tabWidget.resize( 540, 615 );  
    if(Resolution_mode==2)
    tabWidget.resize( 360, 410 );  


    tabWidget.show();

    return a.exec();
}

