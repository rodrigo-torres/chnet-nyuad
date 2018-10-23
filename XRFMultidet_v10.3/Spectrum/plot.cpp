#include <qwt_math.h>
#include <qwt_scale_engine.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qmath.h>
#include <qfile.h>
#include <qstring.h>
#include <qfiledialog.h>
#include <qwt_counter.h>

#include "plot.h"
#include "mainwindow.h"

#if QT_VERSION < 0x040601
#define qExp(x) ::exp(x)
#define qAtan2(y, x) ::atan2(y, x)
#endif

//QwtPlotCurve::setBrush() + QwtPlotCurve::setBaseline() area sotto una curva....

extern int length; extern int resolution; extern bool autoscaleOn;
extern bool provaRun; double max=0; bool EnergyOn=false;
bool aperto=false;
const int ArraySize = 16385;
const int ArraySize1 = 8192;
const int ArraySize2 = 4096;

double channels[ArraySize]={0};
double counts[ArraySize]={0};


double cal_gradient=1;
double cal_offset=0;
double channel_upperbound=length;
double channel_lowerbound=0;
double maxlog,minlog;
double step=1;
double channel1_cal,channel2_cal,energy1_cal,energy2_cal;

double no_offset=0;
double no_step=1;


QString RunFile ="Position.txt";
QString fileName;
extern int *shm; extern int *shmCommand;
extern int shmid; extern int shmidString; 
extern key_t key; extern key_t keyString;
extern int *shared_memory;
extern int *shm_cmd, *shmCommand_cmd, shmid_cmd, shmidString_cmd; 
extern key_t key_cmd; extern key_t keyString_cmd;
extern int *shared_memory_cmd;


extern bool logchecked;

extern void Unchecklog();



//static void CalibratedSpace( double *array, int size, double xmin, double xmax )



Plot::Plot( QWidget *parent ): QwtPlot( parent ) {

    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setBorderRadius( 0 );

    setCanvas( canvas );
    setCanvasBackground( QColor( "White" ) );


    // Grid
    // Instantiating the class QwtPlotGrid through a pointer
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin( true );
    grid->setMajorPen( Qt::black, 0, Qt::DashDotLine );
    grid->setMinorPen( Qt::gray, 0 , Qt::DashDotLine );
    grid->attach( this );

    // Axes
    setAxisTitle( QwtPlot::xBottom, "Instrumental Channel" );
    setAxisTitle( QwtPlot::yLeft, "Event Counts" );
    setAxisScaleEngine( QwtPlot::xBottom, new QwtLinearScaleEngine );
    setAxisScale(QwtPlot::yLeft, 0, max);
    setAxisScale(QwtPlot::xBottom, 0, length);

    // Plot
    // Instantiating the class QwtPlotCurve through a pointer
    d_curve1 = new QwtPlotCurve( "AdcCounts" );

    d_curve1->setRenderHint( QwtPlotItem::RenderAntialiased );
    d_curve1->setPen( Qt::black );
    d_curve1->setYAxis( QwtPlot::yLeft );
    d_curve1->attach( this );
    d_curve1->setStyle(d_curve1->Steps);

    // Marker
    d_marker1 = new QwtPlotMarker();
    d_marker1->setValue( 0.0, 0.0 );
    d_marker1->setLineStyle( QwtPlotMarker::VLine );
    d_marker1->setLabelAlignment( Qt::AlignRight | Qt::AlignBottom );
    d_marker1->setLinePen( Qt::red, 0, Qt::DashDotLine );
    d_marker1->attach( this );

    setAutoReplot( true );

}


/////////////////////////////////////////////////////////////////////////////////////////////



void Plot::showData( const double *frequency, const double *amplitude, int count ) {
    d_curve1->setSamples( frequency, amplitude, count );
}




void Plot::timerRefreshEvent() {
    if ( provaRun ) showPixelHisto();

} 


void Plot::ch1(double c) {
    channel1_cal=c;
}
void Plot::ch2(double c) {
    channel2_cal=c;
}
void Plot::E1(double e) {
    energy1_cal=e;
}
void Plot::E2(double e) {
    energy2_cal=e;
}

void Plot::set_calibrationparam() {

    double m=(energy2_cal-energy1_cal)/(channel2_cal-channel1_cal);
    double q= energy1_cal - m*channel1_cal;
    Calibration(m,q);

}

void Plot::loadCalibration() {

    qDebug()<<"... Loading calibration parameters";

    QFile file("./Calibration.txt");

    if ( file.exists() ) {

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<"[!] Calibration constants cannot be loaded";
            return;
        }

        else {

            QString line = file.readLine();
            cal_gradient=line.toDouble();

            line = file.readLine();
            cal_offset=line.toDouble();

            file.close();
            Calibration(cal_gradient,cal_offset);
        }
    }

    else return;

}

void Plot::setGradient(double grad) {
    Calibration(grad, cal_offset);
}

void Plot::setOffset(double off) {
    Calibration(cal_gradient,off);
}

