#include <qmainwindow.h>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <qtoolbutton.h>
#include <qwt_counter.h>
//#include <plot.h>
#include <QGridLayout>
#include <QtGui>
#include <QToolButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QGroupBox>

#include <qregexp.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qstatusbar.h>

#include <qpicture.h>
#include <qpainter.h>

#include <qwt_counter.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_renderer.h>
#include <qwt_text.h>
#include <qwt_math.h>
#include "pixmaps.h"
#include <QTimer>
#include <QFile>
#include<QGroupBox>
#include<QLabel>
#include<QLineEdit>
#include<QDialogButtonBox>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QPushButton>
#include <QMouseEvent>
#include <QFileDialog>
#include <QDockWidget>
#include <QString>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


class QwtCounter;
class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class Plot;
class QPolygon;


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = 0 );
     ~MainWindow();
    virtual void closeEvent(QCloseEvent *event);

    QTimer *timerRefresh;
    QTimer *timer_SHM;
    QTimer *timer_Info;
    QDialog *dlg;
    QPushButton *buttonOK;
    QPushButton *buttonCANC;
    QwtCounter *cntDamp;
    QwtCounter *cntDamp2;
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
