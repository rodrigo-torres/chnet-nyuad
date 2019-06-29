#include <tty.h>

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
    }
    else {
        QString message = "Keyence: Out Of Range";
        QString style = "QLineEdit {background-color: #E7B416; font-weight: bold; color: white;}";
        emit update_monitor(message, style, 3);
    }

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








