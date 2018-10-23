#include <qmainwindow.h>
#include<QPushButton>
#include<QDialog>
#include <qtoolbutton.h>
#include <qwt_counter.h>
#include <plot.h>
#include <QGridLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>

class QwtCounter;
class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class Plot;
class QPolygon;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = 0 );
     ~MainWindow();
     QTimer *timerRefresh;
     QTimer *timer_SHM;
     QTimer *timer_Info;

    QDialog *dlg;
    QPushButton *buttonOK;
    QPushButton *buttonCANC;
    QwtCounter *cntDamp;
    QwtCounter *cntDamp2;

    QToolButton *Log;

    QCheckBox *Energy_Channel;
    QCheckBox *AutoScale;


//    QGridLayout *f_gridLayout;
    QCheckBox *OnTop;


    QRadioButton *k4;
    QRadioButton *k8;
    QRadioButton *k16;

//    QVBoxLayout *layout;
//    QGroupBox *group;





public Q_SLOTS:
   void enableRunMode(bool run);
     void enableAutoScale(bool);
     void enableOnTop(bool); 
     void showInfo();
     void mousePressEvent(QMouseEvent *event);
     void PreOpen();
     void on_k4(bool);
     void on_k8(bool);
     void on_k16(bool);
     void logSpace();
    

private Q_SLOTS:
   // void moved( const QPoint & );
   // void selected( const QPolygon & );
    void AutoCalibrate();
    bool okClicked();
    bool cancClicked();
    

/*
#ifndef QT_NO_PRINTER
    void print();
#endif
*/
    //void exportDocument();
    void exportTxt();
    void enableZoomMode( );

//    void enableZoom(bool);

private:

    Plot *d_plot;
    QwtPlotZoomer *d_zoomer[2];
    QwtPlotPicker *d_picker;
    QwtPlotPanner *d_panner;
};
