
#ifndef CANVAS_H
#define CANVAS_H

#include <QPushButton>
#include <QLayout>
#include <QTimer>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QScrollArea>

#include <TROOT.h>
#include <TCanvas.h>
#include <TVirtualX.h>
#include <TSystem.h>
#include <TFormula.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TFrame.h>
#include <TTimer.h>
#include <TLine.h>
#include <TLegend.h>
#include <TMath.h>
#include <TSpectrum.h>
#include <TPolyMarker.h>
#include <TMarker.h>
#include <TGraph2D.h>

#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>
#include "Riostream.h"
#include <iostream>
#include <errno.h>
#include <sys/ioctl.h>
using std::cout;
using std::cin;
using std::endl;
using namespace std;
#include <stdlib.h>
#include <TApplication.h>
#include <TSystem.h>
#include <QtWidgets>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox> 
#include <QCheckBox>
#include <QMenu>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QDoubleSpinBox>

#include <QApplication>
#include <QDebug>
#include <QObject>
#include <QSizePolicy>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>
#include <QScreen>
#include <time.h>
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QDir>
#include <termios.h>



#include <QWidget>

class QPaintEvent;
class QResizeEvent;
class QMouseEvent;
class QPushButton;
class QTimer;
class TCanvas;

class QRootCanvas : public QWidget
{
   Q_OBJECT

public:
   QRootCanvas( QWidget *parent = 0);
   virtual ~QRootCanvas() {}
   TCanvas* getCanvas() { return fCanvas;}

protected:
   TCanvas        *fCanvas;

   virtual void    mouseMoveEvent( QMouseEvent *e );
   virtual void    mousePressEvent( QMouseEvent *e );
   virtual void    mouseReleaseEvent( QMouseEvent *e );
   virtual void    paintEvent( QPaintEvent *e );
   virtual void    resizeEvent( QResizeEvent *e );
};

class QMainCanvas : public QWidget
{
   Q_OBJECT

public:
   QMainCanvas( QWidget *parent = 0);
   virtual ~QMainCanvas() {}
   virtual void changeEvent(QEvent * e);

public slots:
   void clicked1();
   void handle_root_events();

protected:
   QRootCanvas    *canvas;
   QPushButton    *b;
   QTimer         *fRootTimer;
};

#endif


