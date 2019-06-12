#ifndef ALL_TTY_H
#define ALL_TTY_H

#include <QObject>
#include <QThread>

class MainWindow;

/* Adding this ifndef statement to avoid conflicts when including all_tty.h in .cpp files*/
//#ifndef MAINWINDOW_H
//#include <mainwindow.h>
//#endif


class all_tty : public QObject {
    Q_OBJECT
    struct stage_t {
        int df_minor_number;
        int stage_init_status;
        int stage_fd;
        int stage_id;
        bool stage_on_target;
    };
public:
    all_tty(MainWindow *ptr = nullptr);
    void stage_check_on_target(stage_t* stage);

public slots:
    void stage_load_param(int fd, const char *file_name);
    int mod_tty_send(int fd, std::string line);
    void stage_init(int ind);

    void abort();

signals:
    void stage_timer_start(int interval);
private:
    MainWindow *_ptr;
    stage_t stage[3];

};

class controller : public QObject {
    Q_OBJECT
    QThread tty_thread;
public:
    controller(MainWindow *ptr) : mainwindow_ptr(ptr) {
        all_tty *all_tty_ptr = new all_tty(mainwindow_ptr);
        all_tty_ptr->moveToThread(&tty_thread);

        connect(all_tty_ptr, &all_tty::stage_timer_start, this, &controller::start_timer);
        connect(&tty_thread, &QThread::finished, all_tty_ptr, &QObject::deleteLater);
        connect(this, &controller::stage_init, all_tty_ptr, &all_tty::stage_init);
        tty_thread.start();
    }
    ~controller() {
        tty_thread.quit();
        tty_thread.wait();
    }
public slots:
    void start_timer(int interval);
signals:
    void stage_init(int ind);
private:
    MainWindow * mainwindow_ptr;
};

#endif // ALL_TTY_H
