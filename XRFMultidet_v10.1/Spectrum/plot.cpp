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

extern int lenght; extern int resolution; extern bool autoscaleOn;
extern bool provaRun; double min=0; double max=0; bool EnergyOn=false;
bool aperto=false;
const int ArraySize = 16384;
const int ArraySize1 = 8192;
const int ArraySize2 = 4096;

double canali[ArraySize]={0};
double counts[ArraySize]={0};


double cal=1;
double offset=0;
double maxcal=lenght;
double mincal=0;
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

extern bool calculate_cal;
extern bool new_calibration_ready;
extern bool logchecked;

extern void Unchecklog();



//static void CalibratedSpace( double *array, int size, double xmin, double xmax )



Plot::Plot( QWidget *parent ):
    QwtPlot( parent )
{

//    setTitle( "Labec XRF Viewer" );

    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setBorderRadius( 10 );

    setCanvas( canvas );
    setCanvasBackground( QColor( "MidnightBlue" ) );


    // grid
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin( true );
    grid->setMajorPen( Qt::white, 0, Qt::DotLine );
    grid->setMinorPen( Qt::gray, 0 , Qt::DotLine );
    grid->attach( this );

    // axes
    setAxisTitle( QwtPlot::xBottom, "ADC Channel" );
    setAxisTitle( QwtPlot::yLeft, "Counts" );

//    setAxisMaxMajor( QwtPlot::xBottom, 6 );
//    setAxisMaxMinor( QwtPlot::xBottom, 9 );
    setAxisScaleEngine( QwtPlot::xBottom, new QwtLinearScaleEngine );
    setAxisScale(QwtPlot::yLeft, 0, max);
//    setAxisScaleEngine( QwtPlot::yLeft, new QwtLogScaleEngine );

    // curves
    d_curve1 = new QwtPlotCurve( "AdcCounts" );
    d_curve1->setRenderHint( QwtPlotItem::RenderAntialiased );
    d_curve1->setPen( Qt::cyan );
    d_curve1->setYAxis( QwtPlot::yLeft );
    d_curve1->attach( this );

    // marker
    d_marker1 = new QwtPlotMarker();
    d_marker1->setValue( 0.0, 0.0 );
    d_marker1->setLineStyle( QwtPlotMarker::VLine );
    d_marker1->setLabelAlignment( Qt::AlignRight | Qt::AlignBottom );
    d_marker1->setLinePen( Qt::green, 0, Qt::DashDotLine );
    d_marker1->attach( this );


    //setDamp( 0.0 );
    setAutoReplot( true );
}


/////////////////////////////////////////////////////////////////////////////////////////////



void Plot::showData( const double *frequency, const double *amplitude, int count )
{d_curve1->setSamples( frequency, amplitude, count );}




void Plot::timerRefreshEvent()
{
if(provaRun) showPixelHisto();
if(calculate_cal) Retta_Cal();
} 


void Plot::ch1(double c)
{channel1_cal=c;}
void Plot::ch2(double c)
{channel2_cal=c;}
void Plot::E1(double e)
{energy1_cal=e;}
void Plot::E2(double e)
{energy2_cal=e;}



void Plot::Retta_Cal()
{
double m=(energy2_cal-energy1_cal)/(channel2_cal-channel1_cal);
//qDebug()<<"coeff angolare"<<m<<'\n';

double q= energy1_cal - m*channel1_cal;
//qDebug()<<"termine noto"<<q<<'\n';
calculate_cal=false;

Offset(q);
Calibration(m);
new_calibration_ready=true;
}



void Plot::Calibration( double constant )
{
 cal=constant;
 QFile file("Calibration.txt");
       file.remove();
       file.open(QIODevice::WriteOnly);
       QTextStream out(&file);
 //qDebug()<<"costante di calibrazione"<<cal<<"offset"<<offset<<'\n';
 //qDebug()<<"max e min"<<max<<min<<'\n';

 if(cal==1 && offset==0){
	 setAxisTitle( QwtPlot::xBottom, "Channel" );
	 *(shared_memory+24)=0;
	out<<cal<<'\n';
	out<<offset<<'\n';
        file.close();
	}
 else {
	setAxisTitle( QwtPlot::xBottom, "Energy (keV)" );
	*(shared_memory+24)=1;
	out<<cal<<'\n';
	out<<offset<<'\n';
        file.close();
	
	}

 maxcal=lenght*cal + offset;
 mincal=offset;
 step=(maxcal-mincal)/lenght;
// qDebug()<<"maxcal e mincal"<<maxcal<<mincal<<'\n';
// qDebug()<<"step"<<step<<'\n';
 const bool doReplot = autoReplot();
 setAutoReplot( true );
 for(int h=0;h<lenght;h++)
  {
   if(EnergyOn) canali[h]=h*step + offset; else canali[h]=h*no_step + no_offset;
//   canali[h]=h*step +offset;
  //qDebug()<<"canale nuovo"<<canali[h]<<"conteggi"<<counts[h]<<'\n';
  }
 // qDebug()<<max<<'\n';

if(autoscaleOn)  setAxisScale(QwtPlot::yLeft, min, max);



  showData(canali, counts, ArraySize );
  setAutoReplot( doReplot );
  replot(); EnergyOn=true;  
}



void Plot::Offset( double off)
{
 offset=off;


 if(cal==1 && offset==0) {
	setAxisTitle( QwtPlot::xBottom, "Channel" );
	*(shared_memory+24)=0;
	}
 else {
	setAxisTitle( QwtPlot::xBottom, "Energy (keV)" );
	*(shared_memory+24)=1;
	
	}

 Calibration(cal); 
}






