#include <tty.h>


tty::tty() {
    for (int id = 0; id < 4; id++) {
        id < 3 ? devices[id]->df_minor = id : devices[id]->df_minor = 0;
        id < 3 ? devices[id]->df_major = "USB" : devices[id]->df_major = "ACM" ;
    }
    stage_x.id_l = 'X'; stage_y.id_l = 'Y'; stage_z.id_l = 'Z'; keyence.id_l = 'K';

    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &tty::timer_event);

    servo_timer = new QTimer(this);
    connect(servo_timer, &QTimer::timeout, this, &tty::servo);
}


void tty::set_df_minor(int id) {
    QSignalMapper *tmp = static_cast<QSignalMapper*>(this->sender());
    QSpinBox *sender = static_cast<QSpinBox*>(tmp->mapping(id));
    int port = sender->value();

    devices[id]->df_minor = port;
}

void tty::tty_init(int id) {

    device_t *dev = devices[id];
    if (id < 3) dev->df_major = "USB";
    else dev->df_major = "ACM";

    if (isatty(dev->dev_fd)){
        qDebug()<<"[!] Connection to this device file has already been established";
        return;
    }

    string comment;
    try {
        comment = dev->tty_conf();
    } catch (const std::runtime_error& e) {
        QString message = QString::fromUtf8(e.what());
        emit update_statbar(message);
        return;
    }

    /* Display serial monitor message */
    QString qcomment = QString::fromStdString(comment);
    QString style = "QLineEdit {background-color: #E7B416; font-weight: bold; color: white;}";
    emit update_monitor(qcomment, style, id);

    if (dev->df_major.compare("ACM") == 0) {
        emit toggle_widgets(3);
    }
}



void tty::abort() {
    if (*(shared_memory_cmd+70) == 1) *(shared_memory_cmd+70) = 0;

    for (int i = 2; i > -1; i--) devices[i]->stop();
    *(shared_memory_cmd+200) = 0;
}

void tty::device_t::stop() {
    if (isatty(dev_fd)) {
        tty_send("HLT");
        tty_send("ERR?");
        string ans = tty_read();

        int ret = ans.compare("10");
        if (ret != 0)
            qDebug()<<"[!] Linear stage with FD "<<dev_fd<<"halted with error code: "<<ans.c_str();
    } // Error code should be 10: "Controller stopped on command"
    else return;
}


void tty::stage_init(int ind) {
    device_t* stage = devices[ind];
    int fd = stage->dev_fd;
    bool* on_target = &(stage->on_target);

    emit toggle_tab1(false);

    if (!isatty(fd)) {
        qDebug()<<"[!] Communication with this port has not yet been initialized";
        return;
    }

    char message[100];
    const char *model[] = { "/M404_8PD.txt", "/M404_8PD.txt",  "/M404_2PD.txt" };
    strncpy(message, "Loading configuration file: ", sizeof(message));
    strcat(message, model[ind]);

    //_ptr->status->showMessage(message, 60);

    stage->load_conf(model[ind]);

    qDebug()<<"... Changing selected stage velocity";
    string vel_param[] = { "VEL 1 15", "VEL 1 10", "VEL 1 5"};
    stage->tty_send(vel_param[ind]);

    qDebug()<<"... Enabling position servo";
    stage->tty_send("SVO 1 1");

    qDebug()<<"... Moving selected stage to its negative limit";
    stage->tty_send("FNL 1");

    do {
        QThread::msleep(100);
        if (*(shared_memory_cmd+200) ==  1) {
            stage->stop();
            emit toggle_tab1(true);
            return;
        }
        stage->check_ont();
    } while (!(*on_target));

    qDebug()<<"... Defining reference position";
    stage->tty_send("DEF 1");
    qDebug()<<"... Going to reference position"
              "\n... Please wait";

    double ref_pos[] = { 100., 100., 25. };
    char stemp[100];
    sprintf(stemp, "%f", ref_pos[ind]);
    string sref(stemp);
    sref = "MOV 1 " + sref;
    stage->tty_send(sref);

    do {
        QThread::msleep(100);
        if (*(shared_memory_cmd+200) ==  1) {
            stage->stop();
            emit toggle_tab1(true);
            return;
        }
        stage->check_ont();
    } while (!(*on_target));

    emit toggle_widgets(ind);

    stage->is_inited = true;
    update_timer->start(250);

    /* Renabling tab1 and enabling tab3 and tab4 wherever applicable */
    emit toggle_tab1(true);
}

