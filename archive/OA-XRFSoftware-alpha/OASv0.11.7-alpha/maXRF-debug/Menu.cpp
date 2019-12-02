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

#include <mainwindow.h>


//QT_BEGIN_NAMESPACE


void MainWindow::createActions()
{

/// In menu FILE

    openAct1 = new QAction(tr("Open"), this);
    openAct1->setStatusTip(tr("Open an existing map"));
    connect(openAct1, SIGNAL(triggered()), this, SLOT(LoadTxt()));

    MergeTxtAct = new QAction(tr("MergeMaps"), this);
    MergeTxtAct->setStatusTip(tr("Merge maps with same area"));
    //connect(MergeTxtAct, SIGNAL(triggered()), this, SLOT(MergeTxt()));

    SaveTxtAct = new QAction(tr("SaveMap"), this);
    SaveTxtAct->setStatusTip(tr("Save map"));
    connect(SaveTxtAct, SIGNAL(triggered()), this, SLOT(SaveTxt()));


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


    SelTime = new QAction(tr("Set acquisition time"), this);
    SelTime->setStatusTip(tr("Set acquisition time"));
    connect(SelTime, SIGNAL(triggered()), this, SLOT(set_PMAcquisitionTime()));

    DigitizerChannel0=new QAction(tr("Select Digitizer Channel 0"), this);
    DigitizerChannel0->setStatusTip(tr("Select Digitizer Channel 0"));
    connect(DigitizerChannel0, SIGNAL(triggered()), this, SLOT(SelDigiCh0()));
    DigitizerChannel0->setCheckable(true);
    DigitizerChannel0->setChecked(true);
    

    DigitizerChannel1=new QAction(tr("Select Digitizer Channel 1"), this);
    DigitizerChannel1->setStatusTip(tr("Select Digitizer Channel 1"));
    connect(DigitizerChannel1, SIGNAL(triggered()), this, SLOT(SelDigiCh1()));
    DigitizerChannel1->setCheckable(true);

    DigitizerChannel0and1=new QAction(tr("Select Digitizer Channel 0 and 1 (Sum)"), this);
    DigitizerChannel0and1->setStatusTip(tr("Select Digitizer Channel 0 and 1 (Sum)"));
    connect(DigitizerChannel0and1, SIGNAL(triggered()), this, SLOT(SelDigiCh0and1()));
    DigitizerChannel0and1->setCheckable(true);

    Changemultidetcalpar=new QAction(tr("Change calibration parameters for multidetector sum"), this);
    Changemultidetcalpar->setStatusTip(tr("Change calibration parameters for multidetector sum"));
    connect(Changemultidetcalpar, SIGNAL(triggered()), this, SLOT(Changeparameters()));
    

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

    HistoAct = new QAction(tr("Histogram display window"), this);
    HistoAct->setStatusTip(tr("Shows X-ray spectrum"));
    connect(HistoAct, SIGNAL(triggered()), this, SLOT(ShowHistogram()));

//    DueDAct = new QAction(tr("2D XRF spectrum"), this);
//    DueDAct->setStatusTip(tr("2D XRF spectrum"));
//    connect(DueDAct, SIGNAL(triggered()), this, SLOT(DueD()));
//    DueDAct->setCheckable(true);

    TreDAct = new QAction(tr("3D surface map"), this);
    TreDAct->setStatusTip(tr("Shows sample surface"));



    connect(TreDAct, SIGNAL(triggered()), this, SLOT(TreD()));

    //In Menu tools
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





void MainWindow::builder_Menu() {

    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(openAct1);
    fileMenu->addAction(MergeTxtAct);
    fileMenu->addAction(SaveTxtAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    MapMenu = menuBar()->addMenu(tr("Show Map"));
    MapMenu->addAction(ChSelAct);
    MapMenu->addAction(PxAct);
    MapMenu->addSeparator();
    MapMenu->addAction(FileReloadAct_SHM);
    MapMenu->addAction(MapSumReloadAct_SHM);
    MapMenu->addAction(MapShowAct_SHM);
    MapMenu->addAction(MapHideAct);
    MapMenu->addSeparator();
    MapMenu->addAction(ViewOnLineAct);

    AcqMenu = menuBar()->addMenu(tr("Acquisition"));
    AcqMenu->addAction(SelTime);
    ActiveChannel=AcqMenu->addMenu("Active Digitizer Channel");
    ActiveChannel->addAction(DigitizerChannel0);
    ActiveChannel->addAction(DigitizerChannel1);
    ActiveChannel->addAction(DigitizerChannel0and1);
    ActiveChannel->addAction(Changemultidetcalpar);
    AcqMenu->addAction(RunVme);
    AcqMenu->addAction(RateAct);
    AcqMenu->addAction(StopVme);

    SpectrumMenu = menuBar()->addMenu(tr("Data display"));
    //SpectrumMenu->addAction(DueDAct);
    SpectrumMenu->addAction(TreDAct);
    SpectrumMenu->addAction(HistoAct);

    //TreDMenu = menuBar()->addMenu(tr("3D Spectrum"));
    //TreDMenu->addAction(DueDAct);
    //TreDMenu->addAction(TreDAct);

    TOOLMenu = menuBar()->addMenu(tr("Tools"));
    TOOLMenu->addAction(actionOpen_settings);

    HowToMenu = menuBar()->addMenu(tr("HowTo"));
    SoftWare = HowToMenu->addMenu( "Software " );
    SoftWare->addAction(actionOpen_Info1_1);
    SoftWare->addAction(actionOpen_Info1_2);
    Kernel = HowToMenu->addMenu( "Kernel " );
    Kernel->addAction(actionOpen_Info2_1);


}


 





