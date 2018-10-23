#include <qmainwindow.h>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <qtoolbutton.h>
#include <qwt_counter.h>
#include <plot.h>
#include <QGridLayout>
#include <QtGui>
#include <QToolButton>
#include <QCheckBox>
#include <QSpinBox>
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
    QLineEdit *fileloaded;

    QToolButton *Log;
    QToolButton *Energy;

    QCheckBox *AutoScale;

    QCheckBox *OnTop;




public Q_SLOTS:
    void enableRunMode(bool run);
    void update_fileStatus(QString filename);
    void enableAutoScale(bool);
    void enableOnTop(bool);
    void showInfo();
    void mousePressEvent(QMouseEvent *event);
    void PreOpen();

    void toggleLogScale();
    void toggleEnergy(bool active);


private Q_SLOTS:
    void AutoCalibrate();
    void exportTxt();
    void enableZoomMode( );

private:

    Plot *d_plot;
    QwtPlotZoomer *d_zoomer[2];
    QwtPlotPicker *d_picker;
    QwtPlotPanner *d_panner;
};