void tty::timer_event() {
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
        if (devices[id]->is_inited) {
            devices[id]->check_ont();
            devices[id]->get_pos();
            QString ret = devices[id]->get_message();

            emit update_monitor(ret, stylesheet3, id);
        }
    }

    /* Check scan */
    if (scanning) scan_loop();
}

void tty::set_target(int id, double target) {
    devices[id]->tar = target;
}

void tty::move_stage(int id) {
    device_t *dev = nullptr;

    if      (id > 6) dev = devices[2];
    else if (id > 4) dev = devices[1];
    else if (id > 2) dev = devices[0];
    else    dev = devices[id];

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

void tty::move_double_click() {
    // To be implemented as a slot, signal implemented in map_mouse

    if (!devices[0]->is_inited || !devices[1]->is_inited) return;
    if (!devices[0]->on_target || !devices[1]->on_target) return;
    if (scanning) return;

    devices[0]->tar = static_cast<double>(*(shared_memory_cmd+64));
    devices[0]->tar = static_cast<double>(*(shared_memory_cmd+65));
    devices[0]->move_totarget();
    devices[1]->move_totarget();

    printf("[!] Moving motors to the position clicked\n");
}

void tty::enable_servo(bool active) {
    if (active) {
        servo_timer->start(servo_interval);
        laser_active = active;
    }
    else {
        servo_timer->stop();
        laser_active = active;
    }
}

void tty::start_servo(bool active) {
    servo_active = active;
}

void tty::set_velocity(double vel) {
    s_vel = vel;
    *(shared_memory5+6) = vel;

    stage_x.tty_send("VEL 1" + to_string(vel));
    stage_y.tty_send("VEL 1" + to_string(vel));
}

void tty::servo() {
    stage_z.get_pos();
    QString ans = stage_z.get_message();

    emit update_monitor(ans, stylesheet3, 2);

    keyence.tty_send("1");
    string diff = keyence.tty_read();
    double value = stod(diff);

    if (value > -15.0 && value < 15.0) {
        QString message = "Keyence: " + QString::fromStdString(diff) + " mm";
        emit update_monitor(message, stylesheet3, 3);

        if (servo_active) {
            stage_z.tar = stage_z.pos + value;
            double velocity = abs(value) / 2;

            if (servo_threshold > abs(value * 1000)) {
                stage_z.tty_send("HLT");
                stage_z.tty_send("ERR?");

                string ans = stage_z.tty_read();
                if (ans.compare("10") != 0) qDebug()<<"[!] Servo motor stopped with error: "
                                                   <<QString::fromStdString(ans);
            }
            else {
                string vel = to_string(velocity);
                vel.insert(0, "VEL 1 ");
                stage_z.tty_send(vel);
                stage_z.move_totarget();
            }
        }
    }
    else {
        stage_z.stop();
        QString message = "Keyence: Out Of Range";
        QString style = "QLineEdit {background-color: #E7B416; font-weight: bold; color: white;}";
        emit update_monitor(message, style, 3);
    }
}

void tty::scan() {
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
        stage_x.check_ont();
        stage_y.check_ont();
    } while (!stage_x.on_target || !stage_y.on_target);

    stage_x.tar = x_min;
    stage_y.tar = y_min;
    stage_x.tty_send("VEL 1 10");
    stage_y.tty_send("VEL 1 10");
    stage_x.move_totarget();
    stage_y.move_totarget();

    /* Waiting for both motors on target */

    do {
        stage_x.check_ont();
        stage_y.check_ont();
    } while (!stage_x.on_target || !stage_y.on_target);

    /* Setting scan velocity and scanning flag to true */
    scanning = true;
    string command = "VEL 1 " + to_string(s_vel);
    stage_x.tty_send(command);

    // Request prompt dialogue from MainWindow
    *(shared_memory_cmd+70) = 1; // Synchronizing with the DAQ

    /* Sending X motor to 1st limit */
    stage_x.tar = x_max;
    stage_x.move_totarget();

    update_timer->start(250);
}

void tty::scan_loop() {
    if (stage_y.on_target && next_line) {
        fabs(stage_x.pos - x_min) < x_min * 0.01 ?
                    stage_x.tar = x_max : stage_x.tar = x_min;
        stage_x.move_totarget();
        next_line = false;
    }
    else if (stage_x.on_target && !next_line) {
        double delta = fabs(stage_y.pos - y_max);
        if (delta > y_max * 0.001) { // Pass to the next line
            stage_y.tar += y_step;
            stage_y.move_totarget();
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
