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

int tty::device_t::tty_send(string command) {
    command = command + '\n'; // Very important, the controller expects a '\n' character
    long n = write(dev_fd, command.data(), command.size());
    tcdrain(dev_fd);

    if (n > 0) return 0;
    else {
        qDebug()<<"[!] Writing to bus failed";
        qDebug()<<strerror(errno);
        return 1;
    }
}

string tty::device_t::tty_read() {
    char buff[100] = { '\0' };

    long n = 0;
    int size = 0;
    while ((n = read(dev_fd, &buff[size], 1)) > 0) {
        size += n;
        if (buff[size - 1] == '\n') break;
    }
    if (n < 0) throw std::runtime_error(strerror(errno));
    if (buff[size-1] == '\n') buff[size-1] = '\0';

    string ans(buff);
    return  ans;
}

string tty::device_t::tty_conf() {
    /* Specifying the name of the (serial port) device file */
    char num[2];
    sprintf(num, "%d", df_minor);

    string df_path;
    df_path = "/dev/tty" + df_major + num;

    /* Specifying the device file interface information (through the termios structure) */
    struct termios my_termios;

    cfsetispeed(&my_termios, B9600);
    my_termios.c_oflag = 0000000;
    // Output as is, no processing or mapping
    my_termios.c_cflag = 0004275;
    // Character size 8 bits, baud-rate 9600, enable receiver, no flow control or parity check
    my_termios.c_lflag = 0000000;
    // No input processing, no signals, cbreak mode (non-canonical), no echoing
    my_termios.c_iflag = 0040200;
    // UTF8 encoding, ignore CR on input, no flow control, no parity checks

    my_termios.c_cc[VMIN] = 0; // we read byte-by-byte, non-blocking
    my_termios.c_cc[VTIME]= 1;

    int serial;
    serial = open(df_path.c_str(), O_RDWR);
    if (serial < 0) {
        char err_message[31];
        strncpy(err_message, "[!] Error opening ", 19);
        strncat(err_message, df_path.c_str(), 13);
        throw std::runtime_error(err_message);
    }

    tcsetattr(serial, TCSANOW, &my_termios); // Perhaps better to use TCSAFLUSH
    dev_fd = serial;

    /* Requesting the device to send an identifying string */
    if (df_major.compare("USB") == 0) {
        tcflush(serial, TCIOFLUSH);
        tty_send("*IDN?");

        string ans(tty_read());
        qDebug()<<"[!] Device identified as: "<<ans.c_str();
    }

    /* Constructing serial monitor message */
    string comment;
    comment = "Stage " + id_l + ": " + df_path;

    return comment;
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

        qDebug()<<"[!] Linear stage with FD "<<dev_fd<<"halted with error code: "<<ans.c_str();
    } // Error code should be 10: "Controller stopped on command"
    else return;
}

int tty::device_t::set_param(string line) {

    string part[4];
    size_t pos = 0, npos = 0;

    int i = 0;
    while (i < 4) {
        npos = line.find(" ", pos);
        part[i++] = line.substr(pos, npos - pos);
        pos = npos + 1;
    }

    tty_send(line);

    part[0] = part[0] + '?' + ' ' + part[1] + ' ' + part[2];
    tty_send(part[0]);

    string ans = tty_read();
    pos = ans.find("=") + 1;
    npos = ans.find("\n");

    ans = ans.substr(pos, npos);
    ans = ans.substr(0, part[3].size());
    int ret = part[3].compare(ans);
    if (ret != 0) qDebug()<<"[!] Parameter not set correctly, identifier"<<part[2].c_str();

    return abs(ret);
}

void tty::device_t::load_conf(const char *file_name) {
    QString file_path_q = QDir::currentPath();
    QByteArray temp = file_path_q.toUtf8();

    char file_path[100];
    strncpy(file_path, temp.data(), sizeof (file_path));
    strcat(file_path, "/conf");
    strcat(file_path, file_name);

    ifstream file;
    file.open(file_path);

    int ret = 0;
    string line;
    if (file.is_open()) {
        while (true) {
            getline(file, line);
            if (line.compare(0, 1, "#") == 0) continue;
            if (file.eof()) break;

            ret |= set_param(line);
        }
        if (ret != 0) qDebug()<<"[!] There were errors in setting the stage parameters"
                                "\n[!] Please check your configuration file: "<<file_name;
        else qDebug()<<"[!] All stage parameters loaded succesfully";
    }
    else qDebug()<<"[!] Configuration file not found";
}

void tty::device_t::check_ont() {
    tty_send("ONT?");
    string ans;
    try {
        ans = tty_read();
    } catch (const runtime_error& e) {
        qDebug()<<e.what();
    }

    if (ans.compare(2, 1, "1") == 0) on_target = true;
    else on_target = false;
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

void tty::device_t::get_pos() {
        tty_send("POS?");

        string ans;
        try {
            ans = tty_read();
        } catch (const runtime_error& e) {
            qDebug()<<e.what();
        }

        pos = atof((ans.substr(2)).c_str());
}

void tty::device_t::move_totarget() {
    string starget = to_string(tar);
    string command = "MOV 1 " + starget;

    tty_send(command);
    on_target = false;
}

void tty::device_t::move_step(bool direction) {
    if (on_target) {
        get_pos();
        direction == true ? tar = pos + 1  : tar = pos - 1;
        move_totarget();
    }
    else return;
}

QString tty::device_t::get_message() {
    string ans = "Stage " + id_l + ": " + to_string(pos) + " mm";
    return QString::fromStdString(ans);
}

void tty::timer_event() {
    if (*(shared_memory_cmd+200) == 1) {
        abort();
        if (servo_active) servo_active = false;
        return;
    }

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
//    scan_velocity=number;
//    int V_adc=scan_velocity*1000;
//    printf("V_adc:%d\n", V_adc);
//    *(shared_memory_cmd+67)=V_adc;
//    time_per_pixel=Px/scan_velocity;
//    qDebug()<<"velocità "<<scan_velocity<<"mm/s"<<"Scrittura posizione ogni "<<time_per_pixel<<" ms";
//    char v[10];
//    sprintf(v,"%f",scan_velocity);
//    tty_send(1,"VEL",v,serialX);
//    tty_send(1,"VEL",v,serialY);
}
















