#ifndef DUMPER_H
#define DUMPER_H

#include "Plot.h"
#include "Digitizer.h"

#define MAX_DUMP_FILE_FAILS 10

typedef struct Dumper_t {
	const char* suffixes[_NUM_PLOT_TYPES_];
	char openTypes[_NUM_PLOT_TYPES_];
	char* dumpFilePrefix; // prefix is supplied through a config file parameter
	char fileNames[_NUM_PLOT_TYPES_][MAX_CH][100];
	FILE* fdump[_NUM_PLOT_TYPES_][MAX_CH];
	int			busy;
	uint64_t	finishTime;
	int			dps; // dumps per second
	int			inited;
} Dumper;

Dumper* newDumper();
void deleteDumper(Dumper*);
int initDumper(Dumper*);
void deinitDumper(Dumper*);
int doDump(Dumper*, Plot*, int);


#endif