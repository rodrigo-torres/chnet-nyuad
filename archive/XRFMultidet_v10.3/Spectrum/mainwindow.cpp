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
#include "plot.h"
#include "mainwindow.h"
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

extern double max;
extern double channel_lowerbound; extern double channel_upperbound;
extern int *shm, *shmCommand, shmid, shmidString; 
extern key_t key; extern key_t keyString;
extern int *shared_memory;

extern int *shm_cmd, *shmCommand_cmd, shmid_cmd, shmidString_cmd; 
extern key_t key_cmd; extern key_t keyString_cmd;
extern int *shared_memory_cmd;

extern QString fileName;
QString strSaveName; 
QString extension;

extern bool EnergyOn;
extern bool aperto;
extern double cal_gradient,cal_offset;
///////////////////////////////////////


bool logchecked=false;
bool autoscaleOn=false;
bool provaRun=false;

int length=16385; int resolution=0;
/////////////////////////////////////////
extern bool digichannel0sel, digichannel1sel, digichannel0and1sel;

class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer( int xAxis, int yAxis, QWidget *canvas ):
        QwtPlotZoomer( xAxis, yAxis, canvas )
    {
        setTrackerMode( QwtPicker::AlwaysOff );
        setRubberBand( QwtPicker::NoRubberBand );

    }
};


MainWindow::MainWindow( QWidget *parent ): QMainWindow( parent ) {

    //int pid=getpid();
    *(shared_memory_cmd+81) = getpid();


    d_plot = new Plot( this );

    const int margin = 5;

    d_plot->setContentsMargins( margin, margin, margin, margin );
    setContextMenuPolicy( Qt::NoContextMenu );


    d_zoomer[0] = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft, d_plot->canvas() );
    d_zoomer[0]->setRubberBand( QwtPicker::RectRubberBand );
    d_zoomer[0]->setRubberBandPen( QColor( Qt::red ) );
    d_zoomer[0]->setTrackerMode( QwtPicker::ActiveOnly );
    d_zoomer[0]->setTrackerPen( QColor( Qt::black ) );

    d_zoomer[1] = new Zoomer( QwtPlot::xTop, QwtPlot::yRight, d_plot->canvas() );


    // Shows plot coordinates next to cursor.

    d_picker = new QwtPlotPicker( QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, d_plot->canvas() );
    d_picker->setStateMachine( new QwtPickerDragPointMachine() );
    d_picker->setRubberBandPen( QColor( Qt::red ) );
    d_picker->setRubberBand( QwtPicker::CrossRubberBand );
    d_picker->setTrackerPen( QColor( Qt::black ) );


    setCentralWidget( d_plot );

    /* ##########   Menu    ########## */

    QToolBar *toolBar = new QToolBar( this );
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::RightToolBarArea);


    QToolButton *btnOpen = new QToolButton( toolBar );
    btnOpen->setText( "Open" );
    btnOpen->setIcon( QPixmap( Open_xpm ) );
//    btnOpen->setCheckable( true );
    btnOpen->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( btnOpen );
    connect( btnOpen, SIGNAL( clicked() ), this, SLOT( PreOpen(  ) ) );


