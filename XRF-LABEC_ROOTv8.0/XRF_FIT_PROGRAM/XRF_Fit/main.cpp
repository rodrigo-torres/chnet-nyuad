#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qmenubar.h>
#include <qapplication.h>
#include <qimage.h>
#include <qtimer.h>
#include "Qt-Root_XRF_Fit.h"
#include "MultiFit_Window.h"
#include <stdlib.h>
#include <TApplication.h>
#include <TSystem.h>

int main(int argc, char** argv)
{
    TApplication rootapp("Qt + ROOT Fit Program", &argc, argv);
    QApplication app(argc,argv);
    MainWindow m(0);

    m.resize(m.sizeHint());
    m.setWindowTitle("XRF Fit");
    m.setGeometry( 100, 100, 700, 550 ); //x,y pixelX,pixelY
    m.show();

    QObject::connect( qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()) );

    return app.exec();
}


