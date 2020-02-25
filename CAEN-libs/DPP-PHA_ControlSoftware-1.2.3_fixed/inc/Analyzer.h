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

#include "dppswitch.h"

#ifndef ANALYSIS_H
#define ANALYSIS_H

#ifdef DPP_TF_NEW
#include <Analyzer_new.h>
#endif

#ifdef DPP_PSD
#include <Analyzer_ng.h>
#endif

#ifdef DPP_CI
#include <Analyzer_ci.h>
#endif

#endif 