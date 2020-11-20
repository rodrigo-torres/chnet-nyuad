#ifndef _PLOT_H_
#define _PLOT_H_

#include<QPushButton>
#include <QSignalMapper>
#include <qwt_math.h>
#include <qwt_scale_engine.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qmath.h>
#include <qfile.h>
#include <qstring.h>
#include <qfiledialog.h>
#include <qwt_counter.h>
#include <qwt_plot.h>

#include <strstream>

class QwtPlotCurve;
class QwtPlotMarker;
class MainWindow;

class Plot: public QwtPlot {
    Q_OBJECT

public:
    Plot( QWidget *parent );
    MainWindow *d_mainwindow;
 
public slots:
    void Calibration(double gradient, double offset);
    void Open();
    void Check_SHM();
    void showPixelHisto();
    void loadCalibration();
signals:
    void statusChanged(QString filename);
    
private:
    QwtPlotCurve *d_curve1;
    QwtPlotMarker *d_marker1;
    QwtPlotMarker *d_marker2;

};

#endif
