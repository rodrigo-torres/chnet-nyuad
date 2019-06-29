#ifndef ALL_TTY_H
#define ALL_TTY_H

#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <fstream>

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QDir>
#include <QSpinBox>
#include <QSignalMapper>

extern int *shared_memory_cmd, *shared_memory;
extern QString stylesheet3;

using namespace std;
class tty : public QObject {
    Q_OBJECT
    typedef struct {
        int dev_fd = -1;
        int df_minor;

        double pos = 0;
        double tar = 0;

        string df_major;
        string id_l;
        bool on_target = false;
        bool is_inited = false;

        int tty_send(string command);
        string tty_read();
        string tty_conf();
        QString get_message();
        void get_pos();

        void check_ont();
        void move_totarget();
        void move_step(bool);

        int set_param(string line);
        void load_conf(const char *file_name);
        void stop();
    } device_t;



public:
    tty();

public slots:
    void stage_init(int);
    void tty_init(int);

    void set_target(int, double);
    void set_df_minor(int);
    void set_velocity(double);
    void move_stage(int);
    void move_double_click();

    void enable_servo(bool);
    void start_servo(bool);

    void abort();

signals:
    void toggle_tab1(bool);
    void update_statbar(QString);
    void update_monitor(QString, QString, int);
    void toggle_widgets(int);

private:
    bool scanning = false;
    bool laser_active = false;
    bool servo_active = false;

    int servo_interval = 250;
    double servo_threshold = 200.0;
    void timer_event();
    void servo();

    device_t stage_x, stage_y, stage_z, keyence;
    device_t *devices[4] = { &stage_x, &stage_y, &stage_z, &keyence };

    QTimer *update_timer;
    QTimer *servo_timer;
};

#endif // ALL_TTY_H
