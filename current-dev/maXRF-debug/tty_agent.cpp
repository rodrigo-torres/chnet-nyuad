#include <tty.h>

tty_agent::tty_agent() {
    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &tty_agent::timer_event);

    servo_timer = new QTimer(this);
    connect(servo_timer, &QTimer::timeout, this, &tty_agent::servo);
}

void tty_agent::relay_command(int com, QString qstr)
{
    string str = qstr.toStdString();
    switch (com)
    {
    case 1:
    {
        /* New tty interface */
        auto it = stoi(str.substr(0, 1));
        if (p_interface[it] == nullptr)
        {
            it != 3 ?   p_interface[it] = new stage(str) :
                        p_interface[it] = new keyence(str);

            QString message = p_interface[it]->get_message();
            emit update_monitor(message, style_yellow, it);
        }
        else
        {
            qDebug()<<"[!] Connection already established";
        }
        break;
    }
    case 2:
    {
        /* Stage init */
        auto id = stoi(str);
        if (p_stage[id] == nullptr)
        {
            auto ptr = dynamic_cast<stage*>(p_interface[id]);
            emit toggle_tab1(false);
            try
            {
                ptr->init();
                p_stage[id] = ptr;
                update_timer->start(250);
                if (id == 2)
                    emit toggle_widgets(3);
                else {
                    emit toggle_widgets(id);
                }
            }
            catch (const char *msg)
            {
                qDebug()<<msg;
            }
            catch (...) {}
            emit toggle_tab1(true);
        }
        else
            qDebug()<<"[!] Linear stage can't be initiliazed";
        break;
    }
    default:
        break;
    }

}

void tty_agent::abort() {
    if (*(shared_memory_cmd+300))
        *(shared_memory_cmd+300) = 0;

    for (auto &i : p_stage)
        i->stop();

    QMutex mutex;
    QMutexLocker locker(&mutex);
    *(shared_memory_cmd+200) = 0;
}

void stage::init()
{
    load_conf();
    sendtty("VEL 1 " + params[1]);
    sendtty("SVO 1 1");
    sendtty("FNL 1");
    if (wait())
        throw "[!] Initialization aborted";

    sendtty("MOV 1 " + params[2]);
    if (wait())
        throw "[!] Initialization aborted";

    is_inited = true;
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
        if (p_stage[id] != nullptr && p_stage[id]->is_inited) {
            p_stage[id]->check_ont();
            p_stage[id]->get_pos();

            emit update_monitor(p_stage[id]->get_message(), style_green, id);
        }
    }

    /* Check scan */
    if (scanning) scan_loop();
}

void tty_agent::set_target(int id, double target) {
    p_stage[id]->tar = target;
}

void tty_agent::move_stage(int id) {
    stage *dev = nullptr;

    if      (id > 6) dev = p_stage[2];
    else if (id > 4) dev = p_stage[1];
    else if (id > 2) dev = p_stage[0];
    else    dev = p_stage[id];

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

    if (!p_stage[0]->is_inited || !p_stage[1]->is_inited) return;
    if (!p_stage[0]->on_target || !p_stage[1]->on_target) return;
    if (scanning) return;

    p_stage[0]->tar = static_cast<double>(*(shared_memory_cmd+64));
    p_stage[0]->tar = static_cast<double>(*(shared_memory_cmd+65));
    p_stage[0]->move_totarget();
    p_stage[1]->move_totarget();

    printf("[!] Moving motors to the position clicked\n");
}

void tty_agent::enable_servo(bool val) {
    if (p_interface[3] != nullptr)
    {
        p_laser = dynamic_cast<keyence*>(p_interface[3]);
    }

    laser_active = val;
    if (val)
        servo_timer->start(servo_interval);
    else
        servo_timer->stop();
}

void tty_agent::start_servo(bool active) {
    servo_active = active;
}

void tty_agent::set_velocity(double vel) {
    s_vel = vel;
    *(shared_memory5+6) = vel;

    p_stage[0]->sendtty("VEL 1" + to_string(vel));
    p_stage[1]->sendtty("VEL 1" + to_string(vel));
}

