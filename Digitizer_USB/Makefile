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
ARCH	=	`uname -m`

#OUTDIR  =    	./bin/$(ARCH)/Release/
#OUTNAME =    	ReadoutTest_DPP_PHA_x724.bin
OUTDIR  =    	.
OUT     =    	$(OUTDIR)/$(OUTNAME)
OUTNAME =    	ADCXRF_USB

CC	=	gcc

COPTS	=	-fPIC -DLINUX -O2 -g

DEPLIBS	=	-lCAENDigitizer -lCAENDPPLib -lm -lc -lrt -lpthread

LIBS	=	-L.. -L/usr/lib

INCLUDEDIR =	-I./include -I/usr/include

OBJS	=	src/ADCXRF.o src/keyb.o src/Functions.o 

INCLUDES = /usr/include/CAENDPPLib.h /usr/include/CAENDPPLibTypes.h ./include/*

#########################################################################

all	:	$(OUT)

clean	:
		/bin/rm -f $(OBJS) $(OUT)

debug   :       $(OBJS)
		/bin/rm -f $(OUT)
		if [ ! -d $(OUTDIR) ]; then mkdir -p $(OUTDIR); fi
		$(CC) -g -o $(OUT) $(OBJS) $(DEPLIBS)

$(OUT)	:	$(OBJS)
		/bin/rm -f $(OUT)
		if [ ! -d $(OUTDIR) ]; then mkdir -p $(OUTDIR); fi
		$(CC) $(FLAGS) -o $(OUT) $(OBJS) $(DEPLIBS)

$(OBJS)	:	$(INCLUDES) Makefile

%.o	:	%.c
		$(CC) $(COPTS) $(INCLUDEDIR) -c -o $@ $<



