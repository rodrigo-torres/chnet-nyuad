#include "h/mainwindow.h"

void MainWindow::create_menu_actions()
{
    /* Instantiate all QActions */
    for (auto &ptr : w_actions)
    {
        ptr = new QAction(this);
    }

    /* FILE Menu */
    typedef widgets::actions ac;
    w_actions.at(ac::file_openmap)->setText("Open map");
    //w_actions.at(ac::file_savetxt)->setText("Save map");
    w_actions.at(ac::file_export)->setText("Export map");
    w_actions.at(ac::file_exit)->setText("Quit");

    connect(w_actions.at(ac::file_openmap), &QAction::triggered, this, &MainWindow::LoadTxt);
    // [[deprecated]] function saveImageXRFData
    //connect(w_actions.at(ac::file_savetxt), &QAction::triggered, this, &MainWindow::saveImageXRFData);
    connect(w_actions.at(ac::file_export), &QAction::triggered, this, &MainWindow::export_map);
    connect(w_actions.at(ac::file_exit), &QAction::triggered, this, &MainWindow::close);

    /* MAP Menu */
    w_actions.at(ac::map_channelfilter)->setText("Set channel interval");
    w_actions.at(ac::map_pixelsize)->setText("Set pixel size");
    w_actions.at(ac::map_reload)->setText("Reload map");
    w_actions.at(ac::map_reloadsum)->setText("Reload composed map");
    w_actions.at(ac::map_show)->setText("Show map");
    w_actions.at(ac::map_viewlive)->setText("View live map");
    w_actions.at(ac::map_viewlive)->setCheckable(true);

    connect(w_actions.at(ac::map_channelfilter), &QAction::triggered, this, &MainWindow::SelectChannels);
    connect(w_actions.at(ac::map_pixelsize), &QAction::triggered, this, &MainWindow::Pixels);
    connect(w_actions.at(ac::map_reload), &QAction::triggered, this, &MainWindow::LoadNewFile_SHM);
    connect(w_actions.at(ac::map_reloadsum), &QAction::triggered, this, &MainWindow::LoadElementsMapSum);
    // Display image doesn't do much when called from this context.
    connect(w_actions.at(ac::map_show), &QAction::triggered, this, &MainWindow::displayImage_SHM);
    connect(w_actions.at(ac::map_viewlive), &QAction::triggered, this, &MainWindow::GoOnLine);

    /* DAQ Menu */
    w_actions.at(ac::daq_time)->setText("Set acquisition time");
    w_actions.at(ac::daq_channel0)->setText("Acquire on MCA channel 0");
    w_actions.at(ac::daq_channel1)->setText("Acquire on MCA channel 1");
    w_actions.at(ac::daq_channel0and1)->setText("Acquire on all MCA channels");
    w_actions.at(ac::daq_detectorparams)->setText("Change alignment parameters");
    w_actions.at(ac::daq_start)->setText("Start acquisition");
    w_actions.at(ac::daq_rate)->setText("Show count rate");
    w_actions.at(ac::daq_stop)->setText("Stop acquisition");

    connect(w_actions.at(ac::daq_time), &QAction::triggered, this, &MainWindow::set_PMAcquisitionTime);
    connect(w_actions.at(ac::daq_channel0), &QAction::triggered, this, &MainWindow::SelDigiCh0);
    connect(w_actions.at(ac::daq_channel1), &QAction::triggered, this, &MainWindow::SelDigiCh1);
    connect(w_actions.at(ac::daq_channel0and1), &QAction::triggered, this, &MainWindow::SelDigiCh0and1);
    connect(w_actions.at(ac::daq_detectorparams), &QAction::triggered, this, &MainWindow::Changeparameters);
    connect(w_actions.at(ac::daq_start), &QAction::triggered, this, &MainWindow::start_point_daq);
    connect(w_actions.at(ac::daq_rate), &QAction::triggered, this, &MainWindow::RateMeter);
    connect(w_actions.at(ac::daq_stop), &QAction::triggered, this, &MainWindow::stop_point_daq);


    /* HISTO Menu */
    w_actions.at(ac::histo_openwindow)->setText("Show XRF histogram");
    w_actions.at(ac::histo_3dim)->setText("Show 3D surface map");

    connect(w_actions.at(ac::histo_openwindow), &QAction::triggered, this, &MainWindow::ShowHistogram);
    connect(w_actions.at(ac::histo_3dim), &QAction::triggered, this, &MainWindow::TreD);

    /* TOOLS Menu */
    w_actions.at(ac::tools_daqparams)->setText("Set DPP-PHA parameters");
    w_actions.at(ac::tools_xraytable)->setText("Open the X-ray table");
    w_actions.at(ac::tools_camera)->setText("Open Camera");

    connect(w_actions.at(ac::tools_daqparams), &QAction::triggered, this, &MainWindow::openDPPInter);
    connect(w_actions.at(ac::tools_xraytable), &QAction::triggered, this, &MainWindow::XrayTable);
    connect(w_actions.at(ac::tools_camera), &QAction::triggered, this, &MainWindow::VLC_interface);

    /* ABOUT Menu */
    w_actions.at(ac::info_open0)->setText("About this software");
    w_actions.at(ac::info_open1)->setText("SHM configuration");
    w_actions.at(ac::info_open3)->setText("CAEN kernel modules");

    connect(w_actions.at(ac::info_open0), &QAction::triggered, this, &MainWindow::Info1_1);
    connect(w_actions.at(ac::info_open1), &QAction::triggered, this, &MainWindow::Info1_2);
    connect(w_actions.at(ac::info_open3), &QAction::triggered, this, &MainWindow::Info2_1);
}