//////////////salvare txt//////////////////////

    QToolButton *btnExportTxt = new QToolButton( toolBar );
    btnExportTxt->setText( "Save" );
    btnExportTxt->setIcon( QPixmap( print_xpm ) );
    btnExportTxt->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( btnExportTxt );
    connect( btnExportTxt, SIGNAL( clicked() ), SLOT( exportTxt() ) );

    toolBar->addSeparator();

    /*          Live acquisition trigger button         */

    QToolButton *AutoRefresh = new QToolButton( toolBar );
    AutoRefresh->setText( "Live" );
    AutoRefresh->setIcon( QPixmap( live_xpm ) );
    AutoRefresh->setCheckable( true );
    AutoRefresh->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( AutoRefresh );
    connect( AutoRefresh, SIGNAL( toggled( bool ) ), SLOT ( enableRunMode( bool ) ));

    QToolButton *refresh = new QToolButton( toolBar );
    refresh->setText( "Refresh" );
    refresh->setIcon( QPixmap(refresh_xpm) );
    refresh->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( refresh );
    connect( refresh, SIGNAL( clicked( ) ), d_plot, SLOT(showPixelHisto()) );


    /*          Zoom out button         */

    QToolButton *btnZoom = new QToolButton( toolBar );
    btnZoom->setText( "Zoom Out" );
    btnZoom->setIcon( QPixmap( zoom_xpm ) );
    btnZoom->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( btnZoom );
    connect( btnZoom, SIGNAL( clicked( ) ), SLOT( enableZoomMode(  ) ) );

    toolBar->addSeparator();

    QCheckBox *OnTop = new QCheckBox( toolBar );
    OnTop->setText( "Win On Top" );
    OnTop->setCheckable( true );
    toolBar->addWidget( OnTop );
    connect( OnTop, SIGNAL( toggled( bool ) ), SLOT ( enableOnTop( bool ) ));

    toolBar->addSeparator();

    QCheckBox *AutoScale = new QCheckBox( toolBar );
    AutoScale->setText( "Autoscale" );
    AutoScale->setCheckable( true );
    toolBar->addWidget( AutoScale );
    connect( AutoScale, SIGNAL( toggled( bool ) ), SLOT ( enableAutoScale( bool ) ));
    AutoScale->setChecked(true);

    toolBar->addSeparator();


    fileloaded = new QLineEdit( this );
    fileloaded->setEnabled(false);
    fileloaded->setText("\tNo file loaded into memory");
    fileloaded->setFixedWidth(500);
    toolBar->addWidget(fileloaded);


    QDockWidget *dock = new QDockWidget(tr("DAQ and Calibration"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget *DockBox = new QWidget(this);

    QGridLayout *layout = new QGridLayout( DockBox );
    layout->setSpacing( 1 );

    Energy = new QToolButton( this );
    Energy->setText( "Toggle \nEnergy " );
    Energy->setCheckable( true );
    layout->addWidget( Energy, 0, 0 );
    connect( Energy, SIGNAL( clicked( bool ) ), SLOT( toggleEnergy(bool) ) );

    Log = new QToolButton( this );
    Log->setText( "LogScale" );
    Log->setIcon( QPixmap( LogScale_xpm ) );
    Log->setCheckable( true );
    Log->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    layout->addWidget( Log, 0, 1 );
    connect( Log, SIGNAL( clicked( bool ) ), SLOT ( toggleLogScale() ));


    layout->addWidget( new QLabel( "  keV/Ch  " ), 0,2 );



    cntDamp = new QwtCounter( this );
    cntDamp->setRange( 0.0, 5.0 );
    cntDamp->setSingleStep( 0.01 );
    cntDamp->setValue( 1.0 );
    layout->addWidget( cntDamp, 0,3 );
    connect( cntDamp, SIGNAL( valueChanged(double)), d_plot, SLOT ( setGradient( double ) ));

    layout->addWidget( new QLabel( "  Offset  " ), 0,4 );

    cntDamp2 = new QwtCounter( this );
    cntDamp2->setRange( -1000.0, 1000.0 );
    cntDamp2->setSingleStep( 0.01 );
    cntDamp2->setValue( 0.0 );
    layout->addWidget( cntDamp2, 0, 5 );
    connect( cntDamp2, SIGNAL( valueChanged( double ) ), d_plot, SLOT ( setOffset( double ) ));



    QToolButton *AutoCal = new QToolButton( this );
    AutoCal->setText( "Calculate\nCalibration" );
    layout->addWidget( AutoCal, 0, 7);
    connect( AutoCal, SIGNAL( clicked( bool ) ), this, SLOT ( AutoCalibrate() ));


    dock->setWidget(DockBox);
    addDockWidget(Qt::BottomDockWidgetArea, dock);

    addToolBar( toolBar );
#ifndef QT_NO_STATUSBAR
    ( void )statusBar();
#endif



    timerRefresh = new QTimer(this);
    connect(timerRefresh, SIGNAL(timeout()), d_plot, SLOT(timerRefreshEvent())); //è quello dello spettro online. Controlla ProvaRun
    timerRefresh->start(500);

    timer_SHM = new QTimer(this);
    connect(timer_SHM, SIGNAL(timeout()), d_plot, SLOT(Check_SHM()));       //controlla se è stato cliccato il mouse e chiama ShowPixelHisto
    timer_SHM->start(400);

    timer_Info = new QTimer(this);
    connect(timer_Info, SIGNAL(timeout()), this, SLOT(showInfo()));    //controlla se c'è uno spettro aperto x mostrarne il nome
    timer_Info->start(3000);

    connect(d_plot, SIGNAL(filenameChanged(QString)), this, SLOT(update_fileStatus(QString)));
    connect(d_plot, SIGNAL(calibrationActive(bool)), this, SLOT(toggleEnergy(bool)));

    d_plot->loadCalibration();

}


void MainWindow::enableRunMode( bool run ) {
    if(run) {
        provaRun=true;
        fileloaded->setText("Live acquisition on");
    }
    else {
        provaRun= false;
        fileloaded->setText("Live acquisition off");
    }
}


void MainWindow::enableAutoScale(bool OnAuto) {
    if (OnAuto) {
        autoscaleOn=true;
    }
    else {
        autoscaleOn=false;
    }
}

void MainWindow::toggleLogScale() {

    if ( Log->isChecked() ){
        logchecked=true;
        qDebug()<<"... Logarithmic scale toggled";
    }
    else {
        logchecked=false;
        qDebug()<<"... Linear scale toggled";
    }
  if (!provaRun) {
      d_plot->showPixelHisto();
  }
}

void MainWindow::toggleEnergy(bool active) {

    if ( active )  {
        EnergyOn=true;
        Energy->setText(" Toggle \n Channels ");
        Energy->setChecked(true);
        cntDamp->setValue(cal_gradient);
        cntDamp2->setValue(cal_offset);
    }
    else {
        EnergyOn=false;
        Energy->setText(" Toggle \n Energy ");
        Energy->setChecked(false);
    }
    if (!provaRun) {
        d_plot->showPixelHisto();
    }
}



void MainWindow::enableOnTop(bool OnTp) {

    Qt::WindowFlags flags = this->windowFlags();
    if ( OnTp ) {
       this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
       this->show();
    }

    else {
        this->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
        this->show();
    }
}



void MainWindow::PreOpen() {
    if (Log->isChecked()) {
        Log->setChecked(false);
        logchecked=false;
    }
    d_plot->Open();
}



void MainWindow::exportTxt() {

    QString directory = QFileDialog::getSaveFileName(this,tr("Save as"), QDir::currentPath());
    QFile file1(directory);
    file1.open(QIODevice::ReadWrite);
    QTextStream out(&file1);

    for(int i=0;i<=16384;i++) {
        out<<*(shared_memory+100+i)<<"\t"<<*(shared_memory+20000+i)<<"\t"<<*(shared_memory+40000+i)<<"\t\n";
        *(shared_memory+100+i)=0;
        *(shared_memory+20000+i)=0;
        *(shared_memory+40000+i)=0;
    }

    file1.close();
    qDebug()<<"... File ready!";
}



void MainWindow::enableZoomMode(  )
{
    d_plot->setAxisScale(QwtPlot::yLeft, 0, max);
    d_plot->setAxisScale(QwtPlot::xBottom, channel_lowerbound,channel_upperbound);
    d_plot->replot();
    qDebug()<<"... Zooming";
}


void MainWindow::showInfo() {
    if (aperto)
    {
     statusBar()->showMessage(fileName);
    }

}


void MainWindow::AutoCalibrate() {

    dlg = new QDialog;
    dlg->resize(480,150);

    QGroupBox *groupBox = new QGroupBox( "Calibration" );
    QLabel *label = new QLabel( "First peak (ch)" );
    QwtCounter *cntDamp1 = new QwtCounter( groupBox );
    cntDamp1->setRange( 0, 16384 );
    cntDamp1->setSingleStep( 1 );
    cntDamp1->setValue( 0 );
    cntDamp1->setNumButtons(1);
    cntDamp1->resize(50,50);
    connect( cntDamp1, SIGNAL( valueChanged( double ) ), d_plot, SLOT ( ch1( double ) ));


QLabel *label1 = new QLabel( "First peak (keV)" );
QwtCounter *cntDamp3 = new QwtCounter( groupBox );
    cntDamp3->setRange( 0, 50 );
    cntDamp3->setSingleStep( 1 );
    cntDamp3->setValue( 0 );
    cntDamp3->setNumButtons(1);
    connect( cntDamp3, SIGNAL( valueChanged( double ) ), d_plot, SLOT ( E1( double ) ));
QLabel *label2 = new QLabel( "Second peak (ch)" );
QwtCounter *cntDamp4 = new QwtCounter( groupBox );
    cntDamp4->setRange( 0, 16384 );
    cntDamp4->setSingleStep( 1 );
    cntDamp4->setValue( 0 );
    cntDamp4->setNumButtons(1);
    connect( cntDamp4, SIGNAL( valueChanged( double ) ), d_plot, SLOT ( ch2( double ) ));
QLabel *label3 = new QLabel( "Second peak (keV)" );
QwtCounter *cntDamp5 = new QwtCounter( groupBox );
    cntDamp5->setRange( 0, 50 );
    cntDamp5->setSingleStep( 1 );
    cntDamp5->setValue( 0 );
    cntDamp5->setNumButtons(1);
    connect( cntDamp5, SIGNAL( valueChanged( double ) ), d_plot, SLOT ( E2( double ) ));

QDialogButtonBox *buttons = new QDialogButtonBox( Qt::Horizontal );
buttonOK = new QPushButton( "Ok" );


connect( buttonOK, SIGNAL(clicked()), dlg, SLOT(close()) );
connect( buttonOK, SIGNAL(clicked()), d_plot, SLOT(set_calibrationparam()) );

buttons->addButton( buttonOK, QDialogButtonBox::AcceptRole );
buttonCANC = new QPushButton( "Cancel" );
connect( buttonCANC, SIGNAL(clicked()), dlg, SLOT(close()) );
buttons->addButton( buttonCANC, QDialogButtonBox::AcceptRole );


QGridLayout *Layout = new QGridLayout( groupBox );
Layout->setSpacing(0);
Layout->addWidget( label,0,0 );
Layout->addWidget( cntDamp1,0,1 );
Layout->addWidget( label1,0,2 );
Layout->addWidget( cntDamp3,0,3 );
Layout->addWidget( label2,1,0 );
Layout->addWidget( cntDamp4,1,1 );
Layout->addWidget( label3,1,2 );
Layout->addWidget( cntDamp5,1,3 );
QVBoxLayout *vLayout = new QVBoxLayout( dlg );
vLayout->addWidget( groupBox );
vLayout->addStretch();
vLayout->addWidget( buttons );

dlg->show();

}







 MainWindow::~MainWindow()
 {
   *(shared_memory_cmd+71)=0; // XRF;
 }





void MainWindow::mousePressEvent(QMouseEvent *event)
{
  
  if (event->buttons() == Qt::RightButton) 
  {
   qDebug()<<"zooming -> last view";
  }
}


// A QSlot to receive the signal from Plot::Open()
void MainWindow::update_fileStatus (QString filename) {
    qDebug()<<"About to change the filename in the line edit";
    fileloaded->setText(filename);
}







