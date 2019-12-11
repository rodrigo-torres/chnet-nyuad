#ifndef HISTOMEM_H
#define HISTOMEM_H


#include "Digitizer.h"

struct HistoCell_t;

typedef struct HistoCell_t {
	uint32_t* data;
	uint64_t  counts;
    uint64_t  runtime, corrtime;
	struct HistoCell_t* next;
} HistoCell;


typedef struct HistoMem_t {
	HistoCell* head;
	HistoCell* tail;
	HistoCell* current;

	int count;
	int cindex;
	int maxhistos, histolen;
} HistoMem;


HistoMem* newHistoMem(int maxhistos, int histolen);
void deleteHistoMem(HistoMem* hm);

void setSize(HistoMem*, int newmax, int newlen);

int appendHisto(HistoMem*, uint32_t* histo, uint64_t counts, uint64_t runtime, uint64_t corrtime);
int clearHisto(HistoMem* hm, int index);
void clearAllHistos(HistoMem* hm);
int removeHisto(HistoMem*, int index);
void removeAllHistos(HistoMem*);

int getHisto(HistoMem*, int index, uint32_t** histo, uint64_t* counts, uint64_t* runtime, uint64_t* corrtime);

int isFull(HistoMem*);
int getCount(HistoMem*);

#endif

