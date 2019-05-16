#include <qapplication.h>
#include <qtabwidget.h>
#include "dialtab.h"
#include <QFile>
#include <QDebug>

int main ( int argc, char **argv ) {
    QApplication a(argc, argv);
    QTabWidget tabWidget;

    DialTab *dialTab = new DialTab();
    dialTab->setAutoFillBackground( true );
    dialTab->setPalette(Qt::darkGray );

    tabWidget.addTab( dialTab, "XRF rate-meter" );
    tabWidget.resize( 540, 615 );  
    if (false) tabWidget.resize( 360, 410 );
    tabWidget.show();

    return a.exec();
}

