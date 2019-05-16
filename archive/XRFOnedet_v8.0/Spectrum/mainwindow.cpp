#include <qregexp.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qstatusbar.h>
//#include <QtGui/QPrinter>
#include <qpicture.h>
#include <qpainter.h>
//#include <QtGui/qprintdialog.h>
//#include <QtGui/QPrintDialog>
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

/// INSERIRE GLI EXTERN
extern double min; extern double max;
extern double mincal; extern double maxcal;
extern int *shm, *shmCommand, shmid, shmidString; 
extern key_t key; extern key_t keyString;
extern int *shared_memory;

extern int *shm_cmd, *shmCommand_cmd, shmid_cmd, shmidString_cmd; 
extern key_t key_cmd; extern key_t keyString_cmd;
extern int *shared_memory_cmd;

extern QString fileName;
QString strSaveName; 
QString extension;

extern bool aperto;
extern double cal,offset;
///////////////////////////////////////
bool calculate_cal=false;
bool new_calibration_ready=false;
bool logchecked=false;
bool autoscaleOn=false;
bool provaRun=false;

int lenght=16384; int resolution=0;


class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer( int xAxis, int yAxis, QWidget *canvas ):
        QwtPlotZoomer( xAxis, yAxis, canvas )
    {
        setTrackerMode( QwtPicker::AlwaysOff );//AlwaysOff );
        setRubberBand( QwtPicker::NoRubberBand );

    }
};


MainWindow::MainWindow( QWidget *parent ):
    QMainWindow( parent )
{
    int pid=getpid();
    *(shared_memory_cmd+81)=pid;


    d_plot = new Plot( this );

    const int margin = 5;
    d_plot->setContentsMargins( margin, margin, margin, margin );

    setContextMenuPolicy( Qt::NoContextMenu );


    d_zoomer[0] = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft,
        d_plot->canvas() );
    d_zoomer[0]->setRubberBand( QwtPicker::RectRubberBand );
    d_zoomer[0]->setRubberBandPen( QColor( Qt::green ) );
    d_zoomer[0]->setTrackerMode( QwtPicker::ActiveOnly );
    d_zoomer[0]->setTrackerPen( QColor( Qt::white ) );

    d_zoomer[1] = new Zoomer( QwtPlot::xTop, QwtPlot::yRight,
         d_plot->canvas() );



    //d_panner = new QwtPlotPanner( d_plot->canvas() );
    //d_panner->setMouseButton( Qt::MidButton );


    d_picker = new QwtPlotPicker( QwtPlot::xBottom, QwtPlot::yLeft,
        QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,
        d_plot->canvas() );
    d_picker->setStateMachine( new QwtPickerDragPointMachine() );
    d_picker->setRubberBandPen( QColor( Qt::green ) );
    d_picker->setRubberBand( QwtPicker::CrossRubberBand );
    d_picker->setTrackerPen( QColor( Qt::white ) );


// void QwtPicker::widgetMouseDoubleClickEvent 


    setCentralWidget( d_plot );

    QToolBar *toolBar = new QToolBar( this );
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::RightToolBarArea);


    QToolButton *btnOpen = new QToolButton( toolBar );
    btnOpen->setText( "Open" );
    btnOpen->setIcon( QPixmap( Open_xpm ) );
//    btnOpen->setCheckable( true );
    btnOpen->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( btnOpen );
    connect( btnOpen, SIGNAL( clicked() ), this, SLOT( PreOpen(  ) ) );

#ifndef QT_NO_PRINTER
    QToolButton *btnPrint = new QToolButton( toolBar );
    btnPrint->setText( "Print" );
    btnPrint->setIcon( QPixmap( print_xpm ) );
    btnPrint->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( btnPrint );
    connect( btnPrint, SIGNAL( clicked() ), SLOT( print() ) );
#endif

//////////////salvare txt//////////////////////

    QToolButton *btnExportTxt = new QToolButton( toolBar );
    btnExportTxt->setText( "Export file" );
    btnExportTxt->setIcon( QPixmap( print_xpm ) );
    btnExportTxt->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( btnExportTxt );
    connect( btnExportTxt, SIGNAL( clicked() ), SLOT( exportTxt() ) );

    toolBar->addSeparator();

////////////////////////////////////////////////////////////////


    QToolButton *AutoRefresh = new QToolButton( toolBar );
    AutoRefresh->setText( "Online" );
    AutoRefresh->setIcon( QPixmap( run_xpm ) );
    AutoRefresh->setCheckable( true );
    AutoRefresh->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( AutoRefresh );
    connect( AutoRefresh, SIGNAL( toggled( bool ) ), SLOT ( enableRunMode( bool ) ));

    toolBar->addSeparator();

    QToolButton *btnZoom = new QToolButton( toolBar );
    btnZoom->setText( "ZoomAll" );
    btnZoom->setIcon( QPixmap( zoom_xpm ) );
