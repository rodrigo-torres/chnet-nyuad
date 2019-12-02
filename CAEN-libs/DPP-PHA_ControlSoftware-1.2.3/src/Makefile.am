AM_CFLAGS = -g -Wall -I../inc -I../../CAENDigitizer/include -DLINUX -DDPP_TF_NEW -D_ENABLE_THREADS_ -D_GNU_SOURCE
#AM_LDFLAGS = -lCAENDigitizer -lpthread -lm -lc

bin_PROGRAMS = DppRunner_PHA.bin
DppRunner_PHA_bin_SOURCES = Analyzer.c Analyzer_new.c Analyzer_ci.c Analyzer_ng.c commands.c Configurator.c Controller.c Digitizer.c Digitizer_new.c Digitizer_ci.c Digitizer_ng.c Dumper.c Errors.c fft.c globs.c keyb.c main.c messages.c Parameter.c params.c params_new.c Plot.c Plot_tf2.c Plotter.c HistoMem.c params_ci.c params_ng.c Plot_ci.c Plot_ng.c CAENBuffer.c CAENDumper.c CAENLogger.c CAENMultiplatform.c CAENTypes.c