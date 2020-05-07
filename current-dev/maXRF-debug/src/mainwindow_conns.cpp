#include "h/mainwindow.h"
#include <../Header.h>
#include <../Shm.h>

void MainWindow::handle_connections() {

    connect(tab1_stop, &QPushButton::clicked, this, &MainWindow::set_abort_flag);


    connect(laser_checkbox, SIGNAL(clicked()), this, SLOT(enable_keyence_reading()));
    connect(servo_checkbox, SIGNAL(clicked()), this, SLOT(enable_servo()));


    /* Tab 1 connections */
    auto ref = widgets::pushbuttons::openttyx;
    for (auto &i : {0, 1, 2, 3, 4, 5, 6})
        connect(buttons[ref + i], &QPushButton::released, this, &MainWindow::handle_pushbuttons);


// Connections for tab4
    int size = sizeof (scan_params) / sizeof (scan_params[0]);
    for (int i = 0; i < size; i++)
    connect(scan_params[i], QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::scan_parameters);
}

void MainWindow::handle_pushbuttons()
{
    /* Identify the sender */
    auto *p_sender = static_cast<QPushButton*>(this->sender());
    auto i = get_index(buttons, p_sender);
    string s;

    switch (i)
    {
    case 0: case 1: case 2:
        /* Prepare message */
        s = to_string(i) + " /dev/ttyUSB" + to_string(tab1_df_number[i]->value());

        /* Send signal to device handler class */
        emit request_tty_action(1, QString::fromStdString(s));
        break;
    case 3:
        /* Prepare message */
        s = to_string(i) + " /dev/ttyACM" + to_string(tab1_df_number[i]->value());

        /* Send signal to device handler class */
        emit request_tty_action(1, QString::fromStdString(s));
        break;
    case 4:
    case 5:
    case 6:
        s = to_string(i - 4);
        emit request_tty_action(2, QString::fromStdString(s));
        break;
    default:
        break;
    }
}