void Plot::Calibration( double gradient = 1, double offset = 0 ) {

    cal_gradient=gradient;
    cal_offset=offset;

    qDebug()<<"... Value of gradient is : "<<gradient;
    qDebug()<<"... Value of offset is : "<<offset;

    QFile file("Calibration.txt");
    file.remove();
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    // Writes the calibration parameters to a file for reference
    if ( cal_gradient == 1 && cal_offset == 0 ) {
        setAxisTitle( QwtPlot::xBottom, "Channel" );
        *(shared_memory+24) = 0;
        out<<cal_gradient<<'\n';
        out<<cal_offset<<'\n';
        file.close();
        emit calibrationActive(false);
    }

    else {
        setAxisTitle( QwtPlot::xBottom, "Energy (keV)" );
        *(shared_memory+24) = 1;
        out<<cal_gradient<<'\n';
        out<<cal_offset<<'\n';
        file.close();
        emit calibrationActive(true);
    }

    channel_upperbound = length*cal_gradient + cal_offset;
    channel_lowerbound = cal_offset;

    // The variable declared unders is just equal to the variable cal
    //step = (channel_upperbound - channel_lowerbound) / length;

    const bool doReplot = autoReplot();
    setAutoReplot( true );

    for(int h=0;h<length;h++) {
        if ( EnergyOn ) {
            channels[h]=h*cal_gradient + cal_offset;
        }
        else {
            channels[h]= h;
        }
    }

    if( autoscaleOn ) {
        setAxisScale(QwtPlot::yLeft, 0, max);
    }

    showData(channels, counts, ArraySize );
    setAutoReplot( doReplot );
    replot();
    EnergyOn=true;
}




void Plot::Open() {

    max=0;

    fileName = QFileDialog::getOpenFileName(this, tr("Open File .plot"),tr("Text Files (*.plot)"));
    if (!fileName.isEmpty()) {

        qDebug()<<"... Resolution: "<< length;
        autoReplot();
        setAutoReplot( true );

        // Erases the plot in the visualization window, but not the values stored in shared memory.

        int k=0;
        while (k < 16385) {
            channels[k]=k;
            counts[k]=0;
            k++;
        }

        int iLinedet1=0;
        //int iLinedet2=0;

        QFile fileOpened(fileName);
        if (!fileOpened.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<"[!] File cannot be opened";
            return;
        }

        else {
            while (!fileOpened.atEnd()) {
                QString line = fileOpened.readLine();
                QStringList list1 = line.split('\t');
                *(shared_memory+100+iLinedet1) = list1[0].toInt();
                *(shared_memory+20000+iLinedet1) = list1[1].toInt();
                // Should probably add a line specifying that if this element doesn't exist, the program should skip the following line.
                *(shared_memory+40000+iLinedet1) = list1[2].toInt();
                if (iLinedet1 == 292) {
                    qDebug()<<list1[0].toInt();
                    qDebug()<<list1[1].toInt();
                    qDebug()<<list1[2].toInt();
                    qDebug()<<*(shared_memory+40000+iLinedet1);
                }

                iLinedet1++;
            }

            fileOpened.close();
            aperto=true;
        }
    }
    showPixelHisto();
    qDebug()<<"... showPixelHisto() loaded";
    qDebug()<<fileName;


    emit filenameChanged(fileName);
}





void Plot::Check_SHM() {

    // Used to show the spectrum corresponding to the pixels selected in the map.
    if ( *(shared_memory+99) == 1 ) {
        showPixelHisto();
        *(shared_memory+99)=0;
    }

}




void Plot::showPixelHisto() {

    max=0;

    if (!EnergyOn) {
        setAxisTitle( QwtPlot::xBottom, "Channel" );
    }

    else {
        setAxisTitle( QwtPlot::xBottom, "Energy (keV)" );
    }

    const bool doReplot = autoReplot();

    int k=0;
    for (int h=0;h<16384;h++) {

        counts[k]=0;
        if ( EnergyOn ) {
            channels[h]=h*cal_gradient + cal_offset;
        }
        else {
            channels[h]=h;
        }

        //*(shared_memory_cmd+100) = 0;

        if ( *(shared_memory_cmd+100) == 0 ) { counts[k]=(int)(*(shared_memory+100+h)); }
        else if ( *(shared_memory_cmd+100) == 1 ) { counts[k]=(int)(*(shared_memory+20000+h)); }
        else if ( *(shared_memory_cmd+100) == 2 ) { counts[k]=(int)(*(shared_memory+40000+h)); }

        if ( logchecked == true && counts[k] > 0 ) {
            counts[k]=(log10(counts[k]));
        }

        if(counts[k]>max) {
            max=counts[k];
        }

        k++;
    }

    //qDebug()<<"... Data loaded from shared memory to vector array";
    if ( autoscaleOn ) {
        setAxisScale(QwtPlot::yLeft, 0, max);
    }

    if ( !provaRun ) {
        setAxisScale(QwtPlot::yLeft, 0, max);
    }

    showData(channels, counts, 16385 );
    setAutoReplot(doReplot);
    replot();
}



