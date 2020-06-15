/********************************************************************************
**Menu Creator by Gugli 
**
********************************************************************************/

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
//#include <QtWidgets>
//#include <QtGui/QWidget>

#include "include/MAXRF/mainwindow.h"


extern int Resolution_mode;

//QT_BEGIN_NAMESPACE


void MainWindow::createActions()
{

/// In menu FILE

    openAct1 = new QAction(tr("Open"), this);
    openAct1->setStatusTip(tr("Open an existing map"));
    connect(openAct1, SIGNAL(triggered()), this, SLOT(LoadTxt()));

    MergeTxtAct = new QAction(tr("MergeMaps"), this);
    MergeTxtAct->setStatusTip(tr("Merge maps with same area"));
    connect(MergeTxtAct, SIGNAL(triggered()), this, SLOT(MergeTxt()));

    SaveTxtAct = new QAction(tr("SaveMap"), this);
    SaveTxtAct->setStatusTip(tr("Save map"));
    connect(SaveTxtAct, SIGNAL(triggered()), this, SLOT(SaveTxt()));

    WriteHAct = new QAction(tr("NewDataFile"), this);
    WriteHAct->setStatusTip(tr("Create new file with XML header"));
    connect(WriteHAct, SIGNAL(triggered()), this, SLOT(NewFileHeader()));

    exitAct = new QAction(tr("Exit"), this);
    exitAct->setStatusTip(tr("Quit the program"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

/// In menu SHOW MAP

    ChSelAct = new QAction(tr("Select channel interval"), this);
    ChSelAct->setStatusTip(tr("Select channel interval for element map"));
    connect(ChSelAct, SIGNAL(triggered()), this, SLOT(SelectChannels()));

    PxAct = new QAction(tr("Change pixel size"), this);
    PxAct->setStatusTip(tr("Zoom the map by increasing pixel size"));
    connect(PxAct, SIGNAL(triggered()), this, SLOT(Pixels()));
 
    CutBordersAct = new QAction(tr("Cut borders"), this);
    CutBordersAct->setStatusTip(tr("Removes saturated pixels at the edge of the image"));
    connect(CutBordersAct, SIGNAL(triggered()), this, SLOT(CutB()));
    CutBordersAct->setCheckable(true);

    CorrectMapAct = new QAction(tr("Map correction"), this);
    CorrectMapAct->setStatusTip(tr("Set X=1,Y=0 if ScanXY; X=0, Y=1 if ScanYX"));
    connect(CorrectMapAct, SIGNAL(triggered()), this, SLOT(MapCorrection()));


    FileReloadAct_SHM = new QAction(tr("Reload file for map"), this);
    FileReloadAct_SHM->setStatusTip(tr("Click in order to process variations of the Show Map menu parameters"));
    connect(FileReloadAct_SHM, SIGNAL(triggered()), this, SLOT(LoadNewFile_SHM()));

    MapSumReloadAct_SHM = new QAction(tr("Reload composed file for map"), this);
    MapSumReloadAct_SHM->setStatusTip(tr("Click in order to load the map sum of three different elements"));
    connect(MapSumReloadAct_SHM, SIGNAL(triggered()), this, SLOT(LoadElementsMapSum()));

    MapShowAct_SHM = new QAction(tr("Show map"), this);
    MapShowAct_SHM->setStatusTip(tr("Shows the map with the parameters entered in the Show Map menu"));
    connect(MapShowAct_SHM, SIGNAL(triggered()), this, SLOT(displayImage_SHM()));

    MapHideAct = new QAction(tr("Hide map"), this);
    MapHideAct->setStatusTip(tr("Hides the map"));
    connect(MapHideAct, SIGNAL(triggered()), this, SLOT(hideImage()));

    ViewOnLineAct = new QAction(tr("Online map"), this);
    ViewOnLineAct->setStatusTip(tr("View online map"));
    connect(ViewOnLineAct, SIGNAL(triggered()), this, SLOT(GoOnLine()));
    ViewOnLineAct->setCheckable(true);

/// In menu ACQUISITION


    SelTime = new QAction(tr("Select measurement time"), this);
    SelTime->setStatusTip(tr("Select measurement time"));
    connect(SelTime, SIGNAL(triggered()), this, SLOT(SelMeasTime()));

    RunVme = new QAction(tr("Start acquisition"), this);
    RunVme->setStatusTip(tr("Start acquisition"));
    connect(RunVme, SIGNAL(triggered()), this, SLOT(StartVme()));

    RateAct = new QAction(tr("Start RateMeter"), this);
    RateAct->setStatusTip(tr("Start RateMeter"));
    connect(RateAct, SIGNAL(triggered()), this, SLOT(RateMeter()));

    StopVme = new QAction(tr("Stop acquisition"), this);
    StopVme->setStatusTip(tr("Stop acquisition"));
    connect(StopVme, SIGNAL(triggered()), this, SLOT(Stop_Vme()));



/// In menu SPECTRUM

    HistoAct = new QAction(tr("Open X-ray spectrum"), this);
    HistoAct->setStatusTip(tr("Shows X-ray spectrum"));
    connect(HistoAct, SIGNAL(triggered()), this, SLOT(ShowHistogram()));

    FitAct = new QAction(tr("Open Fit Program"), this);
    FitAct->setStatusTip(tr("Fit peaks in the spectrum."));
    connect(FitAct, SIGNAL(triggered()), this, SLOT(FitSpectrum()));



/// In menu 3D

    DueDAct = new QAction(tr("2D XRF spectrum"), this);
    DueDAct->setStatusTip(tr("2D XRF spectrum"));
    connect(DueDAct, SIGNAL(triggered()), this, SLOT(DueD()));
    DueDAct->setCheckable(true);

    TreDAct = new QAction(tr("3D map"), this);
    TreDAct->setStatusTip(tr("Shows sample surface"));
    //TreDAct->setCheckable(true);
    //TreDAct->setEnabled("False");

    connect(TreDAct, SIGNAL(triggered()), this, SLOT(TreD()));

/// In menu PyMca

    PyMcaExpAct = new QAction(tr("Export spectrum for PyMca"), this);
    PyMcaExpAct->setStatusTip(tr("Exports the spectrum"));
    connect(PyMcaExpAct, SIGNAL(triggered()), this, SLOT(exportpymca()));

    RunPyMcaAct = new QAction(tr("Run PyMca"), this);
    RunPyMcaAct->setStatusTip(tr("Run PyMca"));
    //connect(RunPyMcaAct, SIGNAL(triggered()), this, SLOT(RunPyMca()));

    InstallPyMcaAct = new QAction(tr("Install PyMca"), this);
    InstallPyMcaAct->setStatusTip(tr("Install PyMca"));
    //connect(InstallPyMcaAct, SIGNAL(triggered()), this, SLOT(InstallPyMca()));

/// In menu TOOLS
    Open_motor_test = new QAction(tr("PI: motors test"), this);
    Open_motor_test->setStatusTip(tr("Tool for testing Pi motors"));
    connect(Open_motor_test, SIGNAL(triggered()), this, SLOT(PI_motor_test()));

    Open_PI_motor_parameter_table = new QAction(tr("PI: parameters table"), this);
    Open_PI_motor_parameter_table->setStatusTip(tr("PI motors parameters table"));
    connect(Open_PI_motor_parameter_table, SIGNAL(triggered()), this, SLOT(PI_motor_table()));

    actionOpen_settings = new QAction(tr("CAEN: oscilloscope"), this);
    actionOpen_settings->setStatusTip(tr("Oscilloscope"));
    //connect(actionOpen_settings, SIGNAL(triggered()), this, SLOT(caenoscilloscope()));

    actionOpen_Info1_1 = new QAction(tr("General info"), this);
    actionOpen_Info1_1->setStatusTip(tr("Software info"));
    connect(actionOpen_Info1_1, SIGNAL(triggered()), this, SLOT(Info1_1()));

    actionOpen_Info1_2 = new QAction(tr("Shared Memory"), this);
    actionOpen_Info1_2->setStatusTip(tr("SHM configuration"));
    connect(actionOpen_Info1_2, SIGNAL(triggered()), this, SLOT(Info1_2()));


    actionOpen_Info2_1 = new QAction(tr("Modules for CAEN"), this);
    actionOpen_Info2_1->setStatusTip(tr("kernel modules for CAEN drivers"));
    connect(actionOpen_Info2_1, SIGNAL(triggered()), this, SLOT(Info2_1()));


}





/*
/    ViewOnLineAct->setCheckable(true);


/*     MapHideAct->setCheckable(true);
     MapShowAct_SHM->setCheckable(true);
     MapShowAct_SHM->setChecked(false);
     MapHideAct->setChecked(true);
     ViewOnLineAct->setChecked(false);
*/


//    QObject::connect(ui->RunDaq, SIGNAL(triggered()),this, SLOT(CallScan()));

//    Digitiser = new QAction(tr("Open settings"), this);
//    connect(Digitiser, SIGNAL(triggered()),this, SLOT(Digitiser2()));

//    PixelCorrectionAct->setCheckable(true);

//}




void MainWindow::CREATE_MENU()
{

//    QMenuBar *fileMenu= new QMenuBar; 

    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(openAct1);
    fileMenu->addAction(MergeTxtAct);
    fileMenu->addAction(SaveTxtAct);
    fileMenu->addAction(WriteHAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    MapMenu = menuBar()->addMenu(tr("Show Map"));
    MapMenu->addAction(ChSelAct);
    MapMenu->addAction(PxAct);
    MapMenu->addAction(CutBordersAct);
    MapMenu->addAction(CorrectMapAct);
    MapMenu->addSeparator();
    MapMenu->addAction(FileReloadAct_SHM);
    MapMenu->addAction(MapSumReloadAct_SHM);
    MapMenu->addAction(MapShowAct_SHM);
    MapMenu->addAction(MapHideAct);
    MapMenu->addSeparator();
    MapMenu->addAction(ViewOnLineAct);

    AcqMenu = menuBar()->addMenu(tr("Acquisition"));
    AcqMenu->addAction(SelTime);
    AcqMenu->addAction(RunVme);
    AcqMenu->addAction(RateAct);
    AcqMenu->addAction(StopVme);

    SpectrumMenu = menuBar()->addMenu(tr("Spectrum"));
    SpectrumMenu->addAction(HistoAct);
    SpectrumMenu->addAction(FitAct);

    TreDMenu = menuBar()->addMenu(tr("3D Spectrum"));
//    TreDMenu->addAction(DueDAct);
    TreDMenu->addAction(TreDAct);

    PyMcaMenu = menuBar()->addMenu(tr("PyMca"));
    PyMcaMenu->addAction(PyMcaExpAct);
    PyMcaMenu->addAction(RunPyMcaAct);
    PyMcaMenu->addAction(InstallPyMcaAct);

    TOOLMenu = menuBar()->addMenu(tr("Tools"));
    TOOLMenu->addAction(Open_motor_test);
    TOOLMenu->addAction(Open_PI_motor_parameter_table);
    TOOLMenu->addSeparator();
    TOOLMenu->addAction(actionOpen_settings);

    HowToMenu = menuBar()->addMenu(tr("HowTo"));
    SoftWare = HowToMenu->addMenu( "Software " );
    SoftWare->addAction(actionOpen_Info1_1);
    SoftWare->addAction(actionOpen_Info1_2);
    Kernel = HowToMenu->addMenu( "Kernel " );
    Kernel->addAction(actionOpen_Info2_1);


//    HowToMenu->addAction(actionOpen_Info);
//    HowToMenu->addAction(actionOpen_Info2);



}


 





