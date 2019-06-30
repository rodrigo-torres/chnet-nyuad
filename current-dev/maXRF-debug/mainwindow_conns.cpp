#include <mainwindow.h>
#include <../Header.h>
#include <../Shm.h>

void MainWindow::handle_connections() {

    connect(tab1_stop, &QPushButton::clicked, this, &MainWindow::set_abort_flag);
    connect(tab3_stop, &QPushButton::clicked, this, &MainWindow::set_abort_flag);


    connect(AUTOFOCUS_ON_pushButton, SIGNAL(clicked()), this, SLOT(enable_keyence_reading()));
    connect(servo_checkbox, SIGNAL(clicked()), this, SLOT(enable_servo()));
// Connections for tab4
    int size = sizeof (scan_params) / sizeof (scan_params[0]);
    for (int i = 0; i < size; i++)
    connect(scan_params[i], QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::scan_parameters);
}




