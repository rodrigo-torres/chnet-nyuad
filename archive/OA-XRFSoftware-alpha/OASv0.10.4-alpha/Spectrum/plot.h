#ifndef _PLOT_H_
#define _PLOT_H_

#include <qwt_plot.h>
#include<QPushButton>

class QwtPlotCurve;
class QwtPlotMarker;
class MainWindow;

class Plot: public QwtPlot 
{
    Q_OBJECT

public:
    Plot( QWidget *parent );
 

public Q_SLOTS:


    void Calibration(double gradient, double offset);
    void setGradient(double grad);
    void setOffset(double off);
    void timerRefreshEvent();
    void Open();
    void Check_SHM();
    void showPixelHisto();
    void set_calibrationparam();
    void ch1(double);
    void ch2(double);
    void E1(double);
    void E2(double);
    void loadCalibration();

signals:
    void filenameChanged(QString filename);
    void calibrationActive(bool active);
    
private:

    void showData( const double *frequency, const double *amplitude, int count );

    MainWindow *d_mainwindow;
    QwtPlotCurve *d_curve1;
    QwtPlotMarker *d_marker1;
    QwtPlotMarker *d_marker2;

};

#endif
