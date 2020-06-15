#include "Dial.h"
#include <qlabel.h>
#include <qlayout.h>
#include <qgridlayout.h>
#include <qwt_dial.h>
#include <qwt_dial_needle.h>
#include <qwt_scale_engine.h>
#include <qwt_transform.h>
#include <qwt_round_scale_draw.h>
//#include "dialbox.h"
#include <qcheckbox.h>
#include <qtimer.h>
#include <qfile.h>
#include <qdebug.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/mman.h>
#include <math.h>
#include <sys/ioctl.h>
#include <../variables.h>
#include <../Shm.h>

extern int *shm_cmd, *shmCommand_cmd, shmid_cmd, shmidString_cmd, *shared_memory_cmd; 
extern key_t key_cmd; extern key_t keyString_cmd;

extern int *shm_rate, *shm_rate, shmid_rate, shmidString_rate, *shared_memory_rate; 
extern key_t key_rate; extern key_t keyString_rate;


int Pippo=1; bool EventIsEnabled=false; bool ini=true;
int OldSize=0; int NewSize=0; int rate=1;

double vmin=10000, vmax=1;

DialBox::DialBox( QWidget *parent, int type ):
    QWidget( parent )
{

   key_cmd = 6900;
   shmid_cmd = shmget (key_cmd, SHMSZ_CMD_STATUS, IPC_CREAT | 0666);
   shared_memory_cmd =(int *) shmat(shmid_cmd, NULL, 0);

   qDebug()<< "Data Memory attached at :" << (int *)shared_memory_cmd<< endl << " with shmid: " << shmid_cmd << endl;

   key_rate = 7500;
   shmid_rate = shmget (7500, SHMSZRATE, IPC_CREAT | 0666);
       if (shmid_rate == -1) qDebug()<<"Errore  "<< strerror(errno)<<'\n';
   shared_memory_rate =(int *) shmat(shmid_rate, NULL, 0);
   qDebug() << "Data Memory attached at :" << (int *)shared_memory_rate<< endl << " with shmid4: " << shmid_rate << endl;


     int pid=getpid();
    *(shared_memory_cmd+83)=pid;


    d_dial = createDial( type );
    d_CBox = new QCheckBox("Enable",this);

    d_label_min = new QLabel( this );
    d_label_min->setAlignment( Qt::AlignLeft );

    d_label = new QLabel( this );
    d_label->setAlignment( Qt::AlignCenter );

    d_label_max = new QLabel( this );
    d_label_max->setAlignment( Qt::AlignRight );

    QVBoxLayout *layout = new QVBoxLayout( this );;
    layout->setSpacing( 0 );
    layout->addWidget( d_CBox );
    layout->addWidget( d_dial, 10 );
//    layout->addWidget( d_label );

//    QHBoxLayout *layout2 = new QHBoxLayout( this );;
    layout->addWidget( d_label_min );
    layout->addWidget( d_label );
    layout->addWidget( d_label_max );
//    layout->addWidget(layout2);
/*
    QGridLayout *layout = new QGridLayout;
    layout->setSpacing( 0 );
    layout->addWidget( d_CBox ,0,1);
    layout->addWidget( d_dial, 1,0,1,2 );
    layout->addWidget( d_label_min,2,0 );
    layout->addWidget( d_label,2,1 );
    layout->addWidget( d_label_max,2,2 );
*/




    connect( d_dial, SIGNAL( valueChanged( double ) ), 
        this, SLOT( setNum( double ) ) );

    connect( d_CBox, SIGNAL( stateChanged( int ) ), 
        this, SLOT( EventEnable(  ) ) );

    setNeedleValue(d_dial->value());
    setNum( d_dial->value() );

    QTimer *d_timer = new QTimer(this);
    connect(d_timer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    d_timer->start(1000);


}

QwtDial *DialBox::createDial( int type ) const
{
    QwtDial *dial = new QwtDial();
    dial->setTracking( true );
    dial->setFocusPolicy( Qt::StrongFocus );
//    dial->setObjectName( QString( "Dial %1" ).arg( type + 1 ) );

    QColor needleColor( Qt::red );

            dial->setOrigin( 135.0 );
            dial->setScaleArc( 0.0, 270.0 );
            dial->setScaleMaxMinor( 9 );
            dial->setScaleEngine( new QwtLogScaleEngine );
//            dial->setScale( 1.0e-1, 1.0e4 );
            dial->setScale( 1.0, 5.0e4 );

    QwtDialSimpleNeedle *needle = new QwtDialSimpleNeedle(
        QwtDialSimpleNeedle::Arrow, true, needleColor,
        QColor( Qt::gray ).light( 130 ) ); //130
    dial->setNeedle( needle );

    //const QColor base( QColor( "DimGray" ) );
    const QColor base( QColor( Qt::darkGray ).dark( 150 ) );

    QPalette palette;
    palette.setColor( QPalette::Base, base );
    palette.setColor( QPalette::Window, base.dark( 150 ) );
    palette.setColor( QPalette::Mid, base.dark( 110 ) );
    palette.setColor( QPalette::Light, base.light( 170 ) );
    palette.setColor( QPalette::Dark, base.dark( 170 ) );
    palette.setColor( QPalette::Text, base.dark( 200 ).light( 800 ) );
    palette.setColor( QPalette::WindowText, base.dark( 200 ) );

 //          dial->setValue( 1.0 );

    dial->setPalette( palette );
    dial->setLineWidth( 4 );
    dial->setFrameShadow( QwtDial::Sunken );

    return dial;
}

void DialBox::setNum( double v )
{

if(v> vmax)vmax=v;

if(v>1&&v<vmin) vmin=v;

//qDebug()<<vmin;
    QString text;
    text.setNum( v, 'f', 2 );
    d_label->setText( text );

    text.setNum( vmin, 'f', 2 );
    text.prepend("min val: ");
    d_label_min->setText( text );

    text.setNum( vmax, 'f', 2 );
    text.prepend("max val: ");
    d_label_max->setText( text );

}



int DialBox::setNeedleValue(int)
{d_dial->setValue( rate );}

void DialBox::TimerEvent()
{
if(EventIsEnabled)
{
  rate = *(shared_memory_rate);
  if(rate<=0)rate=1;}
  else rate=1;

setNeedleValue(rate);
}



void DialBox::EventEnable()
{
if(EventIsEnabled) {EventIsEnabled=false; ini=true;}
else {EventIsEnabled=true; TimerEvent();}
}




DialBox:: ~DialBox()
{
*(shared_memory_cmd+73)=0;
}





