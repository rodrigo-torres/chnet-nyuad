#include <tty.h>
#include <QMutexLocker>

/* TTY_HANDLER class methods */

/* CONSTRUCTOR */
tty_handle::tty_handle(string s)
{ // Sets the device file path and the device id
    auto pos = s.find(" ");
    id_num = stoi(s.substr(0, pos));
    df_path = s.substr(pos + 1);

    set_id();
    tty_conf();
}

void tty_handle::tty_send(string command) {
    command = command + '\n'; // Very important, the controller expects a '\n' character
    long n = write(dev_fd, command.data(), command.size());
    tcdrain(dev_fd);

    if (n < 0)
    {
        qDebug()<<"[!] Writing to bus failed";
        qDebug()<<strerror(errno);
    }
    return;
}

string tty_handle::tty_read() {
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

void tty_handle::tty_conf()
{
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

    const char* serial = df_path.c_str();
    int serial_no = open(serial, O_RDWR);

    dev_fd = serial_no;
    if (dev_fd < 0)
    {
        err_flag = 1;
        err_string = "[!] Error opening " + df_path;
        return;
    }

    tcsetattr(dev_fd, TCSANOW, &my_termios);
    tcflush(dev_fd, TCIOFLUSH);
}

/* Getter functions */
QString tty_handle::get_message() const
{
   return QString::fromStdString(monitor_message);
}

string tty_handle::get_err_message() const
{
    return err_string;
}

bool tty_handle::get_errflag() const
{
    return err_flag;
}

/* Setter functions */
void tty_handle::set_id()
{
    char ctemp[4] = { 'x', 'y', 'z', 'k' };
    string stemp[4] = { "Stage X", "Stage Y", "Stage Z", "Keyence" };
    id_long = stemp[id_num];
    id_short = ctemp[id_num];
}



/* STAGE class methods */

/* Constructor */
stage::stage(string s) : tty_handle(s)
{
    monitor_message = identify();

    ifstream file("conf/user_conf.xml");
    if (file.is_open())
    {
        std::ostringstream sstream;
        sstream << file.rdbuf();
        auto buffer =  sstream.str();

        string base = "stage" + id_short + "_par";
        string::size_type ref = 0, end = string::npos;
        params.resize(param_no);

        /* Load stage parameters from configuration file*/
        for (ulong i = 0; i < param_no; i++)
        {
            string searchtk(base + to_string(i));
            ref = buffer.find(searchtk, ref) + searchtk.length() + 2;
            end = buffer.find('<', ref);

            params[i] = buffer.substr(ref, end - ref);
        }
    }
    file.close();
}

string stage::identify()
{
    /* Requesting the device to send an identifying string */
    tty_send("*IDN?");

    string ans(tty_read());
    qDebug()<<"[!] Device identified as: "<<ans.c_str();


    /* Constructing serial monitor message */
    string comment = id_long + ": " + df_path;
    return comment;
}

void stage::load_conf() {
    QString temp = QDir::currentPath();
    string path = temp.toStdString() + '/' + params[0];


    ifstream file(path);

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
                                "\n[!] Please check your configuration file: "<<QString::fromStdString(params[0]);
        else qDebug()<<"[!] All stage parameters loaded succesfully";
    }
    else qDebug()<<"[!] Configuration file not found";
}

int stage::set_param(string line) {

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

bool stage::wait()
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    do {
        QThread::msleep(100);
        if (*(shared_memory_cmd+200) ==  1)
        {
            stop();
            *(shared_memory_cmd+200) = 0;

            return 1;
        }
        check_ont();
    } while (!on_target);
    return 0;
}

void stage::check_ont() {
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

void stage::stop() {
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


void stage::get_pos()
{
        tty_send("POS?");

        string ans;
        try {
            ans = tty_read();
        } catch (const runtime_error& e) {
            qDebug()<<e.what();
        }

        pos = atof((ans.substr(2)).c_str());
        monitor_message = id_long + ": " + to_string(pos) + " mm";
}

void stage::move_totarget() {
    string starget = to_string(tar);
    string command = "MOV 1 " + starget;

    tty_send(command);
    on_target = false;
}

void stage::move_step(bool direction) {
    if (on_target) {
        get_pos();
        direction == true ? tar = pos + 1  : tar = pos - 1;
        move_totarget();
    }
    else return;
}