void tty_agent::servo() {
    p_stage[2]->get_pos();
    emit update_monitor(p_stage[2]->get_message(), style_green, 2);

    p_laser->sendtty("1");
    string diff = p_laser->readtty();
    double value = stod(diff);

    if (value > -15.0 && value < 15.0) {
        QString message = "Keyence: " + QString::fromStdString(diff) + " mm";
        emit update_monitor(message, style_green, 3);

        if (servo_active) {
            p_stage[2]->tar = p_stage[2]->pos + value;
            double velocity = abs(value) / 2;

            if (servo_threshold > abs(value * 1000)) {
                p_stage[2]->sendtty("HLT");
                p_stage[2]->sendtty("ERR?");

                string ans = p_stage[2]->readtty();
                if (ans.compare("10") != 0) qDebug()<<"[!] Servo motor stopped with error: "
                                                   <<QString::fromStdString(ans);
            }
            else {
                string vel = to_string(velocity);
                vel.insert(0, "VEL 1 ");
                p_stage[2]->sendtty(vel);
                p_stage[2]->move_totarget();
            }
        }
    }
    else {
        p_stage[2]->stop();
        QString message = "Keyence: Out Of Range";
        QString style = "QLineEdit {background-color: #E7B416; font-weight: bold; color: white;}";
        emit update_monitor(message, style, 3);
    }
}

void tty_agent::scan() {
    // Disable conflicting widgets
    update_timer->stop();
    sem_probe = sem_open("/daq_probe", O_CREAT, 0644, 0);
    sem_reply = sem_open("/daq_reply", O_CREAT, 0644, 0);

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
        p_stage[0]->check_ont();
        p_stage[1]->check_ont();
    } while (!p_stage[0]->on_target || !p_stage[1]->on_target);

    p_stage[0]->tar = x_min;
    p_stage[1]->tar = y_min;
    p_stage[0]->sendtty("VEL 1 10");
    p_stage[1]->sendtty("VEL 1 10");
    p_stage[0]->move_totarget();
    p_stage[1]->move_totarget();

    /* Waiting for both motors on target */

    do {
        p_stage[0]->check_ont();
        p_stage[1]->check_ont();
    } while (!p_stage[0]->on_target || !p_stage[1]->on_target);

    /* Setting scan velocity and scanning flag to true */
    scanning = true;
    string command = "VEL 1 " + to_string(s_vel);
    p_stage[0]->sendtty(command);

    // Request prompt dialogue from MainWindow
    shared_memory_cmd[300] = 2;
    system("./app-modules/digitizer &");
    /* Synchronize */
    /* Sending X motor to 1st limit */
    sem_wait(sem_reply);
    p_stage[0]->tar = x_max;
    p_stage[0]->move_totarget();
    sem_post(sem_probe);
    update_timer->start(250);
}

void tty_agent::scan_loop() {
    if (next_line)
    {
        sem_wait(sem_reply);
        fabs(p_stage[0]->pos - x_min) < x_min * 0.01 ?
                    p_stage[0]->tar = x_max : p_stage[0]->tar = x_min;
        p_stage[0]->move_totarget();
        sem_post(sem_probe);
        next_line = false;
    }
    else if (p_stage[0]->on_target && !next_line)
    {
        double delta = fabs(p_stage[1]->pos - y_max);
        if (delta > y_max * 0.001)
        { // Pass to the next line
            p_stage[1]->tar += y_step;
            p_stage[1]->move_totarget();            
            do {
                p_stage[1]->check_ont();
            } while (!p_stage[1]->on_target);
            next_line = true;
        }
        else
        { // The scan is done
            scanning = false;
            sem_wait(sem_reply);
            sem_close(sem_probe);
            sem_close(sem_reply);
            sem_unlink("/daq_probe");
            sem_unlink("/daq_reply");
            emit save_file();
        }
    }
}
