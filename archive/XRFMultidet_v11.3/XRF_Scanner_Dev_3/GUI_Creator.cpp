#include <mainwindow.h>
#include <../Header.h>
#include <../Shm.h>

void MainWindow::GUI_CREATOR() {
    centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    VLC_Button = new QPushButton(centralWidget);
    Digitizer_Button = new QPushButton(centralWidget);
    Export_Button = new QPushButton(centralWidget);
    XRAY_TABLE_Button = new QPushButton(centralWidget);
    XY_SPEED_doubleSpinBox = new QDoubleSpinBox(centralWidget);

    tabWidget = new QTabWidget();
    tab = new QWidget(tabWidget);
    tab_3 = new QWidget(tabWidget);
    tab2_3 = new QWidget(tabWidget);
    tab2_4 = new QWidget(tabWidget);

    QGridLayout *layoutMaster = new QGridLayout(centralWidget);
    layoutMaster->setVerticalSpacing(10);
    layoutMaster->setHorizontalSpacing(15);

    /******************************/
    /********** GEOMETRY **********/
    /******************************/

    scrollArea = new QScrollArea();
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setFixedSize(600, 600);


    QGroupBox *utilityBox = new QGroupBox();
    //utilityBox->setGeometry(QRect(1120, 15, 160, 220));
    utilityBox->setTitle("Utility programs");
    utilityBox->setFixedHeight(200);

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
    tabWidget->setFixedSize(450, 400);
    //tabWidget->setGeometry(QRect(640, 15, 430, 375));

    /* ################################################################# */
    /* ####################   PORTS in tab Widget   #################### */
    /* ################################################################# */

    tab->setObjectName(QString::fromUtf8("tab"));
    tab->setEnabled(true);

    spinBox_assignX = new QSpinBox(tab);
    pushButton_assignX_port = new QPushButton(tab);
    spinBox_assignY = new QSpinBox(tab);
    pushButton_assignY_port = new QPushButton(tab);
    pushButton_tab_2_2X = new QPushButton(tab);
    pushButton_tab_2_2Y = new QPushButton(tab);
    pushButton_tab_2_2ABORT = new QPushButton(tab);
    spinBox_assignZ = new QSpinBox(tab);
    AssignZ_pushButton = new QPushButton(tab);
    INIT_Z_pushButton = new QPushButton(tab);

    QLabel *labelTab1a = new QLabel(tab);
    QLabel *labelTab1b = new QLabel(tab);
    labelTab1a->setText("<b><center>Device File Assignment</center></b>");
    labelTab1b->setText("<b>Assign ports for stages</b>");

    QGridLayout *portTabLayout = new QGridLayout(tab);
    portTabLayout->setHorizontalSpacing(20);
    portTabLayout->setVerticalSpacing(10);

    spinBox_assignX->setValue(2);
    spinBox_assignX->setMaximum(3);
    spinBox_assignX->setPrefix("X-axis: ");

    pushButton_assignX_port->setText("Confirm");
    pushButton_tab_2_2X->setText("Initiate X");

    spinBox_assignY->setValue(0);
    spinBox_assignY->setMaximum(3);
    spinBox_assignY->setPrefix("Y-axis: ");

    pushButton_assignY_port->setText("Confirm");
    pushButton_tab_2_2Y->setText("Initiate Y");

    spinBox_assignZ->setValue(1);
    spinBox_assignZ->setMaximum(3);
    spinBox_assignZ->setPrefix("Z-axis: ");

    AssignZ_pushButton->setText("Confirm");
    INIT_Z_pushButton->setText("Initiate Z");

    //STOP_Z_INIT_pushButton->setText("ABORT Z");
    pushButton_tab_2_2ABORT->setText("ABORT");
    pushButton_tab_2_2ABORT->setMinimumHeight(60);
    pushButton_tab_2_2ABORT->setStyleSheet("QPushButton {background-color: #7C0A02; font-weight: bold; color: white;}");


    portTabLayout->addWidget(labelTab1a, 0, 0, 1, 5);
    portTabLayout->addWidget(labelTab1b, 1, 1, 1, 3);
    portTabLayout->addWidget(spinBox_assignX, 2, 1);
    portTabLayout->addWidget(spinBox_assignY, 3, 1);
    portTabLayout->addWidget(spinBox_assignZ, 4, 1);
    portTabLayout->addWidget(pushButton_assignX_port, 2, 2);
    portTabLayout->addWidget(pushButton_assignY_port, 3, 2);
    portTabLayout->addWidget(AssignZ_pushButton, 4, 2);
    portTabLayout->addWidget(pushButton_tab_2_2X, 2, 3);
    portTabLayout->addWidget(pushButton_tab_2_2Y, 3, 3);
    portTabLayout->addWidget(INIT_Z_pushButton, 4, 3);
    //portTabLayout->addWidget(STOP_Z_INIT_pushButton, 3, 1);
    portTabLayout->addWidget(pushButton_tab_2_2ABORT, 6, 3);
    portTabLayout->setRowStretch(7, 3);
    portTabLayout->setColumnStretch(0, 3);
    portTabLayout->setColumnStretch(4, 3);

    /* ################################################################# */
    /* ####################   ACM in tab Widget   #################### */
    /* ################################################################# */
    //STOP_Z_MOVE_pushButton = new QPushButton(tab_3);
    //STOP_Z_MOVE_pushButton->setText("STOP");

    tab_3->setObjectName(QString::fromUtf8("tab_3"));
    tab_3->setEnabled(true);

    QGridLayout  *acmTabLayout = new QGridLayout(tab_3);
    //QLabel *label_23_tab_4 = new QLabel(tab_3);
    //QLabel *labelTab2Motor = new QLabel(tab_3);
    QLabel *labelTab2Servo = new QLabel(tab_3);

    //labelTab2Motor->setText("<b>Z-axis motor</b>");
    labelTab2Servo->setText("<b><center>Laser Servomechanism</center></b>");


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
    //acmTabLayout->addWidget(STOP_Z_MOVE_pushButton, 3, 0);

    acmTabLayout->addWidget(labelTab2Servo, 0, 0, 1, 4);
    acmTabLayout->addWidget(spinBox_assignACM, 1, 1);
    acmTabLayout->addWidget(AssignACM_pushButton, 1, 2);
    acmTabLayout->addWidget(AUTOFOCUS_ON_pushButton, 2, 1);
    acmTabLayout->addWidget(ENABLE_TRACKING_checkBox, 3, 1);
    acmTabLayout->setColumnStretch(0, 3);
    acmTabLayout->setColumnStretch(3, 3);
    acmTabLayout->setRowStretch(4, 3);



    ///////////////////////////////// WIDGET 1 TAB4

    tabWidget->addTab(tab, QString());
    tabWidget->addTab(tab_3, QString());
    tabWidget->addTab(tab2_3, QString());
    tabWidget->addTab(tab2_4, QString());
    tabWidget->setTabText(tabWidget->indexOf(tab), "Ports && Init");
    tabWidget->setTabText(tabWidget->indexOf(tab_3), "Target Servo");
    tabWidget->setTabText(tabWidget->indexOf(tab2_3), "Move Stages");
    tabWidget->setTabText(tabWidget->indexOf(tab2_4), "Scan");
    tabWidget->setCurrentIndex(0);

    /* ################################################################# */
    /* ####################   MOVE in tab Widget   #################### */
    /* ################################################################# */

    tab2_3->setObjectName(QString::fromUtf8("tab2_3"));
    tab2_3->setEnabled(false);

    QPixmap pixmap1("icons/up.xpm");
    QPixmap pixmap2("icons/down.xpm");
    QPixmap pixmap3("icons/left.xpm");
    QPixmap pixmap4("icons/right.xpm");

    STOPMOVE_pushButton = new QPushButton(tab2_3);
    STOPMOVE_pushButton->setText("STOP");
    STOPMOVE_pushButton->setMinimumHeight(60);
    STOPMOVE_pushButton->setStyleSheet("QPushButton {background-color: #7C0A02; font-weight: bold; color: white;}");

    QPushButton *buttonTab3[9];
    buttonTab3[0] = MOVE_TO_X_pushButton = new QPushButton(tab2_3);
    buttonTab3[1] = MOVE_TO_Y_pushButton = new QPushButton(tab2_3);
    buttonTab3[2] = MOVE_Z_To_pushButton = new QPushButton(tab2_3);
    buttonTab3[3] = MOVERIGHT_pushButton = new QPushButton(pixmap4, "", tab2_3);
    buttonTab3[4] = MOVELEFT_pushButton = new QPushButton(pixmap3, "", tab2_3);
    buttonTab3[5] = MOVEDOWN_pushButton = new QPushButton(pixmap2, "", tab2_3);
    buttonTab3[6] = MOVEUP_pushButton = new QPushButton(pixmap1, "", tab2_3);
    buttonTab3[7] = MOVE_Z_BACKWARD_pushButton = new QPushButton(tab_3);
    buttonTab3[8] = MOVE_Z_FORWARD_pushButton = new QPushButton(tab_3);

    MOVE_Z_FORWARD_pushButton->setText("Forward");
    MOVE_Z_BACKWARD_pushButton->setText("Backward");

    QSignalMapper *mapperMoveStages = new QSignalMapper();
    for (int i = 0; i < 9; i++) {
        mapperMoveStages->setMapping(buttonTab3[i], i);
        connect(buttonTab3[i], SIGNAL(released()), mapperMoveStages, SLOT(map()));
    }
    connect(mapperMoveStages, SIGNAL(mapped(int)), this, SLOT(slotMoveStage(int)));

    QDoubleSpinBox *spinboxTab3[3];
    spinboxTab3[0] = MOVE_TO_X_doubleSpinBox = new QDoubleSpinBox(tab2_3);
    spinboxTab3[1] = MOVE_TO_Y_doubleSpinBox = new QDoubleSpinBox(tab2_3);
    spinboxTab3[2] = MOVE_Z_To_doubleSpinBox = new QDoubleSpinBox(tab_3);
    spinboxTab3[0]->setPrefix("X-axis: ");
    spinboxTab3[1]->setPrefix("Y-axis: ");
    spinboxTab3[2]->setPrefix("Z-axis:  ");

    QGridLayout *moveTabLayout = new QGridLayout(tab2_3);
    moveTabLayout->setHorizontalSpacing(20);
    moveTabLayout->setVerticalSpacing(10);

    QGridLayout *fineLayout = new QGridLayout();
    fineLayout->setHorizontalSpacing(20);
    fineLayout->setVerticalSpacing(10);

    fineLayout->addWidget(MOVELEFT_pushButton, 1, 0);
    fineLayout->addWidget(MOVERIGHT_pushButton, 1, 2);
    fineLayout->addWidget(MOVEDOWN_pushButton, 1, 1);
    fineLayout->addWidget(MOVEUP_pushButton, 0, 1);

    for (int i = 0; i < 3; i++) {
        buttonTab3[i]->setText("Confirm");
        spinboxTab3[i]->setDecimals(3);
        spinboxTab3[i]->setSingleStep(0.5);
        if (i == 0) {
            spinboxTab3[i]->setMaximum(140);
            spinboxTab3[i]->setValue(100);
        }
        else if (i == 1) {
            spinboxTab3[i]->setMaximum(200);
            spinboxTab3[i]->setValue(100);
        }
        else {
            spinboxTab3[i]->setMaximum(50);
            spinboxTab3[i]->setValue(25);
        }
        spinboxTab3[i]->setFixedWidth(220);
        spinboxTab3[i]->setSuffix(" mm");

        moveTabLayout->addWidget(spinboxTab3[i], i + 2, 1, 1, 2);
        moveTabLayout->addWidget(buttonTab3[i], i + 2, 3);
    }


    QLabel *labelTab3a = new QLabel();
    QLabel *labelTab3b = new QLabel();
    QLabel *labelTab3c = new QLabel();
    labelTab3a->setText("<b>Specify stage position</b>");
    labelTab3b->setText("<b>Fine stage adjustments</b>");
    labelTab3c->setText("<b><center>Manual Stage Control</center></b>");

    moveTabLayout->addWidget(labelTab3a, 1, 1, 1, 2);
    moveTabLayout->addWidget(labelTab3b, 5, 1, 1, 2);
    moveTabLayout->addWidget(labelTab3c, 0, 0, 1, 5);
    moveTabLayout->addLayout(fineLayout, 6, 1, 2, 2);
    moveTabLayout->addWidget(MOVE_Z_BACKWARD_pushButton, 8, 1);
    moveTabLayout->addWidget(MOVE_Z_FORWARD_pushButton, 8, 2);
    moveTabLayout->addWidget(STOPMOVE_pushButton, 6, 3, 2, 1);
    moveTabLayout->setColumnStretch(0, 3);
    moveTabLayout->setColumnStretch(4, 3);


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
        if (i == 1) arraySpinBox1[i]->setMaximum(140);
        else arraySpinBox1[i]->setMaximum(200);
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
    scanTabLayout->addWidget(scanLabel1, 0, 1);
    scanTabLayout->addWidget(scanLabel2, 5, 1);
    scanTabLayout->addWidget(SET_SCAN_X_MIN_doubleSpinBox, 1, 1);
    scanTabLayout->addWidget(SET_SCAN_X_MAX_doubleSpinBox, 2, 1);
    scanTabLayout->addWidget(SET_SCAN_Y_MIN_doubleSpinBox, 3, 1);
    scanTabLayout->addWidget(SET_SCAN_Y_MAX_doubleSpinBox, 4, 1);
    scanTabLayout->addWidget(X_STEP_doubleSpinBox, 6, 1);
    scanTabLayout->addWidget(Y_STEP_doubleSpinBox, 7, 1);
    scanTabLayout->addWidget(SCANXY_pushButton, 1, 2, 2, 2);
    scanTabLayout->addWidget(SCAN_ABORT_pushButton, 3, 2, 2, 2);
    scanTabLayout->setColumnStretch(0, 3);
    scanTabLayout->setColumnStretch(4, 3);


    /* ################################################################# */
    /* ####################  Area under tab Widget  #################### */
    /* ################################################################# */


    QGroupBox *groupBox = new QGroupBox("DAQ && Scan Settings");
    QGridLayout *daqLayout = new QGridLayout(groupBox);
    daqLayout->setSpacing(10);

    QRadioButton *radioUSB = new QRadioButton();
    QRadioButton *radioOptical = new QRadioButton();

    //groupBox->setGeometry(QRect(900, 400, 300, 200));
    //groupBox->setTitle("DAQ && Scan Settings");
    groupBox->setAlignment(Qt::AlignCenter);

    radioUSB->setText("USB");
    radioOptical->setText("Optical");
    radioUSB->setChecked(true);

    QLabel *daqLabel = new QLabel();
    daqLabel->setText("Motor scan speed:");

    XY_SPEED_doubleSpinBox->setValue(1);
    XY_SPEED_doubleSpinBox->setDecimals(3);
    XY_SPEED_doubleSpinBox->setMaximum(50);
    XY_SPEED_doubleSpinBox->setSingleStep(0.001);
    XY_SPEED_doubleSpinBox->setSuffix(" mm/s");

    lineEdit_2_tab_4->setEnabled(false);
    lineEdit_2_tab_4->setText("Target distance: ");
    //lineEdit_2_tab_4->setGeometry(QRect(637, 440, 200, 40));

    daqLayout->addWidget(radioUSB, 0, 0);
    daqLayout->addWidget(radioOptical, 0, 1);
    daqLayout->addWidget(daqLabel, 1, 0, 1, 2);
    daqLayout->addWidget(XY_SPEED_doubleSpinBox, 2, 0, 1, 2);
    daqLayout->addWidget(lineEdit_2_tab_4, 3, 0, 1, 2);
    daqLayout->setRowStretch(4, 3);

    QGroupBox *monitorsBox = new QGroupBox("STATUS MONITORS");
    monitorsBox->setFixedWidth(230);
    monitorsBox->setAlignment(Qt::AlignCenter);
    //monitorsBox->setGeometry(QRect(640, 400, 250, 200));
    QVBoxLayout *monitorsLayout = new QVBoxLayout(monitorsBox);

    QLineEdit *arrayMonitors[4] = { };
    arrayMonitors[0] = CurrentActionX = new QLineEdit(centralWidget);
    arrayMonitors[1] = CurrentActionY = new QLineEdit(centralWidget);
    arrayMonitors[2] = CurrentActionZ = new QLineEdit(centralWidget);
    arrayMonitors[3] = CurrentActionACM = new QLineEdit(centralWidget);
    CurrentActionX->setText("Stage X: Disconnected");
    CurrentActionY->setText("Stage Y: Disconnected");
    CurrentActionZ->setText("Stage Z: Disconnected");
    CurrentActionACM->setText("Arduino: Disconnected");

    QString stylesheet1 = "QLineEdit {background-color: #CC3232; font-weight: bold; color: white;}";
    for (int i = 0; i < 4; i++) {
        arrayMonitors[i]->setEnabled(false);
        arrayMonitors[i]->setFixedWidth(200);
        arrayMonitors[i]->setStyleSheet(stylesheet1);
    }

    monitorsLayout->addWidget(CurrentActionX, 0, 0);
    monitorsLayout->addWidget(CurrentActionY, 1, 0);
    monitorsLayout->addWidget(CurrentActionZ, 2, 0);
    monitorsLayout->addWidget(CurrentActionACM, 3, 0);

#ifndef QT_NO_STATUSBAR
    status = statusBar();
    status->showMessage("Ready");
#endif


    layoutMaster->addWidget(scrollArea, 0, 0, 2, 2, Qt::AlignTop);
    layoutMaster->addWidget(tabWidget, 0, 2, 1, 2);
    layoutMaster->addWidget(utilityBox, 0, 4, 1, 1, Qt::AlignTop);
    layoutMaster->addWidget(monitorsBox, 1, 2);
    layoutMaster->addWidget(groupBox, 1, 3);
    layoutMaster->setColumnStretch(6, 3);

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
