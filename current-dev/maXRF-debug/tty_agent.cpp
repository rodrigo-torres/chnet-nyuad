#include <tty.h>
#include <QMutex>

tty_agent::tty_agent() {
    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &tty_agent::timer_event);

    servo_timer = new QTimer(this);
    connect(servo_timer, &QTimer::timeout, this, &tty_agent::servo);
}

void tty_agent::tty_init(QString qs) {
    // The QString received has format "{ID} dev/tty{DF_major_no}{DF_minor_no}
    string s = qs.toStdString();
    auto it = stoi(s.substr(0, 1));

    if (it > 3 && devices.laser == nullptr)
    {
        devices.laser = new keyence(s);
        emit update_monitor(devices.laser->get_message(), style_yellow, it);
    }
    else if (devices.stage[it] == nullptr)
    {
        devices.stage[it] = new stage(s);
        emit update_monitor(devices.stage[it]->get_message(), style_yellow, it);
        emit toggle_widgets(3);
    }
    else
        qDebug()<<"[!] Connection to this device file has already been established";
    return;
}

void tty_agent::abort() {
    if (*(shared_memory_cmd+70) == 1)
        *(shared_memory_cmd+70) = 0;

    for (int i = 2; i > -1; i--)
        devices.stage[i]->stop();

    QMutex m;
    m.lock();
    *(shared_memory_cmd+200) = 0;
    m.unlock();
}

void stage::init()
{
    if (!isatty(dev_fd))
    {
        err_flag = 1;
        err_string = "[!] Communication with this port has not yet been initialized";
        return;
    }

    load_conf();
    tty_send("VEL 1 " + params[1]);
    tty_send("SVO 1 1");
    tty_send("FNL 1");

    if (wait())
    {
        err_flag = 1;
        err_string = "[!] STOP requested";
        return;
    }

    tty_send("MOV 1 " + params[2]);

    if (wait())
    {
        err_flag = 1;
        err_string = "[!] STOP requested";
        return;
    }

    is_inited = true;
}


void tty_agent::stage_init(int id) {
    emit toggle_tab1(false);
    stage *tmp = devices.stage[id];

    tmp->init();
    if (tmp->get_errflag())
    {
        qDebug()<<QString::fromStdString(tmp->get_err_message());
    }
    else
    {
        update_timer->start(250);
    }

    /* Renabling tab1 and enabling tab3 and tab4 wherever applicable */
    emit toggle_tab1(true);
}

void tty_agent::timer_event() {
    /* Check for abort flag */
    if (*(shared_memory_cmd+200) == 1) {
        abort();
        if (servo_active) servo_active = false;
        if (scanning) scanning = false;
        return;
    }

    /* Check motors on target */
    int limit;
    laser_active ? limit = 2 : limit = 3;
    for (int id = 0; id < limit; id++) {
        if (devices.stage[id] != nullptr && devices.stage[id]->is_inited) {
            devices.stage[id]->check_ont();
            devices.stage[id]->get_pos();

            emit update_monitor(devices.stage[id]->get_message(), style_green, id);
        }
    }

    /* Check scan */
    if (scanning) scan_loop();
}

void tty_agent::set_target(int id, double target) {
    devices.stage[id]->tar = target;
}

void tty_agent::move_stage(int id) {
    stage *dev = nullptr;

    if      (id > 6) dev = devices.stage[2];
    else if (id > 4) dev = devices.stage[1];
    else if (id > 2) dev = devices.stage[0];
    else    dev = devices.stage[id];

    switch (id) {
    case 0: case 1: case 2:
        if (dev->on_target) dev->move_totarget();
        else qDebug()<<"[!] Stage not on target, please wait";
        break;

    case 3: case 5: case 7:
        dev->move_step(true);
        break;
    case 4: case 6: case 8:
        dev->move_step(false);
        break;

    }
}

void tty_agent::move_double_click() {
    // To be implemented as a slot, signal implemented in map_mouse

    if (!devices.stage[0]->is_inited || !devices.stage[1]->is_inited) return;
    if (!devices.stage[0]->on_target || !devices.stage[1]->on_target) return;
    if (scanning) return;

    devices.stage[0]->tar = static_cast<double>(*(shared_memory_cmd+64));
    devices.stage[0]->tar = static_cast<double>(*(shared_memory_cmd+65));
    devices.stage[0]->move_totarget();
    devices.stage[1]->move_totarget();

    printf("[!] Moving motors to the position clicked\n");
}

void tty_agent::enable_servo(bool active) {
    if (active) {
        servo_timer->start(servo_interval);
        laser_active = active;
    }
    else {
        servo_timer->stop();
        laser_active = active;
    }
}

void tty_agent::start_servo(bool active) {
    servo_active = active;
}

