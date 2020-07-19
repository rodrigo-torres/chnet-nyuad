#ifndef DIAL_WIDGET_H_
#define DIAL_WIDGET_H_

#include <iostream>

#include <QApplication>
#include <QLabel>
#include <QCheckBox>
#include <QGridLayout>
#include <QTimer>
#include <QWidget>

#include <qwt_dial.h>
#include <qwt_dial_needle.h>
#include <qwt_scale_engine.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#include "../Shm.h"

namespace maxrf::rate {

class DialBox: public QWidget {
    Q_OBJECT
public:
    DialBox(QWidget *parent = nullptr);
    ~DialBox();

private Q_SLOTS:
    void setNum( double v );
    void EventEnable(int state);
    void TimerEvent();
private:
    void createDial();

    QLabel    * rate_label_;
    QLabel    * max_rate_label_;
    QTimer    * update_event_timer_;
    QwtDial   * rate_dial_;

    int * shared_memory_cmd {nullptr};
    double max_rate_ {1};
};

}  //namespace maxrf::rate

#endif
