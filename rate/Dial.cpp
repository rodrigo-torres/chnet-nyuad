#include "Dial.h"
#include <qlabel.h>
#include <qlayout.h>
#include <qgridlayout.h>
#include <qwt_dial.h>
#include <qwt_dial_needle.h>
#include <qwt_scale_engine.h>
#include <qwt_transform.h>
#include <qwt_round_scale_draw.h>
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

key_t key_cmd, key_rate;
int *shared_memory_cmd, *shared_memory_rate;

int rate=1;
double vmin=10000, vmax=1;
bool EventIsEnabled=false, ini=true;


template <typename T> T* assignSHM(key_t key, size_t size) {
    int shmID = shmget(key, size, IPC_CREAT | 0666);
    if (shmID == -1) {
        printf("[!] Couldn't obtain a shared memory segment ID for key:\t%d\n", key);
        printf("%s\n", strerror(errno));
        return nullptr;
    }
    else return static_cast<T*>(shmat(shmID, nullptr, 0));
}

DialBox::DialBox(QWidget *parent, int type): QWidget( parent ) {
    key_cmd = 6900, key_rate = 7500;
    shared_memory_cmd = assignSHM<int>(key_cmd, SHMSZ_CMD_STATUS);
    shared_memory_rate = assignSHM<int>(key_rate, SHMSZRATE);

    *(shared_memory_cmd+83) = getpid();


    d_dial = createDial( type );
    d_CBox = new QCheckBox("Enable", this);

    d_label = new QLabel( this );
    d_label->setAlignment( Qt::AlignCenter );

    d_label_max = new QLabel( this );
    d_label_max->setAlignment( Qt::AlignRight );

    QVBoxLayout *layout = new QVBoxLayout( this );;
    layout->setSpacing( 0 );
    layout->addWidget( d_CBox );
    layout->addWidget( d_dial, 10 );
    layout->addWidget( d_label );
    layout->addWidget( d_label_max );


    connect(d_dial, SIGNAL(valueChanged(double)), this, SLOT(setNum(double)));
    connect(d_CBox, SIGNAL(stateChanged(int)), this, SLOT(EventEnable()));

    d_dial->setValue(0);
    setNum( d_dial->value() );

    QTimer *d_timer = new QTimer(this);
    connect(d_timer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    d_timer->start(1000);


}

QwtDial *DialBox::createDial( int type ) const {
    QwtDial *dial = new QwtDial();
    dial->setTracking( true );
    dial->setFocusPolicy( Qt::StrongFocus );

    QColor needleColor( Qt::red );

    dial->setOrigin( 135.0 );
    dial->setScaleArc( 0.0, 270.0 );
    dial->setScaleMaxMinor( 9 );
    dial->setScaleEngine( new QwtLogScaleEngine );
    dial->setScale( 1.0, 1.0e5 );

    QwtDialSimpleNeedle *needle = new QwtDialSimpleNeedle(
                QwtDialSimpleNeedle::Arrow, true, needleColor,
                QColor( Qt::gray ).light( 130 ) ); //130
    dial->setNeedle( needle );

    const QColor base( QColor( Qt::darkGray ).dark( 150 ) );

    QPalette palette;
    palette.setColor( QPalette::Base, base );
    palette.setColor( QPalette::Window, base.dark( 150 ) );
    palette.setColor( QPalette::Mid, base.dark( 110 ) );
    palette.setColor( QPalette::Light, base.light( 170 ) );
    palette.setColor( QPalette::Dark, base.dark( 170 ) );
    palette.setColor( QPalette::Text, base.dark( 200 ).light( 800 ) );
    palette.setColor( QPalette::WindowText, base.dark( 200 ) );

    dial->setPalette( palette );
    dial->setLineWidth( 4 );
    dial->setFrameShadow( QwtDial::Sunken );

    return dial;
}

void DialBox::setNum(double v) {
    if (v >  vmax) vmax = v;

    QString text;
    text.setNum(v, 'f', 2);
    d_label->setText(text);

    text.setNum(vmax, 'f', 2);
    text.prepend("max val: ");
    d_label_max->setText( text );

}

void DialBox::TimerEvent() {
    if (EventIsEnabled) {
        rate = *(shared_memory_rate);
        if (rate <= 0) rate = 1;
    }
    else rate = 1;

    d_dial->setValue(rate);
}



void DialBox::EventEnable() {
    if (EventIsEnabled) EventIsEnabled = false;
    else {
        EventIsEnabled=true;
        TimerEvent();
    }
}


DialBox:: ~DialBox() {
    *(shared_memory_cmd+73) = 0;
}