void tty_agent::set_velocity(double vel) {
    s_vel = vel;
    *(shared_memory5+6) = vel;

    devices.stage[0]->tty_send("VEL 1" + to_string(vel));
    devices.stage[1]->tty_send("VEL 1" + to_string(vel));
}

void tty_agent::servo() {
    devices.stage[2]->get_pos();
    emit update_monitor(devices.stage[2]->get_message(), style_green, 2);

    devices.laser->tty_send("1");
    string diff = devices.laser->tty_read();
    double value = stod(diff);

    if (value > -15.0 && value < 15.0) {
        QString message = "Keyence: " + QString::fromStdString(diff) + " mm";
        emit update_monitor(message, style_green, 3);

        if (servo_active) {
            devices.stage[2]->tar = devices.stage[2]->pos + value;
            double velocity = abs(value) / 2;

            if (servo_threshold > abs(value * 1000)) {
                devices.stage[2]->tty_send("HLT");
                devices.stage[2]->tty_send("ERR?");

                string ans = devices.stage[2]->tty_read();
                if (ans.compare("10") != 0) qDebug()<<"[!] Servo motor stopped with error: "
                                                   <<QString::fromStdString(ans);
            }
            else {
                string vel = to_string(velocity);
                vel.insert(0, "VEL 1 ");
                devices.stage[2]->tty_send(vel);
                devices.stage[2]->move_totarget();
            }
        }
    }
    else {
        devices.stage[2]->stop();
        QString message = "Keyence: Out Of Range";
        QString style = "QLineEdit {background-color: #E7B416; font-weight: bold; color: white;}";
        emit update_monitor(message, style, 3);
    }
}

void tty_agent::scan() {
    // Disable conflicting widgets
    update_timer->stop();

    /* Getting the scan parameter */
    x_min = *(shared_memory5);
    x_max = *(shared_memory5+1);
    y_min = *(shared_memory5+2);
    y_max = *(shared_memory5+3);
    x_step= *(shared_memory5+4);
    y_step= *(shared_memory5+5);
    s_vel = *(shared_memory5+6);

    /* Calculating the compensation variables */

    double accel_time = s_vel / 200; // Motor acceleration specified as 200 mms-2
    double accel_dist = 0.5 * 100 * (accel_time * accel_time); // units are in um

    x_min -= accel_dist;
    x_max += accel_dist;

    /* Moving to beginning coordinates of the scan */

    do {
        devices.stage[0]->check_ont();
        devices.stage[1]->check_ont();
    } while (!devices.stage[0]->on_target || !devices.stage[1]->on_target);

    devices.stage[0]->tar = x_min;
    devices.stage[1]->tar = y_min;
    devices.stage[0]->tty_send("VEL 1 10");
    devices.stage[1]->tty_send("VEL 1 10");
    devices.stage[0]->move_totarget();
    devices.stage[1]->move_totarget();

    /* Waiting for both motors on target */

    do {
        devices.stage[0]->check_ont();
        devices.stage[1]->check_ont();
    } while (!devices.stage[0]->on_target || !devices.stage[1]->on_target);

    /* Setting scan velocity and scanning flag to true */
    scanning = true;
    string command = "VEL 1 " + to_string(s_vel);
    devices.stage[0]->tty_send(command);

    // Request prompt dialogue from MainWindow
    *(shared_memory_cmd+70) = 1; // Synchronizing with the DAQ

    /* Sending X motor to 1st limit */
    devices.stage[0]->tar = x_max;
    devices.stage[0]->move_totarget();

    update_timer->start(250);
}

void tty_agent::scan_loop() {
    if (devices.stage[1]->on_target && next_line) {
        fabs(devices.stage[0]->pos - x_min) < x_min * 0.01 ?
                    devices.stage[0]->tar = x_max : devices.stage[0]->tar = x_min;
        devices.stage[0]->move_totarget();
        next_line = false;
    }
    else if (devices.stage[0]->on_target && !next_line) {
        double delta = fabs(devices.stage[1]->pos - y_max);
        if (delta > y_max * 0.001) { // Pass to the next line
            devices.stage[1]->tar += y_step;
            devices.stage[1]->move_totarget();
            next_line = true;

            //while (*(shared_memory2+8) != 1) QThread::msleep(100);
            *(shared_memory2+8) = 0;
        }
        else { // The scan is done
            scanning = false;
            next_line = false;
            if (*(shared_memory_cmd+70) == 1) {
                int counting = 0;
                //while (*(shared_memory2+8) != 1 && counting < 5) { QThread::msleep(1000); counting++; }
                *(shared_memory2+8) = 0;
                *(shared_memory_cmd+70) = 0;
                emit save_file();
            }
        }
    }
}
