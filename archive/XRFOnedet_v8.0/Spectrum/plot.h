#ifndef _PLOT_H_
#define _PLOT_H_

#include <qwt_plot.h>
#include<QPushButton>

class QwtPlotCurve;
class QwtPlotMarker;

class Plot: public QwtPlot 
{
    Q_OBJECT

public:
    Plot( QWidget *parent );
 

public Q_SLOTS:

 
    void Energy_Channel_Choice( bool ChEn);
    void Calibration(double constant);
    void Offset(double off);
    void timerRefreshEvent();
    void Open();
    void Check_SHM();
    void showPixelHisto();
    void Retta_Cal();
    void ch1(double);
    void ch2(double);
    void E1(double);
    void E2(double);
   
    
private:

    void showData( const double *frequency, const double *amplitude, int count );

    QwtPlotCurve *d_curve1;
    QwtPlotMarker *d_marker1;
    QwtPlotMarker *d_marker2;

};

#endif
