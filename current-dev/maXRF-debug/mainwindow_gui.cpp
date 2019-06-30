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
    scan_params[6] = new QDoubleSpinBox(centralWidget);

    tabWidget = new QTabWidget();
    tab1 = new QWidget(tabWidget);
    tab2 = new QWidget(tabWidget);
    tab3 = new QWidget(tabWidget);
    tab4 = new QWidget(tabWidget);

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

    /*******************************************************************/
    /********* TAB 1 in Tab Widget. Assign DFs and open FDs   **********/
    /*******************************************************************/

    tab1->setObjectName(QString::fromUtf8("tab"));
    tab1->setEnabled(true);

    QLabel *labelTab1a = new QLabel(tab1);
    QLabel *labelTab1b = new QLabel(tab1);
    labelTab1a->setText("<b><center>Device File Assignment</center></b>");
    labelTab1b->setText("<b>Assign ports for stages</b>");

    for (int i = 0 ; i < 3; i++) {
        tab1_df_number[i] = new QSpinBox(tab1);
        tab1_df_open[i] = new QPushButton(tab1);
        tab1_stage_init[i] = new QPushButton(tab1);

        tab1_df_number[i]->setValue(i);
        tab1_df_number[i]->setMaximum(3);
        tab1_df_open[i]->setText("Confirm");
    }
    tab1_df_number[0]->setPrefix("X-axis: ");
    tab1_df_number[1]->setPrefix("Y-axis: ");
    tab1_df_number[2]->setPrefix("Z-axis: ");

    tab1_stage_init[0]->setText("Initiate X");
    tab1_stage_init[1]->setText("Initiate Y");
    tab1_stage_init[2]->setText("Initiate Z");

    tab1_stop = new QPushButton(tab1);
    tab1_stop->setText("STOP");
    tab1_stop->setMinimumHeight(60);
    tab1_stop->setStyleSheet("QPushButton {background-color: #7C0A02; font-weight: bold; color: white;}");

    QGridLayout *portTabLayout = new QGridLayout(tab1);
    portTabLayout->setHorizontalSpacing(20);
    portTabLayout->setVerticalSpacing(10);

    portTabLayout->addWidget(labelTab1a, 0, 0, 1, 5);
    portTabLayout->addWidget(labelTab1b, 1, 1, 1, 3);
    for (int i = 0 ; i < 3; i++) {
        portTabLayout->addWidget(tab1_df_number[i], i+2, 1);
        portTabLayout->addWidget(tab1_df_open[i], i+2, 2);
        portTabLayout->addWidget(tab1_stage_init[i], i+2, 3);
    }
    portTabLayout->addWidget(tab1_stop, 6, 3);
    portTabLayout->setRowStretch(7, 3);
    portTabLayout->setColumnStretch(0, 3);
    portTabLayout->setColumnStretch(4, 3);

    /*******************************************************************/
    /******* TAB 2 in Tab Widget. Configure and start servo   **********/
    /*******************************************************************/

    tab2->setObjectName(QString::fromUtf8("tab_3"));
    tab2->setEnabled(false);

    QGridLayout  *acmTabLayout = new QGridLayout(tab2);
    QLabel *labelTab2Servo = new QLabel(tab2);
    labelTab2Servo->setText("<b><center>Laser Servomechanism</center></b>");


    tab1_df_number[3] = new QSpinBox(tab2);
    tab1_df_open[3] = new QPushButton(tab2);
    AUTOFOCUS_ON_pushButton = new QCheckBox(tab2);
    servo_checkbox = new QCheckBox(tab2);

    tab1_df_number[3]->setMaximum(3);
    tab1_df_number[3]->setPrefix("Assign ACM to port: ");

    tab1_df_open[3]->setText("Confirm");


    AUTOFOCUS_ON_pushButton->setText("Enable distance reading");
    AUTOFOCUS_ON_pushButton->setEnabled(false);

    servo_checkbox->setText("Enable tracking servo");
    servo_checkbox->setEnabled(false);

    acmTabLayout->setHorizontalSpacing(20);
    acmTabLayout->setVerticalSpacing(10);

    acmTabLayout->addWidget(labelTab2Servo, 0, 0, 1, 4);
    acmTabLayout->addWidget(tab1_df_number[3], 1, 1);
    acmTabLayout->addWidget(tab1_df_open[3], 1, 2);
    acmTabLayout->addWidget(AUTOFOCUS_ON_pushButton, 2, 1);
    acmTabLayout->addWidget(servo_checkbox, 3, 1);
    acmTabLayout->setColumnStretch(0, 3);
    acmTabLayout->setColumnStretch(3, 3);
    acmTabLayout->setRowStretch(4, 3);



    ///////////////////////////////// WIDGET 1 TAB4

    tabWidget->addTab(tab1, QString());
    tabWidget->addTab(tab2, QString());
    tabWidget->addTab(tab3, QString());
    tabWidget->addTab(tab4, QString());
    tabWidget->setTabText(tabWidget->indexOf(tab1), "Ports && Init");
    tabWidget->setTabText(tabWidget->indexOf(tab2), "Target Servo");
    tabWidget->setTabText(tabWidget->indexOf(tab3), "Move Stages");
    tabWidget->setTabText(tabWidget->indexOf(tab4), "Scan");
    tabWidget->setCurrentIndex(0);

    /* ################################################################# */
    /* ####################   MOVE in tab Widget   #################### */
    /* ################################################################# */

    tab3->setObjectName(QString::fromUtf8("tab2_3"));
    tab3->setEnabled(false);

    QPixmap pixmap1("icons/up.xpm");
    QPixmap pixmap2("icons/down.xpm");
    QPixmap pixmap3("icons/left.xpm");
    QPixmap pixmap4("icons/right.xpm");

    tab3_stop = new QPushButton(tab3);
    tab3_stop->setText("STOP");
    tab3_stop->setMinimumHeight(60);
    tab3_stop->setStyleSheet("QPushButton {background-color: #7C0A02; font-weight: bold; color: white;}");

    buttonTab3[0] = new QPushButton(tab3);   // Move X
    buttonTab3[1] = new QPushButton(tab3);   // Move Y
    buttonTab3[2] = new QPushButton(tab3);   // Move Z
    buttonTab3[3] = new QPushButton(pixmap4, "", tab3);  // Right
    buttonTab3[4] = new QPushButton(pixmap3, "", tab3);   // Left
    buttonTab3[5] = new QPushButton(pixmap2, "", tab3);   // Down
    buttonTab3[6] = new QPushButton(pixmap1, "", tab3);     // Up
    buttonTab3[7] = new QPushButton(tab2);         // Backwards
    buttonTab3[8] = new QPushButton(tab2);          // Forwards

    buttonTab3[7]->setText("Backward");
    buttonTab3[8]->setText("Forward");

    spinboxTab3[0] = new QDoubleSpinBox(tab3);
    spinboxTab3[1] = new QDoubleSpinBox(tab3);
    spinboxTab3[2] = new QDoubleSpinBox(tab2);
    spinboxTab3[0]->setPrefix("X: ");
    spinboxTab3[1]->setPrefix("Y: ");
    spinboxTab3[2]->setPrefix("Z:  ");

    QGridLayout *moveTabLayout = new QGridLayout(tab3);
    moveTabLayout->setHorizontalSpacing(20);
    moveTabLayout->setVerticalSpacing(10);

    QGridLayout *fineLayout = new QGridLayout();
    fineLayout->setHorizontalSpacing(20);
    fineLayout->setVerticalSpacing(10);

    fineLayout->addWidget(buttonTab3[3], 1, 2);
    fineLayout->addWidget(buttonTab3[4], 1, 0);
    fineLayout->addWidget(buttonTab3[5], 1, 1);
    fineLayout->addWidget(buttonTab3[6], 0, 1);

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


        connect(spinboxTab3[i], SIGNAL(valueChanged(double)), this, SLOT(tab3_set_target()));

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
    moveTabLayout->addWidget(buttonTab3[7], 8, 1);
    moveTabLayout->addWidget(buttonTab3[8], 8, 2);
    moveTabLayout->addWidget(tab3_stop, 6, 3, 2, 1);
    moveTabLayout->setColumnStretch(0, 3);
    moveTabLayout->setColumnStretch(4, 3);


    /* ################################################################# */
    /* ####################   SCAN in tab Widget   #################### */
    /* ################################################################# */

    tab4->setObjectName(QString::fromUtf8("tab2_4"));
    tab4->setEnabled(true);


    QGridLayout *scanTabLayout = new QGridLayout(tab4);


    tab4_start_scan = new QPushButton();
    SCAN_ABORT_pushButton = new QPushButton();

    QLabel *scanLabel1 = new QLabel;
    scanLabel1->setText("<b>Scan area extrema</b>");

    QLabel *scanLabel2 = new QLabel;
    scanLabel2->setText("<b>Pixel dimensions</b>");


    for (int i = 0; i < 4; i++) {
        scan_params[i] = new QDoubleSpinBox(tab4);
        scan_params[i]->setDecimals(2);
        if (i == 1) scan_params[i]->setMaximum(140);
        else scan_params[i]->setMaximum(200);
        scan_params[i]->setSingleStep(0.5);
        scan_params[i]->setValue(100);
        scan_params[i]->setSuffix(" mm");
    }

    scan_params[0]->setPrefix("Min. X: ");
    scan_params[1]->setPrefix("Max. X: ");
    scan_params[2]->setPrefix("Min. Y: ");
    scan_params[3]->setPrefix("Max. Y: ");

    for (int i = 4; i < 6; i++) {
        scan_params[i] = new QDoubleSpinBox(tab4);
        scan_params[i]->setDecimals(2);
        scan_params[i]->setMaximum(50);
        scan_params[i]->setSingleStep(0.5);
        scan_params[i]->setValue(1);
        scan_params[i]->setSuffix(" mm");
    }

    scan_params[4]->setPrefix("Step X: ");
    scan_params[5]->setPrefix("Step Y: ");

    tab4_start_scan->setText("Start\nScan");
    tab4_start_scan->setMinimumHeight(60);
    tab4_start_scan->setStyleSheet("QPushButton {background-color: #02757D; font-weight: bold; color: white;}");

    SCAN_ABORT_pushButton->setText("ABORT");
    SCAN_ABORT_pushButton->setMinimumHeight(60);
    SCAN_ABORT_pushButton->setStyleSheet("QPushButton {background-color: #7C0A02; font-weight: bold; color: white;}");

    scanTabLayout->setHorizontalSpacing(20);
    scanTabLayout->setVerticalSpacing(10);
    scanTabLayout->addWidget(scanLabel1, 0, 1);
    scanTabLayout->addWidget(scanLabel2, 5, 1);
    scanTabLayout->addWidget(scan_params[0], 1, 1);
    scanTabLayout->addWidget(scan_params[1], 2, 1);
    scanTabLayout->addWidget(scan_params[2], 3, 1);
    scanTabLayout->addWidget(scan_params[3], 4, 1);
    scanTabLayout->addWidget(scan_params[4], 6, 1);
    scanTabLayout->addWidget(scan_params[5], 7, 1);
    scanTabLayout->addWidget(tab4_start_scan, 1, 2, 2, 2);
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

    groupBox->setAlignment(Qt::AlignCenter);

    radioUSB->setText("USB");
    radioOptical->setText("Optical");
    radioUSB->setChecked(true);

    QLabel *daqLabel = new QLabel();
    daqLabel->setText("Motor scan speed:");

    scan_params[6]->setValue(1);
    scan_params[6]->setDecimals(3);
    scan_params[6]->setMaximum(50);
    scan_params[6]->setSingleStep(0.001);
    scan_params[6]->setSuffix(" mm/s");

    daqLayout->addWidget(radioUSB, 0, 0);
    daqLayout->addWidget(radioOptical, 0, 1);
    daqLayout->addWidget(daqLabel, 1, 0, 1, 2);
    daqLayout->addWidget(scan_params[6], 2, 0, 1, 2);
    daqLayout->setRowStretch(4, 3);

    QGroupBox *monitorsBox = new QGroupBox("STATUS MONITORS");
    QVBoxLayout *monitorsLayout = new QVBoxLayout(monitorsBox);
    monitorsBox->setFixedWidth(230);
    monitorsBox->setAlignment(Qt::AlignCenter);

    QString temp_msg[] = { "Stage X: ", "Stage Y: ", "Stage Z: ", "Keyence: " };
    QString stylesheet1 = "QLineEdit {background-color: #CC3232; font-weight: bold; color: white;}";

    for (int i = 0; i < 4; i++) {
        dev_monitor[i] = new QLineEdit(centralWidget);

        dev_monitor[i]->setEnabled(false);
        dev_monitor[i]->setFixedWidth(200);
        dev_monitor[i]->setStyleSheet(stylesheet1);
        dev_monitor[i]->setText(temp_msg[i] + "Disconnected");

        monitorsLayout->addWidget(dev_monitor[i], i, nullptr);
    }


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
