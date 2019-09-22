#ifndef ALL_TTY_H
#define ALL_TTY_H

#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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

extern int *shared_memory_cmd, *shared_memory, *shared_memory2;
extern double *shared_memory5;

using namespace std;

class tty_handle : public QObject
{
    Q_OBJECT
public:
    tty_handle(string);
    ~tty_handle() {}

    void tty_send(string);
    string tty_read();
    QString get_message() const;
    string get_err_message() const;
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

private:
    void set_id();
    void tty_conf();
};

class stage : public tty_handle
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
    int set_param(string);

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

class keyence : public tty_handle
{
    Q_OBJECT
public:
    keyence(string s) : tty_handle(s)
    {
        monitor_message = id_short + ": " + df_path;
    }
    ~keyence() {}

    void read_offset()
    {
        tty_send("1");
        string diff = tty_read();
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
    friend class tty_handle;

public:
    tty_agent();


public slots:
    void tty_init(QString);
    void stage_init(int);
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
    void save_file();

private:
    QString style_green  = "QLineEdit {background-color: #2DC937; font-weight: bold; color: white;}";
    QString style_yellow = "QLineEdit {background-color: #E7B416; font-weight: bold; color: white;}";



    bool scanning = false;
    bool laser_active = false;
    bool servo_active = false;

    int servo_interval = 250;
    double servo_threshold = 200.0;

    bool next_line = false;
    double x_min = 100., x_max = 110., y_min = 100., y_max = 110., x_step = 1, y_step = 1, s_vel = 1;

    void servo();
    void scan_loop();
    void timer_event();

    struct {
        class stage *stage[3] = { nullptr };
        keyence *laser = nullptr;

        bool assigned[4] = { 0 };
    } devices;

    QTimer *update_timer;
    QTimer *servo_timer;
};

#endif // ALL_TTY_H
