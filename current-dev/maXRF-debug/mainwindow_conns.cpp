#include <mainwindow.h>
#include <../Header.h>
#include <../Shm.h>

void MainWindow::handle_connections() {

    connect(tab1_stop, &QPushButton::clicked, this, &MainWindow::set_abort_flag);
    connect(tab3_stop, &QPushButton::clicked, this, &MainWindow::set_abort_flag);


    connect(AUTOFOCUS_ON_pushButton, SIGNAL(clicked()), this, SLOT(enable_keyence_reading()));
    connect(servo_checkbox, SIGNAL(clicked()), this, SLOT(enable_servo()));


    /* Tab 1 connections */
    for (const int &i : {0, 1, 2, 3})
        connect(tab1_df_open[i], &QPushButton::released, this, &MainWindow::handle_device);


// Connections for tab4
    int size = sizeof (scan_params) / sizeof (scan_params[0]);
    for (int i = 0; i < size; i++)
    connect(scan_params[i], QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::scan_parameters);
}

void MainWindow::handle_device()
{
    /* Identify the sender */
    QPushButton *sender = static_cast<QPushButton*>(this->sender());

    int i = 0;
    for (auto &tmp : tab1_df_open)
    {
        if (sender == tmp)
            break;
        else
            i++;
    }

    /* Prepare message */
    auto s = to_string(i) + " /dev/tty";
    i < 3 ? s.append("USB"):
            s.append("ACM");
    s.append(to_string(tab1_df_number[i]->value()));

    /* Send signal to device handler class */
    emit df_open(QString::fromStdString(s));
}