//    btnZoom->setCheckable( true );
    btnZoom->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( btnZoom );
    connect( btnZoom, SIGNAL( clicked( ) ), SLOT( enableZoomMode(  ) ) );

    toolBar->addSeparator();

    QCheckBox *OnTop = new QCheckBox( toolBar );
    OnTop->setText( "Win On Top" );
//    OnTop->setIcon( QPixmap( run_xpm ) );
    OnTop->setCheckable( true );
    toolBar->addWidget( OnTop );
    connect( OnTop, SIGNAL( toggled( bool ) ), SLOT ( enableOnTop( bool ) ));

    toolBar->addSeparator();

    QCheckBox *AutoScale = new QCheckBox( toolBar );
    AutoScale->setText( "Autoscale" );
    AutoScale->setCheckable( true );
    toolBar->addWidget( AutoScale );
    connect( AutoScale, SIGNAL( toggled( bool ) ), SLOT ( enableAutoScale( bool ) ));

    toolBar->addSeparator();

    QRadioButton *k4 = new QRadioButton("4k");
    QRadioButton *k8 = new QRadioButton("8k");
    QRadioButton *k16 = new QRadioButton("16k");
    k16->setChecked(true);


    toolBar->addWidget(k4);
    toolBar->addWidget(k8);
    toolBar->addWidget(k16);

    connect(k4, SIGNAL(toggled(bool)), this, SLOT(on_k4(bool)));
    connect(k8, SIGNAL(toggled(bool)), this, SLOT(on_k8(bool)));
    connect(k16, SIGNAL(toggled(bool)), this, SLOT(on_k16(bool)));


    QDockWidget *dock = new QDockWidget(tr("DAQ and Calibration"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget *DockBox = new QWidget(this);

    QGridLayout *layout = new QGridLayout( DockBox );
    layout->setSpacing( 0 );

    QCheckBox *Energy_Channel = new QCheckBox( this );
    Energy_Channel->setText( "Channel/Energy" );
//    OnTop->setIcon( QPixmap( run_xpm ) );
    Energy_Channel->setCheckable( true );
    layout->addWidget( Energy_Channel, 0, 0 );
    connect( Energy_Channel, SIGNAL( toggled( bool ) ), d_plot, SLOT ( Energy_Channel_Choice ( bool ) ));

    Log = new QToolButton( this );
    Log->setText( "LogScale" );
    Log->setIcon( QPixmap( LogScale_xpm ) );
    Log->setCheckable( true );
    Log->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    layout->addWidget( Log, 0, 1 );
    connect( Log, SIGNAL( clicked( bool ) ), SLOT ( logSpace() ));


    layout->addWidget( new QLabel( "  keV/Ch  " ), 0,2 );

    cntDamp = new QwtCounter( this );
    cntDamp->setRange( 0.0, 5.0 );
    cntDamp->setSingleStep( 0.01 );
    cntDamp->setValue( 1.0 );
    layout->addWidget( cntDamp, 0,3 );
    connect( cntDamp, SIGNAL( valueChanged( double ) ), d_plot, SLOT ( Calibration( double ) ));    

    layout->addWidget( new QLabel( "  Offset  " ), 0,4 );

    cntDamp2 = new QwtCounter( this );
    cntDamp2->setRange( -1000.0, 1000.0 );
    cntDamp2->setSingleStep( 0.01 );
    cntDamp2->setValue( 0.0 );
    layout->addWidget( cntDamp2, 0, 5 );
    connect( cntDamp2, SIGNAL( valueChanged( double ) ), d_plot, SLOT ( Offset( double ) ));

    QToolButton *AutoCal = new QToolButton( this );
    AutoCal->setText( "Calculate\nCalibration" );
    layout->addWidget( AutoCal, 0, 6 );
    connect( AutoCal, SIGNAL( clicked( bool ) ), this, SLOT ( AutoCalibrate() ));


    dock->setWidget(DockBox);
    addDockWidget(Qt::BottomDockWidgetArea, dock);



//    ( void )toolBar->addWidget( hBox );


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
}




//////////////////////////////////////////////////// ARRAY SIZE

void MainWindow::on_k4(bool On4)
{
if (On4) {qDebug()<<"4k"; *(shared_memory+99)=1; lenght= 4096; resolution=2;}
}

void MainWindow::on_k8(bool On8)
{
if (On8) {qDebug()<<"8k"; *(shared_memory+99)=1; lenght=8192; resolution=1;}
}
void MainWindow::on_k16(bool On16)
{
if (On16) {qDebug()<<"16k"; *(shared_memory+99)=1; lenght=16384; resolution=0;}
}

/////////////////////////////////////////////////////////////////
void MainWindow::enableRunMode( bool run )
{if(run) {provaRun=true; qDebug()<<"On"<<'\n'; }
else {provaRun= false; qDebug()<<"Off"<<'\n'; }
}


void MainWindow::enableAutoScale(bool OnAuto)
{
if (OnAuto) autoscaleOn=true;
else autoscaleOn=false;
}

 void MainWindow::logSpace()

{
  if (Log->isChecked()){logchecked=true; printf("logchecked=true\n");}

  else {logchecked=false;printf("logchecked=false\n");}
  if (!provaRun){d_plot->showPixelHisto();}
}



void MainWindow::enableOnTop(bool OnTp)
{ 
    
    Qt::WindowFlags flags = this->windowFlags();
    if (OnTp)
    {
       this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
       this->show();
    }
    else
    {
        this->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
        this->show();
    }

     

//if(OnTp) 
//{qDebug()<<"onTop";
//this->setWindowFlags(Qt::WindowStaysOnTopHint);
//}
}

 
/*
#ifndef QT_NO_PRINTER

void MainWindow::print()
{
    QPrinter printer( QPrinter::HighResolution );

    QString docName = d_plot->title().text();
    if ( !docName.isEmpty() )
    {
        docName.replace ( QRegExp ( QString::fromLatin1 ( "\n" ) ), tr ( " -- " ) );
        printer.setDocName ( docName );
    }

    printer.setCreator( "Plot Histo" );
    printer.setOrientation( QPrinter::Landscape );

    QPrintDialog dialog( &printer );
    if ( dialog.exec() )
    {
        QwtPlotRenderer renderer;

        if ( printer.colorMode() == QPrinter::GrayScale )
        {
            renderer.setDiscardFlag( QwtPlotRenderer::DiscardBackground );
            renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasBackground );
            renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasFrame );
            renderer.setLayoutFlag( QwtPlotRenderer::FrameWithScales );
        }

        renderer.renderTo( d_plot, printer );
    }
}

#endif
*/


void MainWindow::PreOpen()
{
if(Log->isChecked()) {Log->setChecked(false); logchecked=false;}
d_plot->Open();
}



void MainWindow::exportTxt()
{

   QString percorso = QFileDialog::getSaveFileName(this,tr("Save as"), QDir::currentPath());
   QFile file1(percorso);
   //file2.remove();
   file1.open(QIODevice::ReadWrite);
   QTextStream out(&file1);

      for(int i=0;i<=16384;i++)
      {      
       out<<*(shared_memory+100+i)<<'\n';
	//qDebug()<<*(shared_memory+100+i)<<'\n';
        *(shared_memory+100+i)=0;
      }
   file1.close();
   qDebug()<<"file ready!\n";
}



void MainWindow::enableZoomMode(  )
{
    d_plot->setAxisScale(QwtPlot::yLeft, min, max);
    d_plot->setAxisScale(QwtPlot::xBottom, mincal,maxcal);
    d_plot->replot();
    qDebug()<<"Zooming";
}


void MainWindow::showInfo()
{
    if (aperto)
    {
     statusBar()->showMessage(fileName);
    }
    if (new_calibration_ready)
    {
     cntDamp->setValue(cal);
     cntDamp2->setValue(offset);
     new_calibration_ready=false;
     }
//    if (EnergyOn) Energy_Channel->setChecked( true );

}


void MainWindow::AutoCalibrate()
{
dlg= new QDialog;
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
connect( buttonOK, SIGNAL(clicked()), this, SLOT(okClicked()) );
buttons->addButton( buttonOK, QDialogButtonBox::AcceptRole );
buttonCANC = new QPushButton( "Cancel" );
connect( buttonCANC, SIGNAL(clicked()), this, SLOT(cancClicked()) );
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


bool MainWindow::okClicked()
{
calculate_cal=true;
dlg->close();
return calculate_cal;
}

bool MainWindow::cancClicked()
{
calculate_cal=false;
dlg->close();
return calculate_cal;

}

/*
void MainWindow::showInfo( QString text )
{
    if ( text == QString::null )
    {
        if ( d_picker->rubberBand() )
            text = "Cursor Pos: Press left mouse button in plot region";
        else
            text = "Zoom: Press mouse button and drag";
    }

#ifndef QT_NO_STATUSBAR
    statusBar()->showMessage( text );
#endif
}

void MainWindow::moved( const QPoint &pos )
{
    QString info;
    info.sprintf( "Ch=%4.4g, Counts=%3.3g", d_plot->invTransform( QwtPlot::xBottom, pos.x() ), d_plot->invTransform( QwtPlot::yLeft, pos.y()) );
    showInfo( info );
}

void MainWindow::selected( const QPolygon & )
{
    showInfo();
}
*/


 MainWindow::~MainWindow()
 {
   *(shared_memory_cmd+71)=0; // XRF;
 }





void MainWindow::mousePressEvent(QMouseEvent *event)
{
  
  if (event->buttons() == Qt::RightButton) 
  {
   qDebug()<<"zooming -> last view";
//   d_plot->
  }
}













