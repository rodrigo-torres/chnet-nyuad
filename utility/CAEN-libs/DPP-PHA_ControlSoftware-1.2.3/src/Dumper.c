#include <stdlib.h>
#include <stdio.h>

#include "globs.h"
#include "messages.h"
#include "Dumper.h"



Dumper* newDumper() {
	Dumper* dpr = calloc(1,sizeof(Dumper));

	dpr->suffixes[PLOT_EMPTY]	  = "";
	dpr->suffixes[PLOT_WAVEFORMS] = "wf";
	dpr->suffixes[PLOT_FFT]		  = "";
	dpr->suffixes[PLOT_EHISTO]	  = "eh";
	dpr->suffixes[PLOT_THISTO]	  = "th";
	dpr->suffixes[PLOT_HISTO3D]   = "";
	dpr->suffixes[DUMP_LIST]	  = "ls";

	dpr->openTypes[PLOT_EMPTY]	  =  0 ;
	dpr->openTypes[PLOT_WAVEFORMS]= 'a';
	dpr->openTypes[PLOT_FFT]	  =  0 ;
	dpr->openTypes[PLOT_EHISTO]	  = 'w';
	dpr->openTypes[PLOT_THISTO]	  = 'w';
	dpr->openTypes[PLOT_HISTO3D]  = 0;
	dpr->openTypes[DUMP_LIST]	  = 'a';

	return dpr;
}

void deleteDumper(Dumper* dpr) {
	SAFE_FREE(dpr);
}

int initDumper(Dumper* dpr) {
	static const char* dumpFileExtension = "dat";
	static const char* dumpNameFormat = "%s_%s_%d.%s"; /* i.e. prefix_suffix.extension */
	int i,ch;

	for (i=0; i<_NUM_PLOT_TYPES_; i++) {
		for (ch=0; ch<MAX_CH; ch++) {
			sprintf(dpr->fileNames[i][ch], dumpNameFormat, dpr->dumpFilePrefix, dpr->suffixes[i], ch, dumpFileExtension);
			dpr->fdump[i][ch] = NULL;
		}
	}

	dpr->inited = 1;

	return 0;
}

void deinitDumper(Dumper* dpr) {
	int i,ch;
	for (i=0; i<_NUM_PLOT_TYPES_; i++) {
		for (ch=0; ch<MAX_CH; ch++) {
			if (dpr->openTypes[i] == 'a') {
				if (dpr->fdump[i][ch] != NULL) fclose(dpr->fdump[i][ch]);
				dpr->fdump[i][ch] = NULL;
			}
		}
	}
	dpr->inited = 0;
}

int doDump(Dumper* dpr, Plot* pl, int ch) {
	static int fdumpFailCount = 0;
	int tmask = 0, i, s;
	FILE* fdump = NULL;

	/*if (dpr->openTypes[pl->type] == 'a') {
		if (dpr->fdump[pl->type][ch]==NULL) dpr->fdump[pl->type][ch] = fopen(dpr->fileNames[pl->type][ch], "a");
		fdump = dpr->fdump[pl->type][ch];
	} else if (dpr->openTypes[pl->type] == 'w') {
		fdump = fopen(dpr->fileNames[pl->type][ch],"w");
	} else return -1; // if not given an open type, nothing will be dumped*/

    if (dpr->openTypes[pl->type] == 'a') {
        fdump = fopen(dpr->fileNames[pl->type][ch],"a");
    } else if (dpr->openTypes[pl->type] == 'w') {
        fdump = fopen(dpr->fileNames[pl->type][ch],"w");
    } else return -1; // if not given an open type, nothing will be dumped*/


	if (fdump == NULL) {
		fdumpFailCount++;
		if (fdumpFailCount>MAX_DUMP_FILE_FAILS) {
            char msg[]="Error opening dump file. Check write permissions.";
            guiMsg(MSG_ERROR,msg);
            guiMsg(MSG_DMP_OPEN);
            return -1;
        }
		else return 0;
	} else {
		fdumpFailCount = 0;
	}

    for(i=0; i<pl->numTraces; i++)
	    tmask |= getTrace(pl,i)->enabled << i;
	
	for (s=0; tmask > 0; s++) {
		if (pl->type != DUMP_LIST) fprintf(fdump, "%d\t", s);
		for(i=0; i<pl->numTraces; i++) {
			Trace* tr = getTrace(pl,i);
            if (tmask & (1<<i)) {
				float gain = tr->gain;
				int offset = tr->offset;
				switch (tr->type) {
                    case PLOT_DATA_UINT8 : 
                        {uint8_t *data = (uint8_t *)tr->data;
                        fprintf(fdump, "%u\t", (uint32_t)(data[s] * gain + offset));}
                        break;
                    case PLOT_DATA_UINT16 :
                        {uint16_t *data = (uint16_t *)tr->data;
                        fprintf(fdump, "%u\t", (uint32_t)(data[s] * gain + offset));}
                        break;
                    case PLOT_DATA_UINT32 :
                        {uint32_t *data = (uint32_t *)tr->data;
                        fprintf(fdump, "%u\t", (uint32_t)(data[s] * gain + offset));}
                        break;
					case PLOT_DATA_UINT64:
                        {uint64_t *data = (uint64_t *)tr->data;
                        fprintf(fdump, "%llu\t", (uint64_t)(data[s] * gain + offset));}
                        break;
                    case PLOT_DATA_INT8 :
                        {int8_t *data = (int8_t *)tr->data;
                        fprintf(fdump, "%d\t", (int32_t)(data[s] * gain + offset));}
                        break;
                    case PLOT_DATA_INT16 :
                        {int16_t *data = (int16_t *)tr->data;
                        fprintf(fdump, "%d\t", (int32_t)(data[s] * gain + offset));}
                        break;
                    case PLOT_DATA_INT32 :
                        {int32_t *data = (int32_t *)tr->data;
                        fprintf(fdump, "%d\t", (int32_t)(data[s] * gain + offset));}
                        break;
                    case PLOT_DATA_INT64 :
                        {int64_t *data = (int64_t *)tr->data;
                        fprintf(fdump, "%lld\t", (int64_t)(data[s] * gain + offset));}
                        break;
					case PLOT_DATA_FLOAT :
						{float *data = (float *)tr->data;
						fprintf(fdump, "%f\t", data[s] * gain + offset);}
						break;
                    case PLOT_DATA_DOUBLE:
                        {double *data = (double *)tr->data;
                        fprintf(fdump, "%f\t", data[s] * gain + offset);}
                        break;
                    default :
						userMsg("Unsupported dump data type\n");
						fclose(fdump);
                        return -1;
				}
			}
			if (s == (tr->length-1))
				tmask &= ~(1<<i);
		}
		fprintf(fdump, "\n");
	}
	//if (dpr->openTypes[pl->type] == 'w') fclose(fdump);
    fclose(fdump);

	//if (pl->type == PLOT_EHISTO || pl->type == PLOT_THISTO) 
	//	dpr->finishTime = getTime() + 1000;
	//else 
	//	dpr->finishTime = 0;

	return 0;
}


int checkDumperBusy(Dumper* dpr) {
	if (getTime() > dpr->finishTime) dpr->busy = 0;
	return dpr->busy;
}