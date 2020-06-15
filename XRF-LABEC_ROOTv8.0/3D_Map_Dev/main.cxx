#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qmenubar.h>
#include <qapplication.h>
#include <qimage.h>
#include <qtimer.h>

#include "canvas.h"

#include <stdlib.h>
#include <TApplication.h>
#include <TSystem.h>

int main(int argc, char** argv)
{
    TApplication rootapp("Map test: 3D", &argc, argv);
    QApplication app(argc,argv);
    QMainCanvas m(0);

    m.resize(m.sizeHint());
    m.setWindowTitle("3D Map test");
    m.setGeometry( 100, 100, 700, 500 );
    m.show();

    QObject::connect( qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()) );

    return app.exec();
}


