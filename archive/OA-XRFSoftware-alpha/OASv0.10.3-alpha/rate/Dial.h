#ifndef _DIAL_BOX_H_
#define _DIAL_BOX_H_

#include <qwidget.h>
#include <QCheckBox>
#include <QGridLayout>

class QLabel;
class QwtDial;
class CheckBox;

class DialBox: public QWidget
{
    Q_OBJECT
public:
    DialBox( QWidget *parent, int type );
    ~DialBox();

private Q_SLOTS:
    void setNum( double v );
    void EventEnable( void );
    int setNeedleValue( int );    
    void TimerEvent();
private:
    QwtDial *createDial( int type ) const;

    QwtDial *d_dial;
    QLabel *d_label;
    QLabel *d_label_min;
    QLabel *d_label_max;
    QCheckBox *d_CBox;
    QTimer *d_timer;
};

#endif
