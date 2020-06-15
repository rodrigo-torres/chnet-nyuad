
#ifndef EXAMPLE_H
#define EXAMPLE_H

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

#include "Qt-Root_XRF_Fit.h"
#include "MultiFit_Window.h"

#define MAX_NEWSPE  6

//classi di Qt
class QPaintEvent;
class QResizeEvent;
class QMouseEvent;
class QPushButton;
class QTimer;
class QAction;
class QMenu;
class QLineEdit;
class QLabel;


///classi di root
class TCanvas;
class TH1F;
class TLine;
class TLegend;
class TPolyMarker;
class TMarker;
class TSpectrum;

class QRootCanvas : public QWidget
{
   Q_OBJECT

public:
   QRootCanvas( QWidget *parent = 0);
   virtual ~QRootCanvas() {}

   TCanvas        *fCanvas;
   TCanvas* getCanvas() { return fCanvas;}
   TH1F *fnew[MAX_NEWSPE];
   TH1F *f1;
   TLine *marker1;
   TLine *marker2;
   TLine *marker3;
   TLine *marker4;
   TLine *marker5;
   TLine *marker6;

   MultiFit_Window *FWindow;

   void DoDraw();
   void RunFit();
   void DoubleDoDraw();
   void RunDoubleFit();
   void CalculateCal();

protected:
  
   virtual void    mouseMoveEvent( QMouseEvent *e );
   virtual void    mousePressEvent( QMouseEvent *e );
   virtual void    mouseReleaseEvent( QMouseEvent *e );
   virtual void    paintEvent( QPaintEvent *e );
   virtual void    resizeEvent( QResizeEvent *e );

signals:
  void  sendText(QString & newtext); //  const  newText

};

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
     explicit MainWindow(QWidget *parent = 0);
     ~MainWindow(); // {} //virtual 
     virtual void changeEvent(QEvent * e);

   //TCanvas        *fCanvas;
     QLineEdit *xLineEdit;
     QLineEdit *yLineEdit;
     TPolyMarker * pm;
     TMarker *pm_one[12];
     TSpectrum *s;
     TLine *markerEn[100]; //100 puntatori liberi
     TLine *markerE1;
     TLine *markerE2;
     TLine *markerE3;
     TLine *markerE4;
     TLine *markerE5;
     TLine *markerE6;

public slots:
 //  void clicked1();
   void LogStateChanged(int state);
   void GridStateChanged(int state);
   void SetCurrentAction(QString & text); //text
   void handle_root_events();
   void Select_Background();
   void SingleFit();
   void DoubleFit();
   void Calibrate();
   void FindPeaks();
   void LoadSpectrum();
   void LoadCaen();
   void OpenTable();
   void PTtimerEvent();
   void openMultiFitWindow();
   void FP_rimbalzo();
   void IndicatePeaks();
   void Fit_All_Peaks();
   void Clear_MultiFit();
   void ClearSpectrum();
   void SuperImposeNewSpectrum();


private:
     //QVBoxLayout *l;
     QSpacerItem *spacer;
     QGridLayout *l;
     QWidget *window;
     QGroupBox *groupBox;
     QLabel *infoLabel;
     QMenu *fileMenu;
     QMenu *FitMenu;
     QMenu *CalMenu;
     QMenu *SpectrumMenu;
     QAction *openAct;
     QAction *openCaenAct;
     QAction *exitAct;
     QAction *BkgSelAct;
     QAction *SingleFitAct;
     QAction *DoubleFitAct;
     QAction *CalAct;
     QAction *PeaksAct;
     QAction *TableAct;
     QAction *MultiFitAct;
     QAction *ClearAct;
     

     QLabel *x_label;
     QLabel *y_label;
     QLabel *Log_label;
     QLabel *Grid_label;

     QPushButton *Spe2_pushButton;

     QCheckBox *Log_checkBox;
     QCheckBox *Grid_checkBox;
     QFrame *line1; 
     QFrame *line2; 


protected:
   void closeEvent(QCloseEvent *event);
   QRootCanvas    *canvas;
   QPushButton    *b;
   QTimer         *fRootTimer;
   QTimer         *PTabletimer;
};

#endif