void Plot::Energy_Channel_Choice ( bool ChEn )
{if(ChEn) {EnergyOn=true; qDebug()<<"Energy"<<'\n'; }
else {EnergyOn= false; qDebug()<<"Channel"<<'\n'; }
}




void Plot::Open()
{
//if(logchecked==true)Unchecklog();
 max=0;min=0;
int iLinedet1=0, iLinedet2=0, iLineCounts=0; int datotmp=0; int j2count=0, j4count=0;
bool doReplot;
int dato=0, dato1=0, dato2=0;


fileName = QFileDialog::getOpenFileName(this, tr("Open File .plot"),
						      tr("Text Files (*.plot)"));
     if (!fileName.isEmpty())

        { qDebug()<<"resolution: "<< lenght;      
          doReplot = autoReplot();
          setAutoReplot( true ); //era false


          for(int k=0; k<=16384;k++)
          {canali[k]=k; counts[k]=0;}

//          int dato=0, dato1=0, dato2=0;

          QFile fileOpened(fileName);
          if (!fileOpened.open(QIODevice::ReadOnly | QIODevice::Text))
	     {qDebug()<<"file cannot be opened"; return;}

          else
             {
               while (!fileOpened.atEnd())
                { 
                QString line = fileOpened.readLine();
		QStringList list1 = line.split('	');

		*(shared_memory+100+iLinedet1)=list1[0].toInt();
		if(*(shared_memory_cmd+100)==1){
                	*(shared_memory+20000+iLinedet1)=list1[1].toInt();
		}
		if(*(shared_memory_cmd+100)==2){
			iLinedet2=qRound((double)((iLinedet1*(*(shared_memory_cmd+101)))+*(shared_memory_cmd+102))/(*(shared_memory_cmd+103)));
			*(shared_memory+20000+iLinedet2)=list1[1].toInt();
		}

                iLinedet1++;
                }// end while

          fileOpened.close();
          aperto=true;
         }//end else
       }//end if

//    qDebug()<<"max"<<max;
showPixelHisto();

}





void Plot::Check_SHM()
{
if(*(shared_memory+99)==1) {showPixelHisto(); *(shared_memory+99)=0;}
 else ;
}




void Plot::showPixelHisto()
{
    
    max=0;min=0;

  if(maxcal==lenght && mincal==0)setAxisTitle( QwtPlot::xBottom, "Channel" ); //siamo in canali
  else setAxisTitle( QwtPlot::xBottom, "Energy(keV)" );
    const bool doReplot = autoReplot();


int j2count=0, j4count=0; int datotmp=0, dato1=0, dato2=0; int k=0;


 for(int h=0;h<16384;h++)
  {
   counts[k]=0;	
   if(EnergyOn) canali[h]=h*step + offset; else canali[h]=h*no_step + no_offset;

  switch (resolution)
	  {case 0: //16k
		 {
			if(*(shared_memory_cmd+100)==1){counts[k]=(double)(*(shared_memory+20000+h));}
			else
			if(*(shared_memory_cmd+100)==2){counts[k]=(double)(*(shared_memory+100+h))+(double)(*(shared_memory+20000+h));}
                        else
			if(*(shared_memory_cmd+100)==0)
			counts[k]=(double)(*(shared_memory+100+h));
		  
	          if(logchecked==true && counts[k]>0)
		  {
			counts[k]=(log10(counts[k])); 
		  }
                 if(counts[k]>max) max=counts[k]; 		  
                 k++;
		 break;}
  		  
	  case 1: //8k
		 {
			if(*(shared_memory_cmd+100)==0){dato1=(int)(*(shared_memory+100+h));}
			if(*(shared_memory_cmd+100)==1){dato1=(int)(*(shared_memory+20000+h));}
			if(*(shared_memory_cmd+100)==2){dato1=(int)(*(shared_memory+100+h))+(int)(*(shared_memory+20000+h));}			
			 
			datotmp=datotmp+dato1; 
			j2count++;
		  	if(j2count==2) 
			{
				counts[k]=datotmp;
				if(logchecked==true && counts[k]>0)
		  		{
					counts[k]=(log10(counts[k])); 
		  		} 
				if(counts[k]>=max) max=counts[k]; 
				datotmp=0; 
				j2count=0; 
				k++;
			} 
			break;
		}
	 
	  case 2: //4k
		 {
			if(*(shared_memory_cmd+100)==0){dato2=(int)(*(shared_memory+100+h));}
			if(*(shared_memory_cmd+100)==1){dato2=(int)(*(shared_memory+20000+h));}
			if(*(shared_memory_cmd+100)==2){dato2=(int)(*(shared_memory+100+h))+(int)(*(shared_memory+20000+h));}

			datotmp=datotmp+dato2; 
			j4count++;
		  	if(j4count==4) 
			{
				counts[k]=datotmp;
				if(logchecked==true && counts[k]>0)
		  		{
					counts[k]=(log10(counts[k])); 
		  		}  
				if(counts[k]>=max) max=counts[k]; 
				datotmp=0; 
				j4count=0; 
				k++;
			} 
			break;
		}
			 
	  }//end switch

   }
	

if(autoscaleOn)  setAxisScale(QwtPlot::yLeft, min, max); 

if(!provaRun)setAxisScale(QwtPlot::yLeft, min, max); 
//qDebug()<<"standard Daq";

  showData(canali, counts, lenght );
  setAutoReplot(doReplot);
  replot();
}