void MainWindow::create_menus() {

    typedef widgets::actions ac;
    typedef widgets::menus menus;
    w_menus.at(menus::file) = menuBar()->addMenu(tr("File"));
    w_menus.at(menus::file)->addAction(w_actions.at(ac::file_openmap));
    //w_menus.at(menus::file)->addAction(w_actions.at(ac::file_savetxt));
    w_menus.at(menus::file)->addAction(w_actions.at(ac::file_export));
    w_menus.at(menus::file)->addSeparator();
    w_menus.at(menus::file)->addAction(w_actions.at(ac::file_exit));

    w_menus.at(menus::map) = menuBar()->addMenu(tr("Show Map"));
    w_menus.at(menus::map)->addAction(w_actions.at(ac::map_channelfilter));
    w_menus.at(menus::map)->addAction(w_actions.at(ac::map_pixelsize));
    w_menus.at(menus::map)->addSeparator();
    w_menus.at(menus::map)->addAction(w_actions.at(ac::map_reload));
    w_menus.at(menus::map)->addAction(w_actions.at(ac::map_reloadsum));
    w_menus.at(menus::map)->addAction(w_actions.at(ac::map_show));
    w_menus.at(menus::map)->addSeparator();
    w_menus.at(menus::map)->addAction(w_actions.at(ac::map_viewlive));

    w_menus.at(menus::daq) = menuBar()->addMenu(tr("Acquisition"));
    w_menus.at(menus::daq)->addAction(w_actions.at(ac::daq_time));
    w_menus.at(menus::daq_channel)=w_menus.at(menus::daq)->addMenu("Active Digitizer Channel");
    w_menus.at(menus::daq_channel)->addAction(w_actions.at(ac::daq_channel0));
    w_menus.at(menus::daq_channel)->addAction(w_actions.at(ac::daq_channel1));
    w_menus.at(menus::daq_channel)->addAction(w_actions.at(ac::daq_channel0and1));
    w_menus.at(menus::daq_channel)->addAction(w_actions.at(ac::daq_detectorparams));
    w_menus.at(menus::daq)->addAction(w_actions.at(ac::daq_start));
    w_menus.at(menus::daq)->addAction(w_actions.at(ac::daq_rate));
    w_menus.at(menus::daq)->addAction(w_actions.at(ac::daq_stop));

    w_menus.at(menus::histo) = menuBar()->addMenu(tr("Data display"));
    w_menus.at(menus::histo)->addAction(w_actions.at(ac::histo_3dim));
    w_menus.at(menus::histo)->addAction(w_actions.at(ac::histo_openwindow));

    w_menus.at(menus::tools) = menuBar()->addMenu(tr("Tools"));
    w_menus.at(menus::tools)->addAction(w_actions.at(ac::tools_daqparams));
    w_menus.at(menus::tools)->addAction(w_actions.at(ac::tools_xraytable));
    w_menus.at(menus::tools)->addAction(w_actions.at(ac::tools_camera));

    w_menus.at(menus::about) = menuBar()->addMenu(tr("HowTo"));
    w_menus.at(menus::infosoftware) = w_menus.at(menus::about)->addMenu( "Software" );
    w_menus.at(menus::infosoftware)->addAction(w_actions.at(ac::info_open0));
    w_menus.at(menus::infosoftware)->addAction(w_actions.at(ac::info_open1));
    w_menus.at(menus::infokernel) = w_menus.at(menus::about)->addMenu( "Kernel " );
    w_menus.at(menus::infokernel)->addAction(w_actions.at(ac::info_open3));


}
