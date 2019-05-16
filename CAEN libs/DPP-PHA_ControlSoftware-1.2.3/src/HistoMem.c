#include <stdlib.h>
#include <string.h>
#include "globs.h"
#include "HistoMem.h"

#ifdef LINUX
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

static void hm_rewind(HistoMem* hm) {
	hm->cindex = 0;
	hm->current = hm->head;
}

static void chainFree(HistoMem* hm, int howmany) {
	int i;
	for (i=0; hm->head != NULL && i < howmany; i++) {
		HistoCell* hc = hm->head->next;
		free(hm->head->data);
		free(hm->head);
		hm->head = hc;
	}
	hm->count -= min(hm->count, howmany);
	hm_rewind(hm);
}

static HistoCell* chainWalk(HistoMem* hm, int index) {
	if (index < hm->cindex) hm_rewind(hm);

	for (; hm->cindex<index && hm->current->next != NULL; hm->cindex++) {
		hm->current = hm->current->next;
	}
	return hm->current;
}

HistoMem* newHistoMem(int maxhistos, int histolen) {
	HistoMem* hm = calloc(1, sizeof(HistoMem));
	hm->maxhistos = maxhistos;
	hm->histolen = histolen;
	return hm;
}

void deleteHistoMem(HistoMem* hm) {
	removeAllHistos(hm);
	hm->count = 0;
	hm->maxhistos = 0;
	hm->histolen = 0;
}

void setSize(HistoMem* hm, int newmax, int newlen) {
	if (hm->histolen != newlen) {
		removeAllHistos(hm);
		hm->histolen = newlen;
		hm->maxhistos = newmax;
		return;
	}
	if (hm->count > newmax) chainFree(hm, hm->count - newmax);
	hm->maxhistos = newmax;
}

int appendHisto(HistoMem* hm, uint32_t* histo, uint64_t counts, uint64_t runtime, uint64_t corrtime) {
	HistoCell* hc;
	if (hm->count >= hm->maxhistos) return -1;
	hc = calloc(1, sizeof(HistoCell));
	hc->data = calloc(hm->histolen, sizeof(uint32_t));	
	memcpy(hc->data, histo, hm->histolen);
	hc->counts = counts;
	hc->runtime = runtime;
	hc->corrtime = corrtime;
	hc->next = NULL;
	if (hm->count == 0) {
		hm->head = hm->tail = hm->current = hc;
		hm->count++;
		return 0;
	}

	hm->tail->next = hc;
	hm->tail = hc;
	hm->count++;

	return 0;
}

int removeHisto(HistoMem* hm, int index) {
	HistoCell* hc, *hc2;
	if (hm->count == 0 || index >= hm->count) return -1;
	if (hm->count == 1) { /* head-tail removal (single cell) */
		free(hm->head->data);
		free(hm->head);
		hm->head = NULL;
		hm->tail = NULL;
		hm->current = NULL;
		hm->count--;
		return 0;
	}
	if (index == 0) { /* head removal */
		free(hm->head->data);
		hc = hm->head->next;
		if (hm->current == hm->head) hm->current = hc;
		free(hm->head);
		hm->head = hc;
		hm->count--;
		return 0;
	} 

	hc = chainWalk(hm, index-1);
	hc2 = hc->next;
	free(hc2->data);
	hc->next = hc2->next;
	if (hm->tail == hc2) hm->tail = hc;
	if (hm->current == hc2) hm->current = hc;
	free(hc2);
	hm->count--;
	return 0;
}

int getHisto(HistoMem* hm, int index, uint32_t** histo, uint64_t* counts, uint64_t* runtime, uint64_t* corrtime) {
	HistoCell* hc = chainWalk(hm, index);
	if (hc == NULL) return -1;
	*histo = hc->data;
	*counts = hc->counts;
	*runtime = hc->runtime;
	*corrtime = hc->corrtime;
	return 0;
}

int clearHisto(HistoMem* hm, int index) {
	HistoCell* hc = chainWalk(hm, index);
	if (hc == NULL) return -1;
	memset(hc->data, 0, hm->histolen*sizeof(uint32_t));
	hc->counts = 0;
	hc->runtime = 0;
	hc->corrtime = 0;
	return 0;
}

void clearAllHistos(HistoMem* hm) {
	int i;
	for (i=0; i<hm->count; i++)
		clearHisto(hm, i);
}

void removeAllHistos(HistoMem* hm) {
	chainFree(hm, hm->count);
}

int isFull(HistoMem* hm) {
	return hm->maxhistos <= hm->count;
}

int getCount(HistoMem* hm) {
	return hm->count;
}
