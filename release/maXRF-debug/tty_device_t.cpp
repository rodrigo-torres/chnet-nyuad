#include <tty.h>

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







