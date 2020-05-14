#include "include/tty.h"

/* TTY_HANDLER class methods */

/* CONSTRUCTOR */
tty_interface::tty_interface(string s)
{ // Sets the device file path and the device id
    auto pos = s.find(" ");
    id_num = stoi(s.substr(0, pos));
    df_path = s.substr(pos + 1);

    set_id();
    tty_conf();
    monitor_message = id_long + ": " + df_path;
}

void tty_interface::sendtty(string command) {
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

string tty_interface::readtty() {
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

void tty_interface::tty_conf()
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

    dev_fd= open(df_path.c_str(), O_RDWR);
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
QString tty_interface::get_message()
{
    QString temp = QString::fromStdString(monitor_message);
    monitor_message.clear();
    return temp;
}


QString tty_interface::get_err_message()
{
    QString temp = QString::fromStdString(err_string);
    err_string.clear();
    err_flag = 0;
    return temp;
}

bool tty_interface::get_errflag() const
{
    return err_flag;
}

/* Setter functions */
void tty_interface::set_id()
{
    char ctemp[4] = { 'x', 'y', 'z', 'k' };
    string stemp[4] = { "Stage X", "Stage Y", "Stage Z", "Servo" };
    id_long = stemp[id_num];
    id_short = ctemp[id_num];
}



/* STAGE class methods */

/* Constructor */
stage::stage(string s) : tty_interface(s)
{
    monitor_message = identify();
    load_params();
}

void stage::load_params()
{
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
    sendtty("*IDN?");

    string ans(readtty());
    qDebug()<<"[!] Device identified as: "<<ans.c_str();


    /* Constructing serial monitor message */
    string comment = id_long + ": " + df_path;
    return comment;
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

void stage::load_conf() {
    if (!isatty(dev_fd))
    {
        throw "[!] This interface has not yet been configured";
    }

    QString temp = QDir::currentPath();
    string path = temp.toStdString() + '/' + params[0];


    ifstream file(path);
    string line;
    if (file.is_open()) {
        do {
            getline(file, line);
        } while (line.compare("[check]") != 0 && !file.eof());
        getline(file, line);
        string check_pref = line;
        getline(file, line);

        int ret = 0;
        while (getline(file, line))
        {
            sendtty(line);
            auto tokens = split(line, ' ');

            line = check_pref + ' ' + tokens.at(2);
            sendtty(line);
            line = readtty();
            auto ans = split(line, '=');

            ret |= ans.at(1).compare(0, tokens.at(3).length(), tokens.at(3));
        }
        if (ret != 0)
            qDebug()<<"[!] There were errors in setting the stage parameters";
        else
            qDebug()<<"[!] All stage parameters loaded succesfully";
    }
    else qDebug()<<"[!] Configuration file not found";
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
            err_flag = 1;
            err_string = "[!] STOP requested";

            return err_flag;
        }
        check_ont();
    } while (!on_target);
    return 0;
}

void stage::check_ont() {
    sendtty("ONT?");
    string ans;
    try {
        ans = readtty();
    } catch (const runtime_error& e) {
        qDebug()<<e.what();
    }

    if (ans.compare(2, 1, "1") == 0) on_target = true;
    else on_target = false;
}

void stage::stop() {
    if (isatty(dev_fd)) {
        sendtty("HLT");
        sendtty("ERR?");
        string ans = readtty();

        int ret = ans.compare("10");
        if (ret != 0)
            qDebug()<<"[!] Linear stage with FD "<<dev_fd<<"halted with error code: "<<ans.c_str();
    } // Error code should be 10: "Controller stopped on command"
    else return;
}


void stage::get_pos()
{
        sendtty("POS?");

        string ans;
        try {
            ans = readtty();
        } catch (const runtime_error& e) {
            qDebug()<<e.what();
        }

        pos = atof((ans.substr(2)).c_str());
        monitor_message = id_long + ": " + ans.substr(2, 6) + " mm";
}

void stage::move_totarget() {
    string command = "MOV 1 " + to_string(tar);

    sendtty(command);
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





