/******************************************************************************
* 
* CAEN SpA - Front End Division
* Via Vetraia, 11 - 55049 - Viareggio ITALY
* +390594388398 - www.caen.it
*
***************************************************************************//**
* \note TERMS OF USE:
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation. This program is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. The user relies on the 
* software, documentation and results solely at his own risk.
******************************************************************************/

/******************************************************************************
* This file contains the main function, which instantiates all the components
* and manages the operation loop.
* The list of the components which make up Dpprunner follows:
* Plotter - the module that is responsible of the creation and control of the
*			connection to Gnuplot. Its "Do" method is called by the Analyzer  
*			inside its event processing loop, so that each individual event is
*			plotted
* Dumper -  the module that performs a dump of the data to file. Like the
*			Plotter, its Do method is called inside the Analyzer's event loop
* Digitizer - the Digitizer is the module which directly calls library funcs
*			  to interface with the digitizer board. It has methods to open/
*		      close a connection, start/stop the acquisition and read out the
*			  data
* Analyzer - the module responsible for the event analysis. It pulls the events
*			 from the Digitizer board and processes them in a loop inside the
*			 Do method, which also calls the Plotter's and Dumper's Do methods.
* Configurator - it performs the parsing of the configuration file at program
*				 startup and that of the config lines supplied at run time.
*				 It does not feature a Do method, since it is not expected to
*				 function routinely during an acquisition run.
* Controller - it manages user commands and controls the flow of the operation
*			   loop. Its Do method, called first inside the loop, checks the
*			   keyboard buffer for valid user commands and processes them.
*
* The main function sports a nested loop which is used to call the required
* methods to perform an acquisition and analysis cycle.
* The innermost loop is called the "operation loop" and it calls the three main
* modules' Do methods.
* The doControl method processes user commands and can break out of the 
* operation loop by means of the loopctl variable.
* The doAcquisition method performs an acquisition step, that is basically
* reading data from the digitizer into a readout buffer.
* The doAnalysis method perfoms the analysis on the data read in the previous
* step. Events are pulled one by one in an internal loop called "event loop"
* and analyzed: histograms are updated, fft is calculated and the results are 
* plotted and/or dumped to files.
* The outermost loop is the "restart loop" and is walked over at startup and
* whenever the restart command is given. This loop commands the initialization
* and deinitialization of the Digitizer and Analyzer modules, so that in case
* of a restart data structures in both modules are rebuilt anew. The restart
* loop is exited in case of a acquisition or analysis error or when the quit
* command is entered.
******************************************************************************/

#ifndef DPP_TF_CORE


#include <stdlib.h>

#include "globs.h"
#include "Configurator.h"
#include "Digitizer.h"
#include "Analyzer.h"
#include "Controller.h"
#include "Plotter.h"
#include "Dumper.h"
#include <CAENLogger.h>

INIT_C_LOGGER("$(TEMP)/DPPRunnerLog.txt", "main.c");

int main(int argc, char* argv[])  {
	int error;
	int loopctl;
	Plotter*      plo;
	Dumper*		  dpr;
	Digitizer*    dig;
	Analyzer*     ana;
	Configurator* con;
	Controller*	  ctl;

    //THROW_EXCEPTION();

    userMsg("Configuring system\n");
    fflush(stdout);
    plo = newPlotter();
    logMsg(c_logger_Severity_INFO, "Plotter configured");
	dpr = newDumper();
    logMsg(c_logger_Severity_INFO, "Dumper configured");
	dig = newDigitizer();
    logMsg(c_logger_Severity_INFO, "Digitizer configured");
	ana = newAnalyzer(dig, plo, dpr);
    logMsg(c_logger_Severity_INFO, "Analyzer configured");
	con = newConfigurator(argc>1 ? argv[1] : NULL, dig, ana, plo, dpr);
    logMsg(c_logger_Severity_INFO, "Configurator configured");
	ctl = newController(con, dig, ana, plo);
    logMsg(c_logger_Severity_INFO, "Controller configured");

    logMsg(c_logger_Severity_INFO, "System configured");
    logMsg(c_logger_Severity_INFO, "System started");

	if ( (error = parseConfiguration(con)) != 0) quitProgram(error);
	if ( (error = initPlotter(plo)) != 0)		 quitProgram(error);
	if ( (error = initDumper(dpr)) != 0)		 quitProgram(error);
	do { /* loop until quit or error from internal loop */
		
		if ( (error = initDigitizer(dig,0)) != 0)	 break;
		if ( (error = initAnalyzer(ana)) != 0)       break;

		do { /* loop until quit, restart or error */
			if ( (loopctl = doControl(ctl)) != 0)   break; /* loop control is NOCMD, QUIT, or RESTART */
			if ( (error = doAcquisition(dig)) != 0) break;
			if ( (error = doAnalysis(ana)) != 0)    break;
		} while(1);

		deinitAnalyzer(ana);
		deinitDigitizer(dig,0);

	} while(loopctl != CTL_QUIT && !error);

	deinitPlotter(plo);
	deinitDumper(dpr);

	deleteController(ctl);
	deleteConfigurator(con);
	deleteAnalyzer(ana);
	deleteDigitizer(dig);
	deletePlotter(plo);

    logMsg(c_logger_Severity_INFO, "System quitting");
    DEINIT_C_LOGGER();

	quitProgram(error);
	return 0; // control won't really get here. it's just to get rid of warnings
}


#endif