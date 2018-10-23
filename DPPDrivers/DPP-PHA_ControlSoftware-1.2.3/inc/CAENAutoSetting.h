#ifndef __CAENAUTOSETTING_H
#define __CAENAUTOSETTING_H

#include <CAENDPPLibTypes.h>

int _initDefaultParams(CAENDPP_DgtzParams_t *Params);

int ProgramDigitizer(int handle, CAENDPP_AcqMode_t AcqMode, CAENDPP_DgtzParams_t Params);

int CAS_FindParams(int handle, int ch, CAENDPP_DgtzParams_t *Params);

#endif