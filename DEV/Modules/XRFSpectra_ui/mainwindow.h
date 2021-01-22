#include <iostream>
#include <string>

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QtWidgets>

#include <qwt_counter.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_renderer.h>
#include <qwt_text.h>
#include <qwt_math.h>


#include "MAXRF/ipc_methods.h"
#include "pugi/pugixml.hpp"
#include "pixmaps.h"

class QwtCounter;
class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class Plot;
class QPolygon;


extern template class maxrf::ipc::shmarray<int>;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = nullptr);
     ~MainWindow();
    virtual void closeEvent(QCloseEvent *event);

    QTimer *timerRefresh;
    QTimer *timer_SHM;
    QDialog *dlg;
    QPushButton *buttonOK;
    QPushButton *buttonCANC;
//    QwtCounter *cntDamp;
//    QwtCounter *cntDamp2;
    QDoubleSpinBox *cntDamp;
    QDoubleSpinBox *cntDamp2;
    QToolButton *Log;
    QToolButton *Energy;
    QStatusBar *status;
    QCheckBox *AutoScale;
    QCheckBox *OnTop;
    Plot *d_plot;

public slots:
    void liveHistogram(bool run);
    void enableAutoScale(bool);
    void enableOnTop(bool);
    void showInfo(QString filename);
    void mousePressEvent(QMouseEvent *event);
    void preOpen();

    void toggleLogScale();
    void toggleEnergy(bool active);


private Q_SLOTS:
    void dlgCalibration();
    void exportTxt();
    void enableZoomMode( );

private:
    QwtPlotZoomer *d_zoomer[2];
    QwtPlotPicker *d_picker;
    QwtPlotPanner *d_panner;
};
