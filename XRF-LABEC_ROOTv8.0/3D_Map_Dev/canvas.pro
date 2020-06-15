TEMPLATE = app
TARGET = 3D_Map
INCLUDEPATH += .
QT += widgets\
      opengl \
      sql \
      xml \
      gui
CONFIG += c++11
CONFIG += qt warn_on thread
CONFIG += console

win32 {
   QMAKE_CXXFLAGS += -FIw32pragma.h  
}
CONFIG += qt warn_on thread

INCLUDEPATH += $(ROOTSYS)/include
win32:LIBS += -L$(ROOTSYS)/lib -llibCore -llibCint -llibRIO -llibNet \
        -llibHist -llibGraf -llibGraf3d -llibGpad -llibTree \
        -llibRint -llibPostscript -llibMatrix -llibPhysics \
        -llibGui -llibRGL 
else:LIBS += -L$(ROOTSYS)/lib -lCore -lRIO -lNet \
        -lHist -lGraf -lGraf3d -lGpad -lTree \
        -lRint -lPostscript -lMatrix -lPhysics \
        -lGui -lRGL 

HEADERS += canvas.h
SOURCES += canvas.cxx main.cxx


