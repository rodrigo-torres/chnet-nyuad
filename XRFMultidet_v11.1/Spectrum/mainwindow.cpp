#include "mainwindow.h"
#include "plot.h"

#include <QSignalBlocker>

extern bool EnergyOn;
extern QString fileName;
extern int *shared_memory_cmd, *shared_memory;
extern double max, channel_lowerbound, channel_upperbound;
extern double channel1_cal, channel2_cal, energy1_cal, energy2_cal;


extern double cal_gradient, cal_offset;
bool logchecked = false, autoscaleOn = false, liveOn = false;

class Zoomer: public QwtPlotZoomer {
public:
    Zoomer( int xAxis, int yAxis, QWidget *canvas ): QwtPlotZoomer( xAxis, yAxis, canvas ) {
        setTrackerMode( QwtPicker::AlwaysOff );
        setRubberBand( QwtPicker::NoRubberBand );
    }
};

MainWindow::MainWindow(QWidget *parent): QMainWindow( parent ) {

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
    btnOpen->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    toolBar->addWidget( btnOpen );
    connect( btnOpen, SIGNAL( clicked() ), SLOT(preOpen()));


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
    connect( AutoRefresh, SIGNAL( toggled( bool ) ), SLOT ( liveHistogram( bool ) ));

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

    QDockWidget *dock = new QDockWidget(tr("DAQ and Calibration"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget *DockBox = new QWidget(this);
    QGridLayout *layout = new QGridLayout( DockBox );
    layout->setSpacing(0);

    Energy = new QToolButton( this );
    Energy->setText( "Toggle \nEnergy " );
    Energy->setCheckable( true );
    layout->addWidget( Energy, 0, 0 );


    Log = new QToolButton( this );
    Log->setText( "LogScale" );
    Log->setIcon( QPixmap( LogScale_xpm ) );
    Log->setCheckable( true );
    Log->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
    layout->addWidget( Log, 0, 1 );

    QLabel *gradLabel = new QLabel(this);
    gradLabel->setText("Calibration grad. (keV / ch)");
    gradLabel->setMaximumHeight(20);
    layout->addWidget(gradLabel, 1, 0, 1, 2, Qt::AlignHCenter);

    QLabel *offLabel = new QLabel(this);
    offLabel->setText("Calibration off. (channels)");
    offLabel->setMaximumHeight(20);
    layout->addWidget(offLabel, 3, 0, 1, 2, Qt::AlignHCenter);

    cntDamp = new QwtCounter( this );
    cntDamp->setValue(1.0);
    cntDamp->setRange(0.0, 5.0);
    cntDamp->setSingleStep(0.01);
    cntDamp->setMinimumWidth(160);
    cntDamp->setMinimumHeight(40);
    layout->addWidget(cntDamp, 2, 0, 1, 2, Qt::AlignHCenter);


    cntDamp2 = new QwtCounter( this );
    cntDamp2->setValue(0.0);
    cntDamp2->setSingleStep(0.01);
    cntDamp2->setMinimumHeight(40);
    cntDamp2->setMinimumWidth(160);
    cntDamp2->setRange(-10.0, 10.0);
    layout->addWidget(cntDamp2, 4, 0, 1, 2, Qt::AlignHCenter);

    QToolButton *AutoCal = new QToolButton( this );
    AutoCal->setText("Calculate calibration");
    layout->addWidget(AutoCal, 5, 0, 1, 2, Qt::AlignHCenter);

    connect( Energy, SIGNAL( clicked( bool ) ), SLOT( toggleEnergy(bool) ) );
    connect( Log, SIGNAL( clicked( bool ) ), SLOT ( toggleLogScale() ));
    connect( AutoCal, SIGNAL( clicked( bool ) ), this, SLOT ( dlgCalibration() ));

    dock->setWidget(DockBox);
    dock->setMaximumHeight(300);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    addToolBar( toolBar );
#ifndef QT_NO_STATUSBAR
    status = statusBar();
    //( void )statusBar();
#endif



    timerRefresh = new QTimer(this);
    timer_SHM = new QTimer(this);   //controlla se è stato cliccato il mouse e chiama ShowPixelHisto
    timer_SHM->start(400);

    connect(timer_SHM, SIGNAL(timeout()), d_plot, SLOT(Check_SHM()));
    connect(d_plot, SIGNAL(calibrationActive(bool)), this, SLOT(toggleEnergy(bool)));

    connect(timerRefresh, SIGNAL(timeout()), d_plot, SLOT(showPixelHisto()));
    //We also want toggleLogScale to be mapped to showPixelHisto
    d_plot->loadCalibration();



    QSignalMapper *plotMapper = new QSignalMapper(this);
    plotMapper->setMapping(cntDamp, 0);
    plotMapper->setMapping(cntDamp2, 1);

    connect(cntDamp, SIGNAL(valueChanged(double)), plotMapper, SLOT(map()));
    connect(cntDamp2, SIGNAL(valueChanged(double)), plotMapper, SLOT(map()));
    connect(plotMapper, SIGNAL(mapped(int)), d_plot, SLOT(setCalParam(int)));

}

MainWindow::~MainWindow() {
    *(shared_memory_cmd+71) = 0;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    *(shared_memory_cmd+71)=0;
    event->accept();
}

void MainWindow::preOpen() {
    if (Log->isChecked()) { Log->setChecked(false); logchecked = false; }
    d_plot->Open();
}

void MainWindow::enableAutoScale(bool OnAuto) {
    if (OnAuto) autoscaleOn = true;
    else autoscaleOn = false;
}

void MainWindow::toggleLogScale() {
    if (Log->isChecked()) logchecked = true;
    else logchecked = false;
    if (!liveOn) d_plot->showPixelHisto();
}

void MainWindow::liveHistogram(bool run) {
    if (run) {
        liveOn = true;
        timerRefresh->start(500);
        status->showMessage("[!] Live acquisition is on"); }
    else {
        liveOn = false;
        timerRefresh->stop();
        status->showMessage("[!] Live acquisition is off", 5000); }
}

void MainWindow::toggleEnergy(bool active) {
    if (active)  {
        EnergyOn = active;
        Energy->setText("Toggle\nChannels");
        Energy->setChecked(true);

        *(shared_memory+24) = 1;
        qDebug()<<*(shared_memory+24);
        cntDamp->blockSignals(true); cntDamp->setValue(cal_gradient);
        cntDamp->blockSignals(false);
        cntDamp2->blockSignals(true); cntDamp2->setValue(cal_offset);
        cntDamp2->blockSignals(false);
    }
    else {
        EnergyOn = active;
        Energy->setText("Toggle\nEnergy");
        Energy->setChecked(false);

        *(shared_memory+24) = 0;
        qDebug()<<*(shared_memory+24);
    }
    if (!liveOn) d_plot->showPixelHisto();
}

void MainWindow::enableOnTop(bool OnTp) {
    Qt::WindowFlags flags = this->windowFlags();
    if (OnTp) {
        this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        this->show();
    }
    else {
        this->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
        this->show();
    }
}

void MainWindow::exportTxt() {
    QString dataDir = "/home/rtorres/Desktop/XRFData";
    QString directory = QFileDialog::getSaveFileName(this, "Save file as...", dataDir);
    QFile file1(directory);
    file1.open(QIODevice::ReadWrite);
    QTextStream out(&file1);

    for (int i = 0; i <= 16384; i++) {
        out<<*(shared_memory+100+i)<<"\t"<<*(shared_memory+20000+i)<<"\t"<<*(shared_memory+40000+i)<<"\t\n";
        *(shared_memory+100+i) = 0;
        *(shared_memory+20000+i) = 0;
        *(shared_memory+40000+i) = 0;
    }

    file1.close();
    printf("... File ready!");
}

void MainWindow::enableZoomMode() {
    d_plot->setAxisScale(QwtPlot::yLeft, 0, max);
    d_plot->setAxisScale(QwtPlot::xBottom, channel_lowerbound, channel_upperbound);
    d_plot->replot();
}

void MainWindow::showInfo(QString filename) {
    status->showMessage(filename);
}

void MainWindow::dlgCalibration() {
    dlg = new QDialog;
    dlg->resize(480,150);

    QGroupBox *groupBox = new QGroupBox( "Calibration" );

    QLabel *label = new QLabel("First peak (ch)");
    QwtCounter *cntDamp1 = new QwtCounter(groupBox);
    cntDamp1->setRange(0, 16384);
    cntDamp1->setSingleStep(1);
    cntDamp1->setNumButtons(1);
    cntDamp1->setValue(channel1_cal);
    //cntDamp1->resize(50,50);

    QLabel *label1 = new QLabel("First peak (keV)");
    QwtCounter *cntDamp3 = new QwtCounter(groupBox);
    cntDamp3->setRange(0, 70);
    cntDamp3->setSingleStep(1);
    cntDamp3->setNumButtons(1);
    cntDamp3->setValue(energy1_cal);

    QLabel *label2 = new QLabel("Second peak (ch)");
    QwtCounter *cntDamp4 = new QwtCounter(groupBox);
    cntDamp4->setRange(0, 16384);
    cntDamp4->setSingleStep(1);
    cntDamp4->setValue(channel2_cal);
    cntDamp4->setNumButtons(1);

    QLabel *label3 = new QLabel("Second peak (keV)");
    QwtCounter *cntDamp5 = new QwtCounter(groupBox);
    cntDamp5->setRange(0, 70);
    cntDamp5->setSingleStep(1);
    cntDamp5->setNumButtons(1);
    cntDamp5->setValue(energy2_cal);

    buttonOK = new QPushButton("Ok");
    buttonCANC = new QPushButton("Cancel");

    QDialogButtonBox *buttons = new QDialogButtonBox(Qt::Horizontal);
    buttons->addButton(buttonOK, QDialogButtonBox::AcceptRole);
    buttons->addButton(buttonCANC, QDialogButtonBox::AcceptRole);

    QSignalMapper *dlgMapper = new QSignalMapper(this);
    dlgMapper->setMapping(cntDamp1, 0);
    dlgMapper->setMapping(cntDamp3, 1);
    dlgMapper->setMapping(cntDamp4, 2);
    dlgMapper->setMapping(cntDamp5, 3);

    connect(cntDamp1, SIGNAL(valueChanged(double)), dlgMapper, SLOT(map()));
    connect(cntDamp3, SIGNAL(valueChanged(double)), dlgMapper, SLOT(map()));
    connect(cntDamp4, SIGNAL(valueChanged(double)), dlgMapper, SLOT(map()));
    connect(cntDamp5, SIGNAL(valueChanged(double)), dlgMapper, SLOT(map()));
    connect(dlgMapper, SIGNAL(mapped(int)), d_plot, SLOT(writeCalParam(int)));

    connect(buttonOK, SIGNAL(clicked()), dlg, SLOT(close()));
    connect(buttonCANC, SIGNAL(clicked()), dlg, SLOT(close()));
    connect(buttonOK, SIGNAL(clicked()), d_plot, SLOT(setCalParam()));

    QGridLayout *Layout = new QGridLayout( groupBox );
    Layout->setSpacing(1);
    Layout->addWidget(label,0,0);
    Layout->addWidget(cntDamp1,0,1);
    Layout->addWidget(label1,0,2);
    Layout->addWidget(cntDamp3,0,3);
    Layout->addWidget(label2,1,0);
    Layout->addWidget(cntDamp4,1,1);
    Layout->addWidget(label3,1,2);
    Layout->addWidget(cntDamp5,1,3);

    QVBoxLayout *vLayout = new QVBoxLayout( dlg );
    vLayout->addWidget(groupBox);
    vLayout->addStretch();
    vLayout->addWidget(buttons);

    dlg->show();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::RightButton) printf("... Zooming to last view\n");
}




