########################################################################
#
##	 --- CAEN SpA - Computing Division ---
#
##   CAENDigitizer Software Project
#
##   Created  :  October    2009      (Rel. 1.0)
#
##   Auth: A. Lucchesi
#
#########################################################################
OUTDIR  =    	.
OUTNAME =    	daq_protocol
OUT     =    	$(OUTDIR)/$(OUTNAME)

CC	= gcc
CXX = g++

OPTS	= -fPIC -DLINUX -O2
OBJS	= daq_protocol.o
LIBS	= -lCAENDigitizer -lCAENDPPLib
INCLUDE	= -I/usr/include -I./
INCLUDES = ./daq_protocol.h

#########################################################################

all : $(OUT)

clean :
	/bin/rm -f $(OBJS) $(OUT)

$(OUT) : $(OBJS)
	/bin/rm -f $(OUT)
	$(CXX) $(FLAGS) -g -o $(OUT) $(OBJS) $(LIBS)

$(OBJS) : $(INCLUDES) Makefile

%.o	: %.cpp
	$(CXX) $(OPTS) $(INCLUDE) -pthread -c -g -o $@ $<


#daq_protocol.o : daq_protocol.h daq_protocol.cpp
#	$(CXX) -g -c -pthread $(INCLUDE) $(LIBS) daq_protocol.cpp
