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
  TF1 *backFcn;
  TF1 *fitFcn;
  TF1 *g1;
  TLegend *legend;

  void CloseMainWindow();
  void DoDraw();
  void TimeOut();
  void Refresh();
  void endRoot();
  void H2DSurf(int);

  void EnTimeCal();
//  void TimeSqrt(); // vale per shmemsildaq8fAms.cxx

  void Fit_enabled();
  void SingleDraw();
  void PreviewOn();
  void InfoBar();
  void PrintGui();
  void LogScale();
  void Grid();
  void CmndList();
  void TimeSqrt();

  void RunFit();


//  void FitLabel(); // vale per shmemsildaq5f.cxx

  //  void LabelOnHisto();
};
