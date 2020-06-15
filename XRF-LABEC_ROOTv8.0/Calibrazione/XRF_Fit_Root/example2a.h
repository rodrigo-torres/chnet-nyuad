#include <TQObject.h>
#include <RQ_OBJECT.h>



class TGWindow;
class TGMainFrame;
class TRootEmbeddedCanvas;
class TGStatusBar;
class TTF; //text font and size
class TGRadioButton;
class TGButtonGroup;
class TLegend;
class TF1;
class TTimer;
class TLine;
class TH1F;
class TCanvas;

class TGTransientFrame;
class TGLabel;
class TGTextBuffer;
class TGTextEntry;
class TGButton;
class TGTextButton;
class TGFileDialog;

class MyMainFrame {
  RQ_OBJECT("MyMainFrame")
    private:
  TGMainFrame
    *fMain;



  TRootEmbeddedCanvas *fEcanvas;
  TGStatusBar *StatusBar;
 public:
  MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
  virtual ~MyMainFrame();


  TF1 *signalFcn;
  TF1 *signalFcn1;
  TF1 *signalFcn2;
  TF1 *backFcn;
  TF1 *fitFcn;
  TF1 *g1;
  TLegend *legend;
  TTimer *timer1;
  TLine *marker1;
  TLine *marker2;
  TLine *marker3;
  TLine *marker4;
  TLine *marker5;
  TLine *marker6;
  TH1F *f1;
  TCanvas *fCanvas;

 
  TGTextButton *DoFit; 
  TGTextButton *DoDoubleFit;
  TGTextButton *CalButton;


  void CloseMainWindow();
  void DoDraw();
  void TimeOut();
  void Refresh();
  void endRoot();
  void H2DSurf(int);
  void DoubleDoDraw();
  void CalculateCal();

  void SingleDraw();
  void InfoBar();
  void LogScale();
  void Grid();
  void CmndList();

  void RunFit();
  void RunDoubleFit();


//  void FitLabel(); // vale per shmemsildaq5f.cxx

  //  void LabelOnHisto();
};
