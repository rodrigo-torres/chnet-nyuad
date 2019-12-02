#include <mainwindow.h>
#include <../Header.h>
#include <../Shm.h>

void MainWindow::GUI_CREATOR() {
    centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    QFont font;
    font.setFamily(QString::fromUtf8("Cantarell"));
    font.setPointSize(12);
    font.setBold(true);

    CAMERA_label = new QLabel(centralWidget);
    VLC_Button = new QPushButton(centralWidget);
    LASER_label = new QLabel(centralWidget);
    DAQ_label = new QLabel(centralWidget);
    Digitizer_Button = new QPushButton(centralWidget);
    TTY_label= new QLabel(centralWidget);
    Export_Button = new QPushButton(centralWidget);
    XRAY_TABLE_Button = new QPushButton(centralWidget);
    tabWidget = new QTabWidget(centralWidget);
    tab = new QWidget(tabWidget);
    spinBox_assignZ = new QSpinBox(tab);
    AssignZ_pushButton = new QPushButton(tab);

    INIT_Z_pushButton = new QPushButton(tab);
    STOP_Z_INIT_pushButton = new QPushButton(tab);
    tab_2 = new QWidget(tabWidget);
    tab_3 = new QWidget(tabWidget);
    label_17_below_tab = new QLabel(centralWidget);
    spinBox_assignX = new QSpinBox(tab);
    pushButton_assignX_port = new QPushButton(tab);
    spinBox_assignY = new QSpinBox(tab);
    pushButton_assignY_port = new QPushButton(tab);
    pushButton_tab_2_2X = new QPushButton(tab);
    pushButton_tab_2_2Y = new QPushButton(tab);
    pushButton_tab_2_2ABORT = new QPushButton(tab);
    tab2_3 = new QWidget(tabWidget);
    MOVE_label = new QLabel(tab2_3);
    MOVEUP_pushButton = new QPushButton(tab2_3);
    MOVELEFT_pushButton = new QPushButton(tab2_3);
    MOVERIGHT_pushButton = new QPushButton(tab2_3);
    MOVEDOWN_pushButton = new QPushButton(tab2_3);
    STOPMOVE_pushButton = new QPushButton(tab2_3);
    MOVE_TO_X_pushButton = new QPushButton(tab2_3);
    MOVE_TO_X_doubleSpinBox = new QDoubleSpinBox(tab2_3);
    MOVE_TO_Y_pushButton = new QPushButton(tab2_3);
    MOVE_TO_Y_doubleSpinBox = new QDoubleSpinBox(tab2_3);
    tab2_4 = new QWidget(tabWidget);

    XY_SPEED_doubleSpinBox = new QDoubleSpinBox(centralWidget);


    /******************************/
    /********** GEOMETRY **********/
    /******************************/


    QGroupBox *utilityBox = new QGroupBox(centralWidget);
    utilityBox->setGeometry(QRect(1120, 15, 160, 220));
    utilityBox->setTitle("Utility programs");
    QVBoxLayout *utilityLayout = new QVBoxLayout(utilityBox);
    utilityLayout->setSpacing(10);

    VLC_Button->setText("Camera");
    XRAY_TABLE_Button->setText("X-ray Table");
    Digitizer_Button->setText("Digitizer");
    Export_Button->setText("Export Map");

    utilityLayout->addWidget(Digitizer_Button, 0);
    utilityLayout->addWidget(XRAY_TABLE_Button, 1);
    utilityLayout->addWidget(Export_Button, 2);
    utilityLayout->addWidget(VLC_Button, 3);

    tabWidget->setEnabled(true);
    tabWidget->setGeometry(QRect(640, 15, 465, 375));

    /* ################################################################# */
    /* ####################   PORTS in tab Widget   #################### */
    /* ################################################################# */

    tab->setObjectName(QString::fromUtf8("tab"));
    tab->setEnabled(true);

    QGridLayout *portTabLayout = new QGridLayout(tab);
    portTabLayout->setHorizontalSpacing(20);
    portTabLayout->setVerticalSpacing(10);

    spinBox_assignX->setValue(2);
    spinBox_assignX->setMaximum(3);
    spinBox_assignX->setPrefix("Stage X port: ");

    pushButton_assignX_port->setText("Confirm");
    pushButton_tab_2_2X->setText("Initiate X");

    spinBox_assignY->setValue(0);
    spinBox_assignY->setMaximum(3);
    spinBox_assignY->setPrefix("Stage Y port: ");

    pushButton_assignY_port->setText("Confirm");
    pushButton_tab_2_2Y->setText("Initiate Y");

    spinBox_assignZ->setValue(1);
    spinBox_assignZ->setMaximum(3);
    spinBox_assignZ->setPrefix("Stage Z port: ");

    AssignZ_pushButton->setText("Confirm");
    INIT_Z_pushButton->setText("Initiate Z");

    STOP_Z_INIT_pushButton->setText("ABORT Z");
    pushButton_tab_2_2ABORT->setText("ABORT X OR Y");

    portTabLayout->addWidget(spinBox_assignX, 0, 0);
    portTabLayout->addWidget(spinBox_assignY, 1, 0);
    portTabLayout->addWidget(spinBox_assignZ, 2, 0);
    portTabLayout->addWidget(pushButton_assignX_port, 0, 1);
    portTabLayout->addWidget(pushButton_assignY_port, 1, 1);
    portTabLayout->addWidget(AssignZ_pushButton, 2, 1);
    portTabLayout->addWidget(pushButton_tab_2_2X, 0, 2);
    portTabLayout->addWidget(pushButton_tab_2_2Y, 1, 2);
    portTabLayout->addWidget(INIT_Z_pushButton, 2, 2);
    portTabLayout->addWidget(STOP_Z_INIT_pushButton, 3, 1);
    portTabLayout->addWidget(pushButton_tab_2_2ABORT, 3, 2);
    portTabLayout->setRowStretch(4, 3);
    portTabLayout->setColumnStretch(3, 3);

    /* ################################################################# */
    /* ####################   ACM in tab Widget   #################### */
    /* ################################################################# */

    MOVE_Z_To_pushButton = new QPushButton(tab_3);
    MOVE_Z_To_doubleSpinBox = new QDoubleSpinBox(tab_3);
    MOVE_Z_FORWARD_pushButton = new QPushButton(tab_3);
    MOVE_Z_MOTOR_label_2 = new QLabel(tab_3);
    MOVE_Z_BACKWARD_pushButton = new QPushButton(tab_3);
    STOP_Z_MOVE_pushButton = new QPushButton(tab_3);

    MOVE_Z_To_doubleSpinBox->setDecimals(3);
    MOVE_Z_To_doubleSpinBox->setMaximum(50);
    MOVE_Z_To_doubleSpinBox->setSingleStep(0.250);
    MOVE_Z_To_doubleSpinBox->setValue(25);
    MOVE_Z_To_doubleSpinBox->setPrefix("Move Z motor to: ");
    MOVE_Z_To_doubleSpinBox->setSuffix(" mm");
    MOVE_Z_To_pushButton->setText("Confirm");

    MOVE_Z_FORWARD_pushButton->setText("Forward");
    MOVE_Z_MOTOR_label_2->setText("<b>Move</b>");
    MOVE_Z_BACKWARD_pushButton->setText("Backward");

    STOP_Z_MOVE_pushButton->setText("STOP");


    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    tab_3->setEnabled(true);

    QGridLayout  *acmTabLayout = new QGridLayout(tab_3);
    //QLabel *label_23_tab_4 = new QLabel(tab_3);
    QLabel *labelTab2Motor = new QLabel(tab_3);
    QLabel *labelTab2Servo = new QLabel(tab_3);

    labelTab2Motor->setText("<b>Z-axis motor</b>");
    labelTab2Servo->setText("<b>Laser servomechanism</b>");


    spinBox_assignACM = new QSpinBox(tab_3);
    AssignACM_pushButton = new QPushButton(tab_3);
    AUTOFOCUS_ON_pushButton = new QCheckBox(tab_3);


    lineEdit_2_tab_4 = new QLineEdit(centralWidget);
    ENABLE_TRACKING_checkBox = new QCheckBox(tab_3);

    spinBox_assignACM->setMaximum(3);
    spinBox_assignACM->setPrefix("Assign ACM to port: ");

    AssignACM_pushButton->setText("Confirm");


    AUTOFOCUS_ON_pushButton->setText("Enable distance reading");
    AUTOFOCUS_ON_pushButton->setEnabled(false);

    ENABLE_TRACKING_checkBox->setText("Enable tracking servo");
    ENABLE_TRACKING_checkBox->setEnabled(false);

    acmTabLayout->setHorizontalSpacing(20);
    acmTabLayout->setVerticalSpacing(10);
    acmTabLayout->addWidget(labelTab2Motor, 0, 0);
    acmTabLayout->addWidget(MOVE_Z_To_doubleSpinBox, 1, 0);
    acmTabLayout->addWidget(MOVE_Z_To_pushButton, 1, 1);
    acmTabLayout->addWidget(MOVE_Z_MOTOR_label_2, 2, 0);
    acmTabLayout->addWidget(MOVE_Z_BACKWARD_pushButton, 2, 1);
    acmTabLayout->addWidget(MOVE_Z_FORWARD_pushButton, 2, 2);
    acmTabLayout->addWidget(STOP_Z_MOVE_pushButton, 3, 0);

    acmTabLayout->addWidget(labelTab2Servo, 4, 0);
    acmTabLayout->addWidget(spinBox_assignACM, 5, 0);
    acmTabLayout->addWidget(AssignACM_pushButton, 5, 1);
    acmTabLayout->addWidget(AUTOFOCUS_ON_pushButton, 6, 0);
    acmTabLayout->addWidget(ENABLE_TRACKING_checkBox, 7, 0);
    acmTabLayout->setColumnStretch(3, 3);



    ///////////////////////////////// WIDGET 1 TAB4

    tabWidget->addTab(tab, QString());
    tabWidget->addTab(tab_3, QString());
    tabWidget->addTab(tab2_3, QString());
    tabWidget->addTab(tab2_4, QString());
    tabWidget->setTabText(tabWidget->indexOf(tab), "Ports && Init");
    tabWidget->setTabText(tabWidget->indexOf(tab_3), "Z-axis motor && servo");
    tabWidget->setTabText(tabWidget->indexOf(tab2_3), "Move XY");
    tabWidget->setTabText(tabWidget->indexOf(tab2_4), "Scan");
    tabWidget->setCurrentIndex(0);



    ///////////////////////////////// WIDGET 2 TAB3 (MOVE XY)

    tab2_3->setObjectName(QString::fromUtf8("tab2_3"));
    tab2_3->setEnabled(false);

    MOVE_label->setObjectName(QString::fromUtf8("label_22"));
    MOVE_label->setGeometry(QRect(98, 75, 75, 41));
    MOVE_label->setFont(font);
    MOVE_label->setText(QApplication::translate("MyWidget", "Move",nullptr));

    MOVEUP_pushButton->setObjectName(QString::fromUtf8("MOVEUP_pushButton"));
    MOVEUP_pushButton->setGeometry(QRect(90, 30, 75, 38));
    MOVEUP_pushButton->setText(QApplication::translate("MyWidget", "UP",nullptr));

    MOVELEFT_pushButton->setObjectName(QString::fromUtf8("MOVELEFT_pushButton"));
    MOVELEFT_pushButton->setGeometry(QRect(15, 75, 75, 38));
    MOVELEFT_pushButton->setText(QApplication::translate("MyWidget", "LEFT",nullptr));

    MOVERIGHT_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
    MOVERIGHT_pushButton->setGeometry(QRect(165, 75, 75, 38));
    MOVERIGHT_pushButton->setText(QApplication::translate("MyWidget", "RIGHT",nullptr));

    MOVEDOWN_pushButton->setObjectName(QString::fromUtf8("MOVEDOWN_pushButton"));
    MOVEDOWN_pushButton->setGeometry(QRect(90, 120, 75, 38));
    MOVEDOWN_pushButton->setText(QApplication::translate("MyWidget", "DOWN",nullptr));

    STOPMOVE_pushButton->setObjectName(QString::fromUtf8("STOPMOVE_pushButton"));
    STOPMOVE_pushButton->setGeometry(QRect(300, 53, 120, 83));
    STOPMOVE_pushButton->setText(QApplication::translate("MyWidget", "STOP",nullptr));

    MOVE_TO_X_pushButton->setObjectName(QString::fromUtf8("MOVE_TO_X_pushButton"));
    MOVE_TO_X_pushButton->setGeometry(QRect(30, 203, 135, 41));
    MOVE_TO_X_pushButton->setText(QApplication::translate("MyWidget", "Move X to",nullptr));

    MOVE_TO_X_doubleSpinBox->setObjectName(QString::fromUtf8("MOVE_TO_X_doubleSpinBox"));
    MOVE_TO_X_doubleSpinBox->setGeometry(QRect(195, 203, 225, 41));
    MOVE_TO_X_doubleSpinBox->setDecimals(3);
    MOVE_TO_X_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 100; 14/07/2016
    MOVE_TO_X_doubleSpinBox->setSingleStep(0.001);
    MOVE_TO_X_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 50; 14/07/2016

    MOVE_TO_Y_pushButton->setObjectName(QString::fromUtf8("pushButton_12"));
    MOVE_TO_Y_pushButton->setGeometry(QRect(30, 263, 135, 38));
    MOVE_TO_Y_pushButton->setText(QApplication::translate("MyWidget", "Move to Y", 0));

    MOVE_TO_Y_doubleSpinBox->setObjectName(QString::fromUtf8("MOVE_TO_Y_doubleSpinBox"));
    MOVE_TO_Y_doubleSpinBox->setGeometry(QRect(195, 263, 225, 41));
    MOVE_TO_Y_doubleSpinBox->setDecimals(3);
    MOVE_TO_Y_doubleSpinBox->setMaximum(200);//cambiato da anna e chiara, perchè non corrispondeva alle dimensioni dei motori (200mm), prima era 50; 14/07/2016
    MOVE_TO_Y_doubleSpinBox->setSingleStep(0.001);
    MOVE_TO_Y_doubleSpinBox->setValue(100);//cambiato da anna e chiara, LO ABBIAMO RIMESSO NEL PUNTO DI INIZIALIZIONE DEI MOTORI(100), prima era 25; 14/07/2016

    /* ################################################################# */
    /* ####################   SCAN in tab Widget   #################### */
    /* ################################################################# */

    tab2_4->setObjectName(QString::fromUtf8("tab2_4"));
    tab2_4->setEnabled(false);


    QGridLayout *scanTabLayout = new QGridLayout(tab2_4);

    QDoubleSpinBox *arraySpinBox1[6];
    arraySpinBox1[0] = SET_SCAN_X_MIN_doubleSpinBox = new QDoubleSpinBox(tab2_4);
    arraySpinBox1[1] = SET_SCAN_X_MAX_doubleSpinBox = new QDoubleSpinBox(tab2_4);
    arraySpinBox1[2] = SET_SCAN_Y_MIN_doubleSpinBox = new QDoubleSpinBox(tab2_4);
    arraySpinBox1[3] = SET_SCAN_Y_MAX_doubleSpinBox = new QDoubleSpinBox(tab2_4);
    arraySpinBox1[4] = X_STEP_doubleSpinBox = new QDoubleSpinBox(tab2_4);
    arraySpinBox1[5] = Y_STEP_doubleSpinBox = new QDoubleSpinBox(tab2_4 );

    SCANXY_pushButton = new QPushButton();
    SCAN_ABORT_pushButton = new QPushButton();

    QLabel *scanLabel1 = new QLabel;
    scanLabel1->setText("<b>Scan area extrema</b>");

    QLabel *scanLabel2 = new QLabel;
    scanLabel2->setText("<b>Pixel dimensions</b>");


    for (int i = 0; i < 4; i++) {
        arraySpinBox1[i]->setDecimals(2);
        arraySpinBox1[i]->setMaximum(200);
        arraySpinBox1[i]->setSingleStep(0.5);
        arraySpinBox1[i]->setValue(100);
        arraySpinBox1[i]->setSuffix(" mm");
    }

    arraySpinBox1[0]->setPrefix("Min. X: ");
    arraySpinBox1[1]->setPrefix("Max. X: ");
    arraySpinBox1[2]->setPrefix("Min. Y: ");
    arraySpinBox1[3]->setPrefix("Max. Y: ");

    for (int i = 4; i < 6; i++) {
        arraySpinBox1[i]->setDecimals(2);
        arraySpinBox1[i]->setMaximum(50);
        arraySpinBox1[i]->setSingleStep(0.5);
        arraySpinBox1[i]->setValue(1);
        arraySpinBox1[i]->setSuffix(" mm");
    }

    arraySpinBox1[4]->setPrefix("Step X: ");
    arraySpinBox1[5]->setPrefix("Step Y: ");

    SCANXY_pushButton->setText("Start\nScan");
    SCANXY_pushButton->setMinimumHeight(60);
    SCANXY_pushButton->setStyleSheet("QPushButton {background-color: #02757D; font-weight: bold; color: white;}");

    SCAN_ABORT_pushButton->setText("ABORT");
    SCAN_ABORT_pushButton->setMinimumHeight(60);
    SCAN_ABORT_pushButton->setStyleSheet("QPushButton {background-color: #7C0A02; font-weight: bold; color: white;}");

    scanTabLayout->setHorizontalSpacing(20);
    scanTabLayout->setVerticalSpacing(10);
    scanTabLayout->addWidget(scanLabel1, 0, 0);
    scanTabLayout->addWidget(scanLabel2, 5, 0);
    scanTabLayout->addWidget(SET_SCAN_X_MIN_doubleSpinBox, 1, 0);
    scanTabLayout->addWidget(SET_SCAN_X_MAX_doubleSpinBox, 2, 0);
    scanTabLayout->addWidget(SET_SCAN_Y_MIN_doubleSpinBox, 3, 0);
    scanTabLayout->addWidget(SET_SCAN_Y_MAX_doubleSpinBox, 4, 0);
    scanTabLayout->addWidget(X_STEP_doubleSpinBox, 6, 0);
    scanTabLayout->addWidget(Y_STEP_doubleSpinBox, 7, 0);
    scanTabLayout->addWidget(SCANXY_pushButton, 1, 1, 2, 1);
    scanTabLayout->addWidget(SCAN_ABORT_pushButton, 3, 1, 2, 1);
    scanTabLayout->setColumnStretch(2, 3);


    /* ################################################################# */
    /* ####################  Area under tab Widget  #################### */
    /* ################################################################# */


    QGroupBox *groupBox = new QGroupBox(centralWidget);
    QVBoxLayout *daqLayout = new QVBoxLayout(groupBox);
    daqLayout->setSpacing(10);

    QRadioButton *radioUSB = new QRadioButton();
    QRadioButton *radioOptical = new QRadioButton();

    groupBox->setGeometry(QRect(900, 400, 300, 200));
    groupBox->setTitle("DAQ && Scan Settings");

    radioUSB->setText("USB - link");
    radioOptical->setText("Optical - link");
    radioUSB->setChecked(true);

    XY_SPEED_doubleSpinBox->setValue(1);
    XY_SPEED_doubleSpinBox->setDecimals(3);
    XY_SPEED_doubleSpinBox->setMaximum(50);
    XY_SPEED_doubleSpinBox->setSingleStep(0.001);
    XY_SPEED_doubleSpinBox->setPrefix("Scan speed: ");
    XY_SPEED_doubleSpinBox->setSuffix(" mm/s");

    lineEdit_2_tab_4->setEnabled(false);
    lineEdit_2_tab_4->setText("Target distance: ");
    //lineEdit_2_tab_4->setGeometry(QRect(637, 440, 200, 40));

    daqLayout->addWidget(radioUSB, 0);
    daqLayout->addWidget(radioOptical, 1);
    daqLayout->addWidget(XY_SPEED_doubleSpinBox, 2);
    daqLayout->addWidget(lineEdit_2_tab_4, 3);

    QGroupBox *monitorsBox = new QGroupBox("STATUS MONITORS", centralWidget);
    monitorsBox->setGeometry(QRect(640, 400, 250, 200));
    QVBoxLayout *monitorsLayout = new QVBoxLayout(monitorsBox);

    QLineEdit *arrayMonitors[4] = { };
    arrayMonitors[0] = CurrentActionX = new QLineEdit(centralWidget);
    arrayMonitors[1] = CurrentActionY = new QLineEdit(centralWidget);
    arrayMonitors[2] = CurrentActionZ = new QLineEdit(centralWidget);
    arrayMonitors[3] = CurrentActionACM = new QLineEdit(centralWidget);

    QString stylesheet1 = "QLineEdit {background-color: #CC3232; font-weight: bold; color: white;}";
    for (int i = 0; i < 4; i++) {
        arrayMonitors[i]->setEnabled(false);
        arrayMonitors[i]->setMinimumWidth(220);
        arrayMonitors[i]->setStyleSheet(stylesheet1);
    }

    CurrentActionX->setText("Stage X: Disconnected");
    CurrentActionY->setText("Stage Y: Disconnected");
    CurrentActionZ->setText("Stage Z: Disconnected");
    CurrentActionACM->setText("Arduino: Disconnected");

    monitorsLayout->addWidget(CurrentActionX, 0, 0);
    monitorsLayout->addWidget(CurrentActionY, 1, 0);
    monitorsLayout->addWidget(CurrentActionZ, 2, 0);
    monitorsLayout->addWidget(CurrentActionACM, 3, 0);

#ifndef QT_NO_STATUSBAR
    status = statusBar();
    status->showMessage("Ready");
#endif

    centralWidget->setFixedSize(1320, 640);

    ////////////////////////////// BUTTONS CONNECTIONS /////////////////////////////////

    connect(Digitizer_Button, SIGNAL(clicked()), this, SLOT(openDPPInter()));
    connect(VLC_Button, SIGNAL(clicked()), this, SLOT(VLC_interface()));
    connect(Export_Button, SIGNAL(clicked()), this, SLOT(Image_Export()));
    connect(XRAY_TABLE_Button, SIGNAL(clicked()), this, SLOT(XrayTable()));

    /////////////////////////////// USB CONNECTION //////////////////////////////////
    connect(radioUSB, SIGNAL(clicked()), this, SLOT(USB_DAQ()));
    connect(radioOptical, SIGNAL(clicked()), this, SLOT(OPTICAL_DAQ()));

}
