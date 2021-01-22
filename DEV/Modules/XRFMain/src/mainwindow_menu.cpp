/** MAXRF_Main: Main point of access to control the MAXRF instrument
 *  Copyright (C) 2020 Rodrigo Torres and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"

void MAXRF_UI::create_menu_actions()
{
    /* Instantiate all QActions */
    for (auto &ptr : w_actions)
    {
        ptr = new QAction(this);
    }

    /* FILE Menu */
    typedef widgets::actions ac;
    w_actions.at(ac::file_exit)->setText("Quit");
    connect(w_actions.at(ac::file_exit), &QAction::triggered, this, &MAXRF_UI::close);


    /* DAQ Menu */
    w_actions.at(ac::daq_channel0)->setText("Acquire on MCA channel 0");
    w_actions.at(ac::daq_channel1)->setText("Acquire on MCA channel 1");
    w_actions.at(ac::daq_channel0and1)->setText("Acquire on all MCA channels");
    w_actions.at(ac::daq_start)->setText("Start acquisition");
    w_actions.at(ac::daq_rate)->setText("Show count rate");

    connect(w_actions.at(ac::daq_channel0), &QAction::triggered, this, &MAXRF_UI::SelDigiCh0);
    connect(w_actions.at(ac::daq_channel1), &QAction::triggered, this, &MAXRF_UI::SelDigiCh1);
    connect(w_actions.at(ac::daq_channel0and1), &QAction::triggered, this, &MAXRF_UI::SelDigiCh0and1);
    connect(w_actions.at(ac::daq_start), &QAction::triggered, this, &MAXRF_UI::OpenDAQWindow);
    connect(w_actions.at(ac::daq_rate), &QAction::triggered, this, &MAXRF_UI::RateMeter);


    /* HISTO Menu */
    w_actions.at(ac::histo_openwindow)->setText("Show XRF histogram");
    w_actions.at(ac::histo_3dim)->setText("Show 3D surface map");

    connect(w_actions.at(ac::histo_openwindow), &QAction::triggered, this, &MAXRF_UI::ShowHistogram);

    /* TOOLS Menu */
    w_actions.at(ac::tools_xraytable)->setText("Open the X-ray table");

    connect(w_actions.at(ac::tools_xraytable), &QAction::triggered, this, &MAXRF_UI::XrayTable);

    /* ABOUT Menu */
    w_actions.at(ac::info_open0)->setText("About this software");
    w_actions.at(ac::info_open1)->setText("SHM configuration");
    w_actions.at(ac::info_open3)->setText("CAEN kernel modules");

    connect(w_actions.at(ac::info_open0), &QAction::triggered, this, &MAXRF_UI::Info1_1);
    connect(w_actions.at(ac::info_open1), &QAction::triggered, this, &MAXRF_UI::Info1_2);
    connect(w_actions.at(ac::info_open3), &QAction::triggered, this, &MAXRF_UI::Info2_1);
}

void MAXRF_UI::CreateMenu() {

    typedef widgets::actions ac;
    typedef widgets::menus menus;
    w_menus.at(menus::file) = menuBar()->addMenu(tr("File"));
    w_menus.at(menus::file)->addAction(w_actions.at(ac::file_exit));

    w_menus.at(menus::daq) = menuBar()->addMenu(tr("Acquisition"));
    w_menus.at(menus::daq)->addAction(w_actions.at(ac::daq_start));
    w_menus.at(menus::daq)->addAction(w_actions.at(ac::daq_rate));
    w_menus.at(menus::daq_channel)=w_menus.at(menus::daq)->addMenu("Active Digitizer Channel");
    w_menus.at(menus::daq_channel)->addAction(w_actions.at(ac::daq_channel0));
    w_menus.at(menus::daq_channel)->addAction(w_actions.at(ac::daq_channel1));
    w_menus.at(menus::daq_channel)->addAction(w_actions.at(ac::daq_channel0and1));

    w_menus.at(menus::histo) = menuBar()->addMenu(tr("Data display"));
    w_menus.at(menus::histo)->addAction(w_actions.at(ac::histo_3dim));
    w_menus.at(menus::histo)->addAction(w_actions.at(ac::histo_openwindow));

    w_menus.at(menus::tools) = menuBar()->addMenu(tr("Tools"));
    w_menus.at(menus::tools)->addAction(w_actions.at(ac::tools_xraytable));

    w_menus.at(menus::about) = menuBar()->addMenu(tr("HowTo"));
    w_menus.at(menus::infosoftware) = w_menus.at(menus::about)->addMenu( "Software" );
    w_menus.at(menus::infosoftware)->addAction(w_actions.at(ac::info_open0));
    w_menus.at(menus::infosoftware)->addAction(w_actions.at(ac::info_open1));
    w_menus.at(menus::infokernel) = w_menus.at(menus::about)->addMenu( "Kernel " );
    w_menus.at(menus::infokernel)->addAction(w_actions.at(ac::info_open3));


}
