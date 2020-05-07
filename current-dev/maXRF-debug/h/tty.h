#ifndef ALL_TTY_H
#define ALL_TTY_H

#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
/* For synchronization with the DAQ */
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <fstream>
#include <sstream>
#include <math.h>

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QDir>
#include <QSpinBox>
#include <QSignalMapper>

#include <QMutex>
#include <QMutexLocker>



extern int *shared_memory_cmd, *shared_memory;
extern double *shared_memory5;

using namespace std;

class tty_interface : public QObject
{
    Q_OBJECT
public:
    tty_interface(string);
    virtual ~tty_interface() {}

    QString get_message();
    QString get_err_message();
    bool get_errflag() const;

public:
    bool err_flag;
    string err_string;
    string monitor_message;

    int id_num;
    int dev_fd = -1;
    string id_short;
    string id_long;
    string df_path;

//protected:
public:
    void sendtty(string);
    string readtty();

private:
    void set_id();
    void tty_conf();
};

class stage : public tty_interface
{
    Q_OBJECT
    friend class keyence;

public:
    stage(string);
    ~stage() {}

    string identify();

    void init();
    bool wait();
    void load_conf();
    void load_params();

    void stop();
    void check_ont();

    void get_pos();
    void move_totarget();
    void move_step(bool);

    /* Setter functions */
    void set_tar(double val)
    {
        tar = val;
    }

    /* Getter functions */
    bool get_init_status() const
    {
        return  is_inited;
    }

    bool get_on_target() const
    {
        return on_target;
    }


    bool on_target = false;
    bool is_inited = false;
    double pos = 0;
    double tar = 0;

private:
    ulong param_no = 3;
    vector<string> params;
};

class servo
{

};

class scanner
{

};

class keyence : public tty_interface
{
    Q_OBJECT
public:
    keyence(string s) : tty_interface(s)
    {
        monitor_message = id_long + ": " + df_path;
    }
    ~keyence() {}

    void read_offset()
    {
        sendtty("1");
        string diff = readtty();
        offset = stod(diff);
    }
private:
//    bool adjust;

    double offset;
//    const double servo_limit = 15.;
//    const double servo_threshold = 200.;
//    stage *stage_z;
};

class tty_agent : public QObject {
    Q_OBJECT
public:
    tty_agent();


public slots:
    void relay_command(int, QString = "");




public slots:

    void set_target(int, double);
    void set_velocity(double);
    void move_stage(int);
    void move_double_click();
    void enable_servo(bool);
    void start_servo(bool);
    void scan();
    void abort();

signals:
    void toggle_tab1(bool);
    void update_statbar(QString);
    void update_monitor(QString, QString, int);
    void toggle_widgets(int);

private:
    QString style_green  = "QLineEdit {background-color: #2DC937; font-weight: bold; color: white;}";
    QString style_yellow = "QLineEdit {background-color: #E7B416; font-weight: bold; color: white;}";
    sem_t* sem_reply;
    sem_t* sem_probe;


    bool scanning = false, laser_active = false, servo_active = false;
    double x_min = 100., x_max = 110., y_min = 100., y_max = 110., x_step = 1, y_step = 1, s_vel = 1;

    int servo_interval = 250;
    double servo_threshold = 200.0;

    bool next_line = false;

    void servo();
    void scan_loop();
    void timer_event();

    keyence *p_laser = nullptr;
    stage *p_stage [3] = { nullptr };
    tty_interface *p_interface[4] = { nullptr };

    QTimer *update_timer;
    QTimer *servo_timer;
};

#endif // ALL_TTY_H
