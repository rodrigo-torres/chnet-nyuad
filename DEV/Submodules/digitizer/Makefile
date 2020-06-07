########################################################################
#
##              --- CAEN SpA - Computing Division ---
#
##   CAENDigitizer Software Project
#
##   Created  :  October    2009      (Rel. 1.0)
#
##   Auth: A. Lucchesi
#
#########################################################################
OUTDIR  =    	.
OUTNAME =    	digitizer
OUT     =    	$(OUTDIR)/$(OUTNAME)

CXX =	g++
COPTS	=	-fPIC -DLINUX -O2 -g -std=gnu++14
#lpthread and lrt are used for semaphores
DEPLIBS	=	-lCAENDigitizer -lm -lc -lrt -lpthread
INCLUDEDIR = -I./ -I../

OBJS = src/digitizer.o src/main.o src/buffer.o
HEADERS = src/digitizer.h src/Functions.h src/keyb.h src/namespaces.h src/buffer.h

#########################################################################

all	:	$(OUT)

clean	:
		/bin/rm -f $(OBJS) $(OUT)

debug   :       $(OBJS)
		/bin/rm -f $(OUT)
		if [ ! -d $(OUTDIR) ]; then mkdir -p $(OUTDIR); fi
		$(CXX) -g -o $(OUT) $(OBJS) $(DEPLIBS)

$(OUT)	:	$(OBJS)
		/bin/rm -f $(OUT)
		if [ ! -d $(OUTDIR) ]; then mkdir -p $(OUTDIR); fi
		$(CXX) $(FLAGS) -o $(OUT) $(OBJS) $(DEPLIBS)

$(OBJS)	: Makefile $(HEADERS)

%.o	:	%.cpp
		$(CXX) $(COPTS) $(INCLUDEDIR) -c -o $@ $<



