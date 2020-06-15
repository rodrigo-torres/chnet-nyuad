TEMPLATE = app

QT += widgets

win32 {
   QMAKE_CXXFLAGS += -FIw32pragma.h  
}
CONFIG += qt warn_on thread

INCLUDEPATH += $(ROOTSYS)/include /usr/include/root /usr/lib64/root
win32:LIBS += -L$(ROOTSYS)/lib -llibCore -llibCint -llibRIO -llibNet \
        -llibHist -llibGraf -llibGraf3d -llibGpad -llibTree \
        -llibRint -llibPostscript -llibMatrix -llibPhysics \
        -llibGui -llibRGL 
else:LIBS += -L$(ROOTSYS)/lib -lCore -lThread -lRIO -lMathCore -lNet \
        -lHist -lGraf -lGraf3d -lGpad -lTree -lSpectrum\
        -lRint -lPostscript -lMatrix -lPhysics \
        -lGui -lGed -lFTGL -lGLEW -lRGL 

HEADERS += Qt-Root_XRF_Fit.h MultiFit_Window.h
SOURCES += Qt-Root_XRF_Fit.cpp main.cpp MultiFit_Window.cpp


